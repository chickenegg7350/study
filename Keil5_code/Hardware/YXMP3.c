#include "YXMP3.h"
#include "Time_page.h"

bool MP3_OK = false;
bool MP3_Play = false;
bool MP3_Circulation = false;
uint8_t MP3volume = 10;//����
uint16_t MP3MAXfilesnum=300;//�ļ���
uint16_t MP3atsnum=1,MP3UPatsnum=0;//��ǰ����




char Serial2_RxPacket[100];	//����������ݰ�����
uint8_t Serial2_RxFlag;	   //����������ݰ���־λ

extern OLED_ChangePoint OLED_UI_PageStartPoint;

void MP3CMD(uint8_t CMD ,uint16_t data);
void MP3_Read_data(void);

void YXMP3_init(void)
{	
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//����USART2��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);			

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	GPIO_ResetBits(GPIOB, GPIO_Pin_6);	

	
	/*USART��ʼ��*/
	USART_InitTypeDef USART_InitStructure;					//����ṹ�����
	USART_InitStructure.USART_BaudRate = 9600;		   //������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ�������ƣ�����Ҫ
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;           	  //ģʽ������ģʽ�ͽ���ģʽ��ѡ��
	USART_InitStructure.USART_Parity = USART_Parity_No;		     //��żУ�飬����Ҫ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	    //ֹͣλ��ѡ��1λ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ���ѡ��8λ
	USART_Init(USART2, &USART_InitStructure);				  //���ṹ���������USART_Init������USART2
	
	/*�ж��������*/
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);		   //�������ڽ������ݵ��ж�
	

	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;					       //����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		      //ѡ������NVIC��USART2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		    	 //ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //ָ��NVIC��·����ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		   //ָ��NVIC��·����Ӧ���ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStructure);							  //���ṹ���������NVIC_Init������NVIC����
	
	/*USARTʹ��*/
	USART_Cmd(USART2, ENABLE);							   //ʹ��USART2�����ڿ�ʼ����
	
}

