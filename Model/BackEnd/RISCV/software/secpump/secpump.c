// See LICENSE for license details.

#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include <string.h>
#include "plic/plic_driver.h"
#include "encoding.h"
#include <unistd.h>
#include "stdatomic.h"


/*Functions*/
void Parse(uint8_t * packet, uint8_t MODE);

void PacketArbitrer(uint8_t * name, uint8_t * volume, uint8_t * duration, uint8_t MODE);

uint8_t CreateCure(uint8_t * name, uint8_t * volume, uint8_t * duration);

uint8_t ModifyCure(uint8_t * volume, uint8_t * duration);

uint8_t DeleteCure();

uint8_t MatchName(uint8_t * name);

void RunCure();

uint32_t GetDuration(uint32_t currentTime);

uint8_t UpdateCureValues(uint32_t cureDuration);

uint8_t UpdateRestingVolume(uint32_t cureDuration);

uint8_t UpdateRestingDuration();


/*Variables*/
typedef struct CureProg{

	uint8_t name[32];				/*name from chacteristic name*/
	uint16_t initVolume; 		/*initial volume*/
	uint16_t volume;	  		/*volume from chacteristic volume*/
	uint32_t initDuration; 	/*initial duration*/
	uint32_t duration;  		/*duration get from characteristic duration*/	
	uint8_t valid;

} CureProg;

/*Global Variable*/

/*Cure init in the data section*/
CureProg cure = {"" , 0 , 0 , 0 , 0 , 0 }; 

uint8_t packet[48] =  "Morphin\0""100\0""60";

void reset_demo (void);

// Structures for registering different interrupt handlers
// for different parts of the application.
typedef void (*function_ptr_t) (void);
void no_interrupt_handler (void) {};
function_ptr_t g_ext_interrupt_handlers[PLIC_NUM_INTERRUPTS];


// Instance data for the PLIC.
plic_instance_t g_plic;


/**
* use mtime to wait for a specified number of ticks.
* async determins if this is a busy wait, or if
* an irq is scheduled
*/
void mtime_wait( uint64_t ticks, uint32_t async)
{

    volatile uint64_t * mtime  = (uint64_t*) (CLINT_CTRL_ADDR + CLINT_MTIME);
    uint64_t now = *mtime;
    
    //if async, schedule irq
    if(async) {
      volatile uint64_t * mtimecmp = (uint64_t*) (CLINT_CTRL_ADDR + CLINT_MTIMECMP);
      uint64_t then = now + ticks; 
      *mtimecmp = then;
      set_csr(mie, MIP_MTIP);

    } else  {
      //else busy wait
      uint64_t then = now + ticks;
      while(*mtime<then) {}

    }

}

/*Entry Point for PLIC Interrupt Handler*/
void handle_m_ext_interrupt(){
  plic_source int_num  = PLIC_claim_interrupt(&g_plic);
  if ((int_num >=1 ) && (int_num < PLIC_NUM_INTERRUPTS)) {
    g_ext_interrupt_handlers[int_num]();
  }
  else {
    exit(1 + (uintptr_t) int_num);
  }
  PLIC_complete_interrupt(&g_plic, int_num);
}

/*Entry Point for Machine Timer Interrupt Handler*/
void handle_m_time_interrupt(){

  //cleare the timer irq
  clear_csr(mie, MIP_MTIP);

  //schedule next timer irq for 1 second
  mtime_wait(1*RTC_FREQ, 1);

  //flash a led
  GPIO_REG(GPIO_OUTPUT_VAL) ^=	(0x1 << BLUE_LED_OFFSET);
  
  //print the count
  //printf("watchdog reset in %d\n", countdown--);
	if(cure.valid == 1)
	{
		RunCure();
	}

}


