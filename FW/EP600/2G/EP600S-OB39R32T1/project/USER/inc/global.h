/*--------------------------------------------------------------------------
global.H

 extern global resource for C51.
--------------------------------------------------------------------------*/
#ifndef		_GLOBAL_H
#define		_GLOBAL_H

#include "touch_key.h"
#include "SOFT_IIC.h"
#include "common.h"
#include "charge.h"
#include "M26.h"
#include "sys_run.h"


/*------------------------------------------------------------------------------------------
;								datastruct.c
;-----------------------------------------------------------------------------------------*/

//----------------------------�ƾڵ��c--------------------------------------------------------
//typedef	unsigned char	Byte;
//typedef	unsigned int	Word;
//typedef	unsigned long	Dword;
////////////////////////////////////////////////////////////
//typedef unsigned char BYTE;
//typedef unsigned int  WORD;
//typedef unsigned long DWORD;

//typedef unsigned char uchar;
//typedef unsigned int  uint;
//typedef unsigned long ulong;

//typedef unsigned char u8;
//typedef unsigned char U8;
//typedef unsigned int u16;
//typedef unsigned int  U16;

//#define	uCHAR	unsigned char
//#define	sCHAR	signed   char
//#define	uINT	unsigned short
//#define	sINT	signed   short
//#define	uLONG	unsigned long
//#define	sLONG	signed long
//#define	npage	xdata
//#define	zpage	data
////////////////////////////////////////////////////

// mode SYS
#define  				STANDBY				0 //����
#define  				RUNNING				1 //������



#define _test_timeflag(x)    ((x == 1)?1:0) 



//Ӳ��UART0�������ݻ��������鳤�ȶ���
#define U0RxBuff_MAXSIZE   155
extern char idata U0RxBuffer[U0RxBuff_MAXSIZE];
extern unsigned char idata U0RxPtr;


//ģ��UART2�������ݻ��������鳤�ȶ���
#ifdef WIFI_SOFT_IIC
#define U2RxBuff_MAXSIZE   100
extern unsigned char U2RxBuffer[U2RxBuff_MAXSIZE];
extern unsigned char U2RxPtr;
#endif

#ifdef WIFI_SOFT_IIC
#define U2TxBuff_MAXSIZE   100
extern unsigned char U2TxBuffer[U2TxBuff_MAXSIZE];
extern unsigned char U2TxPtr;
extern unsigned char U2LdPtr;
extern unsigned char U2TxBitCount;
extern unsigned char U2TxRate;
extern bit IsU2TxBusy;
extern bit IsU2RxBusy;
#endif

//ģ��UART3�������ݻ��������鳤�ȶ���
#ifdef DEBUG_UART_RX
#define U3RxBuff_MAXSIZE   2
extern unsigned char U3RxBuffer[U3RxBuff_MAXSIZE];
extern unsigned char U3RxPtr;
extern bit IsU3RxBusy;
#endif

#define U3TxBuff_MAXSIZE   150
extern unsigned char U3TxBuffer[U3TxBuff_MAXSIZE];
extern unsigned char U3TxPtr;
extern unsigned char U3LdPtr;
extern unsigned char U3TxBitCount;
extern unsigned char U3TxRate;
extern bit IsU3TxBusy;



extern bit g_1ms_flag;
extern bit g_2ms_flag;
extern bit g_10ms_flag;
extern bit g_100ms_flag;
extern bit g_1s_flag;
extern unsigned long g_1s_times;
//extern bit g_1min_flag;
extern unsigned long idata g_sys_time_ms;

extern unsigned char idata FGcount;

extern unsigned char idata speedBak;	//�������
extern bit gbMotorFGEn;
extern unsigned char idata gBMotorFGSet;
extern unsigned char idata speed_dang;
extern bit sys_mode;
extern bit run_mode;
extern bit gbMotorDelay;
extern bit IsSpeedChanged;
extern bit IsFanRunning; 

extern unsigned int pwmDLbak;

extern signed long beats;