void YXMP3_PWROFF(void)
{		
	USART_Cmd(USART2, DISABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //����GPIOB��ʱ��
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	MP3_OK = false;	
	MP3_Play = false;
	MP3MAXfilesnum=300;
	MP3UPatsnum=0;
}

void YXMP3_DDD1(void)
{
	MP3CMD(YX_set_volume,MP3volume);
	Delay_ms(30);
	if(MP3UPatsnum!=MP3atsnum)
	{MP3CMD(YX_set_folder_Audio,YX_01_folder+MP3atsnum);MP3UPatsnum=MP3atsnum;}
	else MP3CMD(YX_begin_Audio,YX_NULL);
	Delay_ms(100);
	if(MP3_Circulation)MP3CMD(YX_set_Audio_circulation,00);
}

void YXMP3_DDD2(int8_t X,int8_t Y,bool i)
{	
	static int8_t b;
	if(i){if(b<64)b++;
	else b=0;}	
	OLED_DrawRectangle( X,Y,64-12,6 ,OLED_UNFILLED);
	OLED_DrawRectangle( X-12+b,Y,12,6 ,OLED_FILLED);
	OLED_ClearArea( X-12,Y,12,6);
	OLED_ClearArea( X-12+64,Y,12,6);
}


void YXMP3_Homepage(void)
{
	int8_t p,a;
	uint16_t c=0;
	OLED_UI_FadeoutAllArea();
	OLED_Brightness_DODO();
	
	YXMP3_init();
	
	low_pass_filter(80,10,38,38,2);
	
	a= -30;
	
	while(1)
	{	
	   OLED_Clear();	
		if(a>0)
		{			
			if(MP3MAXfilesnum==300)
			{	
				OLED_ShowString(32+2, 10+40-8-2-8-2, "MP3", OLED_6X8_HALF);	
				OLED_ShowString(32+2, 10+40-8-2, "loading...", OLED_6X8_HALF);	
				c++;
				if(c>3600){OLED_ClearArea(32+2,10+40-8-2,60,8);OLED_ShowString(32+2, 10+40-8-2, "ERROR!!!", OLED_6X8_HALF);OLED_Update();Delay_ms(5000);YXMP3_PWROFF();OLED_UI_FadeoutAllArea();OLED_UI_PageStartPoint.CurrentPoint.X = 127;return;}
			}
			else
			{	
				OLED_ShowString(32+2, 10+40-8-2-8-2, "Files", OLED_6X8_HALF);	
				OLED_ShowString(32+2, 10+40-8-2, "Num:", OLED_6X8_HALF);
				OLED_ShowNum(32+2+24,10+40-8-2,MP3MAXfilesnum,3,OLED_6X8_HALF);
				a++;
				if(a>120)break;
			}
		
			YXMP3_DDD2(32+6,10+4,true);
		}
		else a++;
		
	    low_pass_filter(32,10,64,40,0);
		
		MP3_Read_data();
		

	   OLED_Brightness_UPUP();	
	   OLED_Update();
	   Delay_ms(15);
	}
	
	low_pass_filter(32,10,64,40,1);
	OLED_Update();
	Delay_ms(100);
	
	OLED_Clear();
	OLED_DrawRectangle(32,10,64,40,OLED_UNFILLED);
	OLED_Update();
	Delay_ms(100);
	
	OLED_Clear();
	OLED_DrawRectangle(32,10,64,40,OLED_FILLED);
	OLED_Update();
	Delay_ms(100);
	
	OLED_Clear();
	OLED_Update();
	Delay_ms(100);
	
	
	
	uint16_t mun = 0,OFFmun=1;
	float tttx=128;
	extern bool hold;
	
	
	while(1)
	{
		
		
		
		
		OLED_Clear();
		
		MP3_Read_data();
		
		if(tttx>0.3)tttx*=0.95;
		else tttx=0;
		
		Time_page_BAT(90-tttx/2,0);
		Time_page_small_Timer(0+tttx,0);	

		OLED_ShowNum(128-24-10+tttx,32-4,MP3atsnum,3,OLED_8X16_HALF);
		OLED_DrawRectangle(128-24-10+tttx-2,32-2-2,24+4,16+1 ,OLED_UNFILLED);
		
		OLED_ShowString(128-18-6,32+16+1+tttx/3,"/",OLED_6X8_HALF);
		OLED_ShowNum(128-18,32+16+1+tttx/3,MP3MAXfilesnum,3,OLED_6X8_HALF);
		
		OLED_ShowNum(0-tttx/4,64-12,MP3volume,2,OLED_7X12_HALF);
		OLED_ShowString(15-tttx/2,64-8,"/30",OLED_6X8_HALF);

		YXMP3_DDD2(12,32+tttx/4,MP3_Play);
		
		
		if(OFFmun==0)OLED_Power(1);
		else if(OFFmun==500)OLED_Power(0);
		
		if(OFFmun<=500 && hold==false)OFFmun++;

	   OLED_Brightness_UPUP();	
	   OLED_Update();
		
	   Delay_ms(15);
	   p = menu_Roll_event();   
	   if( p == 1){ 
		   MP3atsnum++;
		   if(MP3atsnum>MP3MAXfilesnum)MP3atsnum=1;	   
			YXMP3_DDD1();
		   MP3_Play=true;
		   OFFmun=0;
	   }	
       else if( p == -1){
		   MP3atsnum--;
		   if(MP3atsnum==0)MP3atsnum=MP3MAXfilesnum; 
			YXMP3_DDD1();
		   MP3_Play=true;
		   OFFmun=0;		   
	   }	
		p=menu_Enter_event();
		if(p == 1)
		{	
			MP3_Play=!MP3_Play;
			if(MP3_Play)
			{	
				YXMP3_DDD1();
			}
			else MP3CMD(YX_stop_Audio,YX_NULL);	
			 OFFmun=0;
		}
		else if(p == 2){
			 OFFmun=0;
			OLED_Power(1);
			while(1)
			{
				MP3_Read_data();	
				
			   OLED_ClearArea(32,16,64,32);
				OLED_DrawRectangle(32,16,64,32,OLED_UNFILLED);
				
			    OLED_ShowMixString(3+32+2,16+2,"�˳�",OLED_8X8_FULL,OLED_6X8_HALF);
				
			    OLED_ShowMixString(3+32+2,16+2+8+2,"Vol:",OLED_8X8_FULL,OLED_6X8_HALF);
				OLED_ShowNum(3+32+2+24,16+2+8+2,MP3volume,2,OLED_6X8_HALF);
				
			    if(MP3_Circulation)OLED_ShowString(3+32+2,16+2+8+2+8+2,"<->",OLED_6X8_HALF);
				else OLED_ShowString(3+32+2,16+2+8+2+8+2," ->",OLED_6X8_HALF);
				
				switch(mun)
				{ 
					case 0:OLED_ShowString(3+32+2+30,16+2,"-",OLED_6X8_HALF);break;
					case 1:OLED_ShowString(3+32+2+30+8+2,16+2+8+2,"-",OLED_6X8_HALF);break;
					case 2:OLED_ShowString(3+32+2+30,16+2+8+2+8+2,"-",OLED_6X8_HALF);break;
				}
				
			   OLED_Update();
				
			   Delay_ms(20);
			   p = menu_Roll_event();   
			   if( p == 1){
					switch(mun)
					{ 
						case 0:	OLED_Power(1);YXMP3_PWROFF();OLED_UI_FadeoutAllArea();OLED_UI_PageStartPoint.CurrentPoint.X = 127;Delay_ms(300);return;	
						case 1:if(MP3volume>0)MP3volume--;	MP3CMD(YX_set_volume,MP3volume);Delay_ms(30);break;
						case 2:MP3_Circulation=!MP3_Circulation;MP3CMD(YX_set_Audio_circulation,(int)!MP3_Circulation);break;
					}
				   
			   }	
			   else if( p == -1){
					switch(mun)
					{ 
						case 0:	OLED_Power(1);YXMP3_PWROFF();OLED_UI_FadeoutAllArea();OLED_UI_PageStartPoint.CurrentPoint.X = 127;Delay_ms(300);return;							
						case 1:if(MP3volume<30)MP3volume++;	MP3CMD(YX_set_volume,MP3volume);Delay_ms(30);break;					
						case 2:MP3_Circulation=!MP3_Circulation;MP3CMD(YX_set_Audio_circulation,(int)!MP3_Circulation);break;
					}
			   }	
				p=menu_Enter_event();
				if(p == 1){	
					mun++;
				   if(mun>2)mun=0;

				}
				else if(p == 2){
			
					break;
				}	
		
			}	
			
			
		}	
	}
}



/**
  * ��    �������ڷ���һ���ֽ�
  * ��    ����Byte Ҫ���͵�һ���ֽ�
  * �� �� ֵ����
  */
void Serial2_SendByte(uint8_t Byte)
{
	USART_SendData(USART2, Byte);		//���ֽ�����д�����ݼĴ�����д���USART�Զ�����ʱ����
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	//�ȴ��������
	/*�´�д�����ݼĴ������Զ����������ɱ�־λ���ʴ�ѭ�������������־λ*/
}

void MP3CMD(uint8_t CMD ,uint16_t data)
{
	uint8_t Send_buf[8];
	
    Send_buf[0] = 0x7e;    //ͷ
    Send_buf[1] = 0xff;    //�����ֽ� 
    Send_buf[2] = 0x06;    //����
    Send_buf[3] = CMD;     //����ָ��
    Send_buf[4] = 0x00;   //�Ƿ���Ҫ����
    Send_buf[5] = (uint8_t)(data >> 8);//data
    Send_buf[6] = (uint8_t)(data);     //data
    Send_buf[7] = 0xef;    //β
	
	for (u8 i = 0; i < 8; i ++)		//��������
	{
		Serial2_SendByte(Send_buf[i]);	  //���ε���Serial_SendByte����ÿ���ֽ�����
	}
}

uint8_t ReadLN=0;

void MP3_Read_data(void)
{		
	if (Serial2_RxFlag == 1) //������յ����ݰ�
	{	
		if (Serial2_RxPacket[3] == 0x3F && Serial2_RxPacket[6]==0x02){ MP3_OK = true;ReadLN=1;}//ȷ���Ƿ��ʼ�����
		if (Serial2_RxPacket[3] == 0x4E){ MP3MAXfilesnum =  Serial2_RxPacket[6];ReadLN=0;}//��ȡ�ļ���
		if (Serial2_RxPacket[3] == 0x3D && MP3_Circulation==false){ MP3_Play =  false;}//MP3������
		
		Serial2_RxFlag = 0;//������ɣ��������ݰ���־λ����
	}
	
	
	if(ReadLN==1)MP3CMD(YX_read_folder,YX_01_quantity);
	
}

/**
  * ��    ����USART2�жϺ���
  * ��    ������
  * �� �� ֵ����
  */
void USART2_IRQHandler(void)
{
	static uint8_t RxState = 0;		 //�����ʾ��ǰ״̬��״̬�ľ�̬����
	static uint8_t pRxPacket = 0;	//�����ʾ��ǰ��������λ�õľ�̬����	

	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)	 //�ж��Ƿ���USART�Ľ����¼��������ж�
	{
		uint8_t RxData = USART_ReceiveData(USART2);		   //��ȡ���ݼĴ���������ڽ��յ����ݱ���				
		/*��ǰ״̬Ϊ0���������ݰ���ͷ�������֤*/
		if (RxState == 0)
		{
			if (RxData == 0x7E && Serial2_RxFlag == 0) //�������ȷʵ�ǰ�ͷ��������һ�����ݰ��Ѵ������
			{
				pRxPacket = 0;		   //���ݰ���λ�ù���
				Serial2_RxPacket[pRxPacket] = RxData;//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;
				
				RxState = 1;			//����һ��״̬
				
			}

		}
		/*��ǰ״̬Ϊ1���������ݰ���һ������*/
		else if (RxState == 1)
		{
				Serial2_RxPacket[pRxPacket] = RxData;//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			           //���ݰ���λ������
    			RxState = 2;		              //����һ��״̬

		}
				else if (RxState == 2)
		{
				Serial2_RxPacket[pRxPacket] = RxData;//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			           //���ݰ���λ������
    			RxState = 3;		              //����һ��״̬

		}

				else if (RxState == 3)
		{
				Serial2_RxPacket[pRxPacket] = RxData;//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			           //���ݰ���λ������
    			RxState = 4;		              //����һ��״̬

		}

				else if (RxState == 4)
		{
				Serial2_RxPacket[pRxPacket] = RxData;//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			           //���ݰ���λ������
    			RxState = 5;		              //����һ��״̬

		}
				else if (RxState == 5)
		{
				Serial2_RxPacket[pRxPacket] = RxData;//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			           //���ݰ���λ������
    			RxState = 6;		              //����һ��״̬

		}

				else if (RxState == 6)
		{
				Serial2_RxPacket[pRxPacket] = RxData;//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			           //���ݰ���λ������
    			RxState = 7;		              //����һ��״̬

		}

				else if (RxState == 7)
		{
				Serial2_RxPacket[pRxPacket] = RxData;//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			           //���ݰ���λ������
    			RxState = 8;		              //����һ��״̬

		}

				else if (RxState == 8)
		{
				Serial2_RxPacket[pRxPacket] = RxData;//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			           //���ݰ���λ������
    			RxState = 9;		              //����һ��״̬

		}

				else if (RxState == 9)
		{
				Serial2_RxPacket[pRxPacket] = RxData;	  //�����ݴ������ݰ������ָ��λ��
			    pRxPacket ++;			                 //���ݰ���λ������
//				Serial2_RxPacket[pRxPacket] = '\0';		//���յ����ַ����ݰ����һ���ַ���������־
				Serial2_RxFlag = 1;		               //�������ݰ���־λ��1���ɹ�����һ�����ݰ�
				RxState = 0;			              //״̬��0
		}	

		
			
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);		//�����־λ
	}
}
