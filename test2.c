#include <stdio.h>
#include <stdint.h>
#include <lpc22xx.h>
#include "UART.h"
#include "TIMER.h"
#include "I2C_COMMUNICATION.h"
#include "lidar.h"
#include "DEBUG_HEADER.h"
#include "API.h"
#include <math.h>

/***********************************************************************************
Global variables
************************************************************************************/

Measurement last_measurements_buffer[1000] = {0};

/***********************************************************************************
This function is executed 1 time at power on or at reset.
************************************************************************************/

void setup_function(void)
{
	initUART();
	init_timer0();
	init_timer0_interrupt();
	init_timer1();
	init_timer1_interrupt();
	I2C_init();
	
	// SET P0.7 as GPIO output
	// used for experimental results
	// will measure following times
	// LIDAR_start_measurement call -> I2C start call
	// the time when I2C actually started -> the time we get a result on I2C
	PINSEL0 &= ~((1 << 15) | ( 1 << 14 ));
	IO0DIR |= 1 << 7;
	IO0CLR |= (1 << 7);

	// SET P0.12 as GPIO output
	// used for experimental results
	// will measure following times
	// I2C start call -> I2C actual start
	// I2C result -> API result

	PINSEL0 &= ~((1 << 25) | ( 1 << 24 ));
	IO0DIR |= 1 << 12;
	IO0CLR |= (1 << 12);
}

/***********************************************************************************
Main program
************************************************************************************/



/*
1. init - Lidar_config(6,0); - this is done
   + urmaotarele setari
		- modul de functionare mod1, mod2, mod3
- pe i2c se initializeaza lidaru
- dupa aia ajunge in starea IDLE


2. 3 moduri de functionare
mod1 - citeste continuu si pune datele undeva in niste var globale
de intrebat daca salvez toate masuratorile (intr-un array maybe) sau afisez in continuu ultima
masuratoare

urmeaza sa testez daca merge

mod2 - sta idle si face masuratoare la cerere - this is done
	int LIDAR_read_one_measurement()
			- STARTED_MESUREMENT
			- NOT_READY_YET
			- READY - userul isi poate lua datele din var globale prin intermediul unei functii API

mod3 - prin PWM

3. API
- change config - ARG noul config - de testat
- change bias - ARG noul bias - de testat
- change mode - ARG noul mod - de testat

probabil va trebui sa impart si functia de config in trei functii diferite

- get_lidar_driver_state
		- NOT_STARTED
		- INITIALIZING
		- MEASURING
		- IDLE
		
- int LIDAR_read_one_measurement() - this is done
- start driver - this is done
- start measurment (pentru continous measurment) - this is done
- stop measurment (pentru continous measurment) - this is done
- mai multe (?) functii de luat datele


TASK i2c -

comunare intre ele prin ceva var globale de stare

TASK lidar

*/

/*

USER API: int LIDAR_read_one_measurement()
-- task lidar - new task:  read_measurement --> task i2c new-task(readi2c)- ->response task lidar -> user api



*/

/*
int get_measurement(MEASURE *m)
{
	
}
*/

int main(void)
{	
	LIDAR_config(0);
	LIDAR_bias(1);
	LIDAR_mode(1);
	setup_function();

	LIDAR_start_driver();
	while(lidar_driver_state != IDLE);
	
	
	
//	if(measurements_received == NO_MEASUREMENT) {
//		printf("No measurements available!\n");
//	} else {
//		for(int i = 0; i < measurements_received; i++) {
//			printf("Distance in hex: %04X\n", last_measurements_buffer[i].full_measurement);
//			printf("Distance in dec: %d\n", last_measurements_buffer[i].full_measurement);
//			printf("Distance low byte: %02X\n", last_measurements_buffer[i].low_byte);
//			printf("Distance high byte: %02X\n", last_measurements_buffer[i].high_byte);
//		}
//	}
	
	
	while(1)
	{
		LIDAR_start_measurment();
		for(int i = 0; i < 10000000; i++);
		LIDAR_stop_measurment();
		while(lidar_driver_state != IDLE && lidar_task != NO_TASK);
		for(int i = 0; i < 5000000; i++);
		
		int measurements_received = LIDAR_get_last_measurements(5, last_measurements_buffer);
		for(int i = 0; i < 1000000; i++);
	}
	
	return 0;
}
