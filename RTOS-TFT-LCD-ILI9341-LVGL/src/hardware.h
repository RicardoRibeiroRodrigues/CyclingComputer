/*
 * hardware.h
 *
 * Created: 11/28/2022 9:59:01 PM
 *  Author: riywa
 */ 


#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "asf.h"

#define SENSOR_PIO PIOA
#define SENSOR_PIO_ID ID_PIOA
#define SENSOR_IDX 19
#define SENSOR_IDX_MASK (1 << SENSOR_IDX)

#define RTT_FREQ 5000

extern QueueHandle_t xPulseQueue;


void RTT_init(float freqPrescale, uint32_t IrqNPulses, uint32_t rttIRQSource);
void init_pins(void);



#endif /* HARDWARE_H_ */