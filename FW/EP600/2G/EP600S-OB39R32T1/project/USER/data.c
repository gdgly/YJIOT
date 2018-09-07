#include "dc_motor.h"
#include "global.h"
#include "UV.h"
#include "M26.h"
#include "sys_run.h"



bit g_1ms_flag = 0;
bit g_2ms_flag = 0;
bit g_10ms_flag = 0;
bit g_100ms_flag = 0;
bit g_1s_flag = 0;
unsigned long g_1s_times = 0;
//bit g_1min_flag = 0;
unsigned long idata g_sys_time_ms = 0;



//��������ź�FG�ļ���
unsigned char idata FGcount = 0;


unsigned char idata speedBak = 0;	    //�������
bit gbMotorFGEn = 0;  //�Ƿ����FG����ת�ٱ�־      
unsigned char  idata gBMotorFGSet = 0;    
unsigned char	idata speed_dang = 0;
bit	sys_mode = 0; //ϵͳ����ģʽ����Ϊ����������	
bit	run_mode = 0; //����ģʽ��Ϊ�Զ����ֶ�
bit gbMotorDelay = 0;
bit IsSpeedChanged = 0;  
bit IsFanRunning = 0;
unsigned int pwmDLbak = 0;     //��ǰPWM�����ռ�ձ�

signed long beats = 0;

unsigned int gBuzzerTimes_cnt = 0;
unsigned int gBuzzerEdge_cnt = 0;  //�ñ�������ģ��PWM��ռ�ձ�


//Ӳ��UART0
char idata U0RxBuffer[U0RxBuff_MAXSIZE] = {0};
unsigned char idata U0RxPtr = 0;



#ifdef WIFI_SOFT_IIC
//ģ��UART2
unsigned char U2RxBuffer[U2RxBuff_MAXSIZE];
unsigned char U2RxPtr = 0;
unsigned char U2TxBuffer[U2TxBuff_MAXSIZE];
unsigned char U2TxPtr = 0;
unsigned char U2LdPtr = 0;
unsigned char U2TxBitCount = 0;
unsigned char U2TxRate = 0;
//IsU2TxBusy = 1ʱ����ʾģ��UART2���ڷ������ݣ����������Ҫ���ͣ����ͻ����������ݣ����͵����ݲ��ܷ��ڷ��ͻ�����buffer��0���ֽڣ����Ƿ������ڷ��͵����ݺ���
bit IsU2TxBusy = 0;
bit IsU2RxBusy = 0;
#endif

//ģ��UART3
#ifdef DEBUG_UART_RX
unsigned char U3RxBuffer[U3RxBuff_MAXSIZE];
unsigned char U3RxPtr = 0;
bit IsU3RxBusy = 0;
#endif
unsigned char U3TxBuffer[U3TxBuff_MAXSIZE];
unsigned char U3TxPtr = 0;
unsigned char U3LdPtr = 0;
unsigned char U3TxBitCount = 0;
unsigned char U3TxRate = 0;
//IsU2TxBusy = 1ʱ����ʾģ��UART2���ڷ������ݣ����������Ҫ���ͣ����ͻ����������ݣ����͵����ݲ��ܷ��ڷ��ͻ�����buffer��0���ֽڣ����Ƿ������ڷ��͵����ݺ���
bit IsU3TxBusy = 0; 


//bit IsSysFault = 0;
bit IsStepMotorBusy = 0;


//UV
bit IsUVOn = 0;
bit IsUVfault = 0;
//UV�Ƽ��ʱ����10���ӣ�ÿ������UV��10������һ�Σ��Է�ֹUV�������⣬�Ժ�ÿ��10���Ӽ��һ��
//ÿ��UV�����Ǳ������� uv_check_timeinterval = UV_CHECK_TIME_INTERVAL - 10,��UV_On()������
unsigned int uv_check_timeinterval = UV_CHECK_TIME_INTERVAL - 10;
bit IsUVCheck = 0;
bit uv_check_flag = 0;
//UV������ѹ��⣺ÿ��һ��ʱ���������3�룬ÿ����һ�Σ����ʱ����UV.h�е� UV_CHECK_TIME_INTERVAL �궨��
//ÿ�θ��ϵ�ʱ����һ�μ�⵽��ADC��ֵΪ0�����Գ�ʼ��Ϊ-1���ϵ���ʼ���4�Σ����Ե�1�Σ��Ժ�ÿ�ζ��Ǽ��3��
char uv_check_times = -1; 

