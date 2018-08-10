
#include "RingBuffer.h"



void commands_init(void);
RingBufferError_t commands_UART_Capture();
void commands_send_char(const char c);
void commands_send_string(const char* const s);
