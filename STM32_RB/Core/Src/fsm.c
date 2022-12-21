/*
 * fsm.c
 *
 *  Created on: Nov 20, 2022
 *      Author: PC
 */

#include "fsm.h"
#include "timer.h"

uint8_t buffer[MAX_BUFFER_SIZE];
int index_buffer = 0;
int receive_flag = 0;
uint8_t temp = 0;
int RST_flag = 0;
int OK_flag = 0;
int BUTTON_flag = 0;

#define IDLE 0
#define CMD_WAITING 1
#define CMD_WAITING_O 2
#define CMD_WAITING_OK 3
#define CMD_WAITING_R 4
#define CMD_WAITING_RS 5
#define CMD_WAITING_RST 6
#define CMD_WAITING_B 7
#define CMD_WAITING_B_num 8
#define CMD_WAITING_B_num_CONFIRM 9
#define CMD_B_num_VALUE 10
#define CMD_B_num_VALUE_CONFIRM 11

int cmdParserStatus = IDLE;
int ADC1_value = -1;
int ADC2_value = -1;
int index = 0;

enum IS_BUTTON {NON, LIGHT, PUMP};
enum IS_BUTTON isButton = NON;

int lightButton = 0;
int pumpButton = 0;

void command_parser_fsm(UART_HandleTypeDef* huart2)
{
	char writeStr[10];
	//HAL_UART_Transmit(huart2, writeStr, sprintf(writeStr, "Index %d\r\n", index), 1000);
	//HAL_UART_Transmit(huart2, writeStr, sprintf(writeStr, "Receiving %c\r\n", buffer[index]), 1000);
	switch (cmdParserStatus)
	{
	case IDLE:
		if (buffer[index] == '!')
		{
			cmdParserStatus = CMD_WAITING;
		}
		index++;
		if (receive_flag == 0)
		{
			index_buffer = 0;
			index = 0;
		}
		break;
	case CMD_WAITING:
		if (buffer[index] == 'O')
		{
			cmdParserStatus = CMD_WAITING_O;
		}
		else if (buffer[index] == 'R')
		{
			cmdParserStatus = CMD_WAITING_R;
		}
		else if (buffer[index] == 'B') cmdParserStatus = CMD_WAITING_B_num;
		else cmdParserStatus = IDLE;
		index++;
		if (receive_flag == 0)
		{
			index_buffer = 0;
			index = 0;
		}
		break;
//-----------------------------------------------------
	case CMD_WAITING_O:
		if (buffer[index] == 'K')
		{
			cmdParserStatus = CMD_WAITING_OK;
		}
		else cmdParserStatus = IDLE;
		index++;
		if (receive_flag == 0)
		{
			index_buffer = 0;
			index = 0;
		}
		break;
	case CMD_WAITING_OK:
		if (buffer[index] == '#')
		{
			OK_flag = 1;
			cmdParserStatus = IDLE;
		}
		else cmdParserStatus = IDLE;
		index++;
		if (receive_flag == 0)
		{
			index_buffer = 0;
			index = 0;
		}
		break;
//-----------------------------------------------------
	case CMD_WAITING_R:
		if (buffer[index] == 'S')
		{
			cmdParserStatus = CMD_WAITING_RS;
		}
		else cmdParserStatus = IDLE;
		index++;
		if (receive_flag == 0)
		{
			index_buffer = 0;
			index = 0;
		}
		break;
	case CMD_WAITING_RS:
		if (buffer[index] == 'T')
		{
			cmdParserStatus = CMD_WAITING_RST;
		}
		else cmdParserStatus = IDLE;
		index++;
		if (receive_flag == 0)
		{
			index_buffer = 0;
			index = 0;
		}
		break;
	case CMD_WAITING_RST:
		if (buffer[index] == '#')
		{
			RST_flag = 1;
			setTimer(0, 2000);
			setTimer(1, 3000);
			cmdParserStatus = IDLE;
		}
		else cmdParserStatus = IDLE;
		index++;
		if (receive_flag == 0)
		{
			index_buffer = 0;
			index = 0;
		}
		break;
//-----------------------------------------------------
//	case CMD_WAITING_B:
//		if (buffer[index] == 'B')
//		{
//			cmdParserStatus = CMD_WAITING_B_num;
//		}
//		else cmdParserStatus = IDLE;
//		index++;
//		if (receive_flag == 0)
//		{
//			index_buffer = 0;
//			index = 0;
//		}
//		break;
	case CMD_WAITING_B_num:
		if (buffer[index] == 'L')
		{
			isButton = LIGHT;
			cmdParserStatus = CMD_WAITING_B_num_CONFIRM;
		}
		else if (buffer[index] == 'P')
		{
			isButton = PUMP;
			cmdParserStatus = CMD_WAITING_B_num_CONFIRM;
		}
		else cmdParserStatus = IDLE;
		index++;
		if (receive_flag == 0)
		{
			index_buffer = 0;
			index = 0;
		}
		break;
	case CMD_WAITING_B_num_CONFIRM:
		if (buffer[index] == ':')
		{
			cmdParserStatus = CMD_B_num_VALUE;
		}
		else cmdParserStatus = IDLE;
		index++;
		if (receive_flag == 0)
		{
			index_buffer = 0;
			index = 0;
		}
		break;
	case CMD_B_num_VALUE:
		if (buffer[index] == '0')
		{
			if (isButton == LIGHT) lightButton = 0;
			if (isButton == PUMP) pumpButton = 0;
			cmdParserStatus = CMD_B_num_VALUE_CONFIRM;
		}
		else if (buffer[index] == '1')
		{
			if (isButton == LIGHT) lightButton = 1;
			if (isButton == PUMP) pumpButton = 1;
			cmdParserStatus = CMD_B_num_VALUE_CONFIRM;
		}
		else cmdParserStatus = IDLE;
		index++;
		if (receive_flag == 0)
		{
			index_buffer = 0;
			index = 0;
		}
		break;
	case CMD_B_num_VALUE_CONFIRM:
		if (buffer[index] == '#')
		{
			BUTTON_flag = 1;
			cmdParserStatus = IDLE;
		}
		else cmdParserStatus = IDLE;
		index++;
		if (receive_flag == 0)
		{
			index_buffer = 0;
			index = 0;
		}
		break;
	default:
		break;
	}
}

