#include "OLED_UI_Driver.h"

/*
���ļ�˵������[Ӳ�������]
���ļ�������������������������������Ҫ��ֲ����Ŀ�������ʵ������޸���ش��롣
����ȷ��oled��Ļ�ܹ����������������ܹ���ȷ�����л�������ʱ������ʾ�ַ����ȣ����Ϳ��Կ�ʼ��ֲ
�йذ�����������ȵ����������ˡ�
*/


/**
 * @brief ��ʱ���жϷ������ĳ�ʼ�����������ڲ���20ms�Ķ�ʱ���ж�
 * @param ��
 * @return ��
 */
void Timer_Init(void)
{
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);			//����TIM4��ʱ��
	
	/*����ʱ��Դ*/
	TIM_InternalClockConfig(TIM4);		//ѡ��TIM4Ϊ�ڲ�ʱ�ӣ��������ô˺�����TIMĬ��ҲΪ�ڲ�ʱ��
	
	/*ʱ����Ԫ��ʼ��*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//����ṹ�����
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//ʱ�ӷ�Ƶ��ѡ�񲻷�Ƶ���˲������������˲���ʱ�ӣ���Ӱ��ʱ����Ԫ����
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//������ģʽ��ѡ�����ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = 200 - 1;				//�������ڣ���ARR��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;				//Ԥ��Ƶ������PSC��ֵ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;			//�ظ����������߼���ʱ���Ż��õ�
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);				//���ṹ���������TIM_TimeBaseInit������TIM4��ʱ����Ԫ	
	
	/*�ж��������*/
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);						//�����ʱ�����±�־λ
																//TIM_TimeBaseInit����ĩβ���ֶ������˸����¼�
																//��������˱�־λ�������жϺ󣬻����̽���һ���ж�
																//�������������⣬������˱�־λҲ��
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);					//����TIM4�ĸ����ж�
	
	/*NVIC�жϷ���*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//����NVICΪ����2
																//����ռ���ȼ���Χ��0~3����Ӧ���ȼ���Χ��0~3
																//�˷������������������н������һ��
																//���ж���жϣ����԰Ѵ˴������main�����ڣ�whileѭ��֮ǰ
																//�����ö�����÷���Ĵ��룬���ִ�е����ûḲ����ִ�е�����
	
	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;						//����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;				//ѡ������NVIC��TIM4��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//ָ��NVIC��·����ռ���ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//ָ��NVIC��·����Ӧ���ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStructure);								//���ṹ���������NVIC_Init������NVIC����
	
	/*TIMʹ��*/
	TIM_Cmd(TIM4, ENABLE);			//ʹ��TIM4����ʱ����ʼ����
}

/**
 * @brief ������ʼ�����������ڳ�ʼ������GPIO
 * @param ��
 * @note GPIO����ʼ��Ϊ��������ģʽ
 * @return ��
 */
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}



/**
 * @brief ��������ʼ������������ʱ��1����Ϊ������ģʽ
 * @param ��
 * @return ��
 */
void Encoder_Init(void)
{
	
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//		
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR
//	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC
//	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
//	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
//	
//	TIM_ICInitTypeDef TIM_ICInitStructure;
//	TIM_ICStructInit(&TIM_ICInitStructure);
//	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
//	TIM_ICInitStructure.TIM_ICFilter = 0xF;
//	TIM_ICInit(TIM1, &TIM_ICInitStructure);
//	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
//	TIM_ICInitStructure.TIM_ICFilter = 0xF;
//	TIM_ICInit(TIM1, &TIM_ICInitStructure);
//	
//	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
//	
//	TIM_Cmd(TIM1, ENABLE);
//	
//	TIM_SetCounter(TIM1, 0);
}


/**
 * @brief ������ʹ�ܺ���
 * @param ��
 * @return ��
 */
void Encoder_Enable(void)
{
//	TIM_Cmd(TIM1, ENABLE);
}

/**
 * @brief ������ʧ�ܺ���
 * @param ��
 * @return ��
 */
void Encoder_Disable(void)
{
//	TIM_Cmd(TIM1, DISABLE);
}

/**
 * @brief ��ȡ����������������ֵ���ı�Ƶ���룩
 * 
 * @details �ú���ͨ����ȡ��ʱ��TIM1�ļ���ֵ���Ա������źŽ����ı�Ƶ���봦��
 *          ʹ�þ�̬�����ۻ���������ͨ��������ȡģ����ȥ��������������֣�
 *          ȷ�����ؾ�ȷ������ֵ����Ҫ���ڵ�����ơ�λ�ü���Ӧ�ó�����
 * 
 * @note   �����ڲ����Զ����㶨ʱ������ֵ��ȷ���´ζ�ȡ��׼ȷ��
 * 
 * @return int16_t ���ؽ����ı���������ֵ
 */
int16_t Encoder_Get(void)
{
//    // ��̬�����������ں������ü䱣��δ��4����������
//    static int32_t encoderAccumulator = 0;
//    
//    // ��ȡ��ǰ��ʱ������ֵ
//    int16_t temp = TIM_GetCounter(TIM1);
//    
//    // ���㶨ʱ����������Ϊ�´ζ�ȡ��׼��
//    TIM_SetCounter(TIM1, 0);
//    
//    // ����ǰ��ȡֵ�ۼӵ��ۼ�����
//    encoderAccumulator += temp;
//    
//    // �����ı�Ƶ����������ֵ��ȥ��δ��ɵĲ��֣�
//    int16_t result = encoderAccumulator / 4;
//    
//    // ����δ��4��������������֤����
//    encoderAccumulator %= 4;
//    
//    // ���ؽ���������ֵ
//    return result;
	  return 0;
}



/**
  * @brief  ΢�뼶��ʱ
  * @param  xus ��ʱʱ������Χ��0~233015
  * @retval ��
  */
void Delay_us(uint32_t xus)
{
	SysTick->LOAD = 72 * xus;				//���ö�ʱ����װֵ
	SysTick->VAL = 0x00;					//��յ�ǰ����ֵ
	SysTick->CTRL = 0x00000005;				//����ʱ��ԴΪHCLK��������ʱ��
	while(!(SysTick->CTRL & 0x00010000));	//�ȴ�������0
	SysTick->CTRL = 0x00000004;				//�رն�ʱ��
}

/**
  * @brief  ���뼶��ʱ
  * @param  xms ��ʱʱ������Χ��0~4294967295
  * @retval ��
  */
void Delay_ms(uint32_t xms)
{
	while(xms--)
	{
		Delay_us(1000);
	}
}
 
/**
  * @brief  �뼶��ʱ
  * @param  xs ��ʱʱ������Χ��0~4294967295
  * @retval ��
  */
void Delay_s(uint32_t xs)
{
	while(xs--)
	{
		Delay_ms(1000);
	}
} 


