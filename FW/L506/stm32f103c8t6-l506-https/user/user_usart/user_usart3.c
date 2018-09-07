#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stdio.h"
#include "user_usart3.h"

/// ����USART�����ж�
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

//�������ú�����Ŀǰֻ���䴮��3
void usart3_config(USART_TypeDef* USARTx, uint32_t BaudRate)
{
	USART_InitTypeDef	USART_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	//��������1��GPIOA��ʱ��  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 
	
	//PB10(TX),PB11(RX)��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	//���ô���
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//8������λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;				//����żУ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�������պͷ���
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ��������	
	USART_Init(USARTx, &USART_InitStructure);	//��ʼ������
	
	USART_Cmd(USARTx, ENABLE);	//���ô���
  
  NVIC_Configuration();
  
  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE); 
}


