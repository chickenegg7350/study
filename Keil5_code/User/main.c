#include "OLED_UI.h"
#include "OLED_UI_MenuData.h"
//#include "Bootloader.h"
#include "delay.h"
#include "OLED.h"
#include "MyRTC.h"
#include "AD.h"
#include "AHT20.h" 
#include "stm32_power.h" 
#include "MPU6050.h"
#include "YXMP3.h"
#include "W25Q128_SPI1.h"


//��Ŀ���Ž���Ȳ���������ˣ��Ҿ��Ұ� (



extern MenuPage*  CurrentMenuPage;

void STM32_rouse(void);
int16_t GyroX,UPGyroX=0;
bool hold = false;

int main(){
	//��ʼ��OLED��ʾ��
	
	OLED_Init();
	
	MyRTC_Init();	
	
	delay_init();
	
	Key_Init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);      // ����AFIOʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //��ӳ������,��PB3��PB4��PA15����ͨIO��PA13&14����SWD����

	
	//������һ�ο��������ģ����ǿռ䲻̫�����Ȳ�Ҫ��
//	/*���ݼĴ�������ʹ��*/
//	PWR_BackupAccessCmd(ENABLE);							//ʹ��PWR�����Ա��ݼĴ����ķ���
//	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)			//ͨ��д�뱸�ݼĴ����ı�־λ���ж��Ƿ��ǵ�һ������
//	{	PASS_Aton(); }
//	MyRTC_Init();
	
	YXMP3_PWROFF();
	
	//�����Լ�+����
	
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVICΪ����2
	 
	W25Q128_Init();
	
	AHT20_Init();
	
	Time_page_io_init();
	
	STM32_rouse();                     //�����������õ��ж�����
	
	AD_Init();				       //ADת������ʼ��
	
	Load_Para();
	//20ms�Ķ�ʱ�ж�,����OLED_UI���жϺ�������������
	Timer_Init();
	//������ʼ��	
	//���õ�ǰҳ��Ľṹ��ָ��
	CurrentMenuPage = &MainMenuPage;	//���õ�ǰҳ��Ľṹ��ָ��
	//��ʼ���˵�ҳ�����
	CurrentMenuPageInit();
	uint8_t p;
	uint16_t oodd=100;	
	OLED_Brightness_DODO();
	
	extern bool RAWA;
	extern float Time_page_PXXXX;
	while(oodd)
	{
		OLED_Clear();
		Time_page();	
		OLED_Brightness_UPUP();
		oodd--;
		OLED_Update();
	}	
	while(1){ 
	//����
	OLED_Clear();
		

	
	Time_page();	
	p=GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);
		
	if(oodd>800)
	{p=1;oodd=0;}
	else oodd++;
	if(hold)oodd=0;
	
	if(RAWA)//����Ϩ��
	{
		MPU6050_Get_GXData(&GyroX);
		if( GyroX < -4000 )
		{
			p = 1;
			oodd=0;
		}
	}		
	if( p == 1)
	{
		
		OLED_UI_FadeoutAllArea();
		Time_page_PXXXX=64;
		POWER_OFF=1;
		Power_OFF();
		OLED_Brightness_DODO();
		oodd=100;
		while(oodd)
		{
			OLED_Clear();
			Time_page();	
			OLED_Brightness_UPUP();
			oodd--;
			delay_ms(5);
			OLED_Update();
		}	
	}	

		

	if( GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1)
	{
		oodd=0;
		OLED_UI_EN=true;
		OLED_UI_FadeoutAllArea();
		Time_page_PXXXX=64;
		OLED_UI_MainLoop();
		
		delay_ms(100);
		OLED_Brightness_DODO();
	}	
	OLED_Brightness_UPUP();	
	delay_ms(5);
//	//ˢ��
	OLED_Update();
	}
}

//�жϴ�����
void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) == SET)	   //�ⲿ�ж�0���ߴ������жϣ�ҡ���²�
	{
		POWER_OFF=0;	
		EXTI_ClearITPendingBit(EXTI_Line0);		//����жϱ�־λ																								
	}
}



//�жϺ���
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
		OLED_UI_InterruptHandler();		
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}


extern uint8_t Pick_up;//�������־λ
extern int16_t RAWAtri;//̧�󴥷�ֵ
extern int16_t RAWAtrigger;//ʵ��̧�󴥷�ֵ
uint8_t Addtrigger=0;//�ֱ����ۼ�ֵ

void EXTI9_5_IRQHandler(void)
{	
	
	if (EXTI_GetITStatus(EXTI_Line7) == SET)		//�ⲿ�ж�7���ߴ������жϣ�MPU6050
	{		
		SystemInit();
	    MPU6050_Get_GXData(&GyroX);//��ȡx������������
		if(GyroX>RAWAtrigger)//������ڴ���ֵ
		{ 	
			POWER_OFF = 0; //ȡ������
			Addtrigger= 0;
			RAWAtrigger = RAWAtri*70;//�ָ�����ֵ
		}
		else if(Pick_up)
		{
			if(UPGyroX-5<GyroX && GyroX<UPGyroX+5)//����ֱ�û�ж�
			{ 
				if(Addtrigger<80)
				{	
					Addtrigger++;//�ۼ�80�Σ�ʵ�ʴ�Լ5��
				}
				else RAWAtrigger=UPGyroX+5;//��Ϊ�˶�����ģʽ��һ����⵽�е��˶������ϻ���
			}
			else {Addtrigger = 0;}
			UPGyroX=GyroX;
			
	    }
		EXTI_ClearITPendingBit(EXTI_Line7);		//����жϱ�־λ																								
	}

}


//���ڻ��ѵ��ⲿ�жϣ�ͳһ����������
void STM32_rouse(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	//PA0��Key_Init();�����Ѿ����ù��ˣ�����ֱ�Ӵ�PA7��ʼ	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;	//MPU6050��
	GPIO_Init(GPIOB,&GPIO_InitStructure);
		
	/*AFIOѡ���ж�����*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//���ⲿ�жϵ�0����ӳ�䵽GPIOA
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);//���ⲿ�жϵ�7����ӳ�䵽GPIOB
	
	/*EXTI��ʼ��*/
	EXTI_InitTypeDef EXTI_InitStructure;				     //����ṹ�����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;			    //ָ���ⲿ�ж���ʹ��
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	   //ָ���ⲿ�ж���Ϊ�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//ָ���ⲿ�ж���Ϊ�����ش���

	EXTI_InitStructure.EXTI_Line = EXTI_Line0;			//ѡ�������ⲿ�ж�
	EXTI_Init(&EXTI_InitStructure);					   //���ṹ���������EXTI_Init������EXTI����
	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //ָ���ⲿ�ж���Ϊ�½��ش���
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;			    //ѡ�������ⲿ�ж�
	EXTI_Init(&EXTI_InitStructure);						   //���ṹ���������EXTI_Init������EXTI����
	
	
	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;				       //����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//ָ��NVIC��·����ռ���ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	    //ָ��NVIC��·����Ӧ���ȼ�Ϊ2
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;	  //ѡ������NVIC��0��
	NVIC_Init(&NVIC_InitStructure);						 //����NVIC
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//ָ��NVIC��·����ռ���ȼ�Ϊ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	    //ָ��NVIC��·����Ӧ���ȼ�Ϊ
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;  
	NVIC_Init(&NVIC_InitStructure);		
}

