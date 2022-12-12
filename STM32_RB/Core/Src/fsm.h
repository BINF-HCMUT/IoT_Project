/*
 * fsm.h
 *
 *  Created on: Nov 20, 2022
 *      Author: PC
 */
#include "main.h"

#ifndef SRC_FSM_H_
#define SRC_FSM_H_

#define MAX_BUFFER_SIZE 30

extern uint8_t buffer[MAX_BUFFER_SIZE];
extern int index_buffer;
extern int receive_flag;
extern uint8_t temp;
extern int RST_flag;
extern int OK_flag;


void command_parser_fsm(UART_HandleTypeDef* huart2);
void uart_communication_fsm(UART_HandleTypeDef* huart2, ADC_HandleTypeDef* hadc1, ADC_HandleTypeDef* hadc2);

#endif /* SRC_FSM_H_ */
