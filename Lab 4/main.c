#include "TM4C123GH6PM.h"

void delay()
{
  for(int i=0;i<100000;i++);
}

int main()
{
  SYSCTL_RCGCGPIO_R = 0x04; // Clock Gating Enable
  GPIO_PORTC_DIR_R = 0xF0;  // Data Direction
  GPIO_PORTC_DEN_R = 0xF0;  // Digital Enable
  volatile int i = 0;

  while (1)
  {
    GPIO_PORTC_DATA_R = i<<4;
    delay();
    i = (i+1)%16;
  }
}
