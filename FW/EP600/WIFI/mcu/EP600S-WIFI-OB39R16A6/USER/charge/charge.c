#include <stdio.h>
#include <string.h>
#include "charge.h"
#include "global.h"
#include "timer.h"
#include "wifi_uart.h"
#include "common.h"
#include "debug_uart.h"
#include "touch_key.h"
#include "sys_run.h"

Cost_Typedef cost_info;


//�ϵ��Ȼ�ȡwifi�ļƷ���Ϣ
void charge_init(void)
{
    unsigned long nowtime = 0;
    unsigned long msdelay = 2000;
    unsigned long nexttime = 0;
    //unsigned char receive_buff[13] = {0};
    unsigned char debug_buff[30] = {0};
    //unsigned char recv_num = 0;
    
    cost_info.IsCostType = 0;
    cost_info.IsHavetime = 0;
    cost_info.lefttime = 0;
    
  
    nowtime = get_sys_mstime();
    nexttime = nowtime + msdelay;
    get_wifi_charge_cmd();
   
    //���MCU�޷���üƷ���Ϣ��������
    while(1)
    {
        nowtime = get_sys_mstime();    
        if(_test_timeflag(g_10ms_flag))
        {
          g_10ms_flag = 0;
          Deal_Wifi_Uart_Data();        
        } 
        if(nowtime >= nexttime)
        {
//            mymemset(receive_buff,0,13);
//            recv_num = Wifi_Uart_ReceiveOnePackage(receive_buff,13);
//           
//            mymemset(debug_buff,0,50);
//            sprintf(debug_buff,"recv_num:%d\n",(unsigned int)recv_num);  
//            DEBUG_Uart_Sendbytes(debug_buff,mystrlen(debug_buff));
            if(cost_info.IsGetChargeInfo == 0)
            {
//                mymemset(debug_buff,0,30);
//                sprintf(debug_buff,"get charge info failed\n");  
//                DEBUG_Uart_Sendbytes(debug_buff,strlen(debug_buff));    
              
                get_wifi_charge_cmd();   
                
            }
            else if(cost_info.IsGetChargeInfo == 1)
            {
                mymemset(debug_buff,0,30);
                sprintf(debug_buff,"lefttime:0x%x\n",(unsigned int)cost_info.lefttime);  
                DEBUG_Uart_Sendbytes(debug_buff,strlen(debug_buff));
              
                if(cost_info.IsCostType == 1 && cost_info.IsHavetime == 1)
                {
                    sys_start();  
                }
                Clear_Touch_Info();
                
                break;
            }
            
            nexttime = nowtime + msdelay; 
        }

    }
}



void get_charge_info(void)
{
    static unsigned long time_triger = (GET_TIME_INTERVAL | 0x00);
    //�Ƿ����˻�ȡ�Ʒ���Ϣ�������־λ����ֹ�ظ���������
    static bit Is_cmd_send = 0;
    static bit Is_Power_On = 1;
    unsigned long nowtime = 0;
    //unsigned char debug_buff[20] = {0};
  
    nowtime = get_sys_mstime();
  
    if(Is_Power_On == 1)
    {
        time_triger = nowtime + (GET_TIME_INTERVAL | 0x00);
        Is_Power_On = 0;
       
    }
    if(nowtime <= (GET_TIME_INTERVAL | 0x00) && time_triger >= (0xFFFFFFFF - (GET_TIME_INTERVAL | 0x00)) )
    {
        //��ֹʱ�����
        time_triger = nowtime + (GET_TIME_INTERVAL | 0x00);
    }
    
    //�ȷ��ͻ�ȡ�Ʒ���Ϣ�����1����ټ���Ƿ���յ�������,�����Է��������500msû�з��صĸ����е�ߣ����������1������ݻ�������������
    //2���Ӵ�wifiģ���ȡһ�μƷ���Ϣ����ȡ����Ϣ����ʱ1�뷢����������������ʱ1���������Ƿ��յ��������ԣ����ַ�ʽ�ɹ��ʺܸ�
    if(nowtime >= (time_triger - 1000) && Is_cmd_send == 0 )
    {
        get_wifi_charge_cmd();
        Is_cmd_send = 1;
    }
    if(nowtime >= time_triger)
    {
        if(cost_info.IsGetChargeInfo == 0)
        {
            cost_info.get_charge_info_failtimes += 1;
          
//            mymemset(debug_buff,0,20);
//            sprintf(debug_buff,"failtimes:0x%x\n",(unsigned int)cost_info.get_charge_info_failtimes);  
//            DEBUG_Uart_Sendbytes(debug_buff,mystrlen(debug_buff));
          
            if(cost_info.get_charge_info_failtimes >= UART_FAIL_MAX_TIMES )
            {
                //�����ԼƷ�ģʽ
                cost_info.IsSelfChargeMode = 1;
                cost_info.StartSelfCharge = 1;
            }
          
        }
        else if(cost_info.IsGetChargeInfo == 1)
        {
            cost_info.IsSelfChargeMode = 0;
            cost_info.StartSelfCharge = 0;
            cost_info.get_charge_info_failtimes = 0;
        }
        Is_cmd_send = 0;
        time_triger = nowtime + (GET_TIME_INTERVAL | 0x00);
    }
}


void mcu_self_charge(void)
{
  
   static unsigned long time_triger = 0;
   unsigned long nowtime = 0;
  
   nowtime = get_sys_mstime();
   if(nowtime <= (MIN_TO_MS | 0x00) && time_triger >= (0xFFFFFFFF - (MIN_TO_MS | 0x00)) )
   {
       //��ֹʱ�����
       time_triger = nowtime + (MIN_TO_MS | 0x00);
   }
   
   if(cost_info.StartSelfCharge == 1)
   {
       time_triger = nowtime + (MIN_TO_MS | 0x00);
       cost_info.StartSelfCharge = 0;
   }
   
   if(nowtime >= time_triger)
   {
       if(cost_info.IsCostType == 1 && cost_info.IsHavetime == 1)
       {
           if(cost_info.lefttime > 0)
           {
               cost_info.lefttime -= 1;
           }
           if(cost_info.lefttime == 0)
           {
               cost_info.IsHavetime = 0;
           }
       }
       time_triger = time_triger + (MIN_TO_MS | 0x00);
   }
   
}
