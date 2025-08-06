#ifndef __OLED_UI_DRIVER_H
#define __OLED_UI_DRIVER_H
/*���������Ҫ��ֲ����Ŀ������Ҫ�������º�����ʵ�ַ�ʽ���� */
#include "stm32f10x.h"                  // Device header

//��ȡȷ�ϣ�ȡ�����ϣ��°���״̬�ĺ���(��Q��Ϊʲôʹ�ú궨������Ǻ�����A����Ϊ�����������Ч�ʣ����ٴ�������)
#define Key_GetEnterStatus()    !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)
//#define Key_GetBackStatus()     GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)
#define Key_GetBackStatus()     1
#define Key_GetUpStatus()       !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)
#define Key_GetDownStatus()     !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)


//��ʱ���жϳ�ʼ������
void Timer_Init(void);

//������ʼ������
void Key_Init(void);

//��������ʼ������
void Encoder_Init(void);

// ������ʹ�ܺ���
void Encoder_Enable(void);

//������ʧ�ܺ���
void Encoder_Disable(void);

//��ȡ������������ֵ
int16_t Encoder_Get(void);

//��ʱ����
void Delay_us(uint32_t xus);
void Delay_ms(uint32_t xms);
void Delay_s(uint32_t xs);

#endif
