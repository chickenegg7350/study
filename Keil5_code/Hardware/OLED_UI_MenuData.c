#include "OLED_UI_MenuData.h"
#include "OLED_UI.h"

/*���ļ����ڴ�Ų˵����ݡ�ʵ���ϲ˵����ݿ��Դ�����κεط�������ڴ˴���Ϊ�˹淶�����ģ�黯*/

// ColorMode ��һ����OLED_UI���ж����bool���ͱ��������ڿ���OLED��ʾ����ɫģʽ�� DARKMODE Ϊ��ɫģʽ�� LIGHTMOOD Ϊǳɫģʽ�����ｫ��������Ϊ�˴�����ѡ��˵��
extern bool ColorMode;
extern bool OLED_UI_ShowFps;
// OLED_UI_Brightness ��һ����OLED_UI���ж����int16_t���ͱ��������ڿ���OLED��ʾ�����ȡ����ｫ��������Ϊ�˴����������ȵĻ��������ڣ���Χ0-255��
extern int16_t OLED_UI_Brightness;
float testfloatnum = 0.5;
int16_t testintnum = 1;
#define SPEED 5

extern OLED_ChangePoint OLED_UI_PageStartPoint;

//*******************�ֵ�Ͳ***************************************//

int8_t mubz=50;//�ֵ�ͲĬ������
void shoudt(void)
{	
	int8_t p;
	uint16_t temp;
	
	OLED_UI_FadeoutAllArea();
	OLED_Brightness_DODO();
	
	OLED_Clear();	   
	OLED_Update();
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseSrtructure;        //TIMʱ�������ʼ���ṹ��
	TIM_OCInitTypeDef	TIM_OCInitStructure;                //TIM����ȽϹ��ܽṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   //ʹ��GPIOA����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);    //��ӳ���迪��AFIOʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);    //ʹ��TIM3ʱ�� 
	
	GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2, ENABLE);
	
	/* �˿ڣ�GPIO)���� */
	GPIO_InitTypeDef  GPIO_InitStructure;                   //GPIO��ʼ���ṹ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;                //PA7=T3 Ch2
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;        //�ٶ� 50MHz
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;          //�����������
	GPIO_Init(GPIOB,&GPIO_InitStructure);                  //��ʼ�� GPIOA	
	
	/* TIMʱ�������ʼ�� */
	TIM_TimeBaseSrtructure.TIM_Period= 100 - 1;           //������ TIMx_CNT���� ��0�ۼӵ�ARR�κ�����������Զ���װ��ֵ
	TIM_TimeBaseSrtructure.TIM_Prescaler= 72 - 1;        //����Ԥ��Ƶϵ�� PSC
	TIM_TimeBaseSrtructure.TIM_ClockDivision= 0;                       //����ʱ�ӷָ���⺯����TIM_Clock_Division_CKD���壬���ֲ� 00:Tdts=Tck_int:01:Tdts=2*Tck_int(2��Ƶ��
	TIM_TimeBaseSrtructure.TIM_CounterMode=TIM_CounterMode_Up;        //TIM ���ϼ���ģʽ���
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseSrtructure);					  //��ʼ��TIM3��ʱ��

	/* TIM�����ʼ�� */
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;     			        //ѡ��ʱ��ģʽ��TIM �����ȵ���ģʽ1,ģʽ1Ϊ�Ƚϵ������
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;           //������ԣ�TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;     //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = mubz ;                            //����ռ�ձ�,��CCR
	
	TIM_OC4Init(TIM2,&TIM_OCInitStructure);
		
	/* ��ʼ��CH1 */
	TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable);                  //CH2 Ԥװ��ʹ��
	TIM_ARRPreloadConfig(TIM2,ENABLE);                            //ʹ�� TIMx��ARR�ϵ�Ԥװ�ؼĴ���

	TIM_Cmd(TIM2, ENABLE);                                    //ʹ�� TIM3	
	

	float GP=0 , GP_target;                 //�������ȣ�Ŀ����������
	
	while(1)
	{	
	   OLED_Clear();
		
	   OLED_ShowNum(19,30,mubz,3,OLED_8X16_HALF);
	   OLED_ShowString(45,30,"%",OLED_8X16_HALF);	
		
		GP_target = mubz * 0.9 ;
		if(GP < GP_target)
		{ 
			if( GP_target - GP > 25 ) GP += 5 ;
			else if( GP_target - GP > 7 ) GP += 2.4 ;
			else if( GP_target - GP > 0 ) GP += 0.5 ;
		}
		if(GP > GP_target)
		{ 	
			if( GP - GP_target > 25 ) GP -= 5 ;
			else if( GP - GP_target > 7 ) GP -= 2.4 ;
			else if( GP - GP_target > 0 ) GP -= 0.5 ;
		}
		OLED_DrawRectangle(17, 50, 94 , 10, OLED_UNFILLED);
		OLED_DrawRectangle(19, 52, GP , 6, OLED_FILLED);
		
	   OLED_Brightness_UPUP();	
	   OLED_Update();
		
	   Delay_ms(10);
		
	   p = menu_Roll_event();   
	   if( p == 1){ 
		   
		   mubz++;
		   if(mubz>100)mubz=100;
		   TIM_SetCompare4(TIM2, (uint8_t)mubz);//дCRR
		   
		   if(mubz>90)Delay_ms(10);
		   
	   }	
       else if( p == -1){ 
			mubz--;
		    if(mubz<0)mubz=0;
		    TIM_SetCompare4(TIM2, (uint8_t)mubz);//дCRR
		   
		   if(mubz<20)Delay_ms(60);
	   }	
	   
		p=Key_GetEnterStatus();
		if(p == 0){
			
			OLED_UI_FadeoutAllArea();
		
//			ResetEnterFlag();
			
			TIM_SetCompare4(TIM2, 0);//дCRR
					
			temp = (0xAE<<8) | mubz;
			
			/*���ݼĴ�������ʹ��*/
			PWR_BackupAccessCmd(ENABLE);
			BKP_WriteBackupRegister(BKP_DR2, temp);//�ڱ��ݼĴ���д���Լ��涨�ı�־λ�������ж��ֵ�Ͳ�����ǲ��ǵ�һ��ִ������		
			
			OLED_UI_PageStartPoint.CurrentPoint.X = 127;		
			
			Delay_ms(300);
			
			TIM_Cmd(TIM2, DISABLE);//TIMʧ��
			
			
			
			return;	
		}	
	}
}

