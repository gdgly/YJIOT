#include <stdio.h>
#include "global.h"
#include "sys_run.h"
#include "dc_motor.h"
#include "step_motor.h"
#include "sensor.h"
#include "buzzer.h"
#include "UV.h"
#include "wifi_uart.h"
#include "common.h"
#include "ION.h"
#include "debug_uart.h"
#include "TM1620.h"
#include "timer.h"
#include "M26.h"


extern M26_Cmd_Typedef m26_cmd_info;



//��ֹ���Ż��ˣ�����ʱ������в��ż�⡣������Ż����ٴ򿪷�����ܻ��ջ����
void sys_init_check(void)
{
    //unsigned char delay_1s_times = 0;

    if(!DOOR_CHECK_PIN)
    {
        //�˴��ӵ�Is_Door_Open = 1����Ϊ���ϵ������Is_Door_Open=0��Door_Close()�����л��ж��������Is_Door_Open==0����ֱ�ӷ���
        Is_Door_Open = 1;
        Door_Close();
    }
    else
    {
        return;
    }
    while(1)
    {
        if(_test_timeflag(g_2ms_flag))
        {
            g_2ms_flag = 0;
            TurnMotor();           
        } 
      
        if(_test_timeflag(g_1s_flag))
        {
            g_1s_flag = 0;
            //delay_1s_times += 1;
            if(beats == 0)
            {
                //IsSysFault = 0;
                return;
            }           
        }
    }
}



void sys_start(void)
{
    sys_start_time = get_sys_stime();
    UV_On();
    ION_On();
  
    Buzzer_Power_On();  
  
    //����UV�ƺ���ʱһ��ʱ�䣬���������豸ͬʱ����  
    while(get_sys_stime() < (sys_start_time + 2)) ;
  
    

    if(Is_Door_Open == 0 || (Is_Door_Open == 1 && DOOR_CHECK_PIN == 1))
    {
        Door_Open();
    }
    Start_DC_Motor();
    
    
    sys_mode = RUNNING;
    IsFanRunning = 1;
    IsSpeedChanged = 1;
    TM1620_LED_Control(LED_SLEEP_MODE,LED_OFF);
    TM1620_LED_Control(LED_SPEED_LOW,LED_ON);
    
    Is_close_by_man = 0;
}

void sys_stop(void)
{
    sys_stop_time = get_sys_stime();
    Buzzer_Power_Off();
    sys_mode = STANDBY;
    Stop_DC_Motor();
    Door_Close();
    UV_Off();
    ION_Off();  
    IsSpeedChanged = 1;

    TM1620_LED_Control(LED_ALL,LED_OFF);
    TM1620_LED_Control(LED_SLEEP_MODE,LED_ON);
  
    if(run_mode == 1)
    {  
       stop_sys_smart_mode();
    }
    
    if(is_sys_auto_check == 1)
    {
        stop_sys_auto_check();
    }
  
    IsUVfault = 0;
    Is_close_by_man = 1;
}