const char * instructions_msg = " \
\n\
        VERY SECURE PUMP MODEL\n\
\n\
VVVVVVVV           VVVVVVVV   SSSSSSSSSSSSSSS PPPPPPPPPPPPPPPPP   MMMMMMMM               MMMMMMMM\n\
V::::::V           V::::::V SS:::::::::::::::SP::::::::::::::::P  M:::::::M             M:::::::M\n\
V::::::V           V::::::VS:::::SSSSSS::::::SP::::::PPPPPP:::::P M::::::::M           M::::::::M\n\
V::::::V           V::::::VS:::::S     SSSSSSSPP:::::P     P:::::PM:::::::::M         M:::::::::M\n\
 V:::::V           V:::::V S:::::S              P::::P     P:::::PM::::::::::M       M::::::::::M\n\
  V:::::V         V:::::V  S:::::S              P::::P     P:::::PM:::::::::::M     M:::::::::::M\n\
   V:::::V       V:::::V    S::::SSSS           P::::PPPPPP:::::P M:::::::M::::M   M::::M:::::::M\n\
    V:::::V     V:::::V      SS::::::SSSSS      P:::::::::::::PP  M::::::M M::::M M::::M M::::::M\n\
     V:::::V   V:::::V         SSS::::::::SS    P::::PPPPPPPPP    M::::::M  M::::M::::M  M::::::M\n\
      V:::::V V:::::V             SSSSSS::::S   P::::P            M::::::M   M:::::::M   M::::::M\n\
       V:::::V:::::V                   S:::::S  P::::P            M::::::M    M:::::M    M::::::M\n\
        V:::::::::V                    S:::::S  P::::P            M::::::M     MMMMM     M::::::M\n\
         V:::::::V         SSSSSSS     S:::::SPP::::::PP          M::::::M               M::::::M\n\
          V:::::V          S::::::SSSSSS:::::SP::::::::P          M::::::M               M::::::M\n\
           V:::V           S:::::::::::::::SS P::::::::P          M::::::M               M::::::M\n\
            VVV             SSSSSSSSSSSSSSS   PPPPPPPPPP          MMMMMMMM               MMMMMMMM\n\
\n\
Welcome to the Very Secure Pump Model RISC-V port\n\
A default drug injection is going to be performed\n\
\n\
The default drug injection will expire in 30 seconds resulting in\n\
a software reset.\n\
\n\
Send a correct Packet to prevent a reset\n\
";

void print_instructions() {

  write (STDOUT_FILENO, instructions_msg, strlen(instructions_msg));

}


/**
* Flash the red led for a second, then set up
* blue for blinking during mtime irq
*/
void led_init() {


  // Set up the GPIOs such that the LED GPIO
  // can be used as both Inputs and Outputs.
  GPIO_REG(GPIO_INPUT_EN)    &= ~((0x1<< RED_LED_OFFSET) | (0x1<< GREEN_LED_OFFSET) | (0x1 << BLUE_LED_OFFSET)) ;
  GPIO_REG(GPIO_OUTPUT_EN)   |=  ((0x1<< RED_LED_OFFSET)| (0x1<< GREEN_LED_OFFSET)| (0x1<< BLUE_LED_OFFSET));
  GPIO_REG(GPIO_OUTPUT_VAL)  |=  ((0x1<< RED_LED_OFFSET) | (0x1<< GREEN_LED_OFFSET) | (0x1<< BLUE_LED_OFFSET));

  //flash red led to indicate reset
  GPIO_REG(GPIO_OUTPUT_VAL)  &=   ~(0x1 << RED_LED_OFFSET) ;
  mtime_wait(1*RTC_FREQ,0);
  GPIO_REG(GPIO_OUTPUT_VAL)  |=   (0x1 << RED_LED_OFFSET) ;
}