//*******************���***************************************//
uint8_t timedll[4];
	uint8_t timedlll1[4];
		uint8_t timedlll2[4];
			uint8_t timedlll3[4];

bool timedllKK = false;
void miaobiaotimedllKKO(void)
{	
	for(u8 i=0;i<4;i++)
	{	
		timedll[i]=0;
		timedlll1[i]=0;
		timedlll2[i]=0;
		timedlll3[i]=0;
	}
}

void miaobiao(void)
{	
	int8_t p;	

	OLED_UI_FadeoutAllArea();
	OLED_Brightness_DODO();


	float GP=127;                 //�������ȣ�Ŀ����������
	
	while(1)
	{	
	   OLED_Clear();
		
		GP*=0.9;
		
		OLED_Printf(2+10+GP,2,OLED_8X16_HALF,"%02d:%02d:%02d:%02d",timedll[3],timedll[2],timedll[1],timedll[0]);
		OLED_Printf(2+3+10-GP,2+16+2,OLED_7X12_HALF,"%02d:%02d:%02d:%02d",timedlll1[3],timedlll1[2],timedlll1[1],timedlll1[0]);
		OLED_Printf(2+3+10+GP,2+16+2+12+2,OLED_7X12_HALF,"%02d:%02d:%02d:%02d",timedlll2[3],timedlll2[2],timedlll2[1],timedlll2[0]);
		OLED_Printf(2+3+10-GP,2+16+2+12+2+12+2,OLED_7X12_HALF,"%02d:%02d:%02d:%02d",timedlll3[3],timedlll3[2],timedlll3[1],timedlll3[0]);
	
	   OLED_Brightness_UPUP();	
	   OLED_Update();
		
	   Delay_ms(15);
		
	   p = menu_Roll_event();   
	   if( p == 1){ 		   
			for(u8 i=0;i<4;i++)
			{	
				timedlll3[i]=timedlll2[i];
				timedlll2[i]=timedlll1[i];
				timedlll1[i]=timedll[i];
			}
			Delay_ms(100);
	   }	
       else if( p == -1){ 		   
		    timedllKK = false;
			miaobiaotimedllKKO();   
	   }	
	   
		p=menu_Enter_event();
		if(p == 1)
		{
			timedllKK=!timedllKK;
		}
		
		if(p == 2)
		{
			timedllKK = false;
			miaobiaotimedllKKO();
			OLED_UI_FadeoutAllArea();
			OLED_UI_PageStartPoint.CurrentPoint.X = 127;
			Delay_ms(300);
			return;	
		}
	}
}




