/*
 * myCroutineApp.c
 *  Created on: Nov 14, 2018
 */
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "lcd.h"

#define NULL ((void *) 0)
#define PRIORITY_0 0
#define NUM_COROUTINES 2

const int iFlashRates[ NUM_COROUTINES ] = { 500, 800 };
const int iLEDToFlash[ NUM_COROUTINES ] = { 0, 1 };

void myFirstCorutine( CoRoutineHandle_t xHandle, UBaseType_t uxIndex )
{
   // Co-routines must start with a call to crSTART().
   crSTART( xHandle );

   static int x, xold, y, yold;
   y=yold=0;
   x=xold=0;
   InitLCD();
   LCDSettings();
   LCDClearScreen();
   for( ;; )
   {
	   if(uxIndex == 0)
	   {
		   LCDPutStr("T1", xold, 10, LARGE, BLACK, BLACK);
		   LCDPutStr("T1", ++x, 10, LARGE, WHITE, BLACK);
		   if(x>135){
			   x=-3;
		   }
		   xold=x;
		   crDELAY( xHandle, 600);
	   }else {
		   LCDPutStr('T1', yold, 60, LARGE, BLACK, BLACK);
		   LCDPutStr('T1', ++y, 60, LARGE, WHITE, BLACK);
		   if(y>135){
		   	 y=-3;
		   }
		   yold=y;
		   crDELAY( xHandle, 1100);
	   }

   // Co-routines must end with a call to crEND().
   crEND();
}
}

void vApplicationIdleHook( void )
{
   for( ;; )
   {
      vCoRoutineSchedule();
   }
}

void startMyApp( void )
{
	xCoRoutineCreate( myFirstCorutine, PRIORITY_0, 0 );
	xCoRoutineCreate( myFirstCorutine, PRIORITY_0, 1 );
}
