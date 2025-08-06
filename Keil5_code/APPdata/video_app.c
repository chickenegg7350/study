#include "video_app.h"

extern uint8_t OLED_DisplayBuf[8][128];
bool VIDEOOKK=true;
//��Ϊ��ͬ��Ƶ�����߼�����һ����ֻ��ƫ�Ƶ�ַ���㲻һ��������д�˸�ͨ�õ�һ����
void SHOW_VIDEO(uint32_t Address ,uint16_t mpg)//��֡�� , ��ȡ������ʼ��ַ
{	
		int8_t p;		
	    p0=0,p1=0;
	
		OLED_UI_FadeoutAllArea();

		
		uint8_t temp[3];   //�ļ�ͷ(3�ֽ�)
		uint32_t mydata=0; //��������(�������ļ�ͷ)
	    uint32_t datasite=3; //֡����ƫ�Ƶ�ַ

	
		OLED_Clear();
		OLED_Update(); 
//		OLED_WriteCommand(0x81);	//���öԱȶ�
//		OLED_WriteCommand(0xFF);	//0x00~0xFF
	
		W25Q128_ReadData( Address , temp , 3 );
	
		if(temp[0]!=0xAA) //�������ͷ�Ƿ����
		{	
			OLED_ShowMixString(0,20,"����Ƶ",OLED_12X12_FULL,OLED_7X12_HALF);
			OLED_Update(); 
			
			Delay_ms(500);
//			GUODUsn=1;
//			OLED_transition(0);
			OLED_UI_FadeoutAllArea();

			return;
		}
		
		mydata= (temp[2]<< 8) |  temp[1];  //��ȡ��֡����
		
		OLED_ShowMixString(0,0,"֡��:25FPS",OLED_12X12_FULL,OLED_7X12_HALF);
		OLED_ShowMixString(0,13,"֡��:",OLED_12X12_FULL,OLED_7X12_HALF);
		OLED_ShowNum(38+12,15,mydata,6,OLED_6X8);
		
		mydata *= 1024; 				 //������������
		
		OLED_ShowMixString(0,26,"������(Byte):",OLED_12X12_FULL,OLED_7X12_HALF);
		OLED_ShowNum(0,40,mydata,8,OLED_6X8);
		
		
		YXMP3_init();
				
		OLED_ShowMixString(92,52,"��ȡ��",OLED_12X12_FULL,OLED_7X12_HALF);		//����ʵ������Ϊ���ڸ�mp3���ֵ��ӳ�
		
		OLED_Update(); 
		
		uint8_t temm=0;
					
		while(!MP3_OK)
		{MP3_Read_data();Delay_ms(2000);temm++; if(temm>2){OLED_ShowMixString(0,52,"MP3-ERROR",OLED_12X12_FULL,OLED_7X12_HALF);OLED_Update();Delay_ms(1000);break;}}
		temm=0;
		
		MP3CMD(YX_set_volume,MP3volume);
		
		OLED_UI_FadeoutAllArea();
		
		MP3CMD(YX_set_folder_Audio,mpg);
		
		
		Delay_ms(2000);				//���ֲ����ӳ٣�����ʵ��΢��
//		Delay_ms(2000);
//		Delay_ms(2000);
		
	
//	 uint16_t temm=15;

	while(1)
	{
		if(VIDEOOKK)
		{
			VIDEOOKK=false;
			W25Q128_ReadData( Address+datasite , OLED_DisplayBuf[0] , 1024 );//��ȡһ֡���Դ�
			datasite+=1024;       //��ַƫ�Ƶ���һ֡
		}
		
//		Delay_ms(temm);		//֡���������ʵ��΢��
		
		OLED_Update();  		
//        OLED_transition(1);
		
		
//		p=menu_Roll_event(); //΢��֡���
//		if (p==1){			
//			temm++;
//		}	
//		else if(p == -1){
//		    temm--;
//		}
		
			
		p=menu_Enter_event();  //�ֶ��˳�|ֹͣ
		if (p==1){
		
			do
			{ 
				OLED_Update();  		
				p=menu_Enter_event(); 
				
				if(p == 2){
			
//					if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1)
//					{	
////						MP3_starts(0);
////						Serial2_Power_OFF();
//					
//					}
//					GUODUsn=1;
//					OLED_transition(0);
					YXMP3_PWROFF();
					OLED_UI_FadeoutAllArea();
					Delay_ms(300);
					return;	
				}
				
			
			}
			while(p !=1 ); 
			
			
		}
		else if(p == 2){
			
//			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1)
//			{	
////				MP3_starts(0);
////				Serial2_Power_OFF();
//			
//			}
//			GUODUsn=1;
//			OLED_transition(0);
			YXMP3_PWROFF();
			OLED_UI_FadeoutAllArea();
			Delay_ms(300);
			return;	
		}
		
		
		if(datasite > mydata) //�Ƿ񲥷����
		{	
//			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1)
//			{	
//				
////				MP3_starts(0);				
////				Serial2_Power_OFF();
//			
//			}
			OLED_Clear();
			OLED_ShowMixString(0,20,"�������",OLED_12X12_FULL,OLED_7X12_HALF);
			OLED_Update(); 
			Delay_ms(1000);
//			GUODUsn=1;
//			OLED_transition(0);
			YXMP3_PWROFF();
			OLED_UI_FadeoutAllArea();
			return;
		}
	}

	
}

