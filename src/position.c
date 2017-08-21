#include <stdint.h>

#include "RingBuffer.h"

#include "position.h"
#include "stm32f2xx_conf.h"
#include "misc.h"


#include <string.h>

// UART1, 38.400, UB+NMEA
uint8_t const GPS_MSG_SETBAUD[] = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x00, 0x96, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8F, 0x70};

// Airbone < 1g, Auto 2D/3D
uint8_t const GPS_MSG_SETAIRB[] = {0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x64};

// Enable GPZDA
uint8_t const GPS_MSG_SETDATE[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x08, 0x60, 0xB5, 0x62, 0x06, 0x01, 0x02, 0x00, 0xF0, 0x08, 0x01, 0x19};

void string_Decode(char* str);
void USART6SendChar(const uint8_t c);
void USART6SendString(const uint8_t* str);
void USART6SendRaw(const uint8_t* str, uint8_t count);
void position_message_init(void);
uint8_t getNumToDelim(const char* const str, char delimiter);
void copyString(char* const destination, const char* source, uint8_t charsToCopy, uint8_t max);

RingBuffer_t GPS_USART_RxRingBuffer;

static position_message_t GP_temp;
/*static position_message_t GL_temp;
static position_message_t GA_temp;
static position_message_t GB_temp;
static position_message_t GN_temp;*/

void position_message_init_struct(position_message_t* msg)
{
	msg->accuracy[0] = '0';
	msg->accuracy[1] = '\0';
	msg->altitude[0] = '0';
	msg->altitude[1] = '\0';
	msg->day[0] = '0';
	msg->day[1] = '\0';
	msg->latitude[0] = '0';
	msg->latitude[1] = '\0';
	msg->latitude_dir[0] = '0';
	msg->latitude_dir[1] = '\0';
	msg->locked = 0;
	msg->longitude[0] = '0';
	msg->longitude[1] = '\0';
	msg->longitude_dir[0] = '0';
	msg->longitude_dir[1] = '\0';
	msg->month[0] = '0';
	msg->month[1] = '\0';
	msg->satelites[0] = '0';
	msg->satelites[1] = '\0';
	msg->speed[0] = '0';
	msg->speed[1] = '\0';
	msg->time[0] = '0';
	msg->time[1] = '\0';
	msg->valid[0] = '0';
	msg->valid[1] = '\0';
	msg->year[0] = '0';
	msg->year[1] = '\0';
}

void position_message_init(void)
{
	position_message_init_struct(&GP_temp);
	/*position_message_init_struct(&GL_temp);
	position_message_init_struct(&GA_temp);
	position_message_init_struct(&GB_temp);
	position_message_init_struct(&GN_temp);*/
}

void Position_Init()
{
    //RingBuffer Init
    InitRingBuffer(&GPS_USART_RxRingBuffer);

    // Serial Interface Init
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 38400;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART6, &USART_InitStruct);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART6_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);

    USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);

    USART_Cmd(USART6, ENABLE);

    Position_SendConfig();

    position_message_init();
}

