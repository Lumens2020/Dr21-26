#ifndef _CONFIG_H_
#define _CONFIG_H_

// 地刷电流采样电阻设置 单位mΩ
#define Brush_R             50//100

// 电解水测试使能
#define WaterElecTestEnable 0 

// 0:自动模式 吸水模式倾倒工作前3S污水箱未安装1S消抖检测，后,5S消抖检测
// 1:自动模式 吸水模式倾倒工作前5S污水箱未安装1S消抖检测，后不检测
// 2:自动模式 吸水模式倾倒工作污水箱未安装1S消抖检测
// 污水箱水满一直2S消抖检测
#define SEWAGE_CHECK_MODE  1

// 1: LCD屏幕
// 0: LED屏幕
#define LCDScreenEnableE   1

//#define YKLLCD           1

// 寿命测试使能
#define TEST_LIFE_ENABLE   0 

// 1: 最新烘干通讯协议 20211101
// 0: 老通讯协议，故障拉低电平
#define DRY_BUS_MODE       1

// 0:最新电磁泵
// 1:旧电磁泵
// 2:蠕动泵样品
// 3:隔膜泵测试  730hz 23%
#define NEW_WaterPump      0

// BLDC老版本协议使能开关
#define BLDC_UART_MODE     0

// 充电检测模拟口或者数字口设置
#define CHG_AD_ENABLE      1

// 测试故障记录保留使能
#define TEST_AD_ENABLE     1


// 脏污显示3种色环使能
#define SEWAGE_3LED_ENABLE 1


#endif



