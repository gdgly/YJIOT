#include <stdio.h>
#include <string.h>
#include "UV.h"
#include "ADC.h"
#include "global.h"
#include "debug_uart.h"
#include "common.h"
#include "TM1620.h"


void UV_Pin_Init(void)
{
    //UV_PIN_PxM0 &= ~(1 << UV_PIN_PORTBIT);
    UV_PIN_PxM0 |= (1 << UV_PIN_PORTBIT);
    UV_PIN_PxM1 &= ~(1 << UV_PIN_PORTBIT);
  
    UV_CONTROL_PIN = 0;
  
    ADCInit(d_ADCnEN, d_ADC_CLK_Sel);

}

void UV_On(void)
{
    //unsigned char *wifi_send_buff = "uv on\n";
  
    UV_CONTROL_PIN = 1;
    IsUVOn = 1;
  
    //ÿ�θ��ϵ�ʱ����һ�μ�⵽��ADC��ֵΪ0�����Գ�ʼ��Ϊ-1���ϵ���ʼ���4�Σ����Ե�1�Σ��Ժ�ÿ�ζ��Ǽ��3��
    uv_check_times = -1;
  
    //�ϵ��5����ADC��ֵ
    uv_check_timeinterval = UV_CHECK_TIME_INTERVAL - 5;
  
    TM1620_LED_Control(LED_UV,LED_ON);
   
}

void UV_Off(void)
{  
    UV_CONTROL_PIN = 0;
    IsUVOn = 0;
    TM1620_LED_Control(LED_UV,LED_OFF);
  
     
    ADCfinish = 0;
    IsUVCheck = 0;   
}

void UV_ADC_Start(void)
{
    ADCfinish = 0;
    ADCstart(d_ADC_CH0_IN);
}

unsigned int Get_UV_ADC_Data(void)
{
    unsigned int adc_data = 0;
  
    adc_data = n_data;
    n_data = 0;
    ADCfinish = 0;
    return adc_data;
}

//ÿ�μ��3���ӣ�ÿ1����һ�Σ�3�����ݶ������ſ��ԣ�3�μ���������һ�μ�⵽�����ݲ��������ж�ΪUV������
void UV_Check(void)
{
    unsigned int uv_adc_data;
    unsigned char uv_adc_byte[3] = {0};
    
    unsigned char debug_buff[30] = {0};
  
    if(IsUVOn == 1)
    {           
        if(IsUVCheck == 1 && ADCfinish == 1)
        {
            uv_adc_data = Get_UV_ADC_Data();
         
            if(uv_adc_data < UV_ADC_CRITICAL_VALUE && uv_check_times >= 0)
            {
                IsUVfault = 1;
                UV_Off();
            }
            if(++uv_check_times == 3)
            {   
                IsUVCheck = 0;
                uv_check_flag = 0;
                uv_check_times = 0;     
            }

            mymemset(debug_buff,0,sizeof(debug_buff));
            sprintf(debug_buff,"adc data: %d\r\n",(unsigned int)uv_adc_data);
            DEBUG_Uart_Sendbytes(debug_buff,strlen(debug_buff));            

            
        }
        
        
        if(IsUVCheck)
        {
            //1���Ӽ��һ�Σ��ڶ�ʱ������1
            if(uv_check_flag)
            {
                UV_ADC_Start();
                uv_check_flag = 0;
            }
        }
        

        
        if(uv_check_timeinterval >= UV_CHECK_TIME_INTERVAL)
        {
            IsUVCheck = 1;
            uv_check_timeinterval = 0;
        }
        
        
//        if(IsUVfault == 1)
//        {
//            UV_Off();
//        }
    }
}