uint32_t VIDEO_SITE(uint32_t Address) //�����������Ƶ��ַ������һ����Ƶ��ַ
{
		uint8_t temp[3];   //�ļ�ͷ(3�ֽ�)
	    uint32_t mydata=0; //��������(�������ļ�ͷ)
		W25Q128_ReadData( Address , temp , 3 );

			mydata= (temp[2]<< 8) |  temp[1];  //��ȡ��֡����	
			mydata*=1024;			
			Address += (mydata+3) ;
		
	return Address;	
}


void VIDEO_1(void)
{
//		uint32_t Address=163840;//��Ƶ�ļ�������ʼ��ַ 0x28000	
		uint32_t Address=0;
		SHOW_VIDEO(Address,0x0201); //˳�㲥��tf����02�ļ��е�01����
}

void VIDEO_2(void)
{			
		SHOW_VIDEO( VIDEO_SITE(0) ,0x0202);
}


void VIDEO_3(void)
{				
		SHOW_VIDEO( (VIDEO_SITE( VIDEO_SITE(0) ))  ,0x0203);
}


//ͬ�ϣ�д�˸�ͨ�õ�һ����
void WRITE_VIDEO(uint32_t Address)//������ʼ��ַ
{	
	
		int8_t p;
	
		OLED_UI_FadeoutAllArea();
	
		TIM_Cmd(TIM4, DISABLE);

		Serial1_Init();
	
		
	    p0=0,p1=0 ,we=0;
		
		uint8_t temp[3]={0xAA,0,0};   //�ļ�ͷ(3�ֽ�)
	    uint32_t datasite=3;    //֡����ƫ�Ƶ�ַ
	
	
//		OLED_WriteCommand(0x81);	//���öԱȶ�
//		OLED_WriteCommand(0xFF);	//0x00~0xFF
	
		OLED_Clear();
		OLED_ShowMixString(0,20,"������¼��",OLED_12X12_FULL,OLED_7X12_HALF);
		OLED_Update(); 
	
		

	while(1)
	{
		if(we)//���������
		{	
			W25Q128_unlimited_Program( Address+datasite , OLED_DisplayBuf[0] , 1024 );//��¼һ֡���Դ�
			datasite+=1024;       //��ַƫ�Ƶ���һ֡
			we=0;
		}
		
		
		p=menu_Enter_event();
		if (p){
		
			if( (datasite-3) != 0 ) //���������
			{	
				temp[1]= ((datasite-3)/1024) & 0xFF;    
				temp[2]= ( ( (datasite-3) /1024 ) >> 8 ) & 0xFF;  //�Ѵ�֡��
				
				W25Q128_unlimited_Program( Address , temp , 3 );	//д���Ѵ�ͷ����
				
				OLED_Clear();
				OLED_ShowMixString(0,20,"��Ƶ��¼��",OLED_12X12_FULL,OLED_7X12_HALF);
				OLED_Update(); 
				Serial1_Power_OFF();
										TIM_Cmd(TIM4, ENABLE);
				Delay_ms(500);
//				GUODUsn=1;
//				OLED_transition(0);
				OLED_UI_FadeoutAllArea();
				return;	
			}
			else
			{
				OLED_Clear();
				OLED_ShowMixString(0,20,"δ��⵽��¼������",OLED_12X12_FULL,OLED_7X12_HALF);
				OLED_Update(); 
				Serial1_Power_OFF();
										TIM_Cmd(TIM4, ENABLE);
				Delay_ms(500);
//				GUODUsn=1;
//				OLED_transition(0);
				OLED_UI_FadeoutAllArea();
				return;	
			}
				
		}	
		
	}
}

