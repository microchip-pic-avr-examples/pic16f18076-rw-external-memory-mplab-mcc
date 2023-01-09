#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#include "eeprom.h"
#include "mcc_generated_files/system/system.h"

/* First step of sending a write command to the external EEPROM after 
 * initialization. This command MUST be sent before attempting a write.*/
void eepromWriteEnable(void) 
{
    #if DEBUG == 0b1
        printf("Enabling Write\r\n");
    #endif
    CS1_SetLow();
    SPI1_ByteExchange(WRITE_ENABLE_OPCODE);
    CS1_SetHigh();
    #if DEBUG == 0b1
        printf("Sent WREN\r\n");
    #endif
    while(!(readStatusRegister() & 0x0002))
    {
        __delay_ms(1);
        #if DEBUG == 0b1
            printf("Write Enable: Polling WIP/WEL Bit\nRDSR Register is %x\r\n", readStatusRegister());
        #endif
    }
    #if DEBUG == 0b1
        printf("WEL bit set\r\n");
    #endif 
    return;
}

/*The status register keeps track of the status of the EEPROM module. 
 * This function serves to check that WIP and WEL status bits are cleared and 
 * thus the EEPROM is ready for another command after a write. This step can be 
 * replaced with a 5ms timer instead, however this method will be faster and 
 * more reliable.*/
uint16_t readStatusRegister(void)
{
    #if DEBUG == 0b1
        printf("Reading Status Register\r\n");
    #endif
    
    uint16_t RDSR = 0x0000;
    CS1_SetLow();
    SPI1_ByteExchange(RDSR_OPCODE); //Send Read Status Register OPCODE
    RDSR = (uint16_t)SPI1_ByteExchange(0xff); //Read back incoming RDSR Byte 0
    RDSR |= ((uint16_t)SPI1_ByteExchange(0xff)) << 8; // Read back incoming RDSR Byte 1
    CS1_SetHigh();    
    
    #if DEBUG == 0b1
        printf("RDSR: %x\r\n", RDSR);
    #endif

    return RDSR;
}

/* This command writes the desired byte to the EEPROM. The data structure 
 * consisting of: a write opcode, a high then low address byte, and lastly the 
 * data.*/
void eepromWriteByte (uint24_t address, uint8_t data)
{
    eepromWriteEnable();
    #if DEBUG == 0b1 
        printf("Writing Byte %x to address: %x\r\n", data, address);
    #endif

    uint8_t block[5];
    block[0] = WRITE_OPCODE;
    block[1] = address >> 16;
    block[2] = address >> 8;
    block[3] = address;
    block[4] = data;

    #if DEBUG == 0b1
        printf("block array:\r\n");
        for(int i = 4; i >= 0; --i)
        {
            printf("Bit %d: %x\r\n", i, block[i]);
        }
    #endif    
    CS1_SetLow();
    SPI1_BufferExchange(&block[0], 5);
    CS1_SetHigh();
    
    while((bool)(readStatusRegister() & 0x3))
    {
        __delay_ms(1);
        #if DEBUG == 0b1
            printf("Write Byte: Polling WIP/WEL Bit\nRDSR Register is %x\r\n", readStatusRegister());
        #endif
    }
    #if DEBUG == 0b1
        printf("WIP and WEL bit set\r\n");
    #endif        
    return; 
}

/* Similar to Write byte, but writing a block of multiple bytes. 
 * 
 * IMPORTANT NOTE: Once an EEPROM page is full a Write Cycle must be initiated 
 * before writing more data. In this example an EEPROM page is 64 bytes, 
 * meaning the most bytes the user can write in one command is 64. 
 * 
 * Additionally if the user starts with an address in the middle of a page, the 
 * write will wrap around that page once it reaches the end (i.e. after 0xff 
 * the EEPROM will write to 0x00 and continue on).*/ 
void eepromWriteBlock (uint24_t address, uint8_t *block,int blockSize)
{
    /* Note: After writing 256 consecutive Bytes the EEPROM will wrap around 
     * it's current page and overwrite existing data.*/
    if(blockSize > 256) 
    {
        printf("Error max block size for write is 256 bytes.");
        return;
    }
    uint8_t addressBlock[4];
    addressBlock[0] = WRITE_OPCODE;
    addressBlock[1] = address >> 16;
    addressBlock[2] = address >> 8;
    addressBlock[3] = address;
    
    #if DEBUG == 0b1
        printf("Writing Block\r\n");
    #endif

    eepromWriteEnable();
    CS1_SetLow();
    SPI1_BufferExchange(&addressBlock[0], 4);
    SPI1_BufferExchange(&block[0], blockSize);
    CS1_SetHigh();
    
    while((bool)(readStatusRegister() & 0x3))
    {
        __delay_ms(1);
        #if DEBUG == 0b1
            printf("Write Block: Polling WIP/WEL Bit\nRDSR Register is %x\r\n", readStatusRegister());
        #endif
    }
    #if DEBUG == 0b1
        printf("WIP and WEL bit set\r\n");
    #endif        
    return; 
}

/* After the write cycle, the Status register should be checked for a completed 
 * write cycle*/

/*Read command for a single byte*/
uint8_t eepromReadByte (uint24_t address)
{
    #if DEBUG == 0b1
        printf("Reading Byte\r\n");
    #endif    
        
    uint8_t data;
    uint8_t _address[4];
    _address[0] = READ_OPCODE;
    _address[1] = (uint8_t)(address >> 16);
    _address[2] = (uint8_t)(address >> 8);
    _address[3] = (uint8_t)(address);
    
    #if DEBUG == 0b1
        printf("Read Opcode: %x\r\n", _address[0]);
        printf("Read Address is: %x, %x, %x\r\n", _address[1], _address[2], _address[3]);
    #endif
    
    CS1_SetLow();
    SPI1_BufferExchange(&_address[0], 4); //Send Read opcode and address
    data = SPI1_ByteExchange(0xff); //Receive Byte
    CS1_SetHigh();
    
    #if DEBUG == 0b1
        printf("Data Read, read value: %x\r\n", data);
    #endif
    
    return data;
}

/* Multiple bytes may be read by continuing the clock signal*/
void eepromReadBlock(uint24_t startingAddress, uint8_t *block, int blockSize)
{
    #if DEBUG == 0b1
        printf("Reading Block\r\n");
    #endif

    //uint8_t data;
    uint8_t _address[4];
    _address[0] = READ_OPCODE;
    _address[1] = (uint8_t)(startingAddress >> 16);
    _address[2] = (uint8_t)(startingAddress >> 8);
    _address[3] = (uint8_t)(startingAddress);
    
    #if DEBUG == 0b1
        printf("Read Opcode: %x\r\n", _address[0]);
        printf("Read Address is: %x, %x, %x\r\n", _address[1], _address[2], _address[3]);
    #endif
    
    CS1_SetLow();
    SPI1_BufferExchange(&_address[0], 4); //Send Read opcode and address
    SPI1_BufferExchange(&block[0], blockSize);
    CS1_SetHigh();
    return;
}
