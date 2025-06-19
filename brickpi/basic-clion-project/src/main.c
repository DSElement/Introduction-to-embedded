#include <log.h>
#include "lpc214x.h"
#include <stdio.h>
#include <stdint.h>
#include "spi.h"
#include <string.h>

#define BRICKPI3_GET_MANUFACTURER    0x01
#define BRICKPI3_GET_NAME            0x02
#define BRICKPI3_GET_HARDWARE_VERSION 0x03
#define BRICKPI3_GET_FIRMWARE_VERSION 0x04
#define BRICKPI3_GET_ID              0x05

void timer_init()
{
	T0MR0 = 1200000UL;  //the value to be divided by pclk
	T0MCR = 3;
	T0TCR = 1;
}

void send_x(unsigned char c)
{
	while((U0LSR&(1<<5))==0);
	U0THR=c;
}

char read_x()
{
	while( (U0LSR & 0x01) == 0);	/*Wait till RDR bit becomes 1 which tells that reciver contains valid data */
	return U0RBR;
}

void send_string(const char *s)
{
	while (*s) {
		send_x(*s++);
	}
}

//blinks led once every 4 interrupts
__attribute__((interrupt("FIQ"))) void my_interrupt(void)
{
	static volatile uint32_t n = 0;
	if (n % 4)
	{
		IO0SET = 1u << 31;
	}
	else
	{
		IO0CLR = 1u << 31;
	}
	n++;
	T0IR |= 1;
	VICVectAddr = 0;
}


void init_interrupt(void)
{
	VICIntEnable |= 1 << 4;
	VICIntSelect &= ~(1 << 4);
	VICVectAddr0 = (int32_t)my_interrupt;
	VICVectCntl0 = 4 | ( 1 << 5);
}

void init_interrupt_fiq(void)
{
	VICIntEnable |= 1 << 4;		//enable the timer
	VICIntSelect |= 1 << 4;		//timer will be fiq
}

/*
sets the CCLK to 48 MHz and PCLK to 12 MHz

SPI clock = PCLK / (CPSDVSR × (SCR + 1))
To achieve 500kbps with PCLK =12MHz:
Use, for example, CPSDVSR = 24, SCR = 0 → 12MHz / 24 = 500kHz
That’s a perfect fit—no need to increase PCLK.
*/
void set_pll(void)
{
	PLL0CON = 0x01;
	PLL0CFG = 0x23;		//M = 4 (i.e. actually 3)
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
	while ((PLL0STAT & (1 << 10)) == 0);	// check whether PLL has locked on to the  desired freq
	PLL0CON = 0x03;		//enable & connect pll
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
	VPBDIV = 0;			//PCLK = 1/4 of CCLK
}

// Helper functions
void print_hex(uint8_t val) {
    send_x((val >> 4) < 10 ? '0' + (val >> 4) : 'A' + (val >> 4) - 10);
    send_x((val & 0x0F) < 10 ? '0' + (val & 0x0F) : 'A' + (val & 0x0F) - 10);
}

void print_buffer_hex(uint8_t* buf, uint8_t len) {
    for(int i = 0; i < len; i++) {
        print_hex(buf[i]);
        send_x(' ');
    }
}

// BrickPi3 transaction function - simplified approach
uint8_t brickpi3_transaction(uint8_t command, uint8_t* response, uint8_t max_response_len) {

    spi_start();

    // Method 1: Try the immediate response approach
    uint8_t addr_response = spi_transfer(0x01);  // Send address
    uint8_t cmd_response = spi_transfer(command); // Send command

    send_string("Sent: cmd=0x");
    print_hex(command);
    send_string(", immediate responses: 0x");
    print_hex(addr_response);
    send_string(" 0x");
    print_hex(cmd_response);
    send_string("\n");

    // Continue reading to see if we get ASCII data directly
    send_string("Reading more bytes: ");
    for(int i = 0; i < 16 && i < max_response_len; i++) {
        response[i] = spi_transfer(0x00);
        if(response[i] >= 0x20 && response[i] <= 0x7E) {  // Printable ASCII
            send_x(response[i]);
        } else {
            send_string("[0x");
            print_hex(response[i]);
            send_string("]");
        }

        // Stop if we hit a null or get several zeros in a row
        if(response[i] == 0x00) {
            response[i] = '\0';
            send_string("\nFound null terminator at position ");
            send_x('0' + i);
            send_string("\n");
            spi_stop();
            return i;
        }
    }
    response[16] = '\0';
    send_string("\n");

    spi_stop();

    // Method 2: Try a completely different approach - restart communication
    send_string("Trying method 2...\n");
    for(volatile int delay = 0; delay < 50000; delay++);

    spi_start();

    // Send just the command byte and see what happens
    uint8_t simple_response = spi_transfer(command);
    send_string("Simple cmd response: 0x");
    print_hex(simple_response);

    // Try reading a few more bytes
    send_string(" followed by: ");
    for(int i = 0; i < 8; i++) {
        uint8_t byte = spi_transfer(0x00);
        print_hex(byte);
        send_x(' ');
        if(byte >= 0x20 && byte <= 0x7E && i < max_response_len) {
            response[i] = byte;
        }
    }
    send_string("\n");

    spi_stop();

    return 0;  // Return 0 for now since we're debugging
}

int main(void)
{
	volatile unsigned int i = 0;
	volatile unsigned int j = 0;
	volatile int a = 4;
	unsigned char b = 'U';
	set_pll();
	LOG_init();
	//IODIR0 |= (1 << 31);

	spi_init();

	// Wait for BrickPi3 to fully boot
	send_string("Waiting for BrickPi3 boot...\n");
	for(volatile int delay = 0; delay < 3000000; delay++);

	uint8_t response[32];
	uint8_t len;

	// Test 1: Get Manufacturer
	send_string("\n=== GET_MANUFACTURER ===\n");
	len = brickpi3_transaction(BRICKPI3_GET_MANUFACTURER, response, sizeof(response));
	if(len > 0) {
		send_string("Manufacturer: ");
		send_string((char*)response);
		send_string("\n");
	}

	// Wait between commands
	for(volatile int delay = 0; delay < 500000; delay++);

	// Test 2: Get Name
	send_string("\n=== GET_NAME ===\n");
	len = brickpi3_transaction(BRICKPI3_GET_NAME, response, sizeof(response));
	if(len > 0) {
		send_string("Name: ");
		send_string((char*)response);
		send_string("\n");
	}

	// Wait between commands
	for(volatile int delay = 0; delay < 500000; delay++);

	// Test 3: Get Hardware Version
	send_string("\n=== GET_HARDWARE_VERSION ===\n");
	len = brickpi3_transaction(BRICKPI3_GET_HARDWARE_VERSION, response, sizeof(response));
	if(len > 0) {
		send_string("Hardware Version: ");
		send_string((char*)response);
		send_string("\n");
	}

	// Wait between commands
	for(volatile int delay = 0; delay < 500000; delay++);

	// Test 4: Get Firmware Version
	send_string("\n=== GET_FIRMWARE_VERSION ===\n");
	len = brickpi3_transaction(BRICKPI3_GET_FIRMWARE_VERSION, response, sizeof(response));
	if(len > 0) {
		send_string("Firmware Version: ");
		send_string((char*)response);
		send_string("\n");
	}

	send_string("\n=== BrickPi3 Communication Test Complete ===\n");

	while(1) {
		// Main loop
		for(volatile int delay = 0; delay < 1000000; delay++);
	}
}