void sys_run(void)
{
    unsigned char set_speed = 0;
   
    //��ϼ�����2���ʼ���2G�����ź�
    if(key_info.IsTouchedKey == 1 && key_info.WhichKey == KEY_WIFI)
    {
        Is_signal_check = 1;
        Clear_Touch_Info();
        TM1620_DispalyData(SENSOR_PM25,0);
        Buzzer_Signal_Check();
    }
    
    //������˰Ѳ��Źر��ˣ�������������״̬
    if(is_sys_auto_check == 0)
    {
        check_if_doorclose_manual();
    }
    
    
    
    if(charge_info.IsChargeType == 1 && charge_info.lefttime.l_time > 0 && Is_close_by_man == 0)
    {
        if(sys_mode == STANDBY)
        {          
            sys_start();  

            //���������Ϣ����ֹ�ڿ����豸ǰ�԰����в��������򵱴��豸ʱ��֮ǰ�����İ�������������ִ��
            Clear_Touch_Info();
        }
        
    }
  
    //����ǼƷ�ģʽ������û��ʱ���ˣ��򰴼�ʧЧ
    if(charge_info.IsChargeType == 1 && charge_info.lefttime.l_time == 0)
    {
        if(is_sys_auto_check == 0 && is_sys_manual_check == 0)
        {
            if(sys_mode == RUNNING)
            {
                sys_stop();  
            }
            return;
        }
    }
  
  
    if(key_info.IsTouchedKey == 1)
    {
        switch(key_info.WhichKey)
        {
          case KEY_POWER:
            if(is_sys_manual_check == 1)
            {
                sys_check_info.touch_key_check |= (1 << power_bit);
            }
            
            if(sys_mode == STANDBY)
            {
                sys_start();             
            }
            else if(sys_mode == RUNNING)
            { 
                sys_stop();              
            }
            break;
          case KEY_SPEED:
            if(sys_mode == RUNNING)
            {
                Buzzer_Speed();
                set_speed = speed_dang;
                if(speed_dang < HIGH_SPEED )
                {
                    set_speed++;
                }
                else if(speed_dang == HIGH_SPEED)
                {
                    set_speed = QUIET_SPEED;
                }
                else
                {
                    set_speed = 0;
                }
                Set_DC_Motor_Speed(set_speed);
                
                if(is_sys_manual_check == 1)
                {
                    //sys_check_info.touch_key_check |= (1 << speed_bit);
                    if(speed_dang == QUIET_SPEED)
                    {
                        sys_check_info.touch_key_check |= (1 << quiet_speed_bit);
                    }
                    else if(speed_dang == LOW_SPEED)
                    {
                        sys_check_info.touch_key_check |= (1 << low_speed_bit);
                    }
                    else if(speed_dang == MID_SPEED)
                    {
                        sys_check_info.touch_key_check |= (1 << mid_speed_bit);
                    }
                    else if(speed_dang == HIGH_SPEED)
                    {
                        sys_check_info.touch_key_check |= (1 << high_speed_bit);
                    }
                } 
                
                
                //���Զ�ģʽ�£�����ֶ����ڷ����ˣ���ȡ���Զ�ģʽ
                if(run_mode == 1)
                {
                    stop_sys_smart_mode();
                }
                
            }
            else
            {
                break;
            }
            break;
          case KEY_TIMER:  
            if(is_sys_manual_check == 1)
            {
                sys_check_info.touch_key_check |= (1 << timer_bit);
            }             
            break;
          case KEY_MODE: 
            if(is_sys_manual_check == 1)
            {
                sys_check_info.touch_key_check |= (1 << mode_bit);
            }             
            break;
          case KEY_WIFI:
            //control wifi to easylink mode  
            if(is_sys_manual_check == 1)
            {
                sys_check_info.touch_key_check |= (1 << wifi_bit);
            }           
            break;            
            
            
          default:
            break;
            
        
        }
        if(key_info.IsTouchedKey == 1 && key_info.WhichKey != KEY_WIFI)
        {
            Clear_Touch_Info();
        }
        
    }
    
    if(run_mode == 1)
    {
        sys_smart_mode();
    }

}



#define GET_SENSOR_TIME_INTERVAL    (2)
#define RUN_CONTINUE_TIME    (1*60)
#define STOP_CONTINUE_TIME   (10*60)   //10min
#define STANDBY_UPDATA_DATA_INTERVAL  (15*60)   //15min
//#define RUN_CONTINUE_TIME    (1*60)
//#define STOP_CONTINUE_TIME   (1*60)   //10min
//#define STANDBY_UPDATA_DATA_INTERVAL  (1*60)   //15min

