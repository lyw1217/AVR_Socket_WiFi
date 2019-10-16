/*
 * wifi.h
 * http://www.kccistc.net/
 * Created: 2019-09-02 오전 7:52:06
 *  Author: shkim
 */ 


#ifndef WIFI_H_
#define WIFI_H_
#include <stdbool.h>			/* Include standard boolean library */

#define SREG    _SFR_IO8(0x3F)

#define DEFAULT_BUFFER_SIZE		110
#define DEFAULT_TIMEOUT			10000

/* Connection Mode */
#define SINGLE				0
#define MULTIPLE			1

/* Application Mode */
#define NORMAL				0
#define TRANSPERANT			1

/* Application Mode */
#define STATION				1
#define ACCESSPOINT			2
#define BOTH_STATION_AND_ACCESPOINT	3

/* Select Demo */
#define RECEIVE_DEMO			/* Define RECEIVE demo */
#define SEND_DEMO			/* Define SEND demo */

/* Define Required fields shown below */
#define DOMAIN				"mvl100d.ddns.net"
#define PORT				"8888"

#define SSID				"KCCI_STC_S"
#define PASSWORD			"kcci098#"

enum ESP8266_RESPONSE_STATUS{
	ESP8266_RESPONSE_WAITING,
	ESP8266_RESPONSE_FINISHED,
	ESP8266_RESPONSE_TIMEOUT,
	ESP8266_RESPONSE_BUFFER_FULL,
	ESP8266_RESPONSE_STARTING,
	ESP8266_RESPONSE_ERROR
};

enum ESP8266_CONNECT_STATUS {
	ESP8266_CONNECTED_TO_AP,
	ESP8266_CREATED_TRANSMISSION,
	ESP8266_TRANSMISSION_DISCONNECTED,
	ESP8266_NOT_CONNECTED_TO_AP,
	ESP8266_CONNECT_UNKNOWN_ERROR
};

enum ESP8266_JOINAP_STATUS {
	ESP8266_WIFI_CONNECTED,
	ESP8266_CONNECTION_TIMEOUT,
	ESP8266_WRONG_PASSWORD,
	ESP8266_NOT_FOUND_TARGET_AP,
	ESP8266_CONNECTION_FAILED,
	ESP8266_JOIN_UNKNOWN_ERROR
};
extern void UART1_string(char * str);
void Read_Response(char* _Expected_Response);
void ESP8266_Clear();
void Start_Read_Response(char* _ExpectedResponse);
void GetResponseBody(char* Response, uint16_t ResponseLength);
bool WaitForExpectedResponse(char* ExpectedResponse);
bool SendATandExpectResponse(char* ATCommand, char* ExpectedResponse);
bool ESP8266_ApplicationMode(uint8_t Mode);
bool ESP8266_ConnectionMode(uint8_t Mode);
bool ESP8266_Begin();
bool ESP8266_Close();
bool ESP8266_WIFIMode(uint8_t _mode);
uint8_t ESP8266_JoinAccessPoint(char* _SSID, char* _PASSWORD);
uint8_t ESP8266_connected();
uint8_t ESP8266_Start(uint8_t _ConnectionNumber, char* Domain, char* Port);
uint8_t ESP8266_Send(char* Data);
int16_t ESP8266_DataAvailable();
uint8_t ESP8266_DataRead();
uint16_t Read_Data(char* _buffer);

#endif /* WIFI_H_ */