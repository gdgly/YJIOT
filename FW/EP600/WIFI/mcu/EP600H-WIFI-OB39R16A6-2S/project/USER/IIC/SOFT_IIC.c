#include "SOFT_IIC.h"

#include "sensor.h"

/*
void Set_SDA_Pin_Mode(unsigned char mode)
{
    if(mode == PIN_INPUT)
    {
        //PIN����Ϊ����ʱ��PxM0��Ӧ��λ��0��PxM1��Ӧ��λ��1
        SOFT_IIC_PxM0 &= ~(1 << SOFT_IIC_SDA_PORTBIT);
        SOFT_IIC_PxM1 |= ((1 << SOFT_IIC_SDA_PORTBIT));
    }
    else if(mode == PIN_OUTPUT)
    {      
        SOFT_IIC_PxM0 &= ~(1 << SOFT_IIC_SDA_PORTBIT);
        SOFT_IIC_PxM1 &= ~(1 << SOFT_IIC_SDA_PORTBIT);
    }
}

void Set_SCL_Pin_Mode(unsigned char mode)
{
    if(mode == PIN_INPUT)
    {
        //PIN����Ϊ����ʱ��PxM0��Ӧ��λ��0��PxM1��Ӧ��λ��1
        SOFT_IIC_PxM0 &= ~(1 << SOFT_IIC_SCL_PORTBIT);
        SOFT_IIC_PxM1 |= ((1 << SOFT_IIC_SCL_PORTBIT));
    }
    else if(mode == PIN_OUTPUT)
    {      
        SOFT_IIC_PxM0 &= ~(1 << SOFT_IIC_SCL_PORTBIT);
        SOFT_IIC_PxM1 &= ~(1 << SOFT_IIC_SCL_PORTBIT);     
    }
}

void Soft_IIC_PinConifg(void)
{
    //PIN����Ϊ���������PxM0��Ӧ��λ��1��PxM1��Ӧ��λ��0  
    SOFT_IIC_PxM0 &= ~((1 << SOFT_IIC_SDA_PORTBIT) | (1 << SOFT_IIC_SCL_PORTBIT));
    SOFT_IIC_PxM1 &= ~((1 << SOFT_IIC_SDA_PORTBIT) | (1 << SOFT_IIC_SCL_PORTBIT));
  
    SOFT_IIC_SCL_PIN = 1;
    SOFT_IIC_SDA_PIN = 1;
}

#define IIC_DELAY_COUNT  10

static void i2c_delay(void)
{
	int i = 0;
	for(i = 0;i < IIC_DELAY_COUNT;i++);
}

*/

IIC_Operations_Typedef IIC_Operations;

static void i2c_delay1(unsigned int num)
{
	int i = 0;
	for(i = 0;i < num;i++);
}



//**********������дʱ���Ӻ���**********
void I2C_start(void)
{
  IIC_Operations.Set_SDA_Pin_Mode(PIN_OUTPUT);
	IIC_Operations.SDA_1();
	IIC_Operations.i2c_delay();
	IIC_Operations.SCL_1();
	IIC_Operations.i2c_delay();
	IIC_Operations.SDA_0();
	IIC_Operations.i2c_delay();
	IIC_Operations.SCL_0();
	IIC_Operations.i2c_delay();
}


//**********ֹͣ�����Ӻ���**********
void I2C_stop(void)
{
  IIC_Operations.Set_SDA_Pin_Mode(PIN_OUTPUT);
	IIC_Operations.SDA_0();
	IIC_Operations.i2c_delay();
	IIC_Operations.SCL_1();
	IIC_Operations.i2c_delay();
	IIC_Operations.SDA_1();
	IIC_Operations.i2c_delay();
	
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    �Σ���
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*********************************************************************************************************
*/
unsigned char I2C_WaitAck(void)
{
	unsigned char re = 0;
  
	IIC_Operations.SDA_1();	/* CPU�ͷ�SDA���� */
	
  IIC_Operations.Set_SDA_Pin_Mode(PIN_INPUT);
  
	IIC_Operations.i2c_delay();
	IIC_Operations.SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	IIC_Operations.i2c_delay();
	
	if (IIC_Operations.I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	IIC_Operations.SCL_0();
	IIC_Operations.i2c_delay();
	return re;
}

/*��Ӧ*/
void I2C_Ack(void)
{
  IIC_Operations.Set_SDA_Pin_Mode(PIN_OUTPUT);
	IIC_Operations.SDA_0();	/* CPU����SDA = 0 */
	IIC_Operations.i2c_delay();
	IIC_Operations.SCL_1();	/* CPU����1��ʱ�� */
	IIC_Operations.i2c_delay();
	IIC_Operations.SCL_0();
	IIC_Operations.i2c_delay();
	IIC_Operations.SDA_1();	/* CPU�ͷ�SDA���� */
}


/*
*********************************************************************************************************
*	�� �� ��: i2c_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void I2C_NAck(void)
{
  IIC_Operations.Set_SDA_Pin_Mode(PIN_OUTPUT);
	IIC_Operations.SDA_1();	/* CPU����SDA = 1 */
	IIC_Operations.i2c_delay();
	IIC_Operations.SCL_1();	/* CPU����1��ʱ�� */
	IIC_Operations.i2c_delay();
	IIC_Operations.SCL_0();
	IIC_Operations.i2c_delay();	
}



//**********д��8λ�Ӻ���**********
void I2C_SendOneByte(unsigned char a)
{
	unsigned char temp;
	unsigned char i, j;

  IIC_Operations.Set_SDA_Pin_Mode(PIN_OUTPUT);
	temp = a;
	for(i=0;i<8;i++)
	{
	    j=temp&0x80;
		if(j==0)
		{
			IIC_Operations.SDA_0();
		}
		else
		{
			IIC_Operations.SDA_1();
		}
		IIC_Operations.SCL_1();
		IIC_Operations.i2c_delay();
		IIC_Operations.SCL_0();
    IIC_Operations.i2c_delay();
		temp = temp<<1;
	}
  
}


unsigned char I2C_ReadOneByte(void)
{
	unsigned char i;
	unsigned char value;

	/* ������1��bitΪ���ݵ�bit7 */
	value = 0;
  IIC_Operations.Set_SDA_Pin_Mode(PIN_INPUT);
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		IIC_Operations.SCL_1();
		IIC_Operations.i2c_delay();
		if (IIC_Operations.I2C_SDA_READ())
		{
			value |= 1;
		}
		IIC_Operations.SCL_0();
    IIC_Operations.i2c_delay();
	}
	return value;
}





