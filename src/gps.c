#include "stm32f2xx_conf.h"
#include "misc.h"
#include "RingBuffer.h"

#include "gps.h"

#include <string.h>

// UART1, 38.400, UB+NMEA
uint8_t const GPS_MSG_SETBAUD[] = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x00, 0x96, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8F, 0x70};

// Airbone < 1g, Auto 2D/3D
uint8_t const GPS_MSG_SETAIRB[] = {0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x64};

// Enable GPZDA
uint8_t const GPS_MSG_SETDATE[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x08, 0x60, 0xB5, 0x62, 0x06, 0x01, 0x02, 0x00, 0xF0, 0x08, 0x01, 0x19};

void GPS_Decode(char* str);
void USART6SendChar(const uint8_t c);
void USART6SendString(const uint8_t* str);
void USART6SendRaw(const uint8_t* str, uint8_t count);

RingBuffer_t GPS_USART_RxRingBuffer;

static GPS_Message_t GPS_temp;

void GPS_Init()
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

    GPS_SendConfig();
}

void GPS_SendConfig()
{
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

    USART6SendRaw(GPS_MSG_SETDATE, sizeof(GPS_MSG_SETDATE));
}

RingBufferError_t GPS_UART_Capture()
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
            GPS_Decode(rxString);
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

void GPS_Decode(char* str)
{
    //char nmeaType[6] = "NONE";
    const char delimiter[] = ",";
    char *ptr;

    // initialisieren und ersten Abschnitt erstellen
    ptr = strtok(str, delimiter);

    if (strncmp("GPGGA", ptr, 5) == 0)
    {
        // GPGGA folgt

    	GPS_temp.locked = 1;

        ptr = strtok(NULL, delimiter);  // delete sentence_type

        strncpy(GPS_temp.time, ptr, 6);      // Capture Time
        // naechsten Abschnitt erstellen
        ptr = strtok(NULL, delimiter);

        strncpy(GPS_temp.latitude, ptr, 9);
        ptr = strtok(NULL, delimiter);
        //strncpy(GPS_temp.latitude_dir, ptr, 1);
        ptr = strtok(NULL, delimiter);
        strncpy(GPS_temp.longitude, ptr, 10);
        ptr = strtok(NULL, delimiter);
        //strncpy(GPS_temp.longitude_dir, ptr, 1);
        ptr = strtok(NULL, delimiter);
        strncpy(GPS_temp.valid, ptr, 1);
        ptr = strtok(NULL, delimiter);
        strncpy(GPS_temp.satelites, ptr, 2);
        ptr = strtok(NULL, delimiter);
        ptr = strtok(NULL, delimiter);  // delete precisition
        strncpy(GPS_temp.altitude, ptr, 7);

        const char delimiter_point[] = ".";
        ptr = strtok(GPS_temp.altitude, delimiter_point);
        strncpy(GPS_temp.altitude, ptr, 7);

        GPS_temp.locked = 0;
    }

    if (strncmp("GPZDA", ptr, 5) == 0)
	{
		// GPZDA folgt

		GPS_temp.locked = 1;

		ptr = strtok(NULL, delimiter);  // delete sentence_type

		//strncpy(GPS_temp.time, ptr, 6);      // Capture Time
		// naechsten Abschnitt erstellen
		ptr = strtok(NULL, delimiter);
		strncpy(GPS_temp.day, ptr, 2);
		ptr = strtok(NULL, delimiter);
		strncpy(GPS_temp.month, ptr, 2);
		ptr = strtok(NULL, delimiter);
		strncpy(GPS_temp.year, ptr, 4);

		GPS_temp.locked = 0;
	}
}

void GPS_GetPosition(GPS_Message_t* msg)
{
	uint32_t timeout = 10000;
	while(GPS_temp.locked && (timeout < 1000))
	{
		timeout--;
	}

	// Nachricht gültig
	strncpy(msg->altitude, GPS_temp.altitude, 8);
	strncpy(msg->latitude, GPS_temp.latitude, 10);
	strncpy(msg->latitude_dir, GPS_temp.latitude_dir, 2);
	strncpy(msg->longitude, GPS_temp.longitude, 11);
	strncpy(msg->longitude_dir, GPS_temp.longitude_dir, 2);
	strncpy(msg->satelites, GPS_temp.satelites, 3);
	strncpy(msg->speed, GPS_temp.speed, 4);
	strncpy(msg->time, GPS_temp.time, 11);
	strncpy(msg->valid, GPS_temp.valid, 2);
	strncpy(msg->day, GPS_temp.day, 3);
	strncpy(msg->month, GPS_temp.month, 3);
	strncpy(msg->year, GPS_temp.year, 5);
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