void display_pm_data(void)
{
    static bit change_to_3min_updata = 0;
    static bit IsPowerOn = 0;
    static unsigned long get_pm_trigger_time = 0;
    static unsigned long standby_trigger_time = 0;
    static unsigned int temp_pm_value = 0;
   
    nowtime_s = get_sys_stime();
    
    if(nowtime_s <= GET_SENSOR_TIME_INTERVAL && get_pm_trigger_time >= (0xFFFFFFFF - GET_SENSOR_TIME_INTERVAL))
    {
        get_pm_trigger_time = nowtime_s + GET_SENSOR_TIME_INTERVAL;
    }
    
    // �����Ի�ȡ���������ݣ�ʱ���� GET_SENSOR_TIME_INTERVAL ����λs
    if(nowtime_s >= get_pm_trigger_time)
    {
        PM25_value = Read_PMSensor_Data();
        //get_pm_trigger_time = nowtime + get_sensor_data_time_interval;
        get_pm_trigger_time = nowtime_s + (GET_SENSOR_TIME_INTERVAL | 0x00);
        
      
        //5���Ӹ���һ���������ʾ������
        if(is_sys_auto_check == 0)
        {
            TM1620_DispalyData(SENSOR_PM25,display_pm_value);
        }
        

    }
    if(sys_mode == RUNNING)
    {
        if(IsPowerOn == 0)
        {
            IsPowerOn = 1;
        }
        if(change_to_3min_updata == 1)
        {
            change_to_3min_updata = 0;
        }
        
        if(nowtime_s <= (sys_start_time + (RUN_CONTINUE_TIME | 0x00)))
        {
            //���豸��� 60 ������ʾ����ǰ����ֵ
            if(nowtime_s <= 180)
            {
                //�豸���ϵ磬��ʱ��ʾʵʱ��ֵ
                if(PM25_value_bak < PM25_value)
                {
                    display_pm_value = (unsigned int)PM25_value;
                    PM25_value_bak = (unsigned int)PM25_value;
                }
                else
                {
                    display_pm_value = PM25_value_bak;
                }
            }
            else
            {
                //����������豸�Ѿ��ϵ�һ��ʱ�䣬�Ӵ�����������ģʽ��Ϊ�˷�ֹ��ʾ����ֵ���ǣ���ʾ����ʱ����ֵ
                display_pm_value = (unsigned int)PM25_value_bak;           
              
            }

        }
        else
        {
            //ȡ���λ�ȡ�������ݵ�ƽ��ֵ����ֹ�����������
            display_pm_value = ((unsigned int)PM25_value + PM25_value_bak) / 2;
            PM25_value_bak = (unsigned int)PM25_value;
        }

    }
    else if(sys_mode == STANDBY)
    {
        
        //ϵͳ���������������PM2.5���ݻ��½����˴����ݽ��д�����������10��������������½�����ʾ�մ���ʱ������
        if(IsPowerOn == 0)
        {
            //�տ���ǰ3��������ʾʵʱ����
            if(nowtime_s < (STANDBY_UPDATA_DATA_INTERVAL | 0x00))
            {
                if(PM25_value_bak < (unsigned int)PM25_value)
                {
                    PM25_value_bak = (unsigned int)PM25_value;
                }
                display_pm_value = PM25_value_bak; 
                goto exit;              
            }
            else
            {
                IsPowerOn = 1;
                change_to_3min_updata = 1;
                temp_pm_value = 0;
                standby_trigger_time = nowtime_s + (STANDBY_UPDATA_DATA_INTERVAL | 0x00);
            }
        }
        else
        {
            if(change_to_3min_updata == 0)
            {
                if(nowtime_s < (sys_stop_time + (STOP_CONTINUE_TIME | 0x00)))
                {
                    //������ģʽ����������10min�ڣ�PM�����ݲ���С�ڸս������ģʽʱ������
                    if(PM25_value < PM25_value_bak)
                    {
                        //ȡƽ��ֵ��Ϊ�˷�ֹ��ʾ���ݱ仯��
                        display_pm_value = (PM25_value_bak + display_pm_value) / 2;
                    }
                    else
                    {
                        //��ֹ�ս������ʱ���ݱ仯��Χ̫��
                        display_pm_value = ((unsigned int)PM25_value + display_pm_value + PM25_value_bak) / 3;
                    }                        
                } 
                else
                {
                    change_to_3min_updata = 1; 
                    temp_pm_value = 0;
                    standby_trigger_time = nowtime_s + (STANDBY_UPDATA_DATA_INTERVAL | 0x00);
                }                  
            }
            else
            {
                //��ȡ STANDBY_UPDATA_DATA_INTERVAL  �������������ݣ�������ʾ
                if(temp_pm_value <= (unsigned int)PM25_value)
                {
                    temp_pm_value = (unsigned int)PM25_value;

                }
                
                if(PM25_value_bak < (unsigned int)PM25_value)
                {
                    PM25_value_bak = (unsigned int)PM25_value;
                    display_pm_value = ((unsigned int)PM25_value + display_pm_value) / 2;
                        
                }               
            
                if(nowtime_s >= standby_trigger_time)
                {
                    //�ϴ���ʾ�����ݺ���λ�ȡ�������ֵ��ͣ�Ȼ��ȡƽ��ֵ����Ȼ���ܻᵼ��2����ʾ����������Ƚϴ�
                    display_pm_value = (temp_pm_value + PM25_value_bak) / 2;
                    PM25_value_bak = display_pm_value;
                    temp_pm_value = 0;
                     
                    standby_trigger_time += (STANDBY_UPDATA_DATA_INTERVAL | 0x00);
                  
                  
//                    temp_low = standby_trigger_time & 0xFFFF;     
//                    temp_high = (standby_trigger_time >> 16) & 0xFFFF;   
//                    mymemset(debug_buff,0,30);
//                    sprintf(debug_buff,"trigger time:0x%x%4x\n",(unsigned int)temp_high,(unsigned int)temp_low);
//                    DEBUG_Uart_Sendbytes(debug_buff,mystrlen(debug_buff));  
                } 
              
            }
      
        }
        
    }
    
 exit:
    
    if(display_pm_value < 2)
    {
        display_pm_value = 2;
    }
    
    return;
    
    
}