/**
  * @brief   ʹ��IIC��ȡ����
  * @param   
  * 	@arg ClientAddr:���豸��ַ
  *		@arg pBuffer:����ɴӻ���ȡ�����ݵĻ�����ָ��
  *		@arg readAddr:��ʾ��ȡ���豸�ĸ��Ĵ���������
  *		@arg NumByteToRead:��ȡ�����ݳ���
  * @retval  ��
  */
unsigned int I2C_ReadBytes(unsigned char ClientAddr, unsigned char* pBuffer,unsigned int NumByteToRead)
{
	
	/* ��1��������I2C���������ź� */
	I2C_start();
	
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	I2C_SendOneByte((ClientAddr << 1) | I2C_DIR_RD);	/* �˴��Ƕ�ָ�� */
  	
	/* ��3�����ȴ�ACK */
	if (I2C_WaitAck() != 0)
	{
		goto cmd_fail;	/* ������Ӧ�� */
	}

	while(NumByteToRead) 
	{
     
     if(IIC_Operations.IIC_NUM == TOUCH_KEY_IIC)
     {
       //������оƬͨѶÿ���ͻ��߽�����һ���ֽں������ʱ�ϳ���ʱ�䣬����ͨѶ�������
       i2c_delay1(100);   
     }
     else if(IIC_Operations.IIC_NUM == HPD05_IIC)
     {
       i2c_delay1(10);
     }
     
    
		*pBuffer = I2C_ReadOneByte();
    
		/* ��ָ������ */
		pBuffer++; 
      
		/*�������Լ� */
		NumByteToRead--;

		if (NumByteToRead == 0)
		{
			I2C_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */

			/* ����I2C����ֹͣ�ź� */
			//I2C_stop();
			break;
		}
    
		I2C_Ack();	/* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */ 

 
  }

	/* ����I2C����ֹͣ�ź� */
	I2C_stop();
	return 0;	/* ִ�гɹ� */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	I2C_stop();
	return 1;
}


/**
  * @brief   ʹ��IICд������
  * @param   
  * 	@arg ClientAddr:���豸��ַ
  *		@arg pBuffer:������ָ��
  *     @arg NumByteToWrite:д���ֽ���
  * @retval  ��
  */
unsigned int I2C_WriteBytes(unsigned char ClientAddr, unsigned char* pBuffer, unsigned int NumByteToWrite)
{
	//unsigned int m;	

  /*����0������ֹͣ�źţ������ڲ�д������*/
  I2C_stop();
  
  /* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms 			
    CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
  */		
    /* ��1��������I2C���������ź� */
    I2C_start();
    
    /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    I2C_SendOneByte((ClientAddr << 1) | I2C_DIR_WR);	/* �˴���дָ�� */
    
    /* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
    if (I2C_WaitAck() == 0)
    {
      //break;
    }
	  else
	 {
		  goto cmd_fail;
	 }
  
	
  while(NumByteToWrite)
  {
     if(IIC_Operations.IIC_NUM == TOUCH_KEY_IIC)
     {
       //������оƬͨѶÿ���ͻ��߽�����һ���ֽں������ʱ�ϳ���ʱ�䣬����ͨѶ�������
       i2c_delay1(100);   
     }
     else if(IIC_Operations.IIC_NUM == HPD05_IIC)
     {
       i2c_delay1(10);
     }
	    /* ��4������ʼд������ */
      I2C_SendOneByte(*pBuffer);

      /* ��5�������ACK */
      if (I2C_WaitAck() != 0)
      {
          goto cmd_fail;	/* ������Ӧ�� */
      }
      
      NumByteToWrite--;
      
      if(NumByteToWrite == 0)
      {
          goto end;
      }
      
      pBuffer++;	/* ��ַ��1 */		
      
     
  }
	
	/* ����ִ�гɹ�������I2C����ֹͣ�ź� */
end:
  i2c_delay1(60);
  I2C_stop();
  return 0;

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
  I2C_stop();
  return 1;
}


