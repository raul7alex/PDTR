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
#include "stdlib.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"

/* Demo application includes. */

#include "lcd.h"
#include "partest.h"
#include "blocktim.h"
#include "flash.h"
#include "QPeek.h"
#include "dynamic.h"

#include "AT91SAM7X256.h"
#include "semphr.h"
/* User application includes. */
// #include "user_app.c"

/* Priorities for the demo application tasks. */
#define mainUSB_PRIORITY					( tskIDLE_PRIORITY + 2 )
#define mainBLOCK_Q_PRIORITY				( tskIDLE_PRIORITY + 1 )
#define mainFLASH_PRIORITY                  ( tskIDLE_PRIORITY + 2 )
#define mainGEN_QUEUE_TASK_PRIORITY			( tskIDLE_PRIORITY )

#define mainUSER_PRIORITY					( tskIDLE_PRIORITY )

/* The LED toggle by the tick hook should an error have been found in a task. */
#define mainERROR_LED						( 3 )

static void prvSetupHardware( void );

int apasat = 0;

#define PB1       (1<<24)	// PB24 = LEFT

SemaphoreHandle_t semafor;

void Corutina(CoRoutineHandle_t xHandle, UBaseType_t uxIndex) {
	crSTART( xHandle );

	unsigned long PIOB_Status;

	for (;;) {
		if (PIOB_Status != AT91C_BASE_PIOB->PIO_PDSR) // verifica apasarea butonului pe placa
		{

			PIOB_Status = AT91C_BASE_PIOB->PIO_PDSR;
			if (!(PIOB_Status & PB1)) {
				vSemaphoreTake(semafor,9999);
				apasat=1;
				vSemaphoreGive(semafor);
		 }
		}
	}

	crEND();
}

void task(void* params)
{

}

//QueueHandle_t queue;

void altaCorutina( CoRoutineHandle_t xHandle,UBaseType_t uxIndex )
{
	crSTART( xHandle );

	//QueueHandle_t xQueueCreate( UBaseType_t uxQueueLength,
	//       UBaseType_t uxItemSize );
	//xTaskCreate(task,"task",100,NULL,mainUSER_PRIORITY+1,NULL);


	for( ;; )
	{
		vSemaphoreTake(semafor,9999);
		int buton=apasat;
		apasat=0;
		vSemaphoreGive(semafor);

		if(buton==1)
		{
			crDELAY( xHandle, 100);
			LCDClearScreen();
			LCDPutStr("Apasat!", 0, 0, 5, WHITE, BLACK);
			crDELAY( xHandle, 100);
			LCDClearScreen();
		}
	}

	crEND();
}

int main( void )
{
	/* Setup any hardware that has not already been configured by the low
	level init routines. */
	prvSetupHardware();

	/* Start the USB demo which is just for the SAM7. */
	// vStartUSBTask( mainUSB_PRIORITY );

	/* Start the user_defined app task */
	//vStartUSERTask( mainUSER_PRIORITY );

	//
	char str[]="Nu e apasat butonul!";
	int xpos=10, ypos=10;
	/* (**) daca nu sunt vizibile constantele definite in lcd.h/if constants from lcd.h are not visible */
	//int Size=0, fColor=0xF00, bColor=0xFFF;

	InitLCD();
	LCDSettings();
	LCDPutStr(str, xpos, ypos, LARGE, WHITE, BLACK);


	semafor = vSemaphoreCreateBinary();
	xCoRoutineCreate( Corutina, 0, 0 );
	xCoRoutineCreate( altaCorutina, 0, 0 );

	/* sau vezi/or see (**) */
	// LCDPutStr(str, xpos, ypos, Size, fColor, bColor);

	/* Eg. how to start the standard demo tasks: */
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

void vApplicationIdleHook( void )
{
	vCoRoutineSchedule();
}

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
