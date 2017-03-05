
typedef struct
{
    char time[11];           // hhmmss (UTC)
    char latitude[10];      // DDMM.MMMM
    char latitude_dir[2];   // N or S
    char longitude[11];     // DDDMM.MMMM
    char longitude_dir[2];  // E or W
    char altitude[8];       // MMM.MMM (meters)
    char speed[4];          // kkk (knots)
    char satelites[3];      // # of satelites
    char valid[2];          // '1' = Valid, '0' = invalid
    uint8_t locked;
    char day[3];
    char month[3];
    char year[5];
} GPS_Message_t;

void GPS_Init();
void GPS_SendConfig();
RingBufferError_t GPS_UART_Capture();
void GPS_GetPosition(GPS_Message_t* msg);
