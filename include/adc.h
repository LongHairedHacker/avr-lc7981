#ifndef ADC_H
#define ADC_H ADC_H

#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>


#define ADC_READ_CYCELS 5

uint16_t readADC(uint8_t channel);

#endif /* ADC_H */
