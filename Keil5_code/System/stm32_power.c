#include "stm32f10x.h"    
#include <stdint.h>  
#include <string.h>  
#include <stdio.h> 
#include "OLED_UI.h"
#include "MPU6050.h"

uint8_t POWER_OFF = 0;  //������־λ��1��ʾ��Ҫ��������������Ϊ�Ķ�
uint8_t POWER_OFF_MODE = 0;  //ѡ��͹���ģʽ��0Ϊ�������������Ըߣ�1Ϊ�͹��Ĵ������ĵ�

	

extern uint8_t RAWA;

void Power_OFF(void)
{
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET);
	
	OLED_Clear();				//����Դ�����
	OLED_Update();				//������ʾ������
	
	OLED_Power(0);

	if(RAWA)MPU6050_WriteReg(0X38, 0X01);     //����MPU6050���ݾ����ж�
	while(POWER_OFF)//�����Ҫ����
	{	
//		if(!POWER_OFF_MODE)
		if(RAWA)
		{   
			PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
		}
		else
		{		/*ʹ��WKUP����*/
			PWR_WakeUpPinCmd(ENABLE);	 //ʹ��λ��PA0��WKUP���ţ�WKUP���������ػ��Ѵ���ģʽ
			PWR_EnterSTANDBYMode();		//STM32����ֹͣģʽ�����ȴ�ָ���Ļ����¼���WKUP�����ػ�RTC���ӣ�
		}
	}
	  	SystemInit();               //��������ʱ��

		if(RAWA)MPU6050_WriteReg(0X38, 0X00);     //�ر�MPU6050���ݾ����ж�
	
		OLED_Power(1);

//		OLED_WriteCommand(0x81);	//���öԱȶ�
//		OLED_WriteCommand(0x00);   //0x00~0xFF
     
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET);	
}






extern int8_t mubz;
extern uint8_t RAWAtri;//̧�󴥷�ֵ
extern int16_t RAWAtrigger;//ʵ��̧�󴥷�ֵ
extern float RTI;



//���BKP�Ĵ������鿴�Ƿ�����Ҫ����ı���
void Load_Para(void)   
{
	uint16_t temp;

		/*���ݼĴ�������ʹ��*/
	PWR_BackupAccessCmd(ENABLE);
	
	//BKP_DR1��RTC����
	
	temp = BKP_ReadBackupRegister(BKP_DR2);//��ȡ�����ڱ��ݼĴ�������ֵ�Ͳ����
	if( (temp>>8 )==0xAE ){ mubz = BKP_ReadBackupRegister(BKP_DR2);}//����־λ

//	temp = BKP_ReadBackupRegister(BKP_DR3);
//	if( (temp>>8 )==0xAB ){ Cursor = BKP_ReadBackupRegister(BKP_DR3);}//����־λ

	temp = BKP_ReadBackupRegister(BKP_DR4);
	if( (temp>>8 )==0xAC ){//����־λ
		RAWAtri = BKP_ReadBackupRegister(BKP_DR4);
		RAWAtrigger=RAWAtri*70;
	}
	
	temp = BKP_ReadBackupRegister(BKP_DR5);
	if( (temp>>8 )==0xAD ){//����־λ
		// �ϲ�Ϊuint32_t  
		uint32_t temp = ( (uint32_t)BKP_ReadBackupRegister(BKP_DR6) << 16 ) | (uint32_t)BKP_ReadBackupRegister(BKP_DR7); 
		memcpy(&RTI, &temp, sizeof(float));//ת����float����
	}
	
	

}
