#include "TM4C123GH6PM.h"

/* Read a switch and write it to the LED */
/* This program reads SW1 of Tiva LaunchPad and writes the inverse of the value to the green
   LED. SW1 is low when pressed (Normally High). LED is ON when high.
*/
 
int main(void)
{
    unsigned int read_val=0x00,write_val=0x00,prev_val=0xEE;
 
    SYSCTL_RCGC2_R = 0x00000020;;   /* enable clock to GPIOF */
 
    GPIO_PORTF_DIR_R = 0x0E;         /* set PORTF3,2,1 pin as output (LED) pins */
                                     /* and PORTF4 as input, SW1 is on PORTF4 */
    GPIO_PORTF_DEN_R = 0x1E;         /* set PORTF pins 4-1 as digital pins */
    GPIO_PORTF_PUR_R = 0x10;         /* enable pull up for pin 4 */
 
    while(1) 
    {
        read_val = GPIO_PORTF_DATA_R & 0x10;  /* read data from Pin4 */
        if(read_val & ~prev_val )
        {
          write_val += 0x02;
          write_val = write_val%0x10;
        }

        GPIO_PORTF_DATA_R = write_val;  /* put it on the green LED */
        prev_val = read_val;
    }
}