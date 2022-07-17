#ifndef __IMPROVE_H__
#define __IMPROVE_H__
#include "main.h"

extern uint32_t UART1SendRespondCnt;
extern uint32_t UART2SendRespondCnt;
extern uint32_t BmsCommSucClearErrCnt;
extern uint32_t MonobusCommSucClearErrCnt;

void error_clear(void);
void bms_change_baudrate_clear(void);
void monobus_change_baudrate_clear(void);
void monobus_comm_succeed_clear_error(void);
void bms_comm_succeed_clear_error(void);
void baudrate_handle_monobus(void);
void baudrate_handle_bms(void);

#define UART1_COMM_2S_CNT		(2000/UART1_TICKS_INTERVA)// 2s 串口一帧数据接收结束计时
#define UART2_COMM_2S_CNT		(2000/UART1_TICKS_INTERVA)// 2s 串口二帧数据接收结束计时

#define UART_BAUDRATE_CHANGE_CNT  4		//计次改变波特率

#define DEFAULT_BAUDRATE 9600			//初始波特率

#define BAUDRATE_ADD_NUM	100			//递增量



typedef struct{
	uint8_t BmsBaudRateChangeFlag1;
	uint8_t BmsBaudRateChangeFlag2;
	uint8_t BmsBaudRateChangeFlag3;
	uint8_t BmsBaudRateChangeFlag4;
	uint8_t BmsBaudRateChangeFlag5;
	uint8_t BmsBaudRateChangeFlag6;
	uint8_t BmsBaudRateChangeFlag7;
	uint8_t BmsBaudRateChangeFlag8;
	uint8_t BmsBaudRateChangeFlag9;
	uint8_t BmsBaudRateChangeFlag10;
	uint8_t BmsBaudRateChangeFlag11;
	uint8_t BmsBaudRateChangeFlag12;
	uint8_t BmsBaudRateChangeFlag13;

	uint8_t MonobusBaudRateChangeFlag1;
	uint8_t MonobusBaudRateChangeFlag2;
	uint8_t MonobusBaudRateChangeFlag3;
	uint8_t MonobusBaudRateChangeFlag4;
	uint8_t MonobusBaudRateChangeFlag5;
	uint8_t MonobusBaudRateChangeFlag6;
	uint8_t MonobusBaudRateChangeFlag7;
	uint8_t MonobusBaudRateChangeFlag8;
	uint8_t MonobusBaudRateChangeFlag9;
	uint8_t MonobusBaudRateChangeFlag10;
	uint8_t MonobusBaudRateChangeFlag11;
	uint8_t MonobusBaudRateChangeFlag12;
	uint8_t MonobusBaudRateChangeFlag13;
}BaudRateCha;



#endif




