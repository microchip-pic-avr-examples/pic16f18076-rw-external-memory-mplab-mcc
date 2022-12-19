<!-- Please do not change this logo with link -->

[![MCHP](images/microchip.png)](https://www.microchip.com)

# How to Write and Read to an External EEPROM with a PIC16F18076

<!-- This is where the introduction to the example goes, including mentioning the peripherals used -->
This example will walk the user through how to read and write both a single byte and array of memory from a PIC16F18076 microcontroller on a curiosity nano development board to an external 25CSM04 EEPROM device on a Mikroe EEPROM 7 click board using MPLAB Code Configurator and the MSSP SPI peripheral. MCC will also be used to setup the microcontroller's pins and basic configurations while the UART peripheral will be configured for debugging and outputing the result of the example's included test of the write and read process.

## Related Documentation

<!-- Any information about an application note or tech brief can be linked here. Use unbreakable links!
     In addition a link to the device family landing page and relevant peripheral pages as well:
     - [AN3381 - Brushless DC Fan Speed Control Using Temperature Input and Tachometer Feedback](https://microchip.com/00003381/)
     - [PIC18F-Q10 Family Product Page](https://www.microchip.com/design-centers/8-bit/pic-mcus/device-selection/pic18f-q10-product-family) -->

- TODO: PIC16F18076 Product Page
- TODO: PIC16F18076 Datasheet(TODO: ?)
- TODO: 25CSM04 Product Page
- TODO: 25CSM04 Datasheet

## Software Used

<!-- All software used in this example must be listed here. Use unbreakable links!
     - MPLAB® X IDE 5.30 or newer [(microchip.com/mplab/mplab-x-ide)](http://www.microchip.com/mplab/mplab-x-ide)
     - MPLAB® XC8 2.10 or a newer compiler [(microchip.com/mplab/compilers)](http://www.microchip.com/mplab/compilers)
     - MPLAB® Code Configurator (MCC) 3.95.0 or newer [(microchip.com/mplab/mplab-code-configurator)](https://www.microchip.com/mplab/mplab-code-configurator)
     - MPLAB® Code Configurator (MCC) Device Libraries PIC10 / PIC12 / PIC16 / PIC18 MCUs [(microchip.com/mplab/mplab-code-configurator)](https://www.microchip.com/mplab/mplab-code-configurator)
     - Microchip PIC18F-Q Series Device Support (1.4.109) or newer [(packs.download.microchip.com/)](https://packs.download.microchip.com/) -->

- MPLAB® X IDE 6.0.5 or newer [(MPLAB® X IDE 6.0)](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-x-ide?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_MMTCha_MPAE_Examples&utm_content=pic16f18076-read-write-external-memory-github)
- MPLAB® XC8 2.40.0 or newer compiler [(MPLAB® XC8 2.40)](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_MMTCha_MPAE_Examples&utm_content=pic16f18076-read-write-external-memory-github)
- MPLAB® Code Configurator (MCC) TODO: MCC Version or newer [(microchip.com/mplab/mplab-code-configurator)](https://www.microchip.com/mplab/mplab-code-configurator)
- MPLAB® Code Configurator (MCC) Device Libraries PIC10 / PIC12 / PIC16 / PIC18 MCUs [(microchip.com/mplab/mplab-code-configurator)](https://www.microchip.com/mplab/mplab-code-configurator)
- Microchip TODO: Device Library Series Device Support (TODO: Version Number) or newer [(packs.download.microchip.com/)](https://packs.download.microchip.com/)

## Hardware Used

<!-- All hardware used in this example must be listed here. Use unbreakable links!
     - PIC18F47Q10 Curiosity Nano [(DM182029)](https://www.microchip.com/Developmenttools/ProductDetails/DM182029)
     - Curiosity Nano Base for Click boards™ [(AC164162)](https://www.microchip.com/Developmenttools/ProductDetails/AC164162)
     - POT Click board™ [(MIKROE-3402)](https://www.mikroe.com/pot-click) -->

     - PIC16F18076 Curiosity Nano TODO: UTM
     - Curiosity Nano Base for Click boards™ TODO: UTM
     - EEPROM 7 Click board™ TODO: LINK
     - Logic Analyzer such as Salae Logic 8™ or similar (Optional but highly Recommended) TODO: Link

## Setup

<!-- Explain how to connect hardware and set up software. Depending on complexity, step-by-step instructions and/or tables and/or images can be used -->
### Hardware Setup
The PIC16F18076 curiosity nano development board is connected to the curiosity nano base board with the Mikroe EEPROM 7 click board placed in mikro BUS 1.

### MPLAB Code Configurator Setup

### Configuration Bits
TODO: Config Bits Screenshots
Set External Oscillator Selection bits to "Oscillator Not Enabled" and "Reset Oscillator Selection bits" to "HFINTOSC (32 MHz)".

### Clock Control
TODO: Clock Control Screenshot
Set the "Current Oscillator Source Select" to "HFINTOSC_32MHz" and the "HF Internal Clock" to "32_MHz".

### Pins
Configure Pins to the table below:
TODO: PIN Table
TODO: Screenshot of Pin Grid View
In the "Pins" menu the user should rename pins RB3, RD3, and RD7 to "CS1", "HLD", and "WP" respectively for the function pin names to match the function of the corresponding pins. 
TODO: Screenshot of Pins Menu
IMPORTANT NOTE: The user should also disable the Slew Rate for pins SPI Pins: SCK1, SDI1, SDO1 (RB2, RB1, RB0 respectively). Leaving the Slew Rate limit active on the MSSP SPI pins can cause issues with higher MSSP clock frequencies.

### SPI MSSP1
TODO: MSSP1 Screenshot
The MSSP SPI should be configured to "Host Mode" in "SPI Mode" should be set to "SPI Mode 0", with the input data sampled in the middle. The "Clock Source Selection" should be set to one of the "FOSC" setting, this example uses "FOSC/4" however "FOSC/16" and "FOSC/64" will work too.

### UART2
TODO: UART2 Screenshot
The user may set whatever baud rate they choose. A baud rate of 115200 will be used for this example.
Note: "Redirect Printf to UART" should be enabled for debugging and displaying test results for this demo, however is not necessary if the user only needs to write and read data to the external EEPROM.

### Application Code
Note: For simplicity and readability, the EEPROM OPCODES are defined as shown below and will be identified by their defined names throughout the code.
TODO: DEFINE code snippet

#### eepromWriteEnable()
TODO: eepromWriteEnable code snippet
Before any write can be initiated with the 25CSM04 EEPROM a Write Enable Opcode must be sent over the SDO line to the device. This function sends that opcode then uses the eepromReadStatusRegister() function to check the EEPROM's Status Register for a successful write enable. This function is automatically called in both write functions in this demo.

#### eepromReadStatusRegister()
TODO: eepromReadStatusRegister code snippet
This function sends the RDSR_OPCODE through the MSSP and returns the status register. It's polled automatically in eepromWriteEnable() to check for a successful "Write Enable" operation and polled after every "Write" operation for the completion of that operation. This instruction may be bypassed in favor of a 5ms delay between write operations, however polling the Status Register will always be either quicker or equal to the 5ms delay.  

#### eepromWriteByte()
TODO

#### eepromWriteBlock()
TODO

#### eepromReadByte()
TODO

#### eepromReadBlock()
TODO

#### writeReadByteTest()
TODO

#### writeReadBlockTest()
TODO
TODO: Write vs READ difference warning for data verification

#### main()
TODO

## Operation

<!-- Explain how to operate the example. Depending on complexity, step-by-step instructions and/or tables and/or images can be used -->
The demonstration will automatically run on programming and output the results of it's own internal test out via UART and can be viewed in MPLAB Data Visualizer, or any terminal program configured for a baud rate of 115200. The user may include more detailed information about the exact details of the write and read process by defining DEBUG = 0b1 in the main.c file.

TODO: Include screenshot of Datavisualizer terminal with and without DEBUG defined

## Summary

This example demonstrate basic read and write functionality using a PIC16F18076 microcontroler and an external 25CSM04 EEPROM device on a Mikroe EEPROM 7 click board.
