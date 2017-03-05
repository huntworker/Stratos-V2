/*
 * heating.c
 *
 *  Created on: Aug 23, 2016
 *      Author: mobilworker
 */

#include <stdio.h>
#include <inttypes.h>

#include "stm32f2xx_gpio.h"
#include "stm32f2xx_rcc.h"
#include "MCP9800.h"
#include "usart.h"
#include "heating.h"


void heating_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	heating_disable();
}

void heating_enable()
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, ENABLE);
}

void heating_disable()
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, DISABLE);
}

void heating()
{
	static uint16_t dutyCycle = 0; /* Range: 0..10 */
	int8_t temp = MCP9800_get_tmp(MCP9800_IN1_ADDRESS);
	if (temp == -99)
		temp = MCP9800_get_tmp(MCP9800_PCB_ADDRESS);

	if (temp <= -70)
	{
		heating_disable();
		return;
	}

	if (temp <= 0)
	{
		dutyCycle = 9;
	}
	else if (temp <= 3)
		dutyCycle = 7;
	else if (temp <= 6)
		dutyCycle = 5;
	else if (temp <= 9)
		dutyCycle = 2;
	else
		dutyCycle = 0;

	/*char buffer[40];
	snprintf(buffer, 40, "DC: %"PRIu16"\r\n", dutyCycle);
	usart_send_string(buffer);*/


	static uint8_t count = 0;

	if (dutyCycle > count)
	{
		heating_enable();
	}
	else
	{
		heating_disable();
	}
	count++;
	if (count > 9)
	{
		count = 0;
	}

}
