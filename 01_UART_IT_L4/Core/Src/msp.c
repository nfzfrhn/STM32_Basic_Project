/*
 * msp.c
 *
 *  Created on: Sep 30, 2024
 *      Author: nafiz
 */

#include "main.h"


void HAL_MspInit(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_MspInit could be implemented in the user file
   */
	//Here we will do low level processor specific inits
	//1. Setup the priority grouping of the arm cortex mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//2. Enable the required system exceptions of the arm cortex mx processor
	//Bit 18: USGFAULTENA = Usage fault system exception
	//Bit 17: BUSFAULTENA = Bus fault system exception
	//Bit 16: MEMFAULTENA = Memory fault system exception
	SCB->SHCSR = 0x7 << 16;

	//3. Configure the priority for the system exceptions
	// Lower number mean higher priority
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}

//When the peripheral is not working as expected, then this function should be the first debug spot
//Most people do mistakes in configuring the low level peripheral settings
//Low level initialization of peripheral
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart;
	//We are going to do the low level initialization of the UART2 peripheral

	//1. Enable the clock for the USART2 peripheral as well as for GPIOA peripheral
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//2. Do the pin muxing configurations
	gpio_uart.Pin = GPIO_PIN_2;				//UART2 Tx
	gpio_uart.Mode = GPIO_MODE_AF_PP;
	gpio_uart.Pull = GPIO_PULLUP;
	gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_uart.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &gpio_uart);

	gpio_uart.Pin = GPIO_PIN_3;				//UART2 Rx
	HAL_GPIO_Init(GPIOA, &gpio_uart);

	//3. Enable the IRQ and setup the priority (NVIC settings)
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
}


