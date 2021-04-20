#include <stdint.h> 
#include "tm4c123gh6pm.h"

#define BLACK 0x00U
#define RED 0x02U
#define BLUE 0x04U
#define GREEN 0x08U
#define PURPLE 0x06U
#define YELLOW 0x0AU
#define CYAN 0x0CU
#define WHITE 0x0EU
#define PressedCode1 0x00U
#define PressedCode2 0x00U
#define LeftButton *((unsigned int*)0x40025040)
#define RightButton *((unsigned int*)0x40025004)

// delay n milliseconds (16 MHz CPU clock)
void delayMs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 3180; j++)
            {}  // do nothing for 1 ms
}

// delay n microseconds (16 MHz CPU clock)
void delayUs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 3; j++)
            {}  // do nothing for 1 us
}

int main()
{
  
  
    // Motor Control
  
  // Button
  *((unsigned int*)0x400FE608)=0x20U;
  *((unsigned int*)0x40025400)=0x0EU;
  *((unsigned int*)0x4002551C)=0x1FU;
  *((unsigned int*)0x40025524)=0xFFU;
  *((unsigned int*)0x40025510)=0x11U;
  *((unsigned int*)0x40025520)=0x4C4F434BU;

    int Values[] = {0, 30, 60, 100};
    int counter = 1;
    //PWM1_3_CMPB_R =40*Values[0];
    int buttonPressed_Switch = 0;
    int buttonPressed_Select = 0;
    int clockwise = 1;

    int pw = 0;

    SYSCTL_RCGCPWM_R |= 0x02;        // enable clock to PWM1
    SYSCTL_RCGCGPIO_R |= 0x20;       // enable clock to GPIOF
    SYSCTL_RCGCGPIO_R |= 0x02;       // enable clock to GPIOB

    SYSCTL_RCC_R &= ~0x00100000;     // use system clock for PWM
    PWM1_INVERT_R |= 0x80;           // positive pulse
    PWM1_3_CTL_R = 0;               // disable PWM1_3 during configuration
    PWM1_3_GENB_R = 0x0000080C;     // output high when load and low when match
    PWM1_3_LOAD_R = 3999;           // 4 kHz
    PWM1_3_CTL_R = 1;               // enable PWM1_3
    PWM1_ENABLE_R |= 0x80;           // enable PWM1

    GPIO_PORTF_DIR_R |= 0x08;             // set PORTF 3 pins as output (LED) pin
    GPIO_PORTF_DEN_R |= 0x08;             // set PORTF 3 pins as digital pins
    GPIO_PORTF_AFSEL_R |= 0x08;           // enable alternate function
    GPIO_PORTF_PCTL_R &= ~0x0000F000;     // clear PORTF 3 alternate function
    GPIO_PORTF_PCTL_R |= 0x00005000;      // set PORTF 3 alternate funtion to PWM

    GPIO_PORTB_DEN_R |= 0x0C;             // PORTB 3 as digital pins
    GPIO_PORTB_DIR_R |= 0x0C;             // set PORTB 3 as output
    GPIO_PORTB_DATA_R |= 0x08;            // enable PORTB 3

    while(1)
    {
      
	// Main Code
	if (buttonPressed_Select == 0 && LeftButton == PressedCode1)
        {
              buttonPressed_Select = 1;
              PWM1_3_CMPB_R = 75*Values[counter];
              counter = (counter + 1)%(sizeof(Values)/sizeof(Values[0]));
        }
        else if (buttonPressed_Select == 1 && LeftButton != PressedCode1)
        {
            buttonPressed_Select = 0;
        }
	if (buttonPressed_Switch == 0 && RightButton == PressedCode2)
        {
	    buttonPressed_Switch = 1;
	    if (clockwise == 1)
	    {
		GPIO_PORTB_DATA_R &= ~0x04;
		GPIO_PORTB_DATA_R |= 0x08;
	    }
	    else 
	    {
		GPIO_PORTB_DATA_R &= ~0x08;
		GPIO_PORTB_DATA_R |= 0x04;
	    }
	    
	    clockwise = 1 - clockwise;
        }
        else if (buttonPressed_Switch == 1 && RightButton != PressedCode2)
        {
            buttonPressed_Switch = 0;
        }
	
    }
}