//���Զ�ģʽ�£������λ�л���10�����ڲ������ٴ��л�������PM���ٽ�ֵʱ��ǳ�Ƶ�����л���λ�����鲻��
void sys_smart_mode(void)
{
    
    static bit speed_changed = 0;   //��ֹPMŨ�����ٽ�ֵʱ��Ƶ���л���λ
    static bit IsLedChange = 0;
    static unsigned long speed_changed_time = 0;   //�ϴ��л��絲ʱ��ʱ��
  
    const unsigned char code speed_continue_time = 10;  //10�����ڲ����ٴ��л��絲
    
  
    if((charge_info.IsChargeType == 1 && charge_info.lefttime.l_time == 0) || sys_mode == STANDBY)
    {
        return;
    }
    
    nowtime_s = get_sys_stime();
    
    if(speed_changed == 1)
    {
        //�����λ���л�����10���Ժ�����л�
        if(nowtime_s <= (speed_changed_time + speed_continue_time))
        {
            return;
        }
        else
        {
            speed_changed = 0;
        }
        
    }
    
    if(display_pm_value < PM25_QUIET_SPEED )
    {
        if(speed_dang != QUIET_SPEED)
        {
            speed_dang = QUIET_SPEED;
            IsSpeedChanged = 1;
            speed_changed = 1;
            IsLedChange = 1;
        }       
    }
    else if(display_pm_value < PM25_LOW_SPEED)
    {
        if(speed_dang != LOW_SPEED)
        {
            speed_dang = LOW_SPEED;
            IsSpeedChanged = 1;
            speed_changed = 1;
            IsLedChange = 1;
        }  
    }
    else
    {
        if(speed_dang != HIGH_SPEED)
        {
            speed_dang = HIGH_SPEED;
            IsSpeedChanged = 1;
            speed_changed = 1;
            IsLedChange = 1;
        }
    }   

    if(IsLedChange == 1)
    {
        //�Զ�ģʽ�£��絲���LED��ҲҪ��Ӧ�ı仯
        TM1620_LED_Control(speed_dang + 1,LED_ON);
        IsLedChange = 0; 

        dev_status = 1;          
    }  

    

}





//ϵͳ����ʱ����������ֶ��Ѳ��Źر��ˣ�������������ģʽ
//��ⷽ����100ms���һ�β��ţ��������2���Ӳ��Ŷ��ǹرյģ�������������ģʽ
#define MAX_DOOR_CLOSE_TIMES    20
void check_if_doorclose_manual(void)
{
    static unsigned char check_doorclose_times = 0;
 
    if(sys_mode == RUNNING)
    {
        nowtime_s = get_sys_stime();
        if(nowtime_s >= (sys_start_time + 10))
        {
            if(g_100ms_flag == 1)
            {
                if(DOOR_CHECK_PIN == 1)
                {
                    check_doorclose_times += 1;
                }
                else
                {
                    check_doorclose_times = 0;
                }
                
                if(check_doorclose_times >= MAX_DOOR_CLOSE_TIMES)
                {
                    if(sys_mode == RUNNING)
                    {
                        sys_stop();
                        Is_close_by_man = 1;
                        check_doorclose_times = 0;
                    }

                }
            }
        }
    }

}


