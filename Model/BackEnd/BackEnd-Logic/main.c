#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

/*Functions*/
void Idle();

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

uint8_t UpdateRestingDuration(uint32_t cureDuration);


/*Variables*/
typedef struct CureProg{

	uint8_t name[32];				/*name from chacteristic name*/
	uint16_t initVolume; 		/*initial volume*/
	uint16_t volume;	  		/*volume from chacteristic volume*/
	uint32_t initDuration; 	/*initial duration*/
	uint32_t duration;  		/*duration get from characteristic duration*/	
	uint32_t startingTime;

} CureProg;

/*Global Variable*/

/*Cure init in the data section*/
CureProg cure = {"" , 0 , 0 , 0 , 0 , 0 }; 

uint8_t packet[48] =  "AAAAAAAAAAAAA\088\0""66"; 			 // RANDOM TEST FOR PARSER
uint8_t packet2[48] = "AAAAAAAAAAAAA\099\x00""11";     // RANDOM TEST FOR PARSER
uint8_t packet3[48] = "AAAAAAAAAAAAA\0987\070";				 // RANDOM TEST FOR PARSER

/*State Function Pointer*/
void (* state_fn)(void) = Idle;


int main()
{
	/*Code & Microcontroller Init*/	
	
	/*Turn on an Indicator Light Here*/

	/*Lauching the Finite State Machine*/	
	(*state_fn)();
	
}


/*Idle Master State Function*/
void Idle()
{
	 /*Waiting for State Changing*/
    while(state_fn)
    {
    	if(state_fn == Idle)
			{
				Parse(packet, 'C');
					printf("[*]CURE NAME : %s VOLUME : %d DURATION : %d\n", cure.name, cure.volume, cure.duration);	
				Parse(packet2, 'M');
					printf("[*]CURE NAME : %s VOLUME : %d DURATION : %d\n", cure.name, cure.volume, cure.duration);
				Parse(packet3, 'D');		
					printf("[*]CURE NAME : %s VOLUME : %d DURATION : %d\n", cure.name, cure.volume, cure.duration);
				Parse(packet, 'C');
					printf("[*]CURE NAME : %s VOLUME : %d DURATION : %d\n", cure.name, cure.volume, cure.duration);	
				//printf("%d", startingTime);	
				//	break;
			}
			else
			{
				(*state_fn)();
			}
    }	
}

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
	cure.duration = atoi(duration);
	// REPLACE THIS BY TIME OF TIMER
	cure.startingTime = time(NULL);

	if(cure.initVolume == 0 | cure.initDuration == 0)
	{
		printf("NULL VOLUME AND DURATION\n");
	}
	else
	{
		state_fn = RunCure;
	}

	printf("[*] CURE CREATED\n");
	return 1;
}


uint8_t ModifyCure(uint8_t * volume, uint8_t * duration)
{
	cure.initVolume = atoi(volume);
	cure.volume = atoi(volume);
	cure.initDuration =	atoi(duration);
	cure.duration = atoi(duration);
	// REPLACE THIS BY TIME OF TIMER 
	cure.startingTime = time(NULL);

	if(cure.initVolume == 0 | cure.initDuration == 0)
	{
		printf("NULL VOLUME AND DURATION\n");
	}
	else
	{
		state_fn = RunCure;
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
	cure.startingTime = 0;

	state_fn = Idle;

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
	uint32_t currentTime = 0;
	uint32_t cureDuration = 0;

	//SEND INFORMATION THROUGHT BLE HERE AKA cure.volume, cure.duration, cure.name
	printf("[*]CURE NAME : %s VOLUME : %d DURATION : %d\n", cure.name, cure.volume, cure.duration);	


	//REPLACE THIS BY GET TIME TIMER
	currentTime = time(NULL);

	cureDuration = GetDuration(currentTime);

	if(cureDuration >= cure.initDuration)
	{
		printf("END OF CURE\n");
		DeleteCure();
		return;
	}	

	if(UpdateCureValues(cureDuration))
	{
		printf("CURE UPDATED\n");
	}
}

uint32_t GetDuration(uint32_t currentTime)
{
	return (currentTime - cure.startingTime);
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
	if(UpdateRestingDuration(cureDuration))
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

uint8_t UpdateRestingDuration(uint32_t cureDuration)
{
	cure.duration = cure.initDuration - cureDuration;
	return 1;
}


