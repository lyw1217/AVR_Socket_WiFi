/*
 * main.c
 * http://www.kccistc.net/
 * Created: 2019-09-02 오전 7:52:06
 * Author : shkim
 */ 

#include <avr/io.h>
#include "wifi.h"
#include "adc.h"

#define F_CPU 16000000UL		/* Define CPU Frequency e.g. here its Ext. 16MHz */
#include <avr/io.h>			/* Include AVR std. library file */
#include <util/delay.h>			/* Include Delay header file */
#include <stdbool.h>			/* Include standard boolean library */
#include <string.h>			/* Include string library */
#include <stdio.h>			/* Include standard IO library */
#include <stdlib.h>			/* Include standard library */
#include <avr/interrupt.h>		/* Include avr interrupt header file */
#include "uart.h"		/* Include USART header file */

#define CLIENT_ID  "LYW_AVR"
#define CLIENT_PW  "PASSWD"
#define PARR_CNT 6

void Wifi_init();
FILE OUTPUT = FDEV_SETUP_STREAM((void *)UART0_transmit, NULL, _FDEV_SETUP_WRITE);
extern volatile int cdsValue;
extern volatile int adc0Flag;
volatile unsigned long systic_count;
volatile unsigned long systic_sec;
void TIMER0_init() {
	TCCR0 = 0x0E; //CTC mode(2), OC0 disable ,divide:256, 16Mhz/256 = 16us * 250 = 4ms
	ASSR = 0x00;
	OCR0 = 249;
	TIMSK = 0xff;
	TCNT0 = 0;
}
ISR(TIMER0_COMP_vect)
{
	systic_count++;
	if(!(systic_count%250)) { //1sec
		systic_sec++;
		if(systic_sec >= 60) systic_sec = 0;
//		PORTA ^= 0x01;
	}
}
int main(void)
{
	int i = 0;
	char _buffer[100];
	char *pToken;
	char *pArray[PARR_CNT]={0};
	char senddata[100]={0};
	int getSensorTime=0;	
	unsigned long systic_sec_old=0;
	char getSensorId[20];

	UART0_init();			/* Initiate USART with 115200 baud rate */
	UART1_init();			/* Initiate USART with 115200 baud rate */
	CDS_init();
	TIMER0_init();
	sei();					/* Start global interrupt */
	stdout = &OUTPUT;

	UART0_string("main start!!\r\n");
	Wifi_init();
	DDRA |= 0x01; //PA0 output
	CDS_start();
	while(1)
	{
		if(systic_sec != systic_sec_old) {
//			adc0Flag = 0;
			CDS_start();
			systic_sec_old = systic_sec;
		}
		memset(_buffer, 0, sizeof(_buffer));
		Read_Data(_buffer);		//수신포멧 :  \r\n+IPD,6:hello  끝문자 0x0a
		if(!strncmp(_buffer,"\r\n+IPD",4))			
		{
			senddata[0] = '\0';
			UART0_string(_buffer);	UART0_string("\r\n");
			i = 0;				
			memset(pArray,0x0,sizeof(pArray));
			pToken = strtok(_buffer,"[@]");
			while(pToken != NULL)
			{
				pArray[i] =  pToken;
				if(++i >= PARR_CNT)
					break;
				pToken = strtok(NULL,"[@]");
			}
			//pArray[0] : \r\n+IPD,문자열 길이
			//pArray[1] : 송신 ID
			//pArray[2] : 송신 문자열1
			//pArray[3] : 송신 문자열2..
		
			// senddata form : [CLIENT_ID]destination ID@string1@string2...
			
			//UART0_string(senddata);// UART0_string("\r");
			if(!strcmp(pArray[2],"LED"))
			{
				if(!strcmp(pArray[3],"ON"))
				PORTA |= 0x01;
				else
				PORTA &= ~0x01;
				sprintf(senddata,"[%s]%s@%s@%s",CLIENT_ID,pArray[1],pArray[2], pArray[3]);
			}else if(!strcmp(pArray[2],"GETSENSOR")) {
				if(pArray[3] != NULL) {
					getSensorTime = atoi(pArray[3]);
					sprintf(senddata,"[%s]%s@%s@%s",CLIENT_ID,pArray[1],pArray[2], pArray[3]);
				} else {
					getSensorTime = 0;
					sprintf(senddata,"[%s]%s@%s@%d",CLIENT_ID,pArray[1],pArray[2], cdsValue);
				}
				strcpy(getSensorId,pArray[1]);
			} else if(!strcmp(pArray[3], "CONDITION")) {
				CDS_start();
				sprintf(senddata,"[%s]%s@%d@%d",pArray[2],pArray[3], cdsValue, systic_sec);
			}
			
			if(senddata[0] != '\0')	{
				UART0_string(senddata);// UART0_string("\r");
				if(ESP8266_Send(senddata) != ESP8266_RESPONSE_FINISHED)
				Wifi_init();
			}
		}
		if(getSensorTime !=0 && !(systic_sec % getSensorTime) && (systic_sec != systic_sec_old)) {
			sprintf(senddata,"[%s]%d@%d",getSensorId,getSensorTime,cdsValue);
			UART0_string(senddata);	UART0_string("\r\n");
			ESP8266_Send(senddata);
		}
	}
}

void Wifi_init()
{	
	while(!ESP8266_Begin());
	ESP8266_WIFIMode(BOTH_STATION_AND_ACCESPOINT);/* 3 = Both (AP and STA) */
	ESP8266_ConnectionMode(SINGLE);		/* 0 = Single; 1 = Multi */
	ESP8266_ApplicationMode(NORMAL);	/* 0 = Normal Mode; 1 = Transperant Mode */
	if(ESP8266_connected() == ESP8266_NOT_CONNECTED_TO_AP)
		ESP8266_JoinAccessPoint(SSID, PASSWORD);
	ESP8266_Start(0, DOMAIN, PORT);
	_delay_ms(50);
	ESP8266_Send("["CLIENT_ID"]sign@"CLIENT_PW"");
}