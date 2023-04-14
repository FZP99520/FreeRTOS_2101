#include "ms5611.h"
#include "iic.h"
#include "systick.h"
MS5611_Typedef ms5611;
ALT_Typedef Alt;
double Altitude;
#define dt 0.05f
//static uint8_t ms5611_osr = CMD_ADC_4096;

#define PA_OFFSET_INIT_NUM 100


static u16 ms5611_prom(int8_t coef_num);  
static void ms5611_reset(void);
static u32 ms5611_read_adc(void);
static u8 ms5611_crc(uint16_t *prom);
static float KalmanFilter_Alt(float ResrcData);
static float KalmanFilter_V(float ResrcData);
u8 MS5611_Init(void)
{
    u8 sig;
    u8 i;
    Delay_ms(10); // No idea how long the chip takes to power-up, but let's make it 10ms
    IIC_Read_Buff(MS5611_ADDR, CMD_PROM_RD, &sig, 1);
   
    
    ms5611_reset();
    // read all coefficients
    for (i = 0; i < PROM_NB; i++)
        ms5611.prom[i] = ms5611_prom(i);

    if(ms5611_crc(ms5611.prom) != 0)
        return 0;

    return 1;
		
		
}



static void ms5611_reset(void)
{
    IIC_Write_CMD(MS5611_ADDR,CMD_RESET);
    Delay_ms(10);
}
static u16 ms5611_prom(int8_t coef_num)
{
   uint8_t rxbuf[2] = { 0, 0 };
   IIC_Read_Buff(MS5611_ADDR, CMD_PROM_RD + coef_num * 2,rxbuf, 2); // send PROM READ command
   return rxbuf[0] << 8 | rxbuf[1];

}
static u32 ms5611_read_adc(void)
{
   uint8_t rxbuf[3];
   IIC_Read_Buff(MS5611_ADDR, CMD_ADC_READ, rxbuf, 3); // read ADC
   return (rxbuf[0] << 16) | (rxbuf[1] << 8) | rxbuf[2];
}
 void MS5611_start_ut(void)
{
    IIC_Write_CMD(MS5611_ADDR, CMD_ADC_CONV + CMD_ADC_D2 + CMD_ADC_4096); // D2 (temperature) conversion start!
}
 void MS5611_get_ut(MS5611_Typedef* ms)
{
    ms->ut = ms5611_read_adc();
}
 void MS5611_start_up(void)
{
    IIC_Write_CMD(MS5611_ADDR, CMD_ADC_CONV + CMD_ADC_D1 + CMD_ADC_4096); // D1 (pressure) conversion start!
}

void MS5611_get_up(MS5611_Typedef* ms)
{
    ms->up = ms5611_read_adc();
}

void MS5611_calculate(MS5611_Typedef* ms)
{
    uint32_t press;
    int64_t temp;
    int64_t delt;

    int32_t dT = (int64_t)ms->ut - ((uint64_t)ms->prom[5] * 256);
    int64_t off = ((int64_t)ms->prom[2] << 16) + (((int64_t)ms->prom[4] * dT) >> 7);
    int64_t sens = ((int64_t)ms->prom[1] << 15) + (((int64_t)ms->prom[3] * dT) >> 8);
    temp = 2000 + ((dT * (int64_t)ms->prom[6]) >> 23);

    if (temp < 2000) { // temperature lower than 20degC
        delt = temp - 2000;
        delt = 5 * delt * delt;
        off -= delt >> 1;
        sens -= delt >> 2;

        if (temp < -1500) { // temperature lower than -15degC
            delt = temp + 1500;
            delt = delt * delt;
            off -= 7 * delt;
            sens -= (11 * delt) >> 1;
        }
    }

    press = ((((int64_t)ms->up * sens) >> 21) - off) >> 15;

        ms->pressure = press;
        ms->temperature = temp;
}

