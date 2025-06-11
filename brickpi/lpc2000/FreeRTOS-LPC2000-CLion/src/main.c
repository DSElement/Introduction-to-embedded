#include <lpc22xx.h>
#include <FreeRTOS.h>
#include <serial.h>
#include <task.h>
#include <stdio.h>
#include <string.h>
#include "log.h"

void vApplicationStackOverflowHook( TaskHandle_t xTask, char * pcTaskName )
{
	LOG_SERIAL("Stack overflow for task: %s\r\n", pcTaskName);
}

void vApplicationMallocFailedHook( void )
{
	unsigned int i = 0;
	while (1)
	{
		IO0SET = 1u << 30;
		for (i = 0; i < 100000; i++);
		IO0CLR = 1u << 30;
		for (i = 0; i < 100000; i++);
	}
}

void TaskTest1(void *pvParameters)
{
	static unsigned int n = 0;
	for (;;)
	{
		n++;
		if (n % 2)
		{
			IO0SET = 1u << 30;
		}
		else
		{
			IO0CLR = 1u << 30;
		}
		vTaskDelay(500);
	}
}

void vLEDFlashTask(void * pvParameters)
{
	static unsigned int n = 0;
	static char buffer[128];
	uint32_t nnn = 123;
	for (;;)
	{
		n++;
		/*if (n % 2)
		{
			IO0SET = 1u << 30;
		}
		else
		{
			IO0CLR = 1u << 30;
		}*/

		LOG_SERIAL("Hello World! FreeRTOS Version %s -> nr %d %ld %ld\n", tskKERNEL_VERSION_NUMBER, n, xPortGetFreeHeapSize(), uxTaskGetStackHighWaterMark(NULL));
		vTaskDelay(500);
	}
}

void set_pll(void)
{
	PLLCON = 0x01;
	PLLCFG = 0x23;
	PLLFEED = 0xAA;
	PLLFEED = 0x55;
	while ((PLLSTAT & (1 << 10)) == 0);
	PLLCON = 0x03;
	PLLFEED = 0xAA;
	PLLFEED = 0x55;
	VPBDIV = 0;
}




int main(void)
{
	volatile int a = 4;
	set_pll();

	MAMCR = 2;
	MAMTIM = 4;

	PINSEL1 &= ~((1 << 29) | ( 1 << 28 ));
	PINSEL0 |= 5;
	IO0DIR |= 1 << 30; //P0.30 output
	LOG_init();

	xTaskCreate( vLEDFlashTask, "LEDx", 200, NULL, 2, ( TaskHandle_t * ) NULL );
	xTaskCreate( TaskTest1, "Test1", 10, NULL, 2, ( TaskHandle_t * ) NULL );
	vTaskStartScheduler();


	while(1);

	return 0;
}


