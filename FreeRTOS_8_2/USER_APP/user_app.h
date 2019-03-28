//Author: PopM @ 14.12.15

/* Standard includes. */
#include <string.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* USER application includes. */
#include "lcd.c"
#include "adc.c"

/* push-buttons inputs */
#define PB1       (1<<24)	// PB24 = LEFT
#define PB2       (1<<25)	// PB25 = RIGHT

void vStartUSERTask( unsigned portBASE_TYPE uxPriority );
static void vButtonsTask( void *pvParameters );
static void vDisplayTask( void *pvParameters );
static void vAcquireTask( void *pvParameters );
void INTtoSTR( long Number, char * Result );
