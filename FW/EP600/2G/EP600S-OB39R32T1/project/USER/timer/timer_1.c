#include "timer.h"
#include "global.h"
#include "touch_key.h"
#include "buzzer.h"

/*
//ʱ������ 22.1184MHz
1��Ļ�������Ϊ 22118400/12


1ms����Ҫ�Ļ�������Ϊ 22118.4/12
22118.4/12 = 1843.2(0x0733)
65536 - 1843 = 43418��0xF8CD��

0.5ms��Ҫ�Ļ�������Ϊ 22118.4/12/2 = 921.6
65536 - 921  = 64615(0xFC67)

0.25ms��Ҫ�Ļ�������Ϊ 22118.4/12/2/2 = 460.8
65536 - 461  = 65075(0xFE33)

0.1ms��Ҫ�Ļ�������Ϊ 22118.4/12/10 = 184.32
65536 - 184  = 65352(0xFF48)
256 - 184 = 72(0x48)
256 - 185 = 71(0x47)

0.05ms = 50us ��Ҫ�Ļ�������Ϊ 22118.4/12/10/2 = 92.16
65536 - 92  = 65444(0xFFA4)



*/




void TIMER1_initialize(void)  //Initialize TIMER0
{
    ET1 = 1;
    TMOD  = d_T1MOD;
    TH1   = d_T1_TH1;  //MODE1 16bit
    //TL0   = d_T0_TL0;
}


void Start_Timer1(void)
{
    TIMER1_initialize();
    
    EA   = 1;
    TR1  = 1;
}


/*����ʱ��Ϊ1msʱ TIMER_BASE = 1������ʱ��Ϊ0.5msʱ TIMER_BASE = 2������ʱ��Ϊ0.25msʱ�� TIMER_BASE = 4��...
����ʱ��Ϊ0.05msʱ�� TIMER_BASE = 20��

��������

*/
//#define TIMER_BASE  4

/* 
�޸Ķ�ʱ������ֻ���޸�timer.h�е� TIMER_BASE  d_T0_TH0  d_T0_TL0 ��3���궨�弴��

ÿ���޸Ķ�ʱ����ʱʱ��� TIMER_BASE �����޸ģ�����ʱ�� 1ms 2ms 10ms�ȵȾͶ�����

*/

//0.1ms timer
void TIMER1_ISR(void) interrupt d_T1_Vector
{
    static unsigned char idata times_1ms = 1 * TIMER_BASE;
    static unsigned char idata times_2ms = 2 * TIMER_BASE;
    static unsigned char idata times_10ms = 10 * TIMER_BASE;
    static unsigned int idata times_100ms = 100 * TIMER_BASE;
    static unsigned int idata times_1s = 1000 * TIMER_BASE;
    static unsigned char buzzer_cnt = 0;
  
    if(--times_1ms == 0)
    {
        g_1ms_flag = 1;
        times_1ms = 1 * TIMER_BASE;
        g_sys_time_ms += 1;
        if(g_sys_time_ms == 0xFFFFFFFF)
        {
            g_sys_time_ms = 0;
        }
    }  
    
    if(--times_2ms == 0)
    {
        g_2ms_flag = 1;
        times_2ms = 2 * TIMER_BASE;
    }  
    
  
    if(--times_10ms == 0)
    {
        g_10ms_flag = 1;
        times_10ms = 10 * TIMER_BASE;
    }
    
    if(--times_100ms == 0)
    {
        g_100ms_flag = 1;
        times_100ms = 100 * TIMER_BASE;
    }
    
    if(--times_1s == 0)
    {
        g_1s_flag = 1;
        times_1s = 1000 * TIMER_BASE;
        if(IsUVOn == 1)
        {
            uv_check_timeinterval++;
            if(IsUVCheck)
            {
                uv_check_flag = 1;
            }
        }
        
        g_1s_times ++;
//        if(g_1s_times % 60 == 0)
//        {
//            g_1min_flag = 1;
//        }

     
    }  
    
    //���ڴ�����������ʱ����
    if(key_info.IsCount == 1)
    {
        key_info.count_times++;
    } 
    
   
    if(gBuzzerTimes_cnt > 0)
    { 
        
        buzzer_cnt--;
        if(buzzer_cnt == 0)
        {
            //���ڷ����������غ��½��ص�����
            BUZZER_N_PIN = !BUZZER_N_PIN;
            buzzer_cnt = gBuzzerEdge_cnt;
        }
        
        gBuzzerTimes_cnt --;
        if(gBuzzerTimes_cnt == 0)
        {
            buzzer_cnt = 0 ;
            BUZZER_P_PIN = 0;
            BUZZER_N_PIN = 0;
        }
        else if(gBuzzerTimes_cnt == (200 * TIMER_BASE))
        {
            //���������ʱ��ʣ��200msʱ�����ƹܽŹرգ�β����������������������һ��
            BUZZER_P_PIN = 0;
        }
    }
 
      

    
}

//��ȡϵͳʱ�䣬��λms
unsigned long get_sys_mstime(void)
{
    return g_sys_time_ms;
}

//��ȡϵͳʱ�䣬��λs
unsigned long get_sys_stime(void)
{
    return g_1s_times;
}