//�������ӹ���״̬
bit Is_ION_On = 0;

//���Ŵ򿪱�־���α�־��ɨ��ʱ�·��Ĵ򿪲��ű�־
bit Is_Door_Open = 0;

//�򿪲��ŵ�ʱ��
unsigned long door_open_time = 0;

//����������
unsigned int PM25_value = 0;
unsigned int PM25_value_bak = 0; //���ڼ�¼����ʱPM2.5������
//float PM1_value = 0;
//float PM10_value = 0;
//float HCHO_value = 0;


//ϵͳ�Լ��־λ
//bit Is_selfcheck = 0;


//ϵͳ����ʱ�䣬���ڴ�����������
unsigned long sys_start_time = 0;
//ϵͳ�ر�ʱ�䣬���ڴ�����������
unsigned long sys_stop_time = 0;


unsigned int display_pm_value = 0;


//M26 IMEI��   �ַ�����ʽ����
//char M26_IMEI[20] = {0};
//��RAM�ռ����ƣ�CCID��CHECK_ID����ͬһ������
char ccid[40] = {0};
bit Is_Get_CCID = 0;
char device_id[40] = {0};
//bit Is_Get_IMEI = 0;
bit Is_device_activate = 0;
bit Is_m26_wakeup = 0;

//��ȡ�ź�����ʱ������������Ƿ���0
bit Is_signal_err_code_zero = 0;

//m26������ͷ�����ͨѶ�ĳ�ʼ�������Ƿ����
bit is_m26_fogcloud_init = 0;


unsigned long nowtime_ms = 0;
unsigned long nowtime_s = 0;




//�豸״̬
bit dev_status = 0;
//bit Is_status_sync = 0;
bit sync_this_loop = 0;
//���ͬ�������ϴ�������߳�ʱ������һ��sync����Ҫ�ϴ��豸״̬
bit resync = 0;


//�յ������־λ
bit charge_confirm = 0;
//�յ�DR������践��DR���յ�DR��־λ
bit dr_confirm = 0;
bit Is_send_dr_confirm = 0;

char order_num[41] = {0};

//���ڼ����ϴ����ݵ�ʱ��
unsigned long next_upload_data_time = 0;

//wifiָʾ��״̬
bit wifi_led_state = 0;

//�Ƿ��ֶ��رձ�־λ�������ֶ��رղ��ź��ֶ�ͨ�������ر�
bit Is_close_by_man = 0;


//�´�ִ��ͬ���¼���ʱ��
unsigned long next_sync_stime = 0;
//���ٻ�������ͬ��״̬��־λ�����յ�������ָ���ͬ��ʱ������Ϊ5�룬����3����
bit is_fast_sync = 0;
//����ͬ���Ŀ�ʼʱ��
unsigned long fast_sync_start_time = 0;

//���2G���źű�־λ
bit Is_signal_check = 0;
//����ź�ʱ���ͷ�����ͨѶ�ɹ��Ĵ���
unsigned char signal_check_err_times = 0;


//wifi����MCUϵͳ�Լ��־λ
bit is_sys_auto_check = 0;
bit is_auto_check_complete = 0;

//wifi����MCUϵͳ�Լ��־λ
bit is_sys_manual_check = 0;

sys_check_t sys_check_info;


//�ϴ��Զ����Խ����־λ
bit is_upload_auto_check_result = 0;

//�ϴ��ֶ����Խ����־λ
bit is_upload_manual_check_result = 0;



