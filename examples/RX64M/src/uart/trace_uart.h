
#ifndef _UART_API_H_
#define _UART_API_H_


#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define TRACE_TRANSMIT_BUFFER_SIZE 1024
#define TRACE_RECEIVE_BUFFER_SIZE 1024

#ifndef bool_t
typedef unsigned char bool_t;
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

void trace_initializeUart();
uint32_t trace_write(const uint8_t buffer[], const uint32_t bufferSize, bool_t waitUntilAllBytesAreInBuffer);
uint32_t trace_peak(void);
uint32_t trace_read(uint8_t buffer[], const uint32_t bufferSize);

#ifdef __cplusplus
  }
#endif

#endif
