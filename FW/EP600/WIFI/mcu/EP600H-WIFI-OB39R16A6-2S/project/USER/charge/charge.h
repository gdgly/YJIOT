#ifndef __CHARGE_H__
#define __CHARGE_H__

typedef struct{

  unsigned char IsChargeType;
  //unsigned char IsHavetime;
  
  union{
      unsigned long l_time;  //��λ�Ƿ���
      unsigned char c_time[4];  
  }lefttime;
  unsigned long lefttime_bak;
  //unsigned long lefttime_bak;
  unsigned int eeprom_next_write_addr;
  
  unsigned long next_1min_time;
  
}Charge_Typedef;



#define MIN_TO_MS     (60*1000)
#define MIN_TO_S     (60)

#define MONTH_TO_MIN  (30*24*60)
#define DAY_TO_MIN    (24*60)
#define HOUR_TO_MIN   (100)

//��ȡ�Ʒ���Ϣ��ʱ��������λms
#define GET_TIME_INTERVAL   2000

//GET_TIME_INTERVAL ���Ӽ��һ�Σ�2*60=120�룬�������5���Ӹ�wifiģ���޷�ͨѶ������MCU�ԼƷ�ģʽ
#define UART_FAIL_MAX_TIMES   60


void charge_init(void);
void charge_lefttime_flash_write(void);
void charge_lefttime_count(void);






#endif