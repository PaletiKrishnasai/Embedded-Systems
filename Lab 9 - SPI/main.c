#include "tm4c123gh6pm.h"

/*

Master SPI Module - Module 0
----------------------------
PA2 - SSI0Clk
PA3 - SSI0Fss
PA4 - SSI0Rx
PA5 - SSI0Tx

Slave SPI Module - Module 2
---------------------------
PB4 - SSI2Clk
PB5 - SSI2Fss
PB6 - SSI2Rx
PB7 - SSI2Tx

*/

// Define function to send data
void sendChar(char data)
{
    // wait for transmission to complete
    while ((SSI0_SR_R & (1 << 0) == 0))
        ;
    SSI0_DR_R = data;
}

// Define function to recv a character from slave module
char recvChar(void)
{
    char c;
    while ((SSI2_SR_R & (1 << 2) == 0))
        ;
    c = SSI2_DR_R;

    return c;
}

// Each char stored in TX FIFO buffer and transfer it computer
void printChar(char c)
{
    while ((UART0_FR_R & (1 << 5)) != 0)
        ;
    UART0_DR_R = c;
}

// Print string function send char by char to FIFO txbuffert o comp
void printString(char *string)
{
    while (*string)
        printChar(*(string++));
}

void delayMs(int n)
{
    int i, j;
    for (i = 0; i < n; ++i)
        for (j = 0; j < 3180; ++j)
            ;
}

int decode(char msg)
{
    switch (msg)
    {
    case 'r':
        return 12;
    case 'g':
        return 10;
    case 'b':
        return 6;
    case 'm':
        return 4;
    case 'y':
        return 8;
    case 'c':
        return 2;
    case 'w':
        return 0;
    }
}

int main()
{
    /* Configure the SPI Master Module - Module 0 */
    SYSCTL_RCGCSSI_R |= (1 << 0);                                    // Enable clock on appr SSI Module
    SYSCTL_RCGCGPIO_R |= (1 << 0);                                   // Enable GPIO Clock on PORT A
    GPIO_PORTA_AFSEL_R |= (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5); // Select Alternate functions for pins PA2-PA5
    GPIO_PORTA_PCTL_R = 0x00222200;                                  // Set control register for pins PA2-PA5
    GPIO_PORTA_DEN_R |= (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);   // Enable digital on pins PA2-PA5
    GPIO_PORTA_PUR_R |= (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);   // Selecting pull up registers
    SSI0_CR1_R = 0;                                                  // Disable SSI Module while configuring
    SSI0_CC_R = 0;                                                   // Use the main system clock
    SSI0_CPSR_R = 64;                                                // Selecting divisor 64 for SSI Clock
    SSI0_CR0_R = 0x07;                                               // Set configuration of SSI - freescale mode, 8 bit data, steady low clock
    SSI0_CR1_R |= (1 << 1);                                          // Enable SPI

    /* Configure SPI Slave Module - Module 2 */
    SYSCTL_RCGCSSI_R |= (1 << 2);                                    // Enable clock on appr SSI Module
    SYSCTL_RCGCGPIO_R |= (1 << 1);                                   // Enable GPIO Clock on PORT B
    GPIO_PORTB_AFSEL_R |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7); // Select Alternate functions for pins PB4-PB7
    GPIO_PORTB_PCTL_R = 0x22220000;                                  // Set control register for pins PB4-PB7
    GPIO_PORTB_DEN_R |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);   // Enable digital on pins PP4-PB7
    GPIO_PORTB_PUR_R |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);   // Selecting pull up registers
    SSI2_CR1_R = 0;                                                  // Disable SSI Module while configuring
    SSI2_CR0_R = 0x07;                                               // Set configuration of SSI - freescale mode, 8 bit data, steady low clock
    SSI2_CR1_R |= (1 << 1) | (1 << 2);                               // Enable SPI and set slave

    /* Configure UART Communication to read */
    SYSCTL_RCGCUART_R |= (1 << 0);                // Enable clock to UART0
    GPIO_PORTA_AFSEL_R |= (1 << 0) | (1 << 1);    // Enable Alternate functions in PA0 and PA1
    GPIO_PORTA_PCTL_R |= 0x00000001 | 0x00000010; // Set CTL for pins PA0 and PA1 as UART
    GPIO_PORTA_DEN_R |= (1 << 0) | (1 << 1);      // Enable DEN in PA0 and PA1
    UART0_CTL_R &= ~(1 << 0);                     // Disable UART by clearing UARTEN in CTL
    UART0_IBRD_R = 104;                           // Set BRD - integer portion
    UART0_FBRD_R = 11;                            // Set BRD - fractional portion
    UART0_LCRH_R = (0x3 << 5) | (1 << 4);         // Set line control settings - 8 bit, no parity, 1-stop bit
    UART0_CTL_R = (1 << 0) | (1 << 8) | (1 << 9); // Enable UART by setting UARTEN bit

    SYSCTL_RCGCGPIO_R |= 0x20; // 0b100000 Enable Port F 
    // The RGB LEDs are connected to
    // PF1, PF2 and PF3
    // GPIO Direction (GPIODIR)
    GPIO_PORTF_DIR_R = 0xE; // 0b1110
    // GPIO Digital Enable (GPIODEN)
    GPIO_PORTF_DEN_R = 0xE; // 0b1110

    // Define control variables
    char c;
    char msg[] = {'r', 'g', 'b', 'm', 'y', 'c', 'w'};
    int idx = 0;

    while (1)
    {
        // Sent message
        printString("Sent Message from SPI0: ");
        printChar(c);
        printString("\n\r");

        // Send the next control message from SSI0 to SSI2
        sendChar(msg[idx]);

        // Delay for a while
        delayMs(100);

        // Read the message at SSI2
        c = recvChar();

        // Print the message recvd through UART on the terminal
        printString("Received Message from SPI2: ");
        printChar(c);
        printString("\n\r");

        // Light the LED with the color code correspoding to the recvd message
        GPIO_PORTF_DATA_R = decode(msg[idx]);

        // Delay for a while
        delayMs(2000);

        // Go onto the next control message index
        idx = (idx + 1) % 7;
    }

    return 0;
}