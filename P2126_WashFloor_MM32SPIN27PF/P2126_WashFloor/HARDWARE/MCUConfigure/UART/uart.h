#ifndef _UART_H_
#define _UART_H_

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"
#include "config.h"


//#define BLDC_UART_MODE 0 // BLDC老版本协议使能开关

//***************************UART************************************
#define	UART_TX_30MS						3						//unit:10ms		5 means 50ms
#define	UART_TX_50MS						5						//unit:10ms		5 means 50ms
#define	UART_TX_100MS						10					//unit:10ms		105 means 100ms


typedef enum{
	HEAD=0,
	ID,
	LENTH,
	TxDATA1,
	TxDATA2,
	TxDATA3,
	TxDATA4,
	TxDATA5,
	TxDATA6,
	TxDATA7,
	TxDATA8,
	TxDATA9,
	TxDATA10,
	TxDATA11,
	TxDATA12,
	TxDATA13,
	TxDATA14,
	TxDATA15,
	TxDATA16,
	TxDATA17,
	TxDATA18,
	TxDATA19,
	TxDATA20,
	TxDATA21,//
	TxDATA22,//
	TxDATA23,
	TxDATA24,
	TxDATA25
}TxDataSeq;

typedef enum{
	RxDATA1=3,
	RxDATA2,
	RxDATA3,
	RxDATA4,
	RxDATA5,
	RxDATA6,
	RxDATA7,
	RxDATA8,
	RxDATA9,
	RxDATA10
}RxDataSeq;

void UART1_Init(u32 bound);
void UART2_Init(u32 bound);
void UART2_Service(void);
void UART1_Service(void);
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);
void MYDMA_Disable(DMA_Channel_TypeDef*DMA_CHx);
void Deal_UartComm(void);

#endif

