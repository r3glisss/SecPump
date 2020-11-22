#include <stdio.h>
#include <string.h>
#include <metal/machine/platform.h>

int UART_get_char(char * ch, int blocking)
{
  int busy;

  do {
    UART_probe_rx();
    busy = (rxbuf_head == rxbuf_tail);
  } while (blocking && busy);

  if (busy) {
    return -1;
  }

  *ch = rxbuf[rxbuf_tail++];
  rxbuf_tail &= UART_RXBUF_MASK;
  return 0;
}


int UART_read_n(char * buffer, int max_chars, char terminator, int blocking)
{
  int i;
  for (i=0; i<max_chars; i++) {
    if (0 != UART_get_char(&buffer[i], blocking)) {
        break;
    }
    if ((buffer[i] == terminator) || (buffer[i] == 0)) {
      break;
    }
  }
  return i;
}

int main(int argc, char **argv) {
    uint32_t i = 0;
    uint32_t *reg;

    char buffer[50];

    printf("uart-test start\n\r");

    reg = (uint32_t *)(METAL_SIFIVE_UART0_0_BASE_ADDRESS + METAL_SIFIVE_UART0_TXCTRL);
    printf("reg txctrl: %x, %p\n\r", *reg, reg);
    reg = (uint32_t *)(METAL_SIFIVE_UART0_0_BASE_ADDRESS + METAL_SIFIVE_UART0_RXCTRL);
    printf("reg rxctrl: %x, %p\n\r", *reg, reg);

    while (1){
      for (i = 0; i < sizeof(buffer); i++){
        reg = (uint32_t *)(METAL_SIFIVE_UART0_0_BASE_ADDRESS + METAL_SIFIVE_UART0_RXDATA);
        if (!(*reg & (1<<31))){
	        buffer[i] = (char)((*reg) & 0xff);
        } else {
          buffer[i] = 0x00;
          break;
        }
      }
      for (i = 0; buffer[i] != 0x00; i++) {
        printf("ret: %c (%x)\n\r", buffer[i], buffer[i]);
      }
    }
    memset(buffer, 0x00, sizeof(buffer));

    return 0;
}
