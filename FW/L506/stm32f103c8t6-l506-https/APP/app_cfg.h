#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


/*******************�����������ȼ�*******************/
#define STARTUP_TASK_PRIO       4	   

//#define	TASK_MODBUS_PRIO			5 
#define	TASK_USER_MQTT_PRIO			7 
#define	TASK_L506_PRIO			6 
#define	TASK_UPLOAD_DATA_PRIO			8 
#define	TASK_FOG_INIT_AND_MONITOR_PRIO			9

//#define MOUDBUS_MUTEX_PRIO 10


#define UPLOAD_MUTEX_PRIO     11
#define MQTT_CMD_MUTEX_PRIO     12
#define STDIO_TX_MUTEX_PRIO     13


/************����ջ��С����λΪ OS_STK ��************/
#define STARTUP_TASK_STK_SIZE   80  
//#define	TASK_MODBUS_STK_SIZE		200
#define	TASK_USER_MQTT_STK_SIZE		128
#define	TASK_L506_STK_SIZE		300
#define TASK_UPLOAD_DATA_STK_SIZE  256
//�������ջΪ256ʱmqtt_cmd���д��ݵ�ָ������
#define TASK_FOG_INIT_AND_MONITOR_STK_SIZE  280

#endif

