#include "stm32f2xx_tim.h"
#include "misc.h"
#include "stm32f2xx_rcc.h"
#include "stm32f2xx_gpio.h"
#include "string.h"

#include "DAC.h"

static const uint8_t num_of_startbytes = 25;
static uint16_t txString_length = 7;
static char txString[512] = "\0\0DK0HTW SAAR STRATOS\r\n";

static uint8_t rf_on = 0;


void af_init()
{


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); // 16MHz
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
    TIM_TimeBaseInitStruct.TIM_Period = 4-1;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 13333-1;  // Baudrate = f_tim / Period / Prescaler = 300 + 6,25e-6
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStruct);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM7, ENABLE);
}

void TIM7_IRQHandler() // every 20/6 ms == 300 Hz == 300 Baud
{
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);

        // TX String besteht aus: Startbits (1,0), Datenbits(xxxx xxxx)

        static uint16_t BytePosition = 0;
        static uint8_t BitPosition = 0;

        if (!rf_on) // -> rf_off
        {
            BytePosition = 0;
            BitPosition = 0;
        }
        else
        {
            uint8_t txByte = (uint8_t)txString[BytePosition];

            if (BitPosition == 0)
            {
                transmit(0);
            }
            else if ((BitPosition == 9) || (BitPosition == 10))
            {
                transmit(1);
            }
            else
            {
                transmit((txByte >> (BitPosition-1)) & 1);       // LSB first
            }

            BitPosition++;

            if (BitPosition > 10)
            {
                BytePosition++;
                BitPosition = 0;

                //if (BytePosition >= (sizeof(txString) / sizeof(char)))
                if (BytePosition > txString_length)
                {
                    BytePosition = 0;
                    rf_on = 0;
                    stopSine();
                    GPIO_ResetBits(GPIOA, GPIO_Pin_2);    // Disable PTT
                }
            }
        }
    }
}

void rf_send_string(const char* const buffer)
{
    uint16_t i = 0;

    uint16_t buffer_length = strlen(buffer);

    txString_length = buffer_length + num_of_startbytes;

    if (txString_length > sizeof(txString))
        txString_length = sizeof(txString);

    for (; i < num_of_startbytes; i++)
    {
        txString[i] = '\0';
    }

    for (; i < txString_length; i++)
    {
        txString[i] = buffer[i-num_of_startbytes];
    }

    txString[i++] = '\0';
    txString[i++] = '\0';
    txString[i++] = '\0';

    startSine();
    rf_on = 1;
    GPIO_SetBits(GPIOA, GPIO_Pin_2);    // Enable PTT
}