void set_sys_to_smart_mode(void)
{
    run_mode = 1;
    TM1620_LED_Control(LED_AUTO_MODE,LED_ON);
}

void stop_sys_smart_mode(void)
{
    run_mode = 0;
    TM1620_LED_Control(LED_AUTO_MODE,LED_OFF);
}

//ϵͳ�Լ�
//�����Źر�ʱ�����ż�������Ǹߵ�ƽ�������Ŵ�ʱ�����ż�������ǵ͵�ƽ
//status��ϵͳ����־��bit0��ʾ���ţ�bit1��ʾ�����bit2��ʾuv��

//�˺�����ִ�м��������100ms����ΪFGcount�Ĳɼ�������100ms
void sys_dev_auto_check(void)
{

    //ϵͳ���UV�ͷ����ʱ��
    static const unsigned long code sys_check_time = 30;
    //unsigned long nowtime = 0;
  
    sys_check_info.fg_count = FGcount;
    FGcount = 0;  
  
    //step0  �ػ�
    if(sys_check_info.step == 0)
    {
        if(sys_mode == RUNNING)
        {
            sys_stop();
            is_sys_auto_check = 1;
        }
        sys_check_info.step += 1;
        is_auto_check_complete = 0;
    }     
    else if(sys_check_info.step == 1)  //step1  �������Ƿ���Թرջ��߼�⿪���Ƿ�������
    {        
        if(beats == 0)
        {
            if(DOOR_CHECK_PIN == 0)
            {
                is_auto_check_complete = 1;
                goto exit;
            }
            else
            {
                sys_check_info.step += 1;
            }
        }        
    }
    else if(sys_check_info.step == 2)    //step2  �򿪻���
    {       
        sys_start();
        sys_check_info.step += 1;             
    }
    else if(sys_check_info.step == 3)    //step3 �������Ƿ��
    {
        if(beats == 0)
        {
            if(DOOR_CHECK_PIN == 0)
            {
                sys_check_info.status &= ~(1 << door_bit);
                sys_check_info.step += 1;
            }
            else
            {  
                is_auto_check_complete = 1;
                goto exit;
            }
            FGcount = 0;
        }     
    }
    else if(sys_check_info.step == 4)    //step4  �������UV��
    {
        nowtime_s = get_sys_stime();
        //�������UV����Чʱ��20��
        if(nowtime_s >= (sys_start_time + 10) && nowtime_s <= (sys_start_time + sys_check_time))
        {            
            if(sys_check_info.fg_count < (SPEED_FG_COUNT2 - 1) || sys_check_info.fg_count > (SPEED_FG_COUNT2 + 1))
            //if(sys_check_info.fg_count < 12 || sys_check_info.fg_count > 14)
            {              
                if(sys_check_info.fan_check_fault_times < 0xFF)
                {
                    sys_check_info.fan_check_fault_times += 1;
                }
            }
                         
                        
        }
        else if(nowtime_s > (sys_start_time + sys_check_time))
        {
            //��Dcmoto_adj()�������ж�FGcount�����źţ�100ms���һ�Σ�20�빲���200�Σ������źŲ�����һ���̶�ֵ��������һ����Χ�ڲ��������������ʽ��м��
            if(IsUVfault == 0)
            {
                sys_check_info.status &= ~(1 << uv_bit);
            }
            if(sys_check_info.fan_check_fault_times < 30)
            {
                sys_check_info.status &= ~(1 << fan_bit);
            }
            if((int)PM25_value > 0)
            {
                sys_check_info.status &= ~(1 << pm25_bit);
            }
            is_auto_check_complete = 1;
        }
    }
   
    
exit:
    if(is_auto_check_complete == 1)
    {
        stop_sys_auto_check();            
    }
}


void start_sys_auto_check(void)
{
    is_sys_auto_check = 1;
    sys_check_info.step = 0;
    sys_check_info.status = 0xFF;
    sys_check_info.fan_check_fault_times = 0;  
  
    is_upload_auto_check_result = 0;
    
}

void stop_sys_auto_check(void)
{
    if(is_sys_auto_check == 1)
    {
        is_upload_auto_check_result = 1;
        is_sys_auto_check = 0; 
    }
       
    if(sys_mode == RUNNING)
    {       
        sys_stop();        
    }    
}

