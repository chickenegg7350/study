#ifndef __OLED_DRIVER_H
#define __OLED_DRIVER_H



#include "stm32f10x.h"         
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdarg.h>
#include "stdio.h"
//ʹ�ú궨�壬�ٶȸ��죨�Ĵ�����ʽ��
#define OLED_D0_Clr()  (GPIOA->BRR = GPIO_Pin_8)     // ��λ SCL (�� GPIOB �� 8 ����������)
#define OLED_D0_Set()  (GPIOA->BSRR = GPIO_Pin_8)    // ��λ SCL (�� GPIOB �� 8 ����������)

#define OLED_D1_Clr()  (GPIOB->BRR = GPIO_Pin_15)    // ��λ SDA (�� GPIOB �� 9 ����������)
#define OLED_D1_Set()  (GPIOB->BSRR = GPIO_Pin_15)   // ��λ SDA (�� GPIOB �� 9 ����������)

#define OLED_RES_Clr()  (GPIOB->BRR = GPIO_Pin_14)   // ��λ RES (�� GPIOB �� 5 ����������)
#define OLED_RES_Set()  (GPIOB->BSRR = GPIO_Pin_14)  // ��λ RES (�� GPIOB �� 5 ����������)

#define OLED_DC_Clr()   (GPIOB->BRR = GPIO_Pin_13)   // ��λ DC (�� GPIOB �� 6 ����������)
#define OLED_DC_Set()   (GPIOB->BSRR = GPIO_Pin_13)  // ��λ DC (�� GPIOB �� 6 ����������)

#define OLED_CS_Clr()   (GPIOB->BRR = GPIO_Pin_12)   // ��λ CS (�� GPIOB �� 7 ����������)
#define OLED_CS_Set()   (GPIOB->BSRR = GPIO_Pin_12)  // ��λ CS (�� GPIOB �� 7 ����������)



#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����


//	oled��ʼ������
void OLED_Init(void);
//	oledȫ��ˢ�º���
void OLED_Update(void);
//	oled�ֲ�ˢ�º���
void OLED_UpdateArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height);
// ������ɫģʽ
void OLED_SetColorMode(bool colormode);
// OLED �������Ⱥ���
void OLED_Brightness(int16_t Brightness);
void OLED_Brightness_UPUP(void);
void OLED_Brightness_DODO(void);


void OLED_Write_CMD(uint8_t data);
void OLED_Power(uint8_t Data);



#endif







