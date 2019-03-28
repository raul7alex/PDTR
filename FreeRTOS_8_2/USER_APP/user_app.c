
#include "user_app.h"

volatile float tempReala=0;
volatile int trimitere=0, buton_apasat=0; // 0 nu se trimite, 1 se trimite

void vStartUSERTask( unsigned portBASE_TYPE uxPriority )
{
	xTaskCreate( vButtonsTask, "buttons", configMINIMAL_STACK_SIZE, NULL, uxPriority, NULL);
	xTaskCreate( vDisplayTask, "display", configMINIMAL_STACK_SIZE, NULL, uxPriority, NULL);
	xTaskCreate( vAcquireTask, "acquire", configMINIMAL_STACK_SIZE, NULL, uxPriority, NULL);
}

static void vButtonsTask( void *pvParameters )
{
	( void ) pvParameters; // The parameters are not used in this task.
	unsigned long PIOB_Status;
    portENTER_CRITICAL();
    	/* Init */
		InitLCD();
		LCDSettings();
		LCDWrite130x130bmp();
    portEXIT_CRITICAL();
	for( ;; )
	{
		if (PIOB_Status != AT91C_BASE_PIOB->PIO_PDSR) // apasat buton
		{
			buton_apasat=1;
			PIOB_Status = AT91C_BASE_PIOB->PIO_PDSR;
			if( !( PIOB_Status & PB1 ) )
				LCDPutStr("LEFT            ",0,0,2,WHITE,BLACK);
			else
				if( !( PIOB_Status & PB2 ) )
					LCDPutStr("           RIGHT",0,0,2,WHITE,BLACK);
		}
		else
			buton_apasat=0;
	}
}

static void vDisplayTask( void *pvParameters )
{
	( void ) pvParameters; // The parameters are not used in this task.
	char str[15];
	int tempInt=0;
	int tempFlotanta=0;
	portTickType xLastWakeTime;
	const portTickType xFrequency = 2000;

    portENTER_CRITICAL();
    	/* Init */
    portEXIT_CRITICAL();
	for( ;; )
	{
		if (trimitere && !buton_apasat)
		{
		  tempInt=tempReala; //k-este variabila cu virgula flotanta - citim paertea intreaga
		  tempFlotanta=(tempReala-tempInt)*100; //citim partea dupa virgula in r
		  LCDPutStr( "Current temp.:", 100,10, LARGE, RED, BLACK );
		  INTtoSTR(tempInt, str);
		  LCDPutStr(str, 80, 40, LARGE, GREEN, BLACK);
		  INTtoSTR(tempFlotanta, str);
		  LCDPutStr(str, 90, 56, SMALL, GREEN, BLACK);
		  LCDPutStr("o", 88, 75, SMALL, BLUE, BLACK);
		  LCDPutStr("C", 80, 81, LARGE, BLUE, BLACK);
		  trimitere = 0;
		}
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
	}
}

static void vAcquireTask( void *pvParameters )
{
	( void ) pvParameters; // The parameters are not used in this task.
	unsigned int buffer;
	portTickType xLastWakeTime;
	const portTickType xFrequency = 350;

    portENTER_CRITICAL();
    	/* Init */
    	InitADC();
    portEXIT_CRITICAL();
	for( ;; )
	{
	    // Initialise the xLastWakeTime variable with the current time.
		if(0 == trimitere){
	      xLastWakeTime = xTaskGetTickCount ();
	      buffer=GetAdcChannel(ADC_CHN_6);//se initializeaza citirea portului
		  tempReala=(buffer*23.76)/512; //convertim valoarea citita in grade celsius
		  trimitere = 1;
		}
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
	}
}

void INTtoSTR( long Number, char * Result )
{
	char Minus;
	unsigned char ucPos = 0;

	if (Number == 0)
	{
		Result[ucPos++] = ( '0' ) ;
	}

	Minus = Number < 0;
	if ( Minus )
		Number = ( (-1) * Number);

	while ( Number > 0 )
	{

		Result[ucPos++] = (Number % 10) + ( (int)('0') ) ;
		Number = ( Number / 10 );
	}

	if ( Minus )
		Result[ucPos++] = '-' ;
	Result[ucPos] = 0x00;

	//reverse the string
	unsigned char ucI = 0;
	ucPos--;

	while ( ucI < ucPos )
	{
		unsigned char ucTemp;
		ucTemp = Result[ ucPos ];
		Result[ ucPos ] = Result[ ucI ];
		Result[ ucI ] = ucTemp;
		ucI++;
		ucPos--;
	}
}