void start_sys_manual_check(void)
{
    is_sys_manual_check = 1;
    sys_check_info.touch_key_check = 0;
    
    is_upload_manual_check_result = 0;
  
    sys_check_info.start_time = get_sys_stime();
  
    //�����ֶ����������ϴ�ʱ������Ϊ5�룬����APP���·������������30���ʱ����ܽ��յ�����
    next_sync_stime = sys_check_info.start_time;
    is_fast_sync = 1;
    fast_sync_start_time = sys_check_info.start_time;
  
    Buzzer_Get_Charge();
    
}

void check_if_stop_manual_check(void)
{
    //�ֶ��������ʱ�䣬���3�����ڲ�ֹͣ���Զ�ֹͣ
    const unsigned long check_max_time = 180;
    if(get_sys_stime() >= (sys_check_info.start_time + check_max_time))
    {        
        stop_sys_manual_check();
    }
}


void stop_sys_manual_check(void)
{
    is_sys_manual_check = 0;
    is_upload_manual_check_result = 1;

    if(Is_Door_Open == 0 && charge_info.lefttime.l_time == 0)
    {
        is_fast_sync = 0;
    }
  
    Buzzer_Get_Charge();
}





void sys_check_auto_result_upload(void)
{
    unsigned char result = 0;
  
    if(M26_Upload_Factory_Check_Result() != M26_OK)
    {
        return;
    }
  
    result = sys_check_info.status;
    
    is_upload_auto_check_result = 0;
  
    mymemset(m26_cmd_info.sendtring,0,sizeof(m26_cmd_info.sendtring));
    sprintf(m26_cmd_info.sendtring,"auto_check result:0x%x\r\n",(unsigned int)result);
    DEBUG_Uart_Sendbytes(m26_cmd_info.sendtring,mystrlen(m26_cmd_info.sendtring)); 
    
//    mymemset(m26_cmd_info.sendtring,0,sizeof(m26_cmd_info.sendtring));
//    sprintf(m26_cmd_info.sendtring,"check result:0x%x,door:%s,fan;%s,uv:%s,pm_bit:%s,pm:%d",(unsigned int)result,(unsigned int)AUTO_CHECK_0(result,door_bit),(unsigned int)AUTO_CHECK_0(result,fan_bit),
//                    (unsigned int)AUTO_CHECK_0(result,uv_bit),(unsigned int)AUTO_CHECK_0(result,pm25_bit),(unsigned int)(PM25_value));
//    DEBUG_Uart_Sendbytes(m26_cmd_info.sendtring,mystrlen(m26_cmd_info.sendtring)); 
}



void sys_check_manual_result_upload(void)
{
    unsigned char result = 0;
  
    if(M26_Upload_Factory_Check_Result() != M26_OK)
    {
        return;
    }

    result = sys_check_info.touch_key_check;
    
    is_upload_manual_check_result = 0;
  
    mymemset(m26_cmd_info.sendtring,0,sizeof(m26_cmd_info.sendtring));
    sprintf(m26_cmd_info.sendtring,"man_check result:0x%x\r\n",(unsigned int)result);
    DEBUG_Uart_Sendbytes(m26_cmd_info.sendtring,mystrlen(m26_cmd_info.sendtring));   
  
  
//    mymemset(m26_cmd_info.sendtring,0,sizeof(m26_cmd_info.sendtring));
//    sprintf(m26_cmd_info.sendtring,"check result:0x%x,power:%s,quiet_p;%s,low_p;%s,mid_p;%s,high_p;%s,timer:%s,mode:%s,wifi:%s",result,MANUL_CHECK_0(result,power_bit),MANUL_CHECK_0(result,quiet_speed_bit),
//                    MANUL_CHECK_0(result,low_speed_bit),MANUL_CHECK_0(result,mid_speed_bit),MANUL_CHECK_0(result,high_speed_bit),
//                    MANUL_CHECK_0(result,timer_bit),MANUL_CHECK_0(result,mode_bit),MANUL_CHECK_0(result,wifi_bit));
//    DEBUG_Uart_Sendbytes(m26_cmd_info.sendtring,mystrlen(m26_cmd_info.sendtring)); 
}




