#include <stdio.h>
#include "global.h"
#include "sys_run.h"
#include "dc_motor.h"
#include "step_motor.h"
#include "sensor.h"
#include "buzzer.h"
#include "UV.h"
#include "wifi_uart.h"
//#include "common.h"
#include "ION.h"
#include "debug_uart.h"
#include "TM1620.h"
#include "timer.h"






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
   
    //wifi������Զ��Ч,wifi��������ϰ����������³���2��Ž�������ģʽ����ֹ�����
    if(key_info.IsTouchedKey == 1 && key_info.WhichKey == KEY_WIFI)
    {
        wifi_earse_easylink_cmd();
        Clear_Touch_Info();
    }
    
    //������˰Ѳ��Źر��ˣ�������������״̬
    if(is_sys_auto_check == 0)
    {
        check_if_doorclose_manual();
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

            if(sys_mode == RUNNING)
            {
                if(user_timer_type < USER_TIMER_4H)
                {
                    user_timer_type += 1;
                    set_user_timer(user_timer_type);
                }
                else
                {
                    stop_user_timer();
                }
                Buzzer_Touch_Key();               
            }             
            break;
          case KEY_MODE: 
            if(is_sys_manual_check == 1)
            {
                sys_check_info.touch_key_check |= (1 << mode_bit);
            } 
            if(sys_mode == RUNNING)
            {
                run_mode = !run_mode;
                if(run_mode == 0)
                {
                    stop_sys_smart_mode();
                }
                else
                {
                    set_sys_to_smart_mode();
                }
                Buzzer_Touch_Key();     
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


void display_sensor_data(void)
{  
    if(is_sys_auto_check == 1)
    {
        return;
    }
   
    nowtime_s = get_sys_stime();
    
    if(nowtime_s % 10 == 0)
    {
        TM1620_DispalyData(SENSOR_PM25,(float)display_pm25_value);
    }
    else if(nowtime_s % 10 == 5)
    {
        TM1620_DispalyData(SENSOR_HCHO,display_HCHO_value);
    }    
}




//���Զ�ģʽ�£������λ�л���10�����ڲ������ٴ��л�������PM���ٽ�ֵʱ��ǳ�Ƶ�����л���λ�����鲻��
void sys_smart_mode(void)
{
    
    static bit speed_changed = 0;   //��ֹPMŨ�����ٽ�ֵʱ��Ƶ���л���λ
    static bit IsLedChange = 0;
    static unsigned long speed_changed_time = 0;   //�ϴ��л��絲ʱ��ʱ��
  
    const unsigned char code speed_continue_time = 10;  //10�����ڲ����ٴ��л��絲
    
  
    
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
    
    if(display_pm25_value < PM25_QUIET_SPEED )
    {
        if(speed_dang != QUIET_SPEED)
        {
            speed_dang = QUIET_SPEED;
            IsSpeedChanged = 1;
            speed_changed = 1;
            IsLedChange = 1;
        }       
    }
    else if(display_pm25_value < PM25_LOW_SPEED)
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

//sys_check_info.status��bitλΪ1��ʾ����豸�����⣬0��ʾ����

//�˺�����ִ�м��������100ms����ΪFGcount�Ĳɼ�������100ms
void sys_dev_auto_check(void)
{

    //ϵͳ���UV�ͷ����ʱ��
    static const unsigned long sys_check_time = 30;
    unsigned long nowtime = 0;
  
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
                //���Ź���
                //sys_check_info.status = ((1 << door_bit) | (1 << fan_bit) | (1 << uv_bit));
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
                //��������,��Ӧ��bitλ����
                sys_check_info.status &= ~(1 << door_bit);
                //sys_check_info.status = 0;
                sys_check_info.step += 1;
            }
            else
            {  
                //sys_check_info.status = ((1 << door_bit) | (1 << fan_bit) | (1 << uv_bit));
                is_auto_check_complete = 1;
                goto exit;
            }
        }
          
        
    }
    else if(sys_check_info.step == 4)    //step4  �������UV��
    {
        nowtime = get_sys_stime();
        //�������UV����Чʱ��20��
        if(nowtime >= (sys_start_time + 10) && nowtime <= (sys_start_time + sys_check_time))
        {
//            if(IsUVfault == 1)
//            {
//                sys_check_info.status |= (1 << uv_bit);
//            }
            

            if(sys_check_info.fg_count < (SPEED_FG_COUNT2 - 1) || sys_check_info.fg_count > (SPEED_FG_COUNT2 + 1))
            //if(sys_check_info.fg_count < 12 || sys_check_info.fg_count > 14)
            {              
                if(sys_check_info.fan_check_fault_times < 0xFF)
                {
                    sys_check_info.fan_check_fault_times += 1;
                }
            }
                         
                        
        }
        else if(nowtime > (sys_start_time + sys_check_time))
        {
            //��Dcmoto_adj()�������ж�FGcount�����źţ�100ms���һ�Σ�20�빲���200�Σ������źŲ�����һ���̶�ֵ��������һ����Χ�ڲ�������������ʽ��м��
          
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
    
}

void stop_sys_auto_check(void)
{
    is_sys_auto_check = 0;
    
    if(is_auto_check_complete == 1)
    {
        sys_check_auto_result_upload();
    }
    is_auto_check_complete = 0;
    if(sys_mode == RUNNING)
    {
        sys_stop();
    }
}

void start_sys_manual_check(void)
{
    is_sys_manual_check = 1;
    sys_check_info.touch_key_check = 0;
    sys_check_info.start_time = get_sys_stime();
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
    sys_check_manual_result_upload();
    Buzzer_Get_Charge();
}
















