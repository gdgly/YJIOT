#ifndef __USER_CHARGE_H__
#define __USER_CHARGE_H__

#include "mico_rtos.h"


#define MONTH_TO_MIN   (30*24*60)
#define DAY_TO_MIN     (24*60)
#define HOUR_TO_MIN    (60)


#define FLASH_SECTOR_SIZE           0x1000      //4kb
#define CHARGE_FLASH_BASE           0xC0000
#define LEFT_TIME_ADDR_BASE         CHARGE_FLASH_BASE
//����೤ʱ�䱣��һ��ʣ��ʱ��  ��λ����
#define SAVE_TIME_INTERVAL    60
//#define SAVE_TIME_INTERVAL    2
#define LEFT_TIME_NUMS        6
//ʣ��ʱ��ʹ��5����б��棬��ֹֻʹ�ø�������ǰ10�ֽڰ�FLASHд��,ÿһ��10���ֽ�
#define LEFT_TIME_MAX_OFFSET  (10*LEFT_TIME_NUMS)
#define RECORD_ADDR_BASE            (LEFT_TIME_ADDR_BASE + FLASH_SECTOR_SIZE)
//#define TIMING_ADDR_BASE            0xC0020
#define TIMING_ADDR_BASE            (RECORD_ADDR_BASE + FLASH_SECTOR_SIZE)
//8���ֽ�Ϊ1��Ʒ�����ȡ120�飬120����һ����ѯ��ƽ��ÿ���ֽ�1Сʱ����һ��
#define IN_SECTOR_MAX_OFFSET        (8*120)
//#define IN_SECTOR_MAX_OFFSET        (8*2)
//#define TIMING_MAX_ADDR             (TIMING_MAX_ADDR + 10 * 120)
#define CHARGE_FLASH_MAX_ADDR       (TIMING_ADDR_BASE + 10 * 0x1000)
#define FLASH_MAX_WRITE_TIMES       80000
//#define FLASH_MAX_WRITE_TIMES       4
//#define FLASH_MAX_WRITE_TIMES       0x3

//#define CHARGE_FLASH_MAX_ADDR       (TIMING_ADDR_BASE + 99 * 8)

#define FLASH_MAX_FAULT_TIMES       30

//�����ظô���ʱ��������FLASH�Ѿ�д��
#define FLASH_WRITE_ERR       -4
#define FLASH_READ_ERR       -5
#define FLASH_EARSE_ERR      -6
#define COUNT_ADDR_BEYOND    -7
#define FLASH_UNKNOWN_ERR       -3




#define user_charge_log(M, ...) custom_log("USER", M, ##__VA_ARGS__)
#define user_charge_log_trace() custom_log_trace("USER")


typedef struct{

  unsigned char IsChargeType;
  unsigned char IsHaveTime;
  unsigned long left_time;
  unsigned long flash_write_times;   //��¼flash�����Ĵ��� flash��ַ 0x80000-0x80007��д����
  unsigned long in_sector_offset;
  unsigned char left_time_mon;
  unsigned char left_time_day;
  unsigned char left_time_hour;
  unsigned char left_time_min;

}charge_lefttime_t;

typedef struct{

  unsigned long count_sector_addr_base;
  unsigned long left_time;
  unsigned long flash_write_times;
  unsigned long in_sector_offset;
  unsigned char count_flash_fault_times;

}charge_count_t;


typedef struct{
  
  charge_lefttime_t   lefttime;
  charge_count_t      count;
  unsigned char     IsChargeInit;
  int                 FlashFault;
  unsigned long     flash_fault_times;
  unsigned long     record_flash_write_times;
  mico_mutex_t        charge_mutex;
  
}user_charge_t;


OSStatus charge_flash_init(void);
OSStatus charge_flash_erase(uint32_t start_address, uint32_t end_addr );
OSStatus charge_flash_write( uint32_t start_address, uint8_t* data ,uint32_t length );
OSStatus charge_flash_read(uint32_t start_address, uint8_t* data ,uint32_t length );
OSStatus get_lefttime_register_data(void);
OSStatus get_record_address_register_data(void);
OSStatus updata_charge_lefttime_register(void);
OSStatus updata_charge_record_address_register(void);
OSStatus count_sector_earse_restart(void);
OSStatus updata_charge_count_register(void);
OSStatus charge_init(void);
void stop_charge_mode(void);
void updata_timedata(void);
OSStatus charge_info_confirm(char *ordno, char type,unsigned long charge_time);
OSStatus updata_charge_timedata(const char *ordno, char type,unsigned long charge_time);
OSStatus charge_change_to_next_count_sector(void);
void charge_thread(mico_thread_arg_t arg);





#endif