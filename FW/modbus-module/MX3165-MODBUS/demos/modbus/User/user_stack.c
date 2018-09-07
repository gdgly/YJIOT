#include "user_stack.h"


void rb_initialize(user_ring_buffer_t* pringbuff, uint8_t* buff, uint16_t length)
{
	pringbuff->pbuff = buff;
	pringbuff->pend  = buff + length;
	pringbuff->wp = buff;
	pringbuff->rp = buff;
	pringbuff->length = length;
	pringbuff->flagoverflow = 0;
}

void rb_clear(user_ring_buffer_t* pringbuff)
{
 	pringbuff->wp = pringbuff->pbuff;
	pringbuff->rp = pringbuff->pbuff;
	pringbuff->flagoverflow = 0;
}


/**
  * @brief  ѹ�뵥�ֽڵ�������
  * @param  pRingBuff���������ringbuffer
  *         value��ѹ�������
  * @note   
  * @retval void
  */	
void rb_push(user_ring_buffer_t* pringbuff, uint8_t value)
{
    uint8_t* wp_next = pringbuff->wp + 1;
    if( wp_next == pringbuff->pend ) {
	wp_next -= pringbuff->length; // Rewind pointer when exceeds bound
    }
    if( wp_next != pringbuff->rp ) {
	*pringbuff->wp = value;
	pringbuff->wp = wp_next;
    } else {
	pringbuff->flagoverflow = 1;
    }
}

/**
  * @brief  ѹ�����ֽڵ�������
  * @param  pRingBuff���������ringbuffer   
  * @note   
  * @retval ѹ��������
  */	
uint8_t rb_pop(user_ring_buffer_t *pringbuff)
{
    if( pringbuff->rp == pringbuff->wp ) 
        return 0; // empty
  
    uint8_t ret = *(pringbuff->rp++);
    if( pringbuff->rp == pringbuff->pend ) {
	pringbuff->rp -= pringbuff->length; // Rewind pointer when exceeds bound
    }
    return ret;
}

/**
  * @brief  ��ȡ��������δ������ֽ���
  * @param  pRingBuff���������ringbuffer   
  * @note   
  * @retval ��������ֽ���
  */
uint16_t rb_get_count(const user_ring_buffer_t *pringbuff)
{
    return (pringbuff->wp - pringbuff->rp + pringbuff->length) % pringbuff->length;
}

/**
  * @brief  �жϻ������Ƿ�Ϊ��
  * @param  pRingBuff���������ringbuffer   
  * @note   
  * @retval ��Ϊ1������Ϊ0
  */
int8_t rb_is_empty(const user_ring_buffer_t *pringbuff)
{
    return pringbuff->wp == pringbuff->rp; 
}

/**
  * @brief  �жϻ������Ƿ��
  * @param  pRingBuff���������ringbuffer   
  * @note   
  * @retval ��Ϊ1������Ϊ0
  */
int8_t rb_is_full(const user_ring_buffer_t *pringbuff)
{
    return (pringbuff->rp - pringbuff->wp + pringbuff->length - 1) % pringbuff->length == 0;
}


