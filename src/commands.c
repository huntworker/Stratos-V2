#include "stm32f2xx_conf.h"
#include "misc.h"
#include "commands.h"
#include "main.h"
#include "MS5607.h"
#include "position.h"

#include "stdio.h"
#include "strings.h"
#include "inttypes.h"

RingBuffer_t commands_RX_Buffer;


static void commands_string_decode(char* rx_string);

void commands_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);


	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStruct);

	USART_Cmd(USART1, ENABLE);

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	InitRingBuffer(&commands_RX_Buffer);

}

void commands_send_char(const char c)
{
	USART_SendData(USART1, c);

	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

void commands_send_string(const char* const s)
{
	const char* ptr = s;
	while(*ptr)
	{
		commands_send_char(*ptr++);
	}
}

void USART1_IRQHandler(void)
{
  uint16_t wert;

  if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
    // wenn ein Byte im Empfangspuffer steht
    wert=USART_ReceiveData(USART1);
    // Byte speichern
	RingBufferWrite(&commands_RX_Buffer, (char)wert);
  }
}

RingBufferError_t commands_UART_Capture()
{
    static char rxString[128];
    static uint8_t rxStringWritePos = 0;

    char tmpchar = '\0';
    if (RingBufferRead(&commands_RX_Buffer, &tmpchar) == NO_ERROR)
    {
    	switch (tmpchar)
    	{
    		case '\r':
    		case '\n':
    		case ' ':
    		{
    			rxString[rxStringWritePos++] = '\0';
    			commands_string_decode(rxString);
    			rxStringWritePos = 0;
    			rxString[0] = '\0';
    		} break;

    		default:
    		{
    			rxString[rxStringWritePos++ & 127] = tmpchar;
    		}
    	}

        return NO_ERROR;
    }
    else
    {
    	return BUFFER_EMPTY;
    }
}

static void commands_string_decode(char* rx_string)
{
	if (rx_string[0] == 0)
		return;
	else if (strcasecmp(rx_string, "echo?") == 0)
	{
		commands_send_string("echo!\r\n");
	}
	else if (strcasecmp(rx_string, "data?") == 0)
	{
		settings.plotDataInCommands = 1;
	}
	else if (strcasecmp(rx_string, "gps?") == 0)
	{
		settings.forwardGpsToCommands = 1;
	}
	else if (strcasecmp(rx_string, "cal?") == 0)
	{
		uint32_t temp[7];
		char buffer[64];
		MS5607_get_cal(temp);
		snprintf(buffer, sizeof(buffer) / sizeof(char), "0x%04"PRIX32", 0x%04"PRIX32", 0x%04"PRIX32", 0x%04"PRIX32", 0x%04"PRIX32", 0x%04"PRIX32", 0x%04"PRIX32"\r\n", temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6]);
		commands_send_string(buffer);
	}
	else if (strcasecmp(rx_string, "airb?") == 0)
	{
		position_in_airbone();
		/*
		if (position_in_airbone())
		{
			commands_send_string("airbone OK\r\n");
		}
		else
		{
			commands_send_string("airbone not set!\r\n");
		}*/
	}
	else if (strcasecmp(rx_string, "stop") == 0)
	{
		settings.plotDataInCommands = 0;
		settings.forwardGpsToCommands = 0;
	}
	else
	{
		commands_send_string("Command not found!\r\n");
		return;
	}
}