//*******************��������***************************************//


bool RAWA=0;//̧�������ñ�־λ
bool Pick_up=1;//�������־λ
int16_t RAWAtri=110;//̧�󴥷�ֵ
int16_t RAWAtrigger;//ʵ��̧�󴥷�ֵ

void Raise_Wake(void)
{
		if(RAWA)
		{
			MPU6050_CYCLe_Init();
		}
		else
		{
			MPU6050_RES();
			Pick_up=0;
			RAWAtrigger=RAWAtri*70;
			PWR_BackupAccessCmd(ENABLE);
			BKP_WriteBackupRegister(BKP_DR4, (0xAC<<8)|RAWAtri);//������ֵд��BKP�Ĵ���
		}
	
}

void Raise_Wake_trigger(void)
{
	uint16_t temp;
	RAWAtrigger=RAWAtri*70;//��̧�󴥷�ֵ��Ӧ��ʵ�ʴ���ֵ����ֵ17850
	/*���ݼĴ�������ʹ��*/
	PWR_BackupAccessCmd(ENABLE);
	temp = (0xAC<<8) | RAWAtri;
	BKP_WriteBackupRegister(BKP_DR4, temp);//������ֵд��BKP�Ĵ���
}



//*******************Game_Of_Life_Play***************************************//

extern uint8_t OLED_DisplayBuf[8][128];		//��������

uint8_t Nodes_Cache[8][128];				//���㻺��

void Update_Display(void)					//�ϴ���Ļ
{
	memcpy(OLED_DisplayBuf, Nodes_Cache, 1024);
}

void Point_life(uint8_t X, uint8_t Y)		//д����
{
	Nodes_Cache[(Y/8)][X] |= (0x01 << (Y%8));
}

void Point_death(uint8_t X, uint8_t Y)		//д����
{
	Nodes_Cache[(Y/8)][X] &= ~(0x01 << (Y%8));
}

uint8_t CountPointRound(uint8_t X, uint8_t Y)		//ͳ�Ƶ���Χϸ������
{
	return ( 
	OLED_GetPoint(X-1, Y-1) + 	OLED_GetPoint(X  , Y-1) + 	OLED_GetPoint(X+1, Y-1) + 
	OLED_GetPoint(X-1, Y  ) + 								OLED_GetPoint(X+1, Y  ) + 
	OLED_GetPoint(X-1, Y+1) + 	OLED_GetPoint(X  , Y+1) + 	OLED_GetPoint(X+1, Y+1)
	);
}

void Game_Of_Life_Turn(void)		//ˢ����Ϸ�غ�
{
	uint8_t x, y;
	uint8_t Count;
	
	for(y = 0; y < 64; y ++)
	{
		for(x = 0; x < 128; x ++)
		{
			Count = CountPointRound(x, y);
			if(OLED_GetPoint(x, y))
			{
				if(Count < 2 || Count > 3)
				{
					Point_death(x, y);
				}
			}
			else
			{
				if(Count == 3)
				{
					Point_life(x, y);
				}
			}
		}
	}	
	Update_Display();
}


