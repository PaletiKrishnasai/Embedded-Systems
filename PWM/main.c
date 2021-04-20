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

void delayMs(int n); 

int main(void)
{ 
  // PWM
  int x = 15999;
  // Enable Peripheral Clocks  
  SYSCTL_RCGCPWM_R |= 2;       // enable clock to PWM1  
  SYSCTL_RCGCGPIO_R |= 0x20;   // enable clock to PORTF  
  SYSCTL_RCC_R &= ~0x00100000; // no pre-divide for PWM clock
  // Enable port PF2 for PWM1 M1PWM7  
  GPIO_PORTF_AFSEL_R = 0x0E;   // PF2 uses alternate function  
  GPIO_PORTF_PCTL_R |= 0x000005550; 
  GPIO_PORTF_DEN_R = 0x02;     // pin digital 
  PWM1_2_CTL_R = 0;            // stop counter  
  PWM1_2_GENB_R = 0x0000008C;  // M1PWM6 output set when reload, 
  PWM1_3_CTL_R = 0;            // stop counter  
  PWM1_3_GENA_R = 0x0000008C;  // M1PWM6 output set when reload,
  PWM1_3_GENB_R = 0x0000008C;  // M1PWM6 output set when reload, 
  // clear when match PWMCMPA  
  PWM1_2_LOAD_R = 16000;       // set load value for 1kHz (16MHz/16000)  
  PWM1_2_CMPA_R = 15999;       // set duty cycle to min  
  PWM1_2_CTL_R = 1;            // start timer  
  PWM1_3_LOAD_R = 16000;       // set load value for 1kHz (16MHz/16000)  
  PWM1_3_CMPA_R = 15999;       // set duty cycle to min  
  PWM1_3_CTL_R = 1;            // start timer  
  PWM1_ENABLE_R = 0xE0;        // start PWM1 ch7 
  
  int Colors[] = {RED, GREEN, BLUE};
  int ColorIndex = 0;
  
  for(;;) 
  { 
    GPIO_PORTF_DEN_R = Colors[ColorIndex];       // pin digital 
    ColorIndex = (ColorIndex + 1) % (sizeof(Colors) / sizeof(Colors[0]));
    x = x - 50; 
    if (x <= 0) 
      for(;;)
      { 
        x = x + 50; 
        if (x>=15999) 
          break; 
        PWM1_2_CMPA_R = x; 
        PWM1_3_CMPA_R = x; 
        delayMs(5);
      } 
    PWM1_2_CMPA_R = x; 
    PWM1_3_CMPA_R = x; 
    delayMs(5);
  }
} 

// delay n milliseconds (16 MHz CPU clock)  
void delayMs(int n)
{ 
  int i, j; 
  for(i = 0 ; i < n; i++) 
    for(j = 0; j < 3180; j++) 
    {}  // do nothing for 1 ms  
}