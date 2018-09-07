#ifndef __USER_DEVICE_CHECK_H__
#define __USER_DEVICE_CHECK_H__

#include "mico.h"


#define door_bit   0
#define fan_bit   1
#define uv_bit   2
#define pm25_bit   3

#define power_bit 0
#define quiet_speed_bit 1
#define low_speed_bit   2
#define mid_speed_bit   3
#define high_speed_bit  4
#define timer_bit 5
#define mode_bit 6
#define wifi_bit 7

//mcu��WIFIģ�鷵�ص��Զ����Խ���ж�Ӧ��bitλΪ0��ʾû�����⣬1��ʾ���������
#define AUTO_CHECK_0(result,bit) ((result & (1 << bit))?"FAULT":"OK")
//mcu��WIFIģ�鷵�ص��ֶ����Խ���ж�Ӧ��bitλΪ0��ʾû�м�⣬1��ʾ����
#define MAN_CHECK_0(result,bit) ((result & (1 << bit))?"OK":"FAULT")

#define AUTO_CHECK_1(result,bit) ((result & (1 << bit))?0:1)
#define MAN_CHECK_1(result,bit) ((result & (1 << bit))?1:0)


typedef struct{
  
  unsigned char is_checking;
  char check_id[40];
  unsigned char check_type;
  char check_state;
  unsigned char check_result;
  unsigned int pm25_value;
  
}device_check_t;



enum check_type_t{
  
  auto_check  = 1,
  man_check   = 2
    
};



OSStatus fog_v2_device_check_result_upload(unsigned char check_type);















#endif