extern unsigned int gBuzzerTimes_cnt;
extern unsigned int gBuzzerEdge_cnt;

extern KEY_INFO_Typedef key_info;
extern IIC_Operations_Typedef IIC_Operations;

//extern bit IsSysFault;
extern bit IsStepMotorBusy;

//ADC
extern unsigned int n_data;
extern bit ADCfinish;
//UV
extern bit IsUVOn;
extern bit IsUVfault;
extern unsigned int uv_check_timeinterval;
extern bit IsUVCheck;
extern bit uv_check_flag;
extern char uv_check_times;

//�������ӹ���״̬λ
extern bit Is_ION_On;

//���Ŵ򿪱�־���α�־��ɨ��ʱ�·��Ĵ򿪲��ű�־
extern bit Is_Door_Open;

//�򿪲��ŵ�ʱ��
extern unsigned long door_open_time;

//����������
extern unsigned int PM25_value;
extern unsigned int PM25_value_bak;
//extern float PM1_value;
//extern float PM10_value;
//extern float HCHO_value;


//�Ʒѹ���
extern Charge_Typedef charge_info;




//extern bit Is_selfcheck;

extern unsigned long sys_start_time;
extern unsigned long sys_stop_time;

//�������ʾ��PM����
extern unsigned int display_pm_value;



//M26 busy��־λ
//extern bit IsM26busy;
//extern struct M26_CMD_Typedef m26_cmd_info;


//M26 IMEI��   �ַ�����ʽ����
//extern char M26_IMEI[20];
//��RAM�ռ����ƣ�CCID��CHECK_ID����ͬһ������
extern char ccid[40];
extern bit Is_Get_CCID;
extern char device_id[40];
extern bit Is_Get_IMEI;
//�豸�Ƿ񼤻��־λ
extern bit Is_device_activate;
extern bit Is_m26_wakeup;

//��ȡ�ź�����ʱ������������Ƿ���0
extern bit Is_signal_err_code_zero;


//m26������ͷ�����ͨѶ�ĳ�ʼ�������Ƿ����
extern bit is_m26_fogcloud_init;

//extern struct M26_register_net;




extern unsigned long nowtime_ms;
extern unsigned long nowtime_s;


extern bit dev_status;
//extern bit Is_status_sync;
extern bit sync_this_loop;
//���ͬ�������ϴ�������߳�ʱ������һ��sync����Ҫ�ϴ��豸״̬
extern bit resync;


//�յ������־λ
extern bit charge_confirm;
//�յ�DR������践��DR���յ�DR��־λ
extern bit dr_confirm;
extern bit Is_send_dr_confirm;

extern char order_num[41];

//���ڼ����ϴ����ݵ�ʱ��
extern unsigned long next_upload_data_time;

//wifiָʾ��״̬
extern bit wifi_led_state;


//�Ƿ��ֶ��رձ�־λ�������ֶ��رղ��ź��ֶ�ͨ�������ر�
extern bit Is_close_by_man;


//�´�ִ��ͬ���¼���ʱ��
//extern unsigned long next_sync_mstime;

//�´�ִ��ͬ���¼���ʱ��
extern unsigned long next_sync_stime;

//���ٻ�������ͬ��״̬��־λ�����յ�������ָ���ͬ��ʱ������Ϊ5�룬����3����
extern bit is_fast_sync;

//����ͬ���Ŀ�ʼʱ��
extern unsigned long fast_sync_start_time;


//���2G���źű�־λ
extern bit Is_signal_check;
//����ź�ʱ���ͷ�����ͨѶʧ�ܵĴ���
extern unsigned char signal_check_err_times;


//wifi����MCUϵͳ�Լ��־λ
extern bit is_sys_auto_check;
extern bit is_auto_check_complete;

//wifi����MCUϵͳ�Լ��־λ
extern bit is_sys_manual_check;

extern sys_check_t sys_check_info;


//�ϴ��Զ����Խ����־λ
extern bit is_upload_auto_check_result;

//�ϴ��ֶ����Խ����־λ
extern bit is_upload_manual_check_result;




#endif