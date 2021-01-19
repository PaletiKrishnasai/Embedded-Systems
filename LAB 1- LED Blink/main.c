#define GATING_REG (*(unsigned int *) 0x400FE608)
#define DIR_REG (*(unsigned int*) 0x40025400)               // Port F APB
#define DIG_FUNC_REG (*(unsigned int*)0x4002551C)    // Port F APB
#define DATA_REG (*(unsigned int*)0x40025038)            //Look at addr [9:2] need to set bits 3,4,5 and add offset to 0x40025000 to enable ony required pins

void delayMS(int n)
{
    int i, j;

    for (i = 0; i < n; i++)
        for(j = 0; j < 1000000; j++) //Simulation freq is 10Mhz and the delay function has 10 instructions per run (as seen in disassembly) hence 1000000 runs of loop needed
        {
        }

}

int main(void)
{

    GATING_REG = 0x00000020;        // bits 31:6 =0 bit 5= 1 bits 4:0 = 0  in hex = 0x 00000020
    
    DIR_REG = 0x0000000E;        //bits 31:8 = 0, bits 7:4 also = 0, bits 3:1 = 1 , bit 0 = 0, port 0 is unused as it is a special GPIO PIN which needs to be unlocked to be used
    
    DIG_FUNC_REG = 0x0000000E;  //  
    
    while(1)
    {
      
         DATA_REG = 0x00000002; // red
          
          delayMS(1); // keep red on for 1 sec 
          
          DATA_REG = 0x00000000; // switching it off
          
          DATA_REG = 0x00000004; // blue 
          
          delayMS(2);  // keep blue on for 2 secs

          DATA_REG = 0x00000000; // switching it off
          DATA_REG = 0x00000008; // green
          
          delayMS(3); // keep green on for 3 secs 
           DATA_REG = 0x00000000; // switching it off
    }
    return 0;
    
 }


