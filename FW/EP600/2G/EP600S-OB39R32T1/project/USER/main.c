#include <stdio.h>
#include <string.h>
#include "IIC.h"
//#include "TM1618.h"
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
#include "EEPROM.h"
#include "charge.h"
#include "UART0.h"
#include "common.h"
#include "M26.h"


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

#define test_EEPROM_START_ADDR   0x3D00


extern M26_CMD_Typedef m26_cmd_info;
extern M26_RESGISTER_Typedef M26_register_net;

extern unsigned char m26_action;



void main(void)
{

  unsigned long times_1s = 0;

  unsigned char res = 0;
  
  unsigned int i,j = 0;
  
  unsigned int eeprom_addr = 0;
  
  unsigned char read_data = 0;
  
  //unsigned char debug_buff[40] = {0};
  
  //const char *test_char = "ATI\r\n";
  char at_cmd[20] = {0};


  TM1620_Init();
  
  //Key_IIC_PinConifg();
  Start_Timer1();
  //Start_INT1();

  
//  PWM_Init();
//  PWM_Control(PWM7_NUM,PWM_ON);

  
  
//  Step_Motor_Pin_Init();
//  Buzzer_PinConfig();
//  SENSOR_IIC_PinConifg();
  Soft_UART_Init();
#ifdef WIFI_SOFT_IIC
  Wifi_Uart_Init();
#else
  init_UART0();
#endif 
  DEBUG_Uart_Init();
//  UV_Pin_Init();
//  ION_Pin_Init();

  M26_register_net.Is_m26_registerd = 0;
  M26_register_net.step = 0;
    
  EA = 1;                //Enable all inierrupt
  
//  Buzzer_Power_On();
  TM1620_DispalyData(SENSOR_PM25,0);
//  TM1820_LED_Control(LED_ALL,LED_OFF);
//  TM1820_LED_Control(LED_POWER,LED_ON);
//  TM1820_LED_Control(LED_SLEEP_MODE,LED_ON);

  
  //Ϊ�˱�֤��ȫ����������в��ż�⣬������������򰴼�ʧЧ
  //sys_init_check();
  
  //charge_init();
  
//  mymemset(debug_buff,0,40);
//  sprintf(debug_buff,"step1\n");
//  DEBUG_Uart_Sendbytes(debug_buff,mystrlen(debug_buff));
//  while(M26_register_net.Is_m26_registerd == 0)
//  {
//      if(get_sys_stime < 15)
//      {
//          continue;
//      }
//      if(_test_timeflag(g_1s_flag))
//      {
//          g_1s_flag = 0;
//          //Dcmoto_adj();   
//          //UV_Check();  
//          //Deal_Wifi_Uart_Data();   
//          //M26_wait_response(1);
//          M26_Register_to_Net();       
//          M26_wait_response(); 
//          check_m26_cmd_result();        
//      }
//  }
  
  while(1)
  {
 
    
//      mymemset(debug_buff,0,40);
//      sprintf(debug_buff,"step2\n");
//      DEBUG_Uart_Sendbytes(debug_buff,mystrlen(debug_buff));
   
      if(_test_timeflag(g_1min_flag))
      { 
          g_1min_flag = 0;
              
      }
      
      if(_test_timeflag(g_1s_flag))
      {
          g_1s_flag = 0;
          times_1s++;
        
//      mymemset(debug_buff,0,40);
//      sprintf(debug_buff,"step2\n");
//      DEBUG_Uart_Sendbytes(debug_buff,mystrlen(debug_buff));
        
          //TM1620_DispalyData(SENSOR_PM25,sizeof(M26_ATI_CMD));
        //TM1620_DispalyData(SENSOR_PM25,times_1s);
//        m26_action = 0;
//        m26_action |= (1 << HTTP_UPLOAD_DATA);
//        TM1620_DispalyData(SENSOR_PM25,M26_BIT_CHECK(m26_action,HTTP_UPLOAD_DATA));
        
        
//          if(times_1s > 15)
//          {
//              M26_Register_to_Net();
//          }
          
        
          //M26_Register_to_Net();
        
          //display_pm_data();    

          //Check_If_Close_Door(); 
          if(times_1s % 2 == 0)
          {
              //M26_Device_Activate();
              if(times_1s > 15)
              {
                  //M26_Device_Activate();
                  //M26_Upload_Data();
                  //M26_Status_Sync();
                  M26_Charge_Confirm();
              }
          } 
          if(times_1s % 2 == 1)    
          {
              //M26_wait_response(1); 
          }            

 
        
          if(times_1s % 5 == 0)
          {
//              if(i % 50 == 0)
//              {
//                  eeprom_addr = test_EEPROM_START_ADDR;
//                  EEPROM_Page_Erase(eeprom_addr);
//                  //i = 0;
//                  
//              }  
//              i += 1;              
//              EEPROM_Byte_Program(eeprom_addr,i);
//              read_data = EEPROM_Byte_Read(eeprom_addr);
//              
//              TM1620_DispalyData(SENSOR_PM25,(unsigned char)read_data);
//              
//              mymemset(debug_buff,0,40);
//              sprintf(debug_buff,"i = %d,read data:%d\n",(unsigned int)i,(unsigned int)read_data);
//              DEBUG_Uart_Sendbytes(debug_buff,mystrlen(debug_buff));              
//              
//              eeprom_addr += 1;
            
//              mymemset(debug_buff,0,40);
//              sprintf(debug_buff,"num:%d\n",(unsigned int)sizeof(M26_GET_PINFO_CMD));
//              DEBUG_Uart_Sendbytes(debug_buff,mystrlen(debug_buff)); 



//            mymemset(m26_cmd_info.sendtring,0,sizeof(m26_cmd_info.sendtring));
//            mymemset(at_cmd,0,sizeof(at_cmd));
//            if(j % 5 == 0)
//            {
//                strcpy(m26_cmd_info.sendtring,M26_ATI_CMD);
//                strcpy(at_cmd,M26_ATI_CMD);
//           
//            }
//            else if(j % 5 == 1)
//            {
//                strcpy(m26_cmd_info.sendtring,M26_GSN_CMD);
//                strcpy(at_cmd,M26_GSN_CMD);
//            } 
//            else if(j % 5 == 2)
//            {
//                strcpy(m26_cmd_info.sendtring,M26_QSPN_CMD);
//                strcpy(at_cmd,M26_QSPN_CMD);
//            }
//            else if(j % 5 == 3)
//            {
//                //strcpy(m26_cmd_info.sendtring,M26_QIFGCNT_CMD);
//            }
//            else if(j % 5 == 4)
//            {
//                //strcpy(m26_cmd_info.sendtring,M26_GSN_CMD);
//            }
//            strcat(m26_cmd_info.sendtring,M26_CMD_END_FLAG);
//            
//            M26_Send_data(CMD_TYPE,at_cmd,m26_cmd_info.sendtring);
            
            j+= 1;
              
          }
          
          
      }
  
      if(_test_timeflag(g_1ms_flag))
      {
          g_1ms_flag = 0;
          //sys_run();
          //DirectMotor();       
      }  

      if(_test_timeflag(g_2ms_flag))
      {
          g_2ms_flag = 0;
          //TurnMotor();
      }        
      
      if(_test_timeflag(g_10ms_flag))
      {
          g_10ms_flag = 0;
          //Scan_TouchPad();                 
      }   

      if(_test_timeflag(g_100ms_flag))
      {
          g_100ms_flag = 0;
          //Dcmoto_adj();   
          //UV_Check();  
          //Deal_Wifi_Uart_Data();   
          //M26_wait_response(1);
          m26_http_loop();        
          M26_wait_response(); 
          check_m26_cmd_result();        
      }
      
      //get_charge_info();
      
//      if(cost_info.IsSelfChargeMode == 1)
//      {  
//          //��MCU��WIFIģ������ͨѶ����ﵽһ�������󣬿�����wifiģ���޷����������ˣ���ʱMCU�����ԼƷ�ģʽ��ֱ���ѷ���ʱ������
//          mcu_self_charge();
//      }
     
  }
}
