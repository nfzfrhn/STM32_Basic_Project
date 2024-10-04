/*
 * main.c
 *
 *  Created on: Sep 30, 2024
 *      Author: nafiz
 */

#include "main.h"
#include "string.h"

#define TRUE 	1
#define FALSE 	0

void SystemClockConfig(void);
void UART2_Init(void);
void Error_Handler(void);
uint8_t convert_to_capital(uint8_t);

UART_HandleTypeDef huart2;

char *user_data = "The application is running\r\n";
uint32_t count = 0;
uint8_t rcvd_data;
uint8_t data_buffer[100];
uint8_t reception_complete;

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	UART2_Init();

	uint16_t len_data = strlen(user_data);
	if(HAL_UART_Transmit(&huart2, (uint8_t*) user_data, len_data, HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}


	while(reception_complete != TRUE){
		HAL_UART_Receive_IT(&huart2, &rcvd_data, 1);
	}


	while(1);

	return 0;
}


//You only use when you need special clock setting
void SystemClockConfig(void)
{

}

// High level initialisation of peripheral
void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	if(HAL_UART_Init(&huart2) != HAL_OK){
		//There is a problem
		Error_Handler();
	}
}


uint8_t convert_to_capital(uint8_t data)
{
	if(data >= 'a' && data <= 'z')
	{
		//data = data - ('a' - 'A');
		data -= 32;
	}

	return data;
}


void Error_Handler(){
	while(1){

	}
}


//Receive Callback Function when UART Receive a complete data
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(rcvd_data == '\r')
	{
		reception_complete = TRUE;
		data_buffer[count++] = '\r';
		HAL_UART_Transmit(&huart2, data_buffer, count, HAL_MAX_DELAY);
	}
	else{
		data_buffer[count++] = rcvd_data;
	}

}

