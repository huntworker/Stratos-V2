#define MCP980_ACTIVE
#define MS5607_ACTIVE
//#define RRC_AKKU_ACTIVE
//#define LIS302_ACTIVE

//#define HEATING_ACTIVE


#include "usart.h"
#include "i2c.h"
#include "MCP9800.h"
#include "stm32_ub_fatfs.h"
#include "sdio.h"
//#include "RTC.h"
#include "DAC.h"
#include "stm32f2xx_RNG.h"
#include "stm32f2xx_rcc.h"
#include "stm32f2xx_gpio.h"
#include "AF.h"
#include "gps.h"

#ifdef RRC_AKKU_ACTIVE
#include "RRC_Bat.h"
#endif

#include "watchdog.h"

#ifdef MS5607_ACTIVE
#include "MS5607.h"
#endif

#include "adc.h"

#ifdef LIS302_ACTIVE
#include "LIS302.h"
#endif

#ifdef HEATING_ACTIVE
#include "heating.h"
uint8_t heating_trigger = 0;
#endif

#include "stm32f2xx.h"

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

/*uint32_t flags;
#define FLAG_LOGGING_ENABLE 0 */

int SD_add_string(char* str);

static volatile uint32_t timeout = 0;


int main()
{
	//SystemInit();


	char buffer[128];

	char temperature[4][4];
	char batt_abs_charge[7];
	char batt_avg_ttl[7];
	char pressure[5];

	uint8_t last_reset_by_watchdog = watchdog_init(2000);

	usart_init();

	//my_RTC_Init();

	my_DAC_Init();
	my_ADC_Init();

	af_init();

	GPS_Init();

    i2c_init_tmp(I2C1);
#ifdef MCP980_ACTIVE
	MCP_9800_init();
#endif

#ifdef RRC_AKKU_ACTIVE
    RRC_Bat_init();
#endif

#ifdef MS5607_ACTIVE
    MS5607_init();
#endif

#ifdef LIS302_ACTIVE
    LIS302_Init();
#endif

#ifdef HEATING_ACTIVE
    heating_init();
#endif

    UB_Fatfs_Init();


	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency);

	usart_send_char(0);

	if (last_reset_by_watchdog)
    {
        snprintf(buffer, 40, "\r\n!!WATCHDOG CAUSED RESET!!\r\n");
        usart_send_string(buffer);
        rf_send_string(buffer);
    }


	GPS_Message_t GPS_Message;

    int16_t tmp_i16;
    uint16_t tmp_u16;
    uint32_t last_time = 0;


    usart_send_string("\r\nDate: "__DATE__"\r\nInit done\r\n");
    rf_send_string("\r\nDate: "__DATE__"\r\nInit done\r\n");



	while(1)
	{
	    watchdog_trigger();

	    for (uint8_t i = 0; i < 255; i++)
	    {
	    	if(GPS_UART_Capture() == BUFFER_EMPTY)
	    		break;
	    }


        GPS_GetPosition(&GPS_Message);

        uint32_t time = timeout;//atoi(GPS_Message.time);

        //int16_t x, y, z;

        //LIS302_GetAcc(&x, &y, &z);

#ifdef HEATING_ACTIVE
        if (heating_trigger)
        {
        	heating_trigger = 0;
        	heating();
        }
#endif



        if ((time != last_time) || (timeout > 70))
        {
            // time has changed
            // every second

        	last_time = time;

            if (!(time % 10) || timeout > 70)
            {
                // every 10 sec

                tmp_i16 = MCP9800_get_tmp(MCP9800_PCB_ADDRESS);
                // Least Significant Byte = ganze Temperatur
                // Most Significant Byte = Teiler

                //snprintf(temperature[0], 4, "%"PRIi8"", (int8_t)(tmp_i16 & 0xFF));
                snprintf(temperature[0], 4, "%i", (int8_t)(tmp_i16 & 0xFF));
                //sprintf(temperature[0], "%"PRIi8"", (int8_t)(tmp_i16 & 0xFF));

                tmp_i16 = MCP9800_get_tmp(MCP9800_IN1_ADDRESS);
                //snprintf(temperature[1], 4, "%"PRIi8"", (int8_t)(tmp_i16 & 0xFF));
                snprintf(temperature[1], 4, "%i", (int8_t)(tmp_i16 & 0xFF));

                tmp_i16 = MCP9800_get_tmp(MCP9800_IN2_ADDRESS);
                snprintf(temperature[2], 4, "%i", (int8_t)(tmp_i16 & 0xFF));
                //snprintf(temperature[2], 4, "%"PRIi8"", (int8_t)(tmp_i16 & 0xFF));

                tmp_i16 = MCP9800_get_tmp(MCP9800_EXT_ADDRESS);
                //snprintf(temperature[3], 4, "%"PRIi8"", (int8_t)(tmp_i16 & 0xFF));
                snprintf(temperature[3], 4, "%i", (int8_t)(tmp_i16 & 0xFF));

                MS5607_get_pressure(&tmp_u16, &tmp_i16);
                snprintf(pressure, sizeof(pressure) / sizeof(char), "%"PRIu16"", tmp_u16);

#ifndef RRC_AKKU_ACTIVE
                tmp_u16 = (uint32_t)UB_ADC1_AV_Read(11, 255) * 4000 / 2918;
                snprintf(batt_abs_charge, sizeof(batt_abs_charge) / sizeof(char), "%"PRIu16"", tmp_u16);

                tmp_u16 = (uint32_t)UB_ADC1_AV_Read(10, 255) * 8000 / 3153;
                snprintf(batt_avg_ttl, sizeof(batt_avg_ttl) / sizeof(char), "%"PRIu16"", tmp_u16);
#else
                tmp_u16 = RRC_Bat_get(RRC_BAT_REG_ABS_CHARGE);
                //tmp_u16 = 98;
                snprintf(batt_abs_charge, sizeof(batt_abs_charge) / sizeof(char), "%"PRIu16"", tmp_u16);

                tmp_u16 = RRC_Bat_get(RRC_BAT_REG_AVG_TIME_TO_EMPTY);
                //tmp_u16 = 10000;
                snprintf(batt_avg_ttl, sizeof(batt_avg_ttl) / sizeof(char), "%"PRIu16"", tmp_u16);
#endif

                buffer[0] = '\0';   // buffer "leeren"
                strncat(buffer, "\r\n\r\nDK0HTW", sizeof(buffer)-strlen(buffer));
                strncat(buffer, ",", sizeof(buffer)-strlen(buffer));
                strncat(buffer, GPS_Message.time, sizeof(buffer)-strlen(buffer));
                strncat(buffer, ",", sizeof(buffer)-strlen(buffer));
                strncat(buffer, GPS_Message.latitude, sizeof(buffer)-strlen(buffer));
                strncat(buffer, ",", sizeof(buffer)-strlen(buffer));
                strncat(buffer, GPS_Message.longitude, sizeof(buffer)-strlen(buffer));
                strncat(buffer, ",", sizeof(buffer)-strlen(buffer));
                strncat(buffer, GPS_Message.altitude, sizeof(buffer)-strlen(buffer));
                strncat(buffer, ",", sizeof(buffer)-strlen(buffer));
                strncat(buffer, GPS_Message.satelites, sizeof(buffer)-strlen(buffer));
                strncat(buffer, ",", sizeof(buffer)-strlen(buffer));
                strncat(buffer, GPS_Message.valid, sizeof(buffer)-strlen(buffer));
                strncat(buffer, ",", sizeof(buffer)-strlen(buffer));
                strncat(buffer, temperature[0], sizeof(buffer)-strlen(buffer));
                strncat(buffer, ",", sizeof(buffer)-strlen(buffer));
                strncat(buffer, temperature[1], sizeof(buffer)-strlen(buffer));
                strncat(buffer, ",", sizeof(buffer)-strlen(buffer));
                strncat(buffer, temperature[2], sizeof(buffer)-strlen(buffer));
                strncat(buffer, ",", sizeof(buffer)-strlen(buffer));
                strncat(buffer, temperature[3], sizeof(buffer)-strlen(buffer));
                strncat(buffer, ",", sizeof(buffer)-strlen(buffer));
                strncat(buffer, pressure, sizeof(buffer)-strlen(buffer));
                strncat(buffer, ",", sizeof(buffer)-strlen(buffer));
                strncat(buffer, batt_abs_charge, sizeof(buffer)-strlen(buffer));
                strncat(buffer, ",", sizeof(buffer)-strlen(buffer));
                strncat(buffer, batt_avg_ttl, sizeof(buffer)-strlen(buffer));

                // Baudrate GPS Modul
                //strncat(buffer, "!DEBUG VERSION!", sizeof(buffer)-strlen(buffer));

                strncat(buffer, "*", sizeof(buffer)-strlen(buffer));
                strncat(buffer, GPS_Message.day, sizeof(buffer)-strlen(buffer));
                strncat(buffer, ".", sizeof(buffer)-strlen(buffer));
                strncat(buffer, GPS_Message.month, sizeof(buffer)-strlen(buffer));
                strncat(buffer, ".", sizeof(buffer)-strlen(buffer));
                strncat(buffer, GPS_Message.year, sizeof(buffer)-strlen(buffer));


                if (SD_add_string(buffer) == 1)
                {
                	// Fehler beim beschreiben der SD-Karte
                	strncat(buffer, ",SD_ERR", sizeof(buffer)-strlen(buffer));
                }

                strncat(buffer, "\r\n\r\n", sizeof(buffer)-strlen(buffer));

                usart_send_string(buffer);

                if (!(time % 100) || timeout >= 69)
                {
                    rf_send_string(buffer);

                    if (timeout > 70)
                    {
                    	// reconfigure GPS Modul
                    	GPS_SendConfig();
                    }
                    timeout = 0;
                } // if (!(time % 100) || timeout > 70)
            } // if (!(time % 10) || timeout > 70)
        } // if ((time != last_time) || (timeout > 70))
	} // while(1)
}

void SysTick_Handler(void) // every 1 s
{
    timeout++;
    timeout %= 10;
#ifdef HEATING_ACTIVE
    heating_trigger = 1;
#endif
}

void Default_Handler(void)
{
	while(1);
}

void HardFault_Handler()
{
	while(1);
}

void RTC_Alarm_IRQHandler(void)
{
	while(1);
}

int SD_add_string(char* str)
{
	int error = 1;
    FIL myFile;   // Filehandler
    if(UB_Fatfs_CheckMedia(MMC_0) == FATFS_OK)
    {
        // Media mounten
        if(UB_Fatfs_Mount(MMC_0) == FATFS_OK)
        {
            // File zum schreiben im root öffnen bzw. neu anlegen
            if(UB_Fatfs_OpenFile(&myFile, "0:/Logger.txt", F_WR_NEW)==FATFS_OK)
            {
                UB_Fatfs_WriteString(&myFile,str);

                UB_Fatfs_CloseFile(&myFile);
                error = 0;
            }
        }
        UB_Fatfs_UnMount(MMC_0);
    }

    return error;
}
