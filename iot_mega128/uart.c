/*
 * uart.c
 * http://www.kccistc.net/
 * Created: 2019-09-02 오전 7:52:06
 *  Author: auto
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

#define BUFSIZE 100
volatile int rx0flag;
volatile int rx1flag;
char rx0data[BUFSIZE];
char rx1data[BUFSIZE];

void UART0_init(void)
{
	UCSR0B |= _BV(RXEN0);		// 송수신 가능
	UCSR0B |= _BV(TXEN0);
	UCSR0B |= _BV(RXCIE0);	//RX Complete Interrupt Enable
	// 비동기, 8비트 데이터, 패리티 없음, 1비트 정지 비트 모드
//	UCSR0C |= 0x06;
	UCSR0C |= _BV(UCSZ01); // 1 << 1
	UCSR0C |= _BV(UCSZ00); // 1 << 2	
	UBRR0H = 0;			// 115200 보율로 설정
	UBRR0L = 8;
}
void UART1_init(void)
{
	UCSR1B |= _BV(RXEN1);		// 송수신 가능
	UCSR1B |= _BV(TXEN1);
	UCSR1B |= _BV(RXCIE1);	//RX Complete Interrupt Enable
	// 비동기, 8비트 데이터, 패리티 없음, 1비트 정지 비트 모드
	//	UCSR0C |= 0x06;
	UCSR1C |= _BV(UCSZ11); // 1 << 1
	UCSR1C |= _BV(UCSZ10); // 1 << 2
	UBRR1H = 0;			// 115200 보율로 설정
	UBRR1L = 8;
}
ISR(USART0_RX_vect)
{
	static int i=0;
	rx0data[i] = UDR0;
	if(rx0data[i] == '\r')
	{
		rx0data[i]='\0';
		rx0flag = 1;
		i=0;
	}
	else
		i++;
}
/*ISR(USART1_RX_vect)
{
	static int i=0;
	rx1data[i] = UDR1;
	if(rx1data[i] == '\n')  //0x0d0x0a : CR,LF 수신 문자열 끝
	{
		rx1data[i+1] = '\0';
		rx1flag = 1;
		i=0;
	}
	else
	{
		i++;
		if(i==BUFSIZE)
		{
			rx1data[i-1] = '\0';
			rx1flag = 1;
			i=0;		
		}
	}
}
*/
void UART0_transmit(char data)
{
	while( !(UCSR0A & (1 << UDRE0)) );	// 송신 가능 대기
	UDR0 = data;				// 데이터 전송
}
void UART0_string(char * str)
{
	while(*str != 0)
	{
		UART0_transmit(*str);
		str++;
	}
}
void UART1_transmit(char data)
{
	while( !(UCSR1A & (1 << UDRE1)) );	// 송신 가능 대기
	UDR1 = data;				// 데이터 전송
}
void UART1_string(char * str)
{
	while(*str != 0)
	{
		UART1_transmit(*str);
		str++;
	}
}
unsigned char UART0_receive(void)
{
	while( !(UCSR0A & (1<<RXC0)) );	// 데이터 수신 대기
		return UDR0;
/*	if(UCSR0A & (1<<RXC0))
		return UDR0;
	else
		return 0;
*/
}