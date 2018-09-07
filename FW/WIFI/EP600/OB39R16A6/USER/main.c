#include <stdio.h>
#include <string.h>
#include "IIC.h"
#include "TM1620.h"
#include "sensor.h"
#include "SOFT_IIC.h"
#include "PWM.h"
#include "dc_motor.h"
#include "timer.h"
#include "global.h"
#include "INT1.h"
#include "step_motor.h"
#include "sys_run.h"
#include "buzzer.h"
#include "wifi_uart.h"
#include "debug_uart.h"
#include "soft_uart.h"
#include "ADC.h"
#include "UV.h"
#include "ION.h"
#include "global.h"
#include "charge.h"
#include "UART0.h"


/*
sprintf��ע��ĵط���
1.��ʽ������ʱ�����봫�� unsigned int ��ʽ
*/
/*
������ע�����⣺
1.��ʹ��long���ͺͺ궨��ʱ���׳������⣬���������ԣ�����ʾ��
#define TEST_NUM   60000
unsigned long temp = 0;
temp = TEST_NUM + 10000;

��������Ӧ����70000 = 0x11170,����ʵ����������Ľ����0xffff1170��
�궨�����ֵ���С��0xFFFF,�øú궨���ֵ��һ��С��0xFFFF�������㣬����������0xFFFF������ִ����������˷�����һ����������������
����������� 
#define TEST_NUM   70000  
���궨�����ֵ���� 0xFFFF������������ȷ�ġ�

���ֱ��ʹ�� temp = 60000 + 10000; ֱ��ʹ�����ֽ��м���û������


����취��
temp = (TEST_NUM | 0x00) + 10000;  
������������ȷ


*/


void main(void)
{

  unsigned long times_1s = 0;

//  unsigned char test = 0;
//  unsigned char debug_buff[30] = {0};


  TM1620_Init();
  
  Key_IIC_PinConifg();
  Start_Timer1();
  Start_INT1();

  
  PWM_Init();
  PWM_Control(PWM7_NUM,PWM_ON);

  
  
  Step_Motor_Pin_Init();
  Buzzer_PinConfig();
  SENSOR_IIC_PinConifg();
  Soft_UART_Init();
#ifdef WIFI_SOFT_UART
  Wifi_Uart_Init();
#else
  init_UART0();
#endif 
  DEBUG_Uart_Init();
  UV_Pin_Init();
  ION_Pin_Init();

  
  Buzzer_Power_On();
  TM1620_DispalyData(SENSOR_PM25,0);
  TM1620_LED_Control(LED_ALL,LED_OFF);
  TM1620_LED_Control(LED_POWER,LED_ON);
  TM1620_LED_Control(LED_SLEEP_MODE,LED_ON);
  
  //Ϊ�˱�֤��ȫ����������в��ż�⣬������������򰴼�ʧЧ
  sys_init_check();
  
  charge_init();
  
  while(1)
  {
   
//      if(_test_timeflag(g_1min_flag))
//      { 
//          g_1min_flag = 0;
//              
//      }
      
      if(_test_timeflag(g_1s_flag))
      {
          g_1s_flag = 0;
          times_1s++;
        
          display_pm_data(); 
          
          //�ֻ�����ָ�����ָ������1�����ڲ�֧����رղ���
          Check_If_Close_Door();
          
          if(is_sys_auto_check == 1)
          {
              TM1620_DispalyData(2,sys_check_info.fan_check_fault_times);
              //TM1620_DispalyData(2,sys_check_info.fg_count);
            
          }
          
          //�ֶ�����ʱ�����3�����ڲ�ͨ��APPֹͣ��������Զ�ֹͣ
          if(is_sys_manual_check == 1)
          {
              check_if_stop_manual_check();
          }
         
      }
  
      if(_test_timeflag(g_1ms_flag))
      {
          g_1ms_flag = 0;
          sys_run();
          DirectMotor();       
      }  

      if(_test_timeflag(g_2ms_flag))
      {
          g_2ms_flag = 0;
          TurnMotor();
      }        
      
      if(_test_timeflag(g_10ms_flag))
      {
          g_10ms_flag = 0;
          Scan_TouchPad();  
          Deal_Wifi_Uart_Data();         
      }   

      if(_test_timeflag(g_100ms_flag))
      {
          g_100ms_flag = 0;
        
            
          if(is_sys_auto_check == 1)
          {
              sys_dev_auto_check();
          }     

          Dcmoto_adj();   
          UV_Check();                
      }
      
      get_charge_info();
      
      if(cost_info.IsSelfChargeMode == 1)
      {  
          //��MCU��WIFIģ������ͨѶ����ﵽһ�������󣬿�����wifiģ���޷����������ˣ���ʱMCU�����ԼƷ�ģʽ��ֱ���ѷ���ʱ������
          mcu_self_charge();
      }
     
  }
}
