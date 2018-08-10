

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
    char accuracy[5];
} position_message_t;

typedef struct
{
	uint8_t unread;
	uint8_t class;
	uint8_t id;
	uint16_t lenght;
	char payload[127];
} position_ub_message_t;

typedef struct
{
	position_ub_message_t messages[5];
	uint8_t readpos;
	uint8_t writepos;
} position_ub_message_buffer_t;

void Position_Init();
void Position_SendConfig();
RingBufferError_t Position_UART_Capture();
void Position_Get(position_message_t* msg);
uint8_t position_in_airbone();
void PositionCopy(position_message_t* const dest, const position_message_t* const src);
void position_message_init_struct(position_message_t* msg);
