/* 
 * File:   eeprom.h
 * Author: C65471
 *
 * Created on December 14, 2022, 4:07 PM
 */

#ifndef EEPROM_H
#define	EEPROM_H

#include <stdint.h>
#include <stdbool.h>

#define WRITE_OPCODE        0x02
#define READ_OPCODE         0x03
#define WRDI_OPCODE         0x04 //Reset Write Enable
#define RDSR_OPCODE         0x05 //Read Status Register OPCODE
#define WRITE_ENABLE_OPCODE 0x06
#define WRBP_OPCODE         0x08 //Write Ready/Busy Poll
#define SRST_OPCODE         0x7C //Software device reset OPCODE

void eepromWriteEnable(void);
uint16_t readStatusRegister(void);
void eepromWriteByte (uint24_t address, uint8_t data);
void eepromWriteBlock (uint24_t address, uint8_t *block,int blockSize);
uint8_t eepromReadByte (uint24_t address);
void eepromReadBlock(uint24_t address, uint8_t *block, int blockSize);


#endif	/* EEPROM_H */

