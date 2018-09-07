#include "step_motor.h"
#include "global.h"
//#include "TM1620.h"
#include "dc_motor.h"
#include "timer.h"


//�����Źر�ʱ�����ż�������Ǹߵ�ƽ�������Ŵ�ʱ�����ż�������ǵ͵�ƽ

void Door_Pin_Init(void)
{
    DOOR_CHECK_PxM0 &= ~(1 << DOOR_CHECK_PORTBIT); 
    DOOR_CHECK_PxM1 &= ~(1 << DOOR_CHECK_PORTBIT); 

}


void Step_Motor_Pin_Init(void)
{
    P1M0 |= ((1 << 2) |(1 << 3));
    P1M1 &= ~((1 << 2) |(1 << 3));
  
    P2M0 |= ((1 << 0) | (1 << 1));
    P2M1 &= ~((1 << 0) | (1 << 1));
   
    STEP_MOTOR_A=0;
    STEP_MOTOR_B=0;
    STEP_MOTOR_C=0;
    STEP_MOTOR_D=0;  
  
    Door_Pin_Init();
}



void Door_Open(void)
{
    if(Is_Door_Open == 1)
    {
        return;
    }
    Is_Door_Open = 1;
    door_open_time = get_sys_stime();
    StartMotor(-220);
}

void Door_Close(void)
{
    if(Is_Door_Open == 0)
    {
        return;
    }
    StartMotor(220);
    Is_Door_Open = 0;
}

void StartMotor(signed long angle)
{
//	  beats = (angle*4076)/360;
    beats = (angle*4076)/360;
    IsStepMotorBusy = 1;
}


void Check_If_Close_Door(void)
{
    const unsigned int code time_interval = 60;
  
    //if(charge_info.IsChargeType == 1 && charge_info.lefttime.l_time == 0)
    if(sys_mode == STANDBY)
    {
        if(Is_Door_Open == 1)
        {
            nowtime_s = get_sys_stime();
            if(nowtime_s >= (door_open_time + time_interval))
            {
                Door_Close();
            }
        }
    }
}


/*
void StopMotor(void)
{
	beats = 0;
}
*/

void TurnMotor(void)
{
	static unsigned char index = 0;
  static const unsigned char code BeatCode[8] = {0x01, 0x03, 0x02, 0x06, 0x04, 0x0c, 0x08, 0x09};
  //const unsigned char code BeatCode[8] = {0x01, 0x03, 0x02, 0x06, 0x04, 0x0c, 0x08, 0x09};


	if(beats != 0)
	{
    
		if(beats > 0)
		{
			index++;
			index = index & 0x07;
			beats--;
		}
		else
		{
			index--;
			index = index & 0x07;
			beats++;
		}
    
    if(BeatCode[index]&0x1) {STEP_MOTOR_A=1;} else {STEP_MOTOR_A=0;}
    if(BeatCode[index]&0x2) {STEP_MOTOR_B=1;} else {STEP_MOTOR_B=0;}
    if(BeatCode[index]&0x4) {STEP_MOTOR_C=1;} else {STEP_MOTOR_C=0;}
    if(BeatCode[index]&0x8) {STEP_MOTOR_D=1;} else {STEP_MOTOR_D=0;}
    

	}
	else
	{
    if(IsStepMotorBusy == 1)
    {
    		STEP_MOTOR_A = 0;
        STEP_MOTOR_B = 0;
        STEP_MOTOR_C = 0;
        STEP_MOTOR_D = 0;
        IsStepMotorBusy = 0;
      
        //���������ת�������������⵽����û�е�λ������ǲ��Ż��˲��ܴ򿪣�ֹͣ���������ʧЧ
        //Door_Check();
//        if(IsSysFault == 1)
//        {
//            //�رշ����������(sys_mode == STANDBY) && (IsSpeedChanged == 1) 
//            sys_mode = STANDBY; 
//            IsSpeedChanged = 1;
//            Stop_DC_Motor();
//        }
    }
    
	}
}
