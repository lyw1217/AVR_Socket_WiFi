/*
 * adc.c
 *
 * Created: 2019-09-30 오후 2:32:07
 *  Author: k
 */ 
#define F_CPU 16000000UL		/* Define CPU Frequency e.g. here its Ext. 16MHz */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "adc.h"
volatile int cdsValue;
volatile int adc0Flag;
void CDS_init() {
	DDRF &= ~0x01;  //PF0 : ADC0
	ADCSRA = 0x9c;	//ADC Enable, 500KHz, interrupt enable
	ADMUX = 0x40;
	_delay_us(150);
}
void CDS_start() {
	//ADMUX = 0x40;
	ADCSRA |= 0x40;
}
ISR(ADC_vect) {
	if(ADMUX == 0x40) {
		cdsValue = ADC;  //ADCL + ADCH*256
		adc0Flag = 1;
	}
}