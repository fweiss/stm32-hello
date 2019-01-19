# STM32 Hello
Following the Udemy basic course

## Udemy
[Mastering RTOS: Hands-on FreeRTOS and STM32Fx with Debugging](https://www.udemy.com/mastering-rtos-hands-on-with-freertos-arduino-and-stm32fx)

This course normally costs $89.99. There were some promotions at the beginning of 2019.

## Hardware
ST Microelectronics Nucleo STM32F413ZH.

## Software
See Udemy course.
Windows 7 64-bit, FreeRTOS Workbench, Putty

## Learnings

- how to get free tools and software for STM32 firmware development
- how to set up the STLink included with Nucleo products
- how to set up the toolchain with libraries and Eclipse settings
- how to set up system clock and access documentation
- how to create simple RTOS tasks
- how to use semi-hosting for debug print statements
- how to setup a UART peripheral for logging and access documentation
- how to refer to STM32F4xx peripheral driver source code for API documentation
- how to synchronize two tasks with taskYIELD()

## UART configuration
Because of the flexibility of Cortex M4 GPIO pins and alternate functions, setting up a UART takes some work.
The following need to be considered:

- which UART to use, in the case of using the VCP via STLink, the choice was USART3
- which port pins the USART uses and which is tx/rx
- which GPIO port the pins are on
- which APB (1 or 2) the USART clock enable
- which AHP (usually 1) the GPIO clock enable
- which AF function to assign for the pins

The following documents supplied the information:
- User Manual - 6.9 STLink info
- Datasheet - 2.1 Figure 4 Block Diagram
- Datasheet - 4.9 Table 12 Alternate Functions

## Windows serial
At first Putty wouldn't connect when clicking the Open button in the Serial tab.
Needed to correct the following:

- in Device Manager, open the COM properties and make sure the Port Settings are correct
- in Putty, in the Session tab, make sure Connection type Serial is selected