void Position_SendConfig()
{/*
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;

    USART_Cmd(USART6, DISABLE);
    USART_InitStruct.USART_BaudRate = 4800;
    USART_Init(USART6, &USART_InitStruct);
    USART_Cmd(USART6, ENABLE);
    USART6SendRaw(GPS_MSG_SETBAUD, sizeof(GPS_MSG_SETBAUD));
    USART_Cmd(USART6, DISABLE);

    USART_Cmd(USART6, DISABLE);
    USART_InitStruct.USART_BaudRate = 9600;
    USART_Init(USART6, &USART_InitStruct);
    USART_Cmd(USART6, ENABLE);
    USART6SendRaw(GPS_MSG_SETBAUD, sizeof(GPS_MSG_SETBAUD));

    USART_Cmd(USART6, DISABLE);
    USART_InitStruct.USART_BaudRate = 19200;
    USART_Init(USART6, &USART_InitStruct);
    USART_Cmd(USART6, ENABLE);
    USART6SendRaw(GPS_MSG_SETBAUD, sizeof(GPS_MSG_SETBAUD));

    USART_Cmd(USART6, DISABLE);
    USART_InitStruct.USART_BaudRate = 57600;
    USART_Init(USART6, &USART_InitStruct);
    USART_Cmd(USART6, ENABLE);
    USART6SendRaw(GPS_MSG_SETBAUD, sizeof(GPS_MSG_SETBAUD));

    USART_Cmd(USART6, DISABLE);
    USART_InitStruct.USART_BaudRate = 115200;
    USART_Init(USART6, &USART_InitStruct);
    USART_Cmd(USART6, ENABLE);
    USART6SendRaw(GPS_MSG_SETBAUD, sizeof(GPS_MSG_SETBAUD));

    USART_Cmd(USART6, DISABLE);
    USART_InitStruct.USART_BaudRate = 38400;
    USART_Init(USART6, &USART_InitStruct);
    USART_Cmd(USART6, ENABLE);
    USART6SendRaw(GPS_MSG_SETBAUD, sizeof(GPS_MSG_SETBAUD));

    USART6SendRaw(GPS_MSG_SETAIRB, sizeof(GPS_MSG_SETAIRB));

    USART6SendRaw(GPS_MSG_SETDATE, sizeof(GPS_MSG_SETDATE));*/
}

RingBufferError_t Position_UART_Capture()
{
    static char rxString[128];
    static uint8_t rxStringWritePos = 0;

    char tmpchar = '\0';
    if (RingBufferRead(&GPS_USART_RxRingBuffer, &tmpchar) == NO_ERROR)
    {
        if (tmpchar == '$')
        {
            rxStringWritePos = 0;
        }
        else if (tmpchar == '*')
        {
            rxStringWritePos = 0;
            // kompletter String empfangen
            string_Decode(rxString);
        }
        else
        {
            // einzelnes Datenzeichen Empfangen
            // anhängen des Zeichens an rxString
            rxString[rxStringWritePos++ & 127] = tmpchar;
        }
        return NO_ERROR;
    }
    else
    {
    	return BUFFER_EMPTY;
    }
}

void position_store(char* str, position_message_t* msg)
{

}

