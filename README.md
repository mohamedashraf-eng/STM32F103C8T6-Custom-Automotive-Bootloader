# STM32F103C8T6-Custom-Automotive-Bootloader
Custom automotive bootloader for practicing and learning
# About the project
This project is mainly focused on automotive bootloader which has more added functionality to the normal bootloaders to match the automotive world.
## Notes before use
Project is totally custom and still under developing and lack many of portability things.
# Bootloader 
## What is a bootloader?
The bootloader is the first code to run after power up or reset, and runs before any other software starts on a processor, including an operating system (OS), if an OS exists. For some embedded processors, bootloaders are part of a Board Support Package (BSP), which is used to start up and run the first silicon chips of an embedded MCU, processor, or System on a Chip (SoC). A bootloader (sometimes called a boot manager) is unique to the architecture of the embedded processor it runs on and includes some application specific aspects. A bootloader is necessary for starting processors at the lowest level before starting an operating system (e.g., a computer) or presenting a command line (e.g., an MCU). A bootloader can be sophisticated with lots of commands or simple with few commands; it can be fairly automatic and self-contained or be programmed to wait for a choice to be made, put in as a command, from a human. Commands can be set up to perform a number of modifications to the MCU environment if desired, or the bootloader might have few choices available and perform a self-contained start up for a set appliance or end-product.
## Bootloader Layered Architecture
![CBL_ARCH](https://l.top4top.io/p_2489c7bob1.png)
## About this bootloader
Our bootloader is mainly consists of one main thing, the relation between the host and the device. The host is the application that communicates with the bootloader, the device is the ECU which contains the bootloader application. The host is communicating with the device by pre defined commands that both of them know. Each pre defined command has one function to do.
## How the communication starts
The communication starts by connecting the ECU through a USB-TTL device to start communicate with it. The bootloader has two channels opened when in action, one for the actual channel host-device and another for the debug messages. The host application will communicate via USARTx channel. Simply the bootloader is designed to receive a sepcific defined packet.
![HOSTxBTL_Communiction](https://c.top4top.io/p_2489tqmvs1.png)

## Bootloader expected packet format
![HOSTxBTL_Communiction](https://e.top4top.io/p_2489q5cbu1.png)
The packet consists of 1 byte for the data length, 1 byte for the command, and n bytes for data, and 4 bytes for CRC.  
The Cyclic Redundancy Check (CRC) is used for secure and reliable communication.
