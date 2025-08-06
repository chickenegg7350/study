#ifndef __OLED_UI_MENUDATA_H
#define __OLED_UI_MENUDATA_H
// ����Ƿ���C++������
#ifdef __cplusplus
extern "C" {
#endif
#include "OLED_UI.h"

//����ǰ������
extern //�˵�����������
MenuItem MainMenuItems[],

	SettingsMenuItems[],    
		AboutThisDeviceMenuItems[],
		AboutOLED_UIMenuItems[],

	VIDEOMenuItems[],
		VIDEOWRITEMenuItems[]
;

extern //�˵�����
MenuPage MainMenuPage,

	SettingsMenuPage,
		AboutThisDeviceMenuPage,
		AboutOLED_UIMenuPage,

	VIDEOMenuPage,
		VIDEOWRITEMenuPage
;


#ifdef __cplusplus
}  // extern "C"
#endif

#endif
