#ifndef __UV_H__
#define __UV_H__

#include "OB39R16A6.h"

#define UV_CONTROL_PIN   P2_4
#define UV_PIN_PORTBIT   4
#define UV_PIN_PxM0      P2M0
#define UV_PIN_PxM1      P2M1


//UVÿ��10���Ӽ��һ��
//#define UV_CHECK_TIME_INTERVAL  (10*60)
#define UV_CHECK_TIME_INTERVAL  (20)
//UV ADC���ٽ�ֵ 1.5Vת������������ (1023/5)*1.5 = 307
//#define UV_ADC_CRITICAL_VALUE    307
//UV ADC���ٽ�ֵ 2.8Vת������������ (1023/5)*2.8 = 307
#define UV_ADC_CRITICAL_VALUE    572

void UV_Pin_Init(void);
void UV_On(void);
void UV_Off(void);
void UV_ADC_Start(void);
unsigned int Get_UV_ADC_Data(void);
void UV_Check(void);


#endif