/**
* initialize the watchdog to reset in
* 5 seconds
**/
void watchdog_init()  {


//reset in 10 seconds
AON_REG(AON_WDOGKEY) = AON_WDOGKEY_VALUE;
AON_REG(AON_WDOGCMP) = 61;
//wdogconfig: : wdogrsten | enablealways | reset to 0 | max scale
AON_REG(AON_WDOGKEY) = AON_WDOGKEY_VALUE;
AON_REG(AON_WDOGCFG) |= (AON_WDOGCFG_RSTEN | AON_WDOGCFG_ENALWAYS |\
                        AON_WDOGCFG_ZEROCMP | AON_WDOGCFG_SCALE) ;

}

void reset_demo ()  {

  // Disable the machine & timer interrupts until setup is done.
  clear_csr(mie, MIP_MEIP);
  clear_csr(mie, MIP_MTIP);

  //enable uart input
  UART0_REG(UART_REG_RXCTRL) = UART_RXEN;

  /**************************************************************************
   * Set up the PLIC
   *************************************************************************/
  PLIC_init(&g_plic,
	    PLIC_CTRL_ADDR,
	    PLIC_NUM_INTERRUPTS,
	    PLIC_NUM_PRIORITIES);

  for (int ii = 0; ii < PLIC_NUM_INTERRUPTS; ii ++){
    g_ext_interrupt_handlers[ii] = no_interrupt_handler;
  }


  led_init();
  print_instructions();
  watchdog_init();
  
  // Enable the Machine-External bit in MIE
  set_csr(mie, MIP_MEIP);
  // Enable interrupts in general.
  set_csr(mstatus, MSTATUS_MIE);
}

int main(int argc, char **argv)
{

  reset_demo();
 	Parse(packet, 'C');
 //schedule a 1 second timer 
  mtime_wait(1*RTC_FREQ,1);
	
 
  while (1){
    char c;
    if(((int32_t) UART0_REG(UART_REG_RXFIFO)) > 0){
        //flash green led to indicate Kick
        GPIO_REG(GPIO_OUTPUT_VAL)  &=   ~(0x1 << GREEN_LED_OFFSET) ;  

        printf("Feeding Watchdog AAAAAAAAAAAAAAAAAAAAAAAAA.\nHiFive1 will reset in 10 seconds.\n");
        AON_REG(AON_WDOGKEY) = AON_WDOGKEY_VALUE;
        AON_REG(AON_WDOGFEED) = AON_WDOGFEED_VALUE;

        //busy wait a bit so the user sees the led blink
        mtime_wait(5000, 0);
        //turn off led
        GPIO_REG(GPIO_OUTPUT_VAL)  |=   (0x1 << GREEN_LED_OFFSET) ;
    }
  }

  return 0;

}

/*Parsing Function*/
void Parse(uint8_t * packet, uint8_t MODE)
{
	uint8_t name[36];
	uint8_t volume[8];
	uint8_t duration[12];

	int i; 
	uint8_t * packetPtr = packet;

	for(i=0;i<32;i++, packetPtr++)
	{
		if(*(packetPtr) == '\0')
		{
			packetPtr++;
			break;
		}

		*(name + i) = *(packetPtr);
	}

	if(i == 32)
	{
		packetPtr++;
	}

	*(name + i) = '\0';

	for(i=0;i<4;i++, packetPtr++)
	{
		if(*(packetPtr) == '\0')
		{
			packetPtr++;
			break;
		}

		*(volume + i) = *(packetPtr);
	}
	
	if(i == 4)
	{
		packetPtr++;
	}

	*(volume + i) = '\0';

	for(i=0;i<8;i++, packetPtr++)
	{
		if(*(packetPtr) == '\0')
		{
			packetPtr++;
			break;
		}	
		*(duration + i) = *(packetPtr);
	}
	
	*(duration + i) = '\0';

	//Debug
	//printf("[*]NAME : %s\n[*]VOLUME : %s\n[*]DURATION : %s\n", name, volume, duration);

	PacketArbitrer(name, volume, duration, MODE);
		
}