void Game_Of_Life_Play(void)		//��Ϸ��ʼ
{
	OLED_UI_FadeoutAllArea();
	OLED_Brightness_DODO();

	uint8_t x1 = 8, x2 = 16, y1 = 32, y2 = 32;
	int8_t shift = -1;
	
	
	OLED_Clear();
	uint8_t i, j;
	for (j = 0; j < 8; j ++)				//����8ҳ
	{
		for (i = 0; i < 128; i ++)			//����128��
		{
			Nodes_Cache[j][i] = 0x00;	//���Դ���������ȫ������
		}
	}

	int8_t p;
	while(1)
	{
		Game_Of_Life_Turn();
		
		if(shift > 0) {y2 += menu_Roll_event()*8;}
		else {x2 += menu_Roll_event()*8;}
		x2 %= 128;
		y2 %= 64;
		OLED_DrawLine(x1, y1, x2, y2);
		if((x2 - x1) > 1) {x1 += (x2 - x1) / 8 + 1;}
		else if((x2 - x1) < -1) {x1 += (x2 - x1) / 8 - 1;}
		else {x1 = x2;}
		if((y2 - y1) > 1) {y1 += (y2 - y1) / 2 +1;}
		else if((y2 - y1) < -1) {y1 += (y2 - y1) / 2 - 1;}
		else{y1 = y2;}
		
		OLED_Brightness_UPUP();
		
		OLED_Update();

		//Delay_ms(100);
		
		p=menu_Enter_event();
		if(p==1) {shift = -shift;}	
		else if ( p==2 ){
			OLED_UI_FadeoutAllArea();
			OLED_UI_PageStartPoint.CurrentPoint.X = 127;
			Delay_ms(300);
			return;
		}	
	}
}




//���ڴ��ڵĽṹ��
MenuWindow SetBrightnessWindow = {
	.General_Width = 80,								//���ڿ��
	.General_Height = 28, 							//���ڸ߶�
	.Text_String = "�����ٶ�",					//���ڱ���
	.Text_FontSize = OLED_UI_FONT_12,				//�ָ�
	.Text_FontSideDistance = 4,							//����������ľ���
	.Text_FontTopDistance = 3,							//������붥���ľ���
	.General_WindowType = WINDOW_ROUNDRECTANGLE, 	//��������
	.General_ContinueTime = 4.0,						//���ڳ���ʱ��

	.Prob_Data_Int = &RAWAtri,				//��ʾ�ı�����ַ
	.Prob_DataStep = 5,								//����
	.Prob_MinData = 5,									//��Сֵ
	.Prob_MaxData = 255, 								//���ֵ
	.Prob_BottomDistance = 3,							//�ײ����
	.Prob_LineHeight = 8,								//�������߶�
	.Prob_SideDistance = 4,								//�߾�
};


/**
 * @brief ��������
 */
void BrightnessWindow(void){
	OLED_UI_CreateWindow(&SetBrightnessWindow);
	Raise_Wake_trigger();
}


//��LOGO�ƶ��Ľṹ��
OLED_ChangePoint LogoMove;
//��LOGO�����ƶ��Ľṹ��
OLED_ChangePoint LogoTextMove;
//welcome�����ƶ��Ľṹ��
OLED_ChangePoint WelcomeTextMove;

extern OLED_ChangePoint OLED_UI_PageStartPoint ;



/**
 * @brief ���ر���
 */
void ReturnTimepageWindow(void){
	OLED_UI_EN=false;
	OLED_UI_FadeoutAllArea();
}




