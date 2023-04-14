#ifndef _MS5611_H
#define _MS5611_H
#include "stm32f4xx.h"
#include "stdbool.h"
#include <math.h>
typedef struct
{
	u8 OffsetReq;
	u8 OffsetFinished;
	u32 ut;
	u32 up;
	u16 prom[8]; //内部标准数据
	u32 temperature;
	u32 pressure;
	u32 offset_pa;
}MS5611_Typedef;
typedef struct
{
	double now;
	double last;
	double vel; //速度
}ALT_Typedef;
extern MS5611_Typedef ms5611;
extern ALT_Typedef Alt;
extern double Altitude;
// MS5611, Standard address 0x77
#define MS5611_ADDR             0xEE

#define CMD_RESET               0x1E // ADC reset command
#define CMD_ADC_READ            0x00 // ADC read command
#define CMD_ADC_CONV            0x40 // ADC conversion command
#define CMD_ADC_D1              0x00 // ADC D1 conversion
#define CMD_ADC_D2              0x10 // ADC D2 conversion
#define CMD_ADC_256             0x00 // ADC OSR=256
#define CMD_ADC_512             0x02 // ADC OSR=512
#define CMD_ADC_1024            0x04 // ADC OSR=1024
#define CMD_ADC_2048            0x06 // ADC OSR=2048
#define CMD_ADC_4096            0x08 // ADC OSR=4096
#define CMD_PROM_RD             0xA0 // Prom read command

#define PROM_NB                 8

#define MS5611_OSR_4096_CONV_DELAY       8220   // 8.22ms
#define MS5611_OSR_2018_CONV_DELAY       4130   // 4.13ms
#define MS5611_OSR_1024_CONV_DELAY       2080   
#define MS5611_OSR_512_CONV_DELAY        1060   
#define MS5611_OSR_256_CONV_DELAY        540    

u8 MS5611_Init(void);  

void MS5611_start_ut(void);
void MS5611_get_ut(MS5611_Typedef* ms);
void MS5611_start_up(void);
void MS5611_get_up(MS5611_Typedef* ms);
void MS5611_calculate(MS5611_Typedef* ms);
double MS5611_Get_Altitude(MS5611_Typedef* ms);
void MS5611_Data_Pros(MS5611_Typedef* ms,float call_time);

#endif
