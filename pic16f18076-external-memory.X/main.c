 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
*/

/*
© [2022] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
#include "mcc_generated_files/system/system.h"
#include "eeprom.h"

//#define DEBUG 0b1 //If DEBUG == 0b1 Debugging is enabled and actions will be output in UART (Baud 9600)

void writeReadByteTest(uint24_t startingAddress);
void writeReadBlockTest(uint24_t startingAddress);


/*
    Main application
*/

int main(void)
{
    SYSTEM_Initialize();
    SPI1_Open(0);
    HLD_SetHigh();
    WP_SetHigh();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts 
    // Use the following macros to: 

    // Enable the Global Interrupts 
    //INTERRUPT_GlobalInterruptEnable(); 

    // Disable the Global Interrupts 
    //INTERRUPT_GlobalInterruptDisable(); 

    // Enable the Peripheral Interrupts 
    //INTERRUPT_PeripheralInterruptEnable(); 

    // Disable the Peripheral Interrupts 
    //INTERRUPT_PeripheralInterruptDisable(); 
    #if DEBUG == 0b1
        printf("Successfully Initialized\r\n");
    #endif
    
    uint24_t startingAddress = 0x000100;
    int blockSize = 10;
    CS1_SetHigh();
    while(1)
    {
        writeReadByteTest(startingAddress);
        writeReadBlockTest(startingAddress);
        __delay_ms(5000);
    }    
}

void writeReadByteTest(uint24_t startingAddress)
{
    printf("\nInitiating Write/Read Byte Test\r\n");
    
    uint24_t address;
    uint8_t readValue;
    for(uint8_t i = 0; i < 3; i++)
        {
            address = startingAddress + (uint24_t)i;
            printf("Writing %x to address %x\r\n",i , address);
            eepromWriteByte(startingAddress + (uint24_t)i, i);
            readValue = eepromReadByte(address);
            if(i == readValue)
            {
                printf("Success, Read matches Write!\r\n");
            }
            else
            {
                printf("Error: Write was %u and Read was %u\r\n", i, readValue);
            }
        
        }
}

void writeReadBlockTest(uint24_t startingAddress)
{
    printf("\nInitiating Write/Read Block Test\r\n");
 
    uint8_t writeData[10]; 
    uint8_t readData[10];
    uint8_t verifyWrite[10];
    
    for(int i = 9; i >= 0; i--)
    {
        writeData[i] = i;
        verifyWrite[i] = i;
    }
    
    #if DEBUG == 0b1
        printf("Writing Block\r\n");
        for(int i = 0; i < 10; i++)
        {
            printf("writeData[%u] = %u\r\n", i, writeData[i]);
        }
    #endif

    eepromWriteBlock(startingAddress, &writeData[0], 10);
    
    #if DEBUG == 0b1
        printf("Reading Block\r\n");
    #endif 
        
    eepromReadBlock(startingAddress, &readData[0], 10);
    
    #if DEBUG == 0b1
        printf("Comparing Values...\r\n");
    #endif 
        
    for(int i = 0; i < 10; i++)
    {
        if( verifyWrite[i] != readData[i])
        {
            printf("Error: Read Value does not match write value at index %u\r\n", i);
            printf("Write Value: %x != Read Value: %x\r\n", writeData[i], readData[i]);
            return;
        }
    }
        printf("Test Successful: All Values between read and write match!\r\n");
    return;   
}