//���ò˵���ĸ�����ʾ����
void SettingAuxFunc(void){
			
//	//�ڹ涨λ����ʾLOGO
//	if(fabs(OLED_UI_PageStartPoint.CurrentPoint.X - OLED_UI_PageStartPoint.TargetPoint.X) < 4){
//		
//		if(tp)TY-=0.5;
//		else TY+=0.5;
//		
		LogoMove.TargetPoint.X = 0;
		LogoMove.TargetPoint.Y = 0;
//		
//		if(TY>64)tp=1;
//		else if(TY<0)tp=0;
//		
//	}
	
	//��LOGOTEXT�ƶ�����Ļ�Ҳ࿴�����ĵط�
	LogoTextMove.TargetPoint.X = 129;
	LogoTextMove.TargetPoint.Y = 0;
	//��Welcome�����ƶ�����Ļ�ײ��������ĵط�
	WelcomeTextMove.TargetPoint.X = 128;
	WelcomeTextMove.TargetPoint.Y = 0;
	
	ChangePoint(&LogoMove);
	OLED_ShowImageArea(LogoMove.CurrentPoint.X,LogoMove.CurrentPoint.Y,32,64,0,0,128,128,OLED_UI_SettingsLogo);
	
//	ChangePoint(&LogoTextMove);
//	OLED_ShowImageArea(LogoTextMove.CurrentPoint.X,LogoTextMove.CurrentPoint.Y,26,64,0,0,128,128,OLED_UI_LOGOTEXT64);
//	ChangePoint(&WelcomeTextMove);
//	OLED_ShowImageArea(WelcomeTextMove.CurrentPoint.X,WelcomeTextMove.CurrentPoint.Y,16,64,0,0,128,128,OLED_UI_LOGOGithub);
}


//���˵��ĸ�����ʾ����
void MainAuxFunc(void){
	//����ʾ
	LogoMove.TargetPoint.X = -200;
	LogoMove.TargetPoint.Y = 0;
	LogoMove.CurrentPoint.X = -200;
	LogoMove.CurrentPoint.Y = 0;

	LogoTextMove.TargetPoint.X = 129;
	LogoTextMove.TargetPoint.Y = 0;
	LogoTextMove.CurrentPoint.X = 129;
	LogoTextMove.CurrentPoint.Y = 0;
	
	WelcomeTextMove.TargetPoint.X = 128;
	WelcomeTextMove.TargetPoint.Y = 0;
	WelcomeTextMove.CurrentPoint.X = 128;
	WelcomeTextMove.CurrentPoint.Y = 0;
}

