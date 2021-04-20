#include "tm4c123gh6pm.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#define RED 0x02U 
#define BLUE 0x04U
#define GREEN 0x08U
#define OFF 0x00U
#define MAGENTA 0x06U
#define YELLOW 0x0AU
#define CYAN 0x0CU
#define WHITE 0x0EU


void delay()
{
    int volatile counter=0;
    while(counter<1000000)
        ++counter;
}

void printChar_sender(char c)
{
    while((UART7_FR_R & (1<<5)) != 0);
    UART7_DR_R = c;
}

char readChar_sender(void) 
{
    char c;
    while((UART7_FR_R & (1<<4)) != 0);
    c = UART7_DR_R;
    return c;
}

void printChar_receiver(char c)
{
    while((UART1_FR_R & (1<<5)) != 0);
    UART1_DR_R = c;
}

char readChar_receiver(void) 
{
    char c;
    while((UART1_FR_R & (1<<4)) != 0);
    c = UART1_DR_R;
    return c;
}




void printString_sender(char * string)
{
    while(*string)
        printChar_sender(*(string++));
}


char* readString_sender(char delimiter)
{
    int stringSize = 0;
    char* string = (char*)calloc(10,sizeof(char));
    char c = readChar_sender();
    printChar_sender(c);
    while(c!=delimiter)
    {
        *(string+stringSize) = c;
        stringSize++;
        c = readChar_sender();
        printChar_sender(c); 
        }
    return string;
}



void printString_receiver(char * string)
{
    while(*string)
        printChar_receiver(*(string++));
}


char* readString_receiver(char delimiter)
{
    int stringSize = 0;
    char* string = (char*)calloc(10,sizeof(char));
    char c = readChar_receiver();
    printChar_receiver(c);
    while(c!=delimiter)
    {
        *(string+stringSize) = c;
        stringSize++;
        c = readChar_receiver();
        printChar_receiver(c); // display the character the user typed
        }
    return string;
}

  int main()
{


    SYSCTL_RCGCUART_R|= (1<<7) | (1<<1); //Enable the UART1, UART7 modules
    SYSCTL_RCGCGPIO_R |= (1<<5)|(1<<4) | (1 << 1); //enable clock to PORTE, B, F

    GPIO_PORTE_AFSEL_R=(1<<1)|(1<<0); //Enable PE0 and PE1 As Alternate Function PIN
    GPIO_PORTE_PCTL_R|=0x00000001|0x00000010; // make PE0 PE1 UART output pin
    GPIO_PORTE_DEN_R|=(1<<0)|(1<<1); //Enable digital on PE0 PE1

    GPIO_PORTB_AFSEL_R=(1<<1)|(1<<0); //Enable PB0 and PB1 As Alternate Function PIN
    GPIO_PORTB_PCTL_R|=0x00000001|0x00000010; // make PB0 PB1 UART output pin
    GPIO_PORTB_DEN_R|=(1<<0)|(1<<1); //Enable digital on PB0 PB1

    UART7_CTL_R&= ~(1<<0); //Disable the UART by clearing the UARTEN
    UART7_IBRD_R=104; // integer portion of the BRD
    UART7_FBRD_R=11; //fractional portion of the BRD
    UART7_LCRH_R=(0x3<<5)|(1<<4); // 8-bit, no parity, 1-stop bit
    UART7_CTL_R = (1<<0)|(1<<8)|(1<<9); //Enable the UART by setting the UARTEN bit

    UART1_CTL_R&= ~(1<<0); //Disable the UART by clearing the UARTEN
    UART1_IBRD_R=104; // integer portion of the BRD
    UART1_FBRD_R=11; //fractional portion of the BRD
    UART1_LCRH_R=(0x3<<5)|(1<<4); // 8-bit, no parity, 1-stop bit
    UART1_CTL_R = (1<<0)|(1<<8)|(1<<9); //Enable the UART by setting the UARTEN bit


    SYSCTL_RCGCGPIO_R |= 0x20; /* enable clock to PORTF */
    GPIO_PORTF_LOCK_R = 0x4C4F434B;  //unlock GPIO F
    GPIO_PORTF_CR_R = 0x1FU;

    GPIO_PORTF_DIR_R = 0x0EU; //set the direction of input to PF4 and PF1-3 as output
    GPIO_PORTF_DEN_R = 0x1FU; // enable IO of PF1 - 4.
    GPIO_PORTF_PUR_R = 0x11; //enable PF4 pullup
    GPIO_PORTF_AMSEL_R = 0x00;        // disable analog on PF
    GPIO_PORTF_PCTL_R = 0x00000000;// PF4 PCTL GPIO

    GPIO_PORTF_AFSEL_R = 0x00;   // disable alt funct on PF7-0



    int state1, state2, counter_sender = -1, sender = 1, counter_receiver = -1;

    int colours_sender[] = {RED, YELLOW, GREEN, BLUE, CYAN};

    int colours_receiver[] = {WHITE, MAGENTA, OFF};

    char sender_str, receiver_str;

    int sent = 0, replied = 0, question, answer;



    while(1)
    {
        state1 = (GPIO_PORTF_DATA_R & 0x10); // when SW1 is pressed
        state2 = (GPIO_PORTF_DATA_R & 0x01);// when SW2 is pressed

        if (!state1 || !state2)
        {
            delay();
            if (!state1)
            {
                if (sender)
                {
                    counter_sender = (counter_sender + 1) % 5;
                    GPIO_PORTF_DATA_R = colours_sender[counter_sender];
                }
                else
                {
                    counter_receiver = (counter_receiver + 1) % 3;
                    GPIO_PORTF_DATA_R = colours_receiver[counter_receiver];
                }

            }
            else if (!state2)
            {

                if (sender)
                {
                    sprintf(&sender_str, "%1d", (counter_sender));
                    printChar_sender(sender_str);
                    counter_sender = -1;
                    sent = 1;
                }
                else
                {
                    sprintf(&receiver_str, "%1d", (counter_receiver));
                    printChar_receiver(receiver_str);
                    counter_receiver = -1;
                    replied = 1;
                }
                sender = !sender;
                continue;

            }
        }

        if (sent)
        {

            question = (int)(readChar_receiver());
            GPIO_PORTF_DATA_R = colours_sender[question];
            delay();
            GPIO_PORTF_DATA_R = OFF;
            sent = 0;

        }
        else if (replied)
        {
             answer = (int)(readChar_sender());
            GPIO_PORTF_DATA_R = colours_receiver[answer];
            delay();
            GPIO_PORTF_DATA_R = OFF;
            replied = 0;

        }

    }
}

