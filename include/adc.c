#include "adc.h"


uint16_t readADC(uint8_t channel) {
	uint16_t result;
	uint8_t i;

	//ADC aktiv, Prescaler 16
	ADCSRA = (1 << ADEN) | (1 << ADPS2);

	//ADC0 kein mutiplexing
	ADMUX = channel;

	// AVcc als Versorgungspannung
	ADMUX |= (1 << REFS0);

	//ADC starten mit Dummyreadout
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));

	result = 0;

	for(i = 0; i < ADC_READ_CYCELS; i++) {
		ADCSRA |= (1 << ADSC);
		while(ADCSRA & (1 << ADSC));
		result += ADCW / ADC_READ_CYCELS;
		_delay_us(1);
	}

	//ADC aus
	ADCSRA &= ~(1 << ADEN);


	return result;
}
