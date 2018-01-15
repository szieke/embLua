#ifndef GLOBAL_INCLUDE_H
#define GLOBAL_INCLUDE_H

#include "stdint.h"
#include "FreeRTOS.h"
#include "task.h"
#include "driverlib.h"


typedef unsigned char bool_t;

#ifndef NULL
#define NULL (void*)0
#endif

#ifndef TRUE
#define TRUE 1U
#endif

#ifndef FALSE
#define FALSE 0U
#endif

#ifndef CONST
#define CONST const
#endif

#ifndef STATIC
#define STATIC static
#endif

#endif

