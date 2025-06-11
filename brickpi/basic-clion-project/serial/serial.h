//
// Created by valy on 11/22/24.
//

#ifndef SERIAL_H
#define SERIAL_H
#include <stdint.h>


void xSerialPortInitMinimal(uint32_t baudrate, uint32_t queue_size);
void vSerialPutString(void *serial, uint8_t* buff, uint32_t len);

#endif //SERIAL_H