static u8 ms5611_crc(u16 *prom)
{
    int32_t i, j;
    uint32_t res = 0;
    uint8_t zero = 1;
    uint8_t crc = prom[7] & 0xF;
    prom[7] &= 0xFF00;

    // if eeprom is all zeros, we're probably fucked - BUT this will return valid CRC lol
    for (i = 0; i < 8; i++) {
        if (prom[i] != 0)
            zero = 0;
    }

    if (zero)
        return -1;

    for (i = 0; i < 16; i++) {
        if (i & 1)
            res ^= ((prom[i >> 1]) & 0x00FF);
        else
            res ^= (prom[i >> 1] >> 8);
        for (j = 8; j > 0; j--) {
            if (res & 0x8000)
                res ^= 0x1800;
            res <<= 1;
        }
    }

    prom[7] |= crc;

    if (crc == ((res >> 12) & 0xF))
        return 0;

    return -1;
}

/*
 * Get Altitude (cm)
 */
double MS5611_Get_Altitude(MS5611_Typedef* ms)
{
	 static u8 delay=0;
	 static u8 cnt=0;
	 static u32 pa=0;
	 double alt;
   if(ms->OffsetReq == 1 )
	 { 
		 
		 if(delay<100) //丢弃前20次数据
		 {
			 delay++;
			 return 0;
		 }
			  ms->OffsetFinished = 0;
        if(cnt >= PA_OFFSET_INIT_NUM)
				{
            ms->offset_pa = pa/cnt;
            ms->OffsetFinished = 1;
					  ms->OffsetReq = 0;
					  cnt=0;
					  pa = 0;
					  delay = 0;
         }
				else
				{
           pa += ms->pressure; 
           cnt++;		
					return 0;
				}			
    }
		if(ms->OffsetFinished == 1)
         alt = 4433000.0f * (1.0f - powf((((float) ms->pressure) / ms->offset_pa), 0.190295f));
		else 
			   alt = 0;
    return alt; 
}
void MS5611_Data_Pros(MS5611_Typedef* ms,float call_time)
{
	static u8 count=0;
	u8 j;
	j = 10.0f/call_time;
	double alt;
	count++;
	if(count == j)
	    MS5611_start_ut();
	else if(count == j*2)
	    MS5611_get_ut(&ms5611);
	else if(count == j*3)
		  MS5611_start_up();
	else if(count == j*4)
		 MS5611_get_up(&ms5611);
	else if(count == j*5)
	{
		 count = 0;
		 MS5611_calculate(&ms5611);
		 alt = MS5611_Get_Altitude(&ms5611);
		 Alt.now = KalmanFilter_Alt(alt);
		 Alt.vel = KalmanFilter_V( (Alt.now - Alt.last)/dt );
		 Alt.last = Alt.now;
		 Altitude = Alt.now;
	}
	else;
}

#define KALMAN_Q 0.03f
#define KALMAN_R 15.0f
static float KalmanFilter_Alt(float ResrcData)
{
	double R = KALMAN_R;
	double Q = KALMAN_Q;
	static double x_last;
	double x_mid;
	double x_now;
	static float p_last;
	double p_mid;
	double p_now;
	double kg;
	
	x_mid = x_last;
	p_mid = p_last + Q;
	kg = p_mid/(p_mid+R);
	x_now = x_mid + kg*(ResrcData - x_mid);
	p_now = (1-kg)*p_mid;
	p_last = p_now;
	x_last = x_now;
	
	return x_now;
	
}
static float KalmanFilter_V(float ResrcData)
{
	double R = KALMAN_R;
	double Q = KALMAN_Q;
	static double x_last;
	double x_mid;
	double x_now;
	static float p_last;
	double p_mid;
	double p_now;
	double kg;
	
	x_mid = x_last;
	p_mid = p_last + Q;
	kg = p_mid/(p_mid+R);
	x_now = x_mid + kg*(ResrcData - x_mid);
	p_now = (1-kg)*p_mid;
	p_last = p_now;
	x_last = x_now;
	
	return x_now;
	
}
