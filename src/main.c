#include "stm32f4xx.h"

#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>

void helloTask1(void*);
void helloTask2(void*);

void setupUsart();

//#define USE_SEMIHOSTING
#ifdef USE_SEMIHOSTING
extern void initialise_monitor_handles();
#endif USE_SEMIHOSTING

void sendMessage(char* msg);

int main(void)
{
#ifdef USE_SEMIHOSTING
	// semihosting for logging with printf
	initialise_monitor_handles();
	printf("this is hello sample code\n");
#endif USE_SEMIHOSTING

	// turn off the pll clock generator, prescaler etc
	// pull cpu clock from 100 MHz down to 16 MHz (default)
	// and recompute the SystemCoreClock
	// WHY: don't need 100 MHz for this simple UART project
	RCC_DeInit();
	SystemCoreClockUpdate();

	setupUsart();

	sendMessage("It's alive!\r");

	TaskHandle_t task1Handle = NULL;
	TaskHandle_t task2Handle = NULL;

	xTaskCreate(helloTask1, "Hello task 1", configMINIMAL_STACK_SIZE, "task 1", 2, &task1Handle);
	xTaskCreate(helloTask2, "Hello task 2", configMINIMAL_STACK_SIZE, "task 2", 2, &task2Handle);

	vTaskStartScheduler(); // never returns?

	for(;;);
}

uint8_t sendMessageAvailable = 1;

void helloTask1(void* parameter) {
	for(;;) {
#ifdef USE_SEMIHOSTING
		printf("Hello from task 1\n");
#endif USE_SEMIHOSTING

		if (sendMessageAvailable) {
			sendMessageAvailable = 0;
			sendMessage("Task 1\r");
			sendMessageAvailable = 1;
			taskYIELD();
		}
	}
}

void helloTask2(void* parameter) {
	for(;;) {
#ifdef USE_SEMIHOSTING
		printf("Hello from task 2\n");
#endif USE_SEMIHOSTING

		if (sendMessageAvailable) {
			sendMessageAvailable = 0;
			sendMessage("Task 2\r");
			sendMessageAvailable = 1;
			taskYIELD();
		}
	}
}

USART_TypeDef* usartPeripheral = USART3;

void setupUsart() {
	// On Nucleo STMF413ZH, STLink bridges VCP to USART3 via PD8 and PD9
	// PD8 USART3 TX
	// PD9 USART3 RX
	// PD8, PD9 are on APB1, AHB1, GPIOD
	// see User Manual and Datasheet for details

	// enable peripheral and gpio clocks
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// configure GPIO pins
	GPIO_InitTypeDef gpioPins;
	gpioPins.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpioPins.GPIO_Mode = GPIO_Mode_AF;
	gpioPins.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &gpioPins);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);

	// configure and enable UART
	USART_InitTypeDef usart;
	USART_StructInit(&usart); // except for baud rate, the defaults look fine
	usart.USART_BaudRate = 115200;
	USART_Init(usartPeripheral, &usart);
	USART_Cmd(usartPeripheral, ENABLE);
}

void sendMessage(char* msg) {
	for ( ; *msg; msg++) {
		while (USART_GetFlagStatus(usartPeripheral, USART_FLAG_TXE) == RESET);
		USART_SendData(usartPeripheral, *msg);
	}
}
