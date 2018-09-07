#include "includes.h"

/*
 * ��������BSP_Init
 * ����  ��ʱ�ӳ�ʼ����Ӳ����ʼ��
 * ����  ����
 * ���  ����
 */
extern void usart1_config(USART_TypeDef* USARTx, uint32_t BaudRate);
void BSP_Init(void)
{
    SystemInit();		/* ����ϵͳʱ��Ϊ72M */	
    //LED_GPIO_Config();  /* LED �˿ڳ�ʼ�� */
  
    //debug and user config usart
    usart3_config(USART3,115200);
  
    //wifi usart
    usart2_config(WIFI_USART,9600);
}

/*
 * ��������SysTick_init
 * ����  ������SysTick��ʱ��
 * ����  ����
 * ���  ����
 */
void SysTick_init(void)
{
    SysTick_Config(SystemFrequency/OS_TICKS_PER_SEC);//��ʼ����ʹ��SysTick��ʱ��
}
