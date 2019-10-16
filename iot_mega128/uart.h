/*
 * uart.h
 *
 * Created: 2018-12-07 오후 2:20:46
 *  Author: auto
 */ 


#ifndef UART_H_
#define UART_H_

void UART0_init(void);
void UART0_transmit(char data);
unsigned char UART0_receive(void);
void UART0_string(char * str);
void UART1_init(void);
void UART1_transmit(char data);
void UART1_string(char * str);


#endif /* UART_H_ */