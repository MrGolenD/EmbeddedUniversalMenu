#ifndef __H_APP_MENU_H__
#define __H_APP_MENU_H__


#include <stdarg.h>		  //  标准头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"
#include "H_APP_TFTLCD.h"
#include "H_APP_Turret.h"
//#include "H_MID_FileConfig.h"
//#include "H_DRV_OLED.h"
//#include "H_APP_Motor.h"
//#include "H_DRV_DRV8870.h"



//菜单列表配置
typedef struct
{
  //列表菜单坐标
  uint16_t X_Start;
  uint16_t Y_Start;
  uint16_t X_END;
  uint16_t Y_END;
  //参数字体配置
  uint8_t font_weight;
  uint8_t font_height;

  //行间隔
  uint16_t RowInterval;
  //菜单总参数
  uint8_t Total_Param;
  //菜单单页总行数
  uint8_t Total_Row;
  //菜单总页数
  uint8_t Total_Page;
} H_APP_MenuList_T;

//彩屏菜单颜色
typedef struct 
{
  uint16_t ClearColor;
  uint16_t ParamColor;
  uint16_t ChoseColor;
	uint16_t ParamBackColor;
}H_APP_MenuColor_T;


//参数类型
typedef enum {
  Menu_TYPE_Uint16t,
  Menu_TYPE_Int16t,
  Menu_TYPE_Float,
}H_APP_Menu_ParamType_T;

//参数修改
typedef enum
{
  Menu_ParamAdd = 1,
  Menu_ParamSub = -1,
  Menu_EN_Modify = 2,
  Menu_NO_Modify = 3,
}H_APP_Menu_ParamModify_T;

//列表菜单参数配置
typedef struct {
  void *value;          // 指向变量的指针（int* 或 float*）
  H_APP_Menu_ParamType_T type;       // 参数类型
  H_APP_Menu_ParamModify_T is_modify;       // 是否可修改
  float step;           // 步进（1 对 int，0.1 对 float）
  float min, max;       // 限制范围
  char *format;         // 显示格式字符串
} H_APP_Menu_ParamItem_T;

//菜单状态机状态
typedef enum
{
  MenuSM_Statu_Idle = 0,
  MenuSM_Statu_Chose,
  MenuSM_Statu_Modify,
  MenuSM_Statu_MAX,
}H_APP_MenuSM_Status;

//菜单状态机事件
typedef enum 
{
  MenuSM_Event_Idle = 0,
  MenuSM_Event_Up,
  MenuSM_Event_Down,
  MenuSM_Event_Confirm,
  MenuSM_Event_Return,
  MenuSM_Event_MAX,
}H_APP_MenuEvent;

//参数列表菜单状态机
typedef struct 
{
  //当前状态
  H_APP_MenuSM_Status Statu;
  //当前事件
  H_APP_MenuEvent Event;
  //当前位置
  uint8_t Crrent_Param;
  //当前参数列表菜单页
  uint8_t Crrent_Page;
}H_APP_Menu_ParamListSM_T;

extern H_APP_Menu_ParamListSM_T Menu_ParamListSM;

void H_APP_Menu_Init();
void H_APP_Menu_Update();


void H_APP_MenuSetCall();


#endif