void string_Decode(char* str)
{
	position_message_t* Gx_temp;

	// first char must be 'G'
	if (*str++ != 'G')
	{
		return;
	}

	// second char defines type (P == GPS, L == GLONASS, A == Galileo, B == BeiDou)
	switch (*str++)
	{
		case 'P':
		{
			Gx_temp = &GP_temp;
		} break;
		/*case 'L':
		{
			Gx_temp = &GL_temp;
		} break;
		case 'A':
		{
			Gx_temp = &GA_temp;
		} break;
		case 'B':
		{
			Gx_temp = &GB_temp;
		} break;
		case 'N':
		{
			Gx_temp = &GN_temp;
		} break;*/
		default:
		{
			return;
		} break;
	}

    const char delimiter = ',';
    char* ptr = str;
    char temp[16];
    uint8_t charsToCopy;

    charsToCopy = getNumToDelim(ptr, delimiter);
    copyString(temp, ptr, charsToCopy, 6);
    ptr += charsToCopy + 1;

    // GxGGA is used for positioning
    if (strncmp("GGA", temp, 3) == 0)
    {
    	Gx_temp->locked = 1;

    	charsToCopy = getNumToDelim(ptr, delimiter);
		copyString(Gx_temp->time, ptr, charsToCopy, 6);
		ptr += charsToCopy + 1;

		charsToCopy = getNumToDelim(ptr, delimiter);
		copyString(Gx_temp->latitude, ptr, charsToCopy, 9);
		ptr += charsToCopy + 1;

		charsToCopy = getNumToDelim(ptr, delimiter);
		copyString(Gx_temp->latitude_dir, ptr, charsToCopy, 1);
		ptr += charsToCopy + 1;

		charsToCopy = getNumToDelim(ptr, delimiter);
		copyString(Gx_temp->longitude, ptr, charsToCopy, 10);
		ptr += charsToCopy + 1;

		charsToCopy = getNumToDelim(ptr, delimiter);
		copyString(Gx_temp->longitude_dir, ptr, charsToCopy, 1);
		ptr += charsToCopy + 1;

		charsToCopy = getNumToDelim(ptr, delimiter);
		copyString(Gx_temp->valid, ptr, charsToCopy, 1);
		ptr += charsToCopy + 1;

		charsToCopy = getNumToDelim(ptr, delimiter);
		copyString(Gx_temp->satelites, ptr, charsToCopy, 2);
		ptr += charsToCopy + 1;

		charsToCopy = getNumToDelim(ptr, delimiter);
		copyString(Gx_temp->accuracy, ptr, charsToCopy, 4);
		ptr += charsToCopy + 1;

		charsToCopy = getNumToDelim(ptr, delimiter);
		uint8_t temp_len = getNumToDelim(ptr, '.');
		if (charsToCopy > temp_len)
		{
			charsToCopy = temp_len;
		}
		copyString(Gx_temp->altitude, ptr, charsToCopy, 7);
		//ptr += charsToCopy + 1;

		// erst folgen jetzt Dezimalstellen von Höhe

        Gx_temp->locked = 0;
    }

    // GxZDA is used for date
    if (strncmp("ZDA", temp, 3) == 0)
	{
		Gx_temp->locked = 1;

		charsToCopy = getNumToDelim(ptr, delimiter);
		// time
		ptr += charsToCopy + 1;

		charsToCopy = getNumToDelim(ptr, delimiter);
		copyString(Gx_temp->day, ptr, charsToCopy, 2);
		ptr += charsToCopy + 1;

		charsToCopy = getNumToDelim(ptr, delimiter);
		copyString(Gx_temp->month, ptr, charsToCopy, 2);
		ptr += charsToCopy + 1;

		charsToCopy = getNumToDelim(ptr, delimiter);
		copyString(Gx_temp->year, ptr, charsToCopy, 4);
		ptr += charsToCopy + 1;

		Gx_temp->locked = 0;
	}
}

void Position_Get(position_message_t* msg)
{
	uint32_t timeout = 10000;
	while(GP_temp.locked && (timeout < 1000))
	{
		timeout--;
	}

	// Nachricht gültig
	PositionCopy(msg, &GP_temp);
}

void USART6_IRQHandler()
{
	if (USART_GetITStatus(USART6,USART_IT_RXNE) != RESET)
    {
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);

		RingBufferWrite(&GPS_USART_RxRingBuffer, (char)USART_ReceiveData(USART6));
	}
}

void USART6SendChar(const uint8_t c)
{
	USART_SendData(USART6, (uint16_t) c);
	while(USART_GetFlagStatus(USART6,USART_FLAG_TC) == RESET);
}

void USART6SendString(const uint8_t * str)
{
	while(*str)
    {
		USART6SendChar(*str++);
	}
}

void USART6SendRaw(const uint8_t* str, uint8_t count)
{
	for (uint8_t i = 0; i < count; i++)
	{
		USART6SendChar(str[i]);
	}
}

uint8_t getNumToDelim(const char* const str, char delimiter)
{
	const char* ptr = str;
	uint8_t num = 0;
	while ( (*ptr != delimiter) || (*ptr == '\0') )
	{
		ptr++;
		num++;
	}

	return num;
}

void copyString(char* const destination, const char* source, uint8_t charsToCopy, uint8_t max)
{
	if (charsToCopy > max)
	{
		charsToCopy = max;
	}

	if (charsToCopy == 0)
	{
		destination[0] = '0';
		destination[1] = '\0';
	}
	else
	{
		for (uint8_t i = 0; i < charsToCopy; i++)
		{
			destination[i] = source[i];
		}
		destination[charsToCopy] = '\0';
	}

}

void PositionCopy(position_message_t* const dest, const position_message_t* const src)
{
	memcpy(dest, src, sizeof(position_message_t));
}