void PacketArbitrer(uint8_t * name, uint8_t * volume, uint8_t * duration, uint8_t MODE)
{
	if (MODE == 'C')
	{
		if(!CreateCure(name, volume, duration))
		{
			printf("[!] ERROR CREATING CURE\n");
		}
	}	

	else if (MODE == 'M')
	{
		if(MatchName(name))
		{
			if(!ModifyCure(volume, duration))
			{
				printf("[!] ERROR MODIFYING CURE\n");
			}
		}
		else
		{
			printf("[!] ERROR MATCHING NAME IN MODIF\n");
		} 
	}
	
	else if (MODE == 'D')
	{
		if(MatchName(name))
		{
			if(!DeleteCure())
			{
				printf("[!] ERROR DELETING CURE\n");
			}
		}
		else
		{
				printf("[!] ERROR MATCHING NAME IN DEL\n");
		} 
	}
	else
	{
			printf("[!] ERROR NO MODE IN PACKET ARBITRER\n");
	} 
}


uint8_t CreateCure(uint8_t * name, uint8_t * volume, uint8_t * duration)
{
	strncpy(cure.name, name, 31);
	cure.initVolume = atoi(volume);  
	cure.volume = atoi(volume);
	cure.initDuration = atoi(duration);
	cure.duration = 0;

	if(cure.initVolume == 0 | cure.initDuration == 0)
	{
		printf("NULL VOLUME AND DURATION\n");
		cure.valid=0;
	}
	else
	{
		cure.valid=1;
	}

	printf("[*] CURE CREATED\n");
	return 1;
}


uint8_t ModifyCure(uint8_t * volume, uint8_t * duration)
{
	cure.initVolume = atoi(volume);
	cure.volume = atoi(volume);
	cure.initDuration =	atoi(duration);
	//cure.duration = atoi(duration);
	// REPLACE THIS BY TIME OF TIMER 

	if(cure.initVolume == 0 | cure.initDuration == 0)
	{
		printf("NULL VOLUME AND DURATION\n");
		cure.valid=0;
	}
	else
	{
		cure.valid=0;
	}

	printf("[*] CURE MODIFIED\n");
	return 1;
}


uint8_t DeleteCure()
{
	memset(cure.name, 0x0, 32);
	cure.volume = 0;
	cure.initVolume = 0;
	cure.initDuration = 0;
	cure.duration = 0;

	cure.valid=0;

	printf("[*] CURE DELETED\n");
	return 1;
}

uint8_t MatchName(uint8_t * name)
{
	if(!strcmp(name, cure.name))
	{
		return 1;
	}
	return 0;	
}


void RunCure()
{
	//SEND INFORMATION THROUGHT BLE HERE AKA cure.volume, cure.duration, cure.name
	printf("[*]CURE NAME : %s VOLUME : %d DURATION : %d\n", cure.name, cure.volume, cure.duration);	

	//REPLACE THIS BY GET TIME TIMER
	if(cure.duration >= cure.initDuration)
	{
		printf("END OF CURE\n");
		DeleteCure();
		return;
	}	

	if(UpdateCureValues(cure.duration))
	{
		printf("CURE UPDATED\n");
	}
}

uint8_t UpdateCureValues(uint32_t cureDuration)
{
	if(UpdateRestingVolume(cureDuration))
	{
		printf("RESTING VOLUME UPDATED\n");
	}
	else
	{
		printf("ERROR VOLUME UPDATE\n");
	}
	if(UpdateRestingDuration())
	{
		printf("RESTING TIME UPDATED\n");
	}
	else
	{
		printf("ERROR TIME UPDATE\n");
	}	
	return 1;
}

uint8_t UpdateRestingVolume(uint32_t cureDuration)
{
	cure.volume = cure.initVolume - ((cure.initVolume * cureDuration) / cure.initDuration);
	return 1;
}

uint8_t UpdateRestingDuration()
{
	cure.duration += 1;
	return 1;
}

