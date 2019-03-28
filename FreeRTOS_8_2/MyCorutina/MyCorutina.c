/*
 * MyCorutina.c
 *
 *  Created on: Nov 19, 2018
 *      Author: student
 */

#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "lcd.h"
#include "partest.h"


void vFlashCoRoutine( CoRoutineHandle_t xHandle, UBaseType_t uxIndex )
{
	crSTART(xHandle);
	for(;;){
		vParTestToggleLED(0);
		crDELAY(xHandle,600);
	}
	crEND();
}

void vScrollCorutina(CoRoutineHandle_t xHandle, UBaseType_t uxIndex)
{
	static int xold = 10;
	static int x = 10;
	crSTART(xHandle);
	for(;;){
		LCDPutStr("Rares",xold,10,LARGE,BLACK,BLACK);
		LCDPutStr("Rares",++x,10,LARGE,WHITE,BLACK);
		x=xold;
		if(x>135) x=-5;
		crDELAY(xHandle,400);
	}
	crEND();
}
void vApplicationIdleHook(void)
{
	for(;;)
	{
		vCoRoutineSchedule(void);
	}
}

void MyStart(){
	// aici se creeaza corutinele
	xCoRoutineCreate(vFlashCoRoutine,0,0);
	xCoRoutineCreate(vScrollCorutina,0,1);
}
