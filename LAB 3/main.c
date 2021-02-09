#include "TM4C123GH6PM.h"
// Flashed on Teammates board . Ordered a board(delivery due)
/* Read a switch and write it to the LED */
/* Sw 1 -> red LED
   Sw 2 -> blue LED
   Sw1 and Sw2 -> green LED
*/
 
int main(void)
{
    SYSCTL_RCGC2_R = 0x00000020;   /* enable clock to GPIOF */
    
    GPIO_PORTF_LOCK_R = 0x4C4F434B;  /* unlock GPIOCR */
    GPIO_PORTF_CR_R = 0xff;          /*Set GPIO_CR to 0xff to unlock locked pins*/
 
    GPIO_PORTF_DIR_R = 0x0E;         /* set PORTF3,2,1 pin as output (LED) pins */
                                     /* and PORTF4,PORTF) as input, SW1 is on PORTF4, and SW2 is on PORTF0 */
    GPIO_PORTF_DEN_R = 0x1F;         /* set PORTF pins 4-0 as digital pins */
    GPIO_PORTF_PUR_R = 0x11;         /* enable pull up for pin 4,0 as they are connected to switches */

    volatile int data;    
    
    while(1) 
    {   
      switch (~GPIO_PORTF_DATA_R & 0x11)                     //Negate GPIO_PORTF_DATA_R value due to active low
      {
        case 0x11:                                           // both switches are pressed
          GPIO_PORTF_DATA_R =(1<<3);                              // 1<<1 == 0b10, turn on red led
          break;
          
        case 0x10:
          GPIO_PORTF_DATA_R =(1<<2);                              // 1<<3 == 0b1000 turn on the green led
          break;
        
        case 0x01:
          GPIO_PORTF_DATA_R =(1<<1);                              // 1<<2 == 0b100 turn on blue led
          break;
        
        default:
          GPIO_PORTF_DATA_R =0;                                   // If no switch pressed keep led off
      }
    }
}
