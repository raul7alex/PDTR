/* FreeRTOS V8.2.3 - Copyright (C) 2015 Real Time Engineers Ltd.
 * This demo includes a (basic) USB mouse driver.  It uses the small
 * joystick to provide the mouse inputs.
 *
 * LEDs D1 to D3 are controlled by the standard 'flash' tasks - each will
 * toggle at a different fixed frequency.
 *
 * A tick hook function is used to monitor the standard demo tasks - with LED
 * D4 being used to indicate the system status.  D4 toggling every 5 seconds
 * indicates that all the standard demo tasks are executing without error.  The
 * toggle rate increasing to 500ms is indicative of an error having been found
 * in at least one demo task.
 */

/* Standard includes. */
#include <stdlib.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Demo application includes. */

#include "lcd.h"
#include "partest.h"
#include "USBSample.h"
#include "BlockQ.h"
#include "blocktim.h"
#include "flash.h"
#include "QPeek.h"
#include "dynamic.h"

#include "AT91SAM7X256.h"
#include "semphr.h"
#include "task.h"
/* User application includes. */
#include "MyCorutina.h"

/* Priorities for the demo application tasks. */
#define mainUSB_PRIORITY					( tskIDLE_PRIORITY + 2 )
#define mainBLOCK_Q_PRIORITY				( tskIDLE_PRIORITY + 1 )
#define mainFLASH_PRIORITY                  ( tskIDLE_PRIORITY + 2 )
#define mainGEN_QUEUE_TASK_PRIORITY			( tskIDLE_PRIORITY )

#define mainUSER_PRIORITY					( tskIDLE_PRIORITY )

/* The LED toggle by the tick hook should an error have been found in a task. */
#define mainERROR_LED						( 3 )

static void prvSetupHardware( void );

SemaphoreHandle_t semaphoreHandle;

void signalErrorTask()
{

}

#define PB1       (1<<24)	// PB24 = LEFT
#define PB2       (1<<25)	// PB25 = RIGHT

void blinkLedTask(void* params)
{
	unsigned long PIOB_Status;
	while(1)
	{
		if (PIOB_Status != AT91C_BASE_PIOB->PIO_PDSR)
		{
			PIOB_Status = AT91C_BASE_PIOB->PIO_PDSR;
			if(!( PIOB_Status & PB1 ))
			{
			xSemaphoreGive(semaphoreHandle);
			taskYIELD();
			vTaskDelay(500);
			}
		}
	}
}

void readInputTask(void* params)
{
	while(1)
	{
		xSemaphoreTake(semaphoreHandle,portMAX_DELAY);

		LCDClearScreen();
		LCDPutStr("LEFT",0,0,2,WHITE,BLACK);
		vParTestToggleLED( 0 );
		vTaskDelay(500);
		vParTestToggleLED( 0 );
		LCDClearScreen();
	}
}

void xTaskTest(void* params)
{
   int x=1;
	 int Xold=1;

		while(1)
		{
				xSemaphoreTake(semaphoreHandle,portMAX_DELAY);

				LCDPutStr("TEST",0,0,Xold,BLACK,BLACK);
				LCDPutStr("TEST",0,0,++x,WHITE,BLACK);
				Xold=x;
				if(x>135) x=-5;
				vTaskDelay(500);
		}
}

void applicationStart()
{
	TaskHandle_t errorTaskHandle=NULL;
	vParTestSetLED(0,0);
	xTaskCreate(signalErrorTask,"taskErroare",128,NULL,tskIDLE_PRIORITY+100,&errorTaskHandle);
	vTaskSuspend(errorTaskHandle);

	if(NULL==(semaphoreHandle=xSemaphoreCreateBinary()))
	{
	vTaskResume(errorTaskHandle);
	}
	else
	{
	xTaskCreate(blinkLedTask,"blinkLedTask",128,NULL,tskIDLE_PRIORITY+1,NULL);
	xTaskCreate(readInputTask,"readINputTask",128,NULL,tskIDLE_PRIORITY+1,NULL);
		xTaskCreate(xTaskTest,"xTaskTest",128,NULL,tskIDLE_PRIORITY+1,NULL);
	}
}

void vApplicationIdleHook()
{
}

int main( void )
{
	/* Setup any hardware that has not already been configured by the low
	level init routines. */
	prvSetupHardware();

	/* Start the USB demo which is just for the SAM7. */
    // vStartUSBTask( mainUSB_PRIORITY );

    char str[]="TEST";
    int x=10,y=10,Size=0,fColor=0xF00,bColor=0xFFF;

    InitLCD();
    LCDSettings();
    LCDPutStr(str, x, y, LARGE, WHITE, BLACK);
    //MyStart();

    applicationStart();

    /* Start the user_defined app task */
    //vStartUSERTask( mainUSER_PRIORITY );

	/* Start the standard demo tasks. */
	// vStartBlockingQueueTasks( mainBLOCK_Q_PRIORITY );
    // vCreateBlockTimeTasks();
    // vStartLEDFlashTasks( mainFLASH_PRIORITY );
    // vStartGenericQueueTasks( mainGEN_QUEUE_TASK_PRIORITY );
    // vStartQueuePeekTasks();
    // vStartDynamicPriorityTasks();

	/* Start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */

	vTaskStartScheduler();

	/* We should never get here as control is now taken by the scheduler. */
  	return 0;
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	portDISABLE_INTERRUPTS();

	/* When using the JTAG debugger the hardware is not always initialised to
	the correct default state.  This line just ensures that this does not
	cause all interrupts to be masked at the start. */
	AT91C_BASE_AIC->AIC_EOICR = 0;

	/* Most setup is performed by the low level init function called from the
	startup asm file. */

	/* Enable the peripheral clock. */
    AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_PIOA;
    AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_PIOB;

	/* Initialise the LED outputs for use by the demo application tasks. */
	vParTestInitialise();

}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
static unsigned long ulCallCount = 0, ulErrorFound = pdFALSE;

/* The rate at which LED D4 will toggle if an error has been found in one or
more of the standard demo tasks. */
const unsigned long ulErrorFlashRate = 500 / portTICK_PERIOD_MS;

/* The rate at which LED D4 will toggle if no errors have been found in any
of the standard demo tasks. */
const unsigned long ulNoErrorCheckRate = 5000 / portTICK_PERIOD_MS;

	ulCallCount++;

	if( ulErrorFound != pdFALSE )
	{
		/* We have already found an error, so flash the LED with the appropriate
		frequency. */
		if( ulCallCount > ulErrorFlashRate )
		{
			ulCallCount = 0;
			vParTestToggleLED( mainERROR_LED );
		}
	}
	else
	{
		if( ulCallCount > ulNoErrorCheckRate )
		{
			ulCallCount = 0;

			/* We have not yet found an error.  Check all the demo tasks to ensure
			this is still the case. */

			if( xAreBlockingQueuesStillRunning() != pdTRUE )
			{
				ulErrorFound |= 0x01;
			}

			if( xAreBlockTimeTestTasksStillRunning() != pdTRUE )
			{
				ulErrorFound |= 0x02;
			}

			if( xAreDynamicPriorityTasksStillRunning() != pdTRUE )
			{
				ulErrorFound |= 0x04;
			}

			if( xAreGenericQueueTasksStillRunning() != pdTRUE )
			{
				ulErrorFound |= 0x08;
			}

			if( xAreQueuePeekTasksStillRunning() != pdTRUE )
			{
				ulErrorFound |= 0x10;
			}

			vParTestToggleLED( mainERROR_LED );
		}
	}
}