void VIDEO_write_1(void)
{
//		uint32_t Address=163840;//������ʼ��ַ 0x28000	
		WRITE_VIDEO(0);	
}

void VIDEO_write_2(void)
{	
		WRITE_VIDEO( VIDEO_SITE(0) );	
}


void VIDEO_write_3(void)
{		
	    WRITE_VIDEO( VIDEO_SITE(VIDEO_SITE(0)) );	
}										

void VIDEO_erase(void)
{	
	int8_t p,y=-1;	

	OLED_UI_FadeoutAllArea();
	
	while(1)
	{	
		OLED_Clear();	
		OLED_ShowMixString(0,2,"�Ƿ����?",OLED_12X12_FULL,OLED_7X12_HALF);
		OLED_ShowMixString(0,2+13,"Are you sure?",OLED_12X12_FULL,OLED_7X12_HALF);
		
		if(y==-1) OLED_ShowMixString(0,2+13+20,"�� NO",OLED_12X12_FULL,OLED_7X12_HALF);
		else OLED_ShowMixString(0,2+13+20,"�� YES",OLED_12X12_FULL,OLED_7X12_HALF);
		
		OLED_Update();
		
	
		p=menu_Roll_event(); //΢��֡���
		if (p==1){	
				y=1;
		}	
		else if(p == -1){
			y=-1;	
		}
		p=menu_Enter_event();
		if (p){	
			if(y==-1){OLED_UI_FadeoutAllArea();Delay_ms(300);return;}
			else break;			
		}

	}
	
	
	
//		GUODUsn=1;
//		uint32_t data = 163840; 
		OLED_Clear();
		OLED_ShowMixString(0,20,"������",OLED_12X12_FULL,OLED_7X12_HALF);
		OLED_Update();
	
		W25Q128_WaitBusy();							//�ȴ�æ
		W25Q128_WriteEnable();						//дʹ��
		
		MySPI_Start();								//SPI��ʼ
		MySPI_SwapByte(W25Q128_CHIP_ERASE);	   //������������������ָ��
		MySPI_Stop();								//SPI��ֹ
	
	   
		W25Q128_WaitBusy();                         //�ȴ�æ		
//		OLED_WriteCommand(0x81);	//���öԱȶ�
//		OLED_WriteCommand(0xFF);	//0x00~0xFF
		
			OLED_Clear();
			OLED_ShowMixString(0,20,"�������",OLED_12X12_FULL,OLED_7X12_HALF);
			OLED_Update();
			
			OLED_Update(); 
			Delay_ms(1000);
//			GUODUsn=1;
//			OLED_transition(0);
			OLED_UI_FadeoutAllArea();
			return;	
}




	
	