void uart_communication_fsm(UART_HandleTypeDef* huart2, ADC_HandleTypeDef* hadc1, ADC_HandleTypeDef* hadc2)
{
	char writeStr[50];
	if (RST_flag == 1)
	{

		//HAL_UART_Transmit(huart2, writeStr, sprintf(writeStr, "RST\n"), 1000);
		//ADC_value = HAL_ADC_GetValue(&hadc1);
			//HAL_UART_Transmit(huart2, writeStr, sprintf(writeStr, "Different\n"), 1000);
		if (timer_timeout(0))
		{
			ADC1_value = HAL_ADC_GetValue(hadc1);
			HAL_UART_Transmit(huart2, writeStr, sprintf(writeStr, "!TEMP:%d#", ADC1_value), 1000);
			setTimer(0, 2000);
		}
		if (timer_timeout(1))
		{
			ADC2_value = HAL_ADC_GetValue(hadc2);
			HAL_UART_Transmit(huart2, writeStr, sprintf(writeStr, "!HUMID:%d#", ADC2_value), 1000);
			setTimer(1, 3000);
		}
		//RST_flag = 0;
		//ADC_value = HAL_ADC_GetValue(&hadc1);
	}
	if (OK_flag == 1)
	{
		OK_flag = 0;
		RST_flag = 0;
		ADC1_value = -1;
		ADC2_value = -1;
	}
	if (BUTTON_flag == 1)
	{
		BUTTON_flag = 0;
		if (isButton == LIGHT)
		{
			HAL_GPIO_WritePin(D13_GPIO_Port, D13_Pin, lightButton);
			//HAL_UART_Transmit(huart2, writeStr, sprintf(writeStr, "!LIGHT:%d", lightButton), 1000);
		}
		if (isButton == PUMP)
		{
			HAL_GPIO_WritePin(D12_GPIO_Port, D12_Pin, pumpButton);
			//HAL_UART_Transmit(huart2, writeStr, sprintf(writeStr, "!PUMP:%d", pumpButton), 1000);
		}
	}
}