//���˵��Ĳ˵���
MenuItem MainMenuItems[] = {

	{.General_item_text = "���ر���",.General_callback = ReturnTimepageWindow,.General_SubMenuPage =NULL,.Tiles_Icon = Image_BIPAN},
	{.General_item_text = "��ѹ��",.General_callback = voltage_mt,.General_SubMenuPage = NULL,.Tiles_Icon = Image_ADDV},
	{.General_item_text = "ʾ����",.General_callback = voltage_waveform,.General_SubMenuPage = NULL,.Tiles_Icon = Image_ADDV2},
	{.General_item_text = "[MUSIC]",.General_callback = YXMP3_Homepage,.General_SubMenuPage = NULL,.Tiles_Icon = Image_MUSIC},
	{.General_item_text = "[VIDEO]",.General_callback = NULL,.General_SubMenuPage = &VIDEOMenuPage,.Tiles_Icon = Image_VIDEO},
	{.General_item_text = "[LED]",.General_callback = shoudt,.General_SubMenuPage = NULL,.Tiles_Icon = Image_flashlight},
	{.General_item_text = "���",.General_callback = miaobiao,.General_SubMenuPage = NULL,.Tiles_Icon = Image_second},
//	{.General_item_text = "�տ���",.General_callback = NULL,.General_SubMenuPage = NULL,.Tiles_Icon = Image_QRcode},
//	{.General_item_text = "��ʪ��",.General_callback = NULL,.General_SubMenuPage = NULL,.Tiles_Icon = Image_humiture},
	{.General_item_text = " [GAME]",.General_callback = Game_Of_Life_Play,.General_SubMenuPage = NULL,.Tiles_Icon = Image_GAME},
//	{.General_item_text = "������",.General_callback = Calculator,.General_SubMenuPage = NULL,.Tiles_Icon = Image_calc},
////	{.General_item_text = "Night",.General_callback = NULL,.General_SubMenuPage = NULL,.Tiles_Icon = Image_night},
////	{.General_item_text = "More",.General_callback = NULL,.General_SubMenuPage = &MoreMenuPage,.Tiles_Icon = Image_more},
	{.General_item_text = "SETTINTS",.General_callback = NULL,.General_SubMenuPage = &SettingsMenuPage,.Tiles_Icon = Image_setings},
	{.General_item_text = NULL},/*���һ���General_item_text��ΪNULL����ʾ����Ϊ�ָ���*/

};
extern bool hold;
//���ò˵�����������
MenuItem SettingsMenuItems[] = {
	{.General_item_text = "�ڰ�ģʽ",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = &ColorMode},
	{.General_item_text = "��������",.General_callback = Raise_Wake,.General_SubMenuPage = NULL,.List_BoolRadioBox = &RAWA},
	{.General_item_text = "�����ٶ�",.General_callback = BrightnessWindow,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "��ĻHold",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = &hold},
	{.General_item_text = "ʱ�����",.General_callback = Time_page_Adjust,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "��ʾ֡��",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = &OLED_UI_ShowFps},
	{.General_item_text = "���豸",.General_callback = NULL,.General_SubMenuPage = &AboutThisDeviceMenuPage,.List_BoolRadioBox = NULL},
	{.General_item_text = "[����]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},

	{.General_item_text = NULL},/*���һ���General_item_text��ΪNULL����ʾ����Ϊ�ָ���*/
};

MenuItem AboutThisDeviceMenuItems[] = {
	{.General_item_text = "-[MCU:]",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " STM32F103C8T6",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " CLK:72Mhz",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " RAM:163833Bit",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " ROM:524267Bit",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "-[Screen:]",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " SSD1306 128x64 OLED",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " MPU-6050",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " AHT20",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "-[Periph:]",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " W25Q128",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " YX6100-16s",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = ">���ڸ�WATCH",.General_callback = NULL,.General_SubMenuPage = &AboutOLED_UIMenuPage,.List_BoolRadioBox = NULL},
	{.General_item_text = "[����]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},

	{.General_item_text = NULL},/*���һ���General_item_text��ΪNULL����ʾ����Ϊ�ָ���*/
};

MenuItem AboutOLED_UIMenuItems[] = {
	{.General_item_text = "-[Author:]",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " bili @ʱ�����",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " OLED_UI���:",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " bili @��nm������",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " Ver:3.1",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "[����]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},

	{.General_item_text = NULL},/*���һ���General_item_text��ΪNULL����ʾ����Ϊ�ָ���*/
};

MenuItem VIDEOWRITEMenuItems[] = {
	{.General_item_text = "[����]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "��¼��Ƶ1",.General_callback = VIDEO_write_1,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "��¼��Ƶ2",.General_callback = VIDEO_write_2,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "��¼��Ƶ3",.General_callback = VIDEO_write_3,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "[���ط��ط���]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "[���ط��ط���]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "[���ط��ط���]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "����������Ƶ",.General_callback = VIDEO_erase,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "[���ط��ط���]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = NULL},/*���һ���General_item_text��ΪNULL����ʾ����Ϊ�ָ���*/
};


MenuItem VIDEOMenuItems[] = {
	{.General_item_text = "[����]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "������Ƶ1",.General_callback = VIDEO_1,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "������Ƶ2",.General_callback = VIDEO_2,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "������Ƶ3",.General_callback = VIDEO_3,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "��¼��Ƶ",.General_callback = NULL,.General_SubMenuPage = &VIDEOWRITEMenuPage,.List_BoolRadioBox = NULL},
	{.General_item_text = NULL},/*���һ���General_item_text��ΪNULL����ʾ����Ϊ�ָ���*/
};







MenuPage MainMenuPage = {
	//ͨ�����ԣ�����
	.General_MenuType = MENU_TYPE_TILES,  		 //�˵�����Ϊ��������
	.General_CursorStyle = NOT_SHOW,			 //�������
	.General_FontSize = OLED_UI_FONT_16,			//�ָ�
	.General_ParentMenuPage = NULL,				//�������Ǹ��˵������Ը��˵�ΪNULL
	.General_LineSpace = 10,						//������� ��λ�����أ����ڴ������Ͳ˵�����ֵ��ʾÿ������֮��ļ�࣬�����б����Ͳ˵�����ֵ��ʾ�м�ࣩ
	.General_MoveStyle = UNLINEAR,				//�ƶ���ʽ
	.General_MovingSpeed = 3,					//�����ƶ��ٶ�(��ֵ����ʵ��Ч������)
	.General_ShowAuxiliaryFunction = MainAuxFunc,		 //��ʾ��������
	.General_MenuItems = MainMenuItems,			//�˵�����������

	//�������ԣ�����.General_MenuType������ѡ��
	.Tiles_ScreenHeight = 64,					//��Ļ�߶�
	.Tiles_ScreenWidth = 128,						//��Ļ���
	.Tiles_TileWidth = 32,						 //�������
	.Tiles_TileHeight = 32,						 //�����߶�
};


MenuPage SettingsMenuPage = {
	//ͨ�����ԣ�����
	.General_MenuType = MENU_TYPE_LIST,  		 //�˵�����Ϊ�б�����
	.General_CursorStyle = REVERSE_ROUNDRECTANGLE,	 //�������Ϊ����
	.General_FontSize = OLED_UI_FONT_12,			//�ָ�
	.General_ParentMenuPage = &MainMenuPage,		 //���˵�Ϊ���˵�
	.General_LineSpace = 4,						//�м�� ��λ������
	.General_MoveStyle = UNLINEAR,				//�ƶ���ʽΪ���������߶���
	.General_MovingSpeed = SPEED,					//�����ƶ��ٶ�(��ֵ����ʵ��Ч������)
	.General_ShowAuxiliaryFunction = SettingAuxFunc,		 //��ʾ��������
	.General_MenuItems = SettingsMenuItems,		 //�˵�����������

	//�������ԣ�����.General_MenuType������ѡ��
	.List_MenuArea = {32, 0, 95, 64},			 //�б���ʾ����
	.List_IfDrawFrame = false,					 //�Ƿ���ʾ�߿�
	.List_IfDrawLinePerfix = true,				 //�Ƿ���ʾ��ǰ׺
	.List_StartPointX = 4,                        //�б���ʼ��X����
	.List_StartPointY = 2,                        //�б���ʼ��Y����
};

MenuPage AboutThisDeviceMenuPage = {
	//ͨ�����ԣ�����
	.General_MenuType = MENU_TYPE_LIST,  		 //�˵�����Ϊ�б�����
	.General_CursorStyle = REVERSE_ROUNDRECTANGLE,	 //�������ΪԲ�Ǿ���
	.General_FontSize = OLED_UI_FONT_12,			//�ָ�
	.General_ParentMenuPage = &SettingsMenuPage,		 //���˵�Ϊ���˵�
	.General_LineSpace = 4,						//�м�� ��λ������
	.General_MoveStyle = UNLINEAR,				//�ƶ���ʽΪ���������߶���
	.General_MovingSpeed = SPEED,					//�����ƶ��ٶ�(��ֵ����ʵ��Ч������)
	.General_ShowAuxiliaryFunction = NULL,		 //��ʾ��������
	.General_MenuItems = AboutThisDeviceMenuItems,		 //�˵�����������

	//�������ԣ�����.General_MenuType������ѡ��
	.List_MenuArea = {0, 0, 128, 64},			 //�б���ʾ����
	.List_IfDrawFrame = false,					 //�Ƿ���ʾ�߿�
	.List_IfDrawLinePerfix = false,				 //�Ƿ���ʾ��ǰ׺
	.List_StartPointX = 4,                        //�б���ʼ��X����
	.List_StartPointY = 2,                        //�б���ʼ��Y����

};

MenuPage AboutOLED_UIMenuPage = {
	//ͨ�����ԣ�����
	.General_MenuType = MENU_TYPE_LIST,  		 //�˵�����Ϊ�б�����
	.General_CursorStyle = REVERSE_BLOCK,	 //�������ΪԲ�Ǿ���
	.General_FontSize = OLED_UI_FONT_12,			//�ָ�
	.General_ParentMenuPage = &SettingsMenuPage,		 //���˵�Ϊ���˵�
	.General_LineSpace = 4,						//�м�� ��λ������
	.General_MoveStyle = UNLINEAR,				//�ƶ���ʽΪ���������߶���
	.General_MovingSpeed = SPEED,					//�����ƶ��ٶ�(��ֵ����ʵ��Ч������)
	.General_ShowAuxiliaryFunction = NULL,		 //��ʾ��������
	.General_MenuItems = AboutOLED_UIMenuItems,		 //�˵�����������

	//�������ԣ�����.General_MenuType������ѡ��
	.List_MenuArea = {0, 0, 128, 64},			 //�б���ʾ����
	.List_IfDrawFrame = false,					 //�Ƿ���ʾ�߿�
	.List_IfDrawLinePerfix = false,				 //�Ƿ���ʾ��ǰ׺
	.List_StartPointX = 4,                        //�б���ʼ��X����
	.List_StartPointY = 2,                        //�б���ʼ��Y����

};



MenuPage VIDEOMenuPage = {
	//ͨ�����ԣ�����
	.General_MenuType = MENU_TYPE_LIST,  		 //�˵�����Ϊ�б�����
	.General_CursorStyle = REVERSE_ROUNDRECTANGLE,	 //�������ΪԲ�Ǿ���
	.General_FontSize = OLED_UI_FONT_12,			//�ָ�
	.General_ParentMenuPage = &MainMenuPage,		 //���˵�Ϊ���˵�
	.General_LineSpace = 4,						//�м�� ��λ������
	.General_MoveStyle = UNLINEAR,				//�ƶ���ʽΪ���������߶���
	.General_MovingSpeed = SPEED,					//�����ƶ��ٶ�(��ֵ����ʵ��Ч������)
	.General_ShowAuxiliaryFunction = NULL,		 //��ʾ��������
	.General_MenuItems = VIDEOMenuItems,		 //�˵�����������

	//�������ԣ�����.General_MenuType������ѡ��
	.List_MenuArea = {0, 0, 128, 64},			 //�б���ʾ����
	.List_IfDrawFrame = false,					 //�Ƿ���ʾ�߿�
	.List_IfDrawLinePerfix = true,				 //�Ƿ���ʾ��ǰ׺
	.List_StartPointX = 4,                        //�б���ʼ��X����
	.List_StartPointY = 2,                        //�б���ʼ��Y����
};


MenuPage VIDEOWRITEMenuPage = {
	//ͨ�����ԣ�����
	.General_MenuType = MENU_TYPE_LIST,  		 //�˵�����Ϊ�б�����
	.General_CursorStyle = REVERSE_ROUNDRECTANGLE,	 //�������ΪԲ�Ǿ���
	.General_FontSize = OLED_UI_FONT_12,			//�ָ�
	.General_ParentMenuPage = &VIDEOMenuPage,		 //���˵�Ϊ���˵�
	.General_LineSpace = 4,						//�м�� ��λ������
	.General_MoveStyle = UNLINEAR,				//�ƶ���ʽΪ���������߶���
	.General_MovingSpeed = SPEED,					//�����ƶ��ٶ�(��ֵ����ʵ��Ч������)
	.General_ShowAuxiliaryFunction = NULL,		 //��ʾ��������
	.General_MenuItems = VIDEOWRITEMenuItems,		 //�˵�����������

	//�������ԣ�����.General_MenuType������ѡ��
	.List_MenuArea = {0, 0, 128, 64},			 //�б���ʾ����
	.List_IfDrawFrame = false,					 //�Ƿ���ʾ�߿�
	.List_IfDrawLinePerfix = true,				 //�Ƿ���ʾ��ǰ׺
	.List_StartPointX = 4,                        //�б���ʼ��X����
	.List_StartPointY = 2,                        //�б���ʼ��Y����
};
