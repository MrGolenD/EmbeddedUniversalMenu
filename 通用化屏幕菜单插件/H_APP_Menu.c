#include "H_APP_Menu.h"



//#define Use6x8_OeldShow
#define Use12x24_TFT_Show
//#define Use_Others_Show

//参数列表菜单
H_APP_MenuList_T Menu_ParamList;
//参数列表菜单状态机
H_APP_Menu_ParamListSM_T Menu_ParamListSM;
//彩屏显示颜色
H_APP_MenuColor_T MenuColor;

//////////////////////////////////////////接口部分////////////////////////////////////////////



/*
  参数列表配置:
  显示参数地址 , 参数类型 , 是否可修改 , 步进 , 最小值 , 最大值 , 显示格式 
*/
H_APP_Menu_ParamItem_T MenuParamItems[] = {
	
	{ &connon_volt.charge_status,Menu_TYPE_Int16t, Menu_NO_Modify, 0, 0, 0, "CHRG_STA:%d " },
	{ &connon_volt.set_volt,Menu_TYPE_Float, Menu_EN_Modify, 10.0f, 50.0f, 200.0f, "ConSetV:%.1f " },
	{ &connon_volt.real_volt,Menu_TYPE_Float, Menu_NO_Modify, 0, 0, 0, "ConRealV:%.1f " },
	{ &connon_volt.dac_volt,Menu_TYPE_Float, Menu_NO_Modify, 0, 0, 0, "ConDAC_V:%.2f " },
	{ &connon_volt.adc_volt,Menu_TYPE_Float, Menu_NO_Modify, 0, 0, 0, "ConADC_V:%.2f " },

	{ &motor_speed.L_SetSpeed,Menu_TYPE_Float, Menu_NO_Modify, 0, 0, 0, "L_St_Spd:%.1f " },
	{ &motor_speed.R_SetSpeed,Menu_TYPE_Float, Menu_NO_Modify, 0, 0, 0, "R_St_Spd:%.1f " },
	{ &motor_speed.L_MotorSpeed,Menu_TYPE_Int16t, Menu_NO_Modify, 0, 0, 0, "L_M_Spd:%d " },
  { &motor_speed.R_MotorSpeed,Menu_TYPE_Int16t, Menu_NO_Modify, 0, 0, 0, "R_M_Spd:%d " },
	
	{ &Turret_Infor.frame_angle, Menu_TYPE_Float,Menu_NO_Modify,0,0,0,"Frame:%.1f "},
	{ &Turret_Infor.base_angle, Menu_TYPE_Float,Menu_NO_Modify,0,0,0,"Basic:%.1f "},
	{ &Turret_Infor.is_laser,Menu_TYPE_Uint16t,Menu_NO_Modify,0,0,0,"Laser:%d"},
	

};


/*
  菜单配置接口,菜单分为左右两列
  默认菜单左列为可修改参数列,菜单右列为显示参数列
*/
void MenuPortInit()
{
  #ifdef  Use6x8_OeldShow
  Menu_ParamList=(H_APP_MenuList_T)
  {
    //左侧留出空间作菜单选中
    .X_Start=10,
    .Y_Start=15,
    .X_END=128,
    .Y_END=64,
    .font_weight=6,
    .font_height=8,
  };
  
  #endif

  #ifdef Use12x24_TFT_Show
  Menu_ParamList=(H_APP_MenuList_T)
  {
    //左侧留出空间作菜单选中
    .X_Start=40,
    .Y_Start=40,
    .X_END=480,
    .Y_END=320,
    .font_weight=12,
    .font_height=24,
  };
  MenuColor=(H_APP_MenuColor_T)
  {
    .ClearColor=BLACK,   
    .ParamColor=WHITE,
		.ParamBackColor=BLACK,
    .ChoseColor=WaterBlue,
  };
  #endif

  #ifdef Use_Others_Show
  Menu_ParamList=(H_APP_MenuList_T)
  {
    //左侧留出空间作菜单选中
    .X_Start=,
    .Y_Start=,
    .X_END=,
    .Y_END=,
    .font_weight=,
    .font_height=,
  };
  MenuColor=(H_APP_MenuColor_T)
  {
    .ClearColor=,   
    .ParamColor=,
		.ParamBackColor=,
    .ChoseColor=,
  };
  #endif

  /*计算参数列表菜单信息*/
  Menu_ParamList.RowInterval=Menu_ParamList.font_height+Menu_ParamList.font_height/8;
  Menu_ParamList.Total_Param=sizeof(MenuParamItems)/sizeof(MenuParamItems[0]);
  //留出一格用作页数显示
  Menu_ParamList.Total_Row=(Menu_ParamList.Y_END-Menu_ParamList.Y_Start)/Menu_ParamList.RowInterval-1;
  Menu_ParamList.Total_Page=Menu_ParamList.Total_Param/Menu_ParamList.Total_Row;
  //补余数
  if(Menu_ParamList.Total_Param%Menu_ParamList.Total_Row>0)Menu_ParamList.Total_Page++;

  //参数列表菜单 状态机初始化
  Menu_ParamListSM=(H_APP_Menu_ParamListSM_T)
  {
    .Statu=MenuSM_Statu_Idle,
    .Event=MenuSM_Event_Idle,
    .Crrent_Param=Menu_ParamList.Total_Param,
    .Crrent_Page=0,
  };
}



/**
  * @brief 清空指定区域
  * @param X 指定区域左上角的横坐标
  * @param Y 指定区域左上角的纵坐标
  * @param Width 指定区域的宽度
  * @param Height 指定区域的高度  
*/
void MenuClearArea(int16_t X, int16_t Y, int16_t Width, int16_t Height)
{
	#ifdef  Use6x8_OeldShow
  OLED_ClearArea(X,Y,Width,Height);
	#endif

  #ifdef Use12x24_TFT_Show
  H_APP_TFT_Draw_Rectangle(X,Y,X+Width,Y+Height,MenuColor.ClearColor);
	#endif

  #ifdef Use_Others_Show
  #endif
}

/**
  * @brief 画线
  * @param X0 指定一个端点的横坐标
  * @param Y0 指定一个端点的纵坐标
  * @param X1 指定另一个端点的横坐标
  * @param Y1 指定另一个端点的纵坐标
  */
void MenuDrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1)
{
  //驱动显示接口
  #ifdef Use6x8_OeldShow
  OLED_DrawLine(X0,Y0,X1,Y1);
  #endif

  #ifdef Use12x24_TFT_Show
	H_APP_TFT_DrawLine(X0,Y0,X1,Y1,MenuColor.ChoseColor);
	#endif

  #ifdef Use_Others_Show
  #endif
}

/**
  * @brief 画三角形
  * @param X0 指定第一个端点的横坐标
  * @param Y0 指定第一个端点的纵坐标
  * @param X1 指定第二个端点的横坐标
  * @param Y1 指定第二个端点的纵坐标
  * @param X2 指定第三个端点的横坐标
  * @param Y2 指定第三个端点的纵坐标
  * @param forColor 显示颜色
  * @param IsFilled 指定三角形是否填充 1填充 0不填充
*/
void MenuDrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2,uint8_t IsFilled)
{
  //驱动显示接口
  #ifdef Use6x8_OeldShow
  OLED_DrawTriangle(X0,Y0,X1,Y1,X2,Y2,IsFilled);
  #endif

	#ifdef Use12x24_TFT_Show
	H_APP_DrawTriangle(X0,Y0,X1,Y1,X2,Y2,MenuColor.ChoseColor,IsFilled);
	#endif

  #ifdef Use_Others_Show
  #endif
}

/**
 * @brief: 格式化参数显示接口
 * @param  x x起始坐标
 * @param  y y起始坐标
 * @param char 和printf一样输入需打印的文本
 * @param forColor 颜色显示
 */
void ParamShow(uint16_t x,uint16_t y,char *format, ...)
{
  char buf[32];  // 存放格式化后的字符串
  va_list args;  // 定义一个可变参数列表
  va_start(args, format);                 // 初始化可变参数
  vsnprintf(buf, sizeof(buf), format, args); // 格式化输出到 buf
  va_end(args);                           // 清理可变参数列表
  //驱动显示接口
  #ifdef Use6x8_OeldShow
  OLED_ShowString(x,y,buf,OLED_6X8);
  #endif

	#ifdef Use12x24_TFT_Show
	H_APP_TFT_Dis_12X24_String(x,y,buf,MenuColor.ParamBackColor,MenuColor.ParamColor);
	#endif

  #ifdef Use_Others_Show
  #endif
}

///////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////封装部分////////////////////////////////////////////


///////////////////参数列表菜单/////////////


//参数列表菜单更新
void ParamList_Update()
{
  //状态标签
	static const char *StatuStr[] = {"  [IDLE]"," [CHOSE]","[MODIFY]"};
	
	uint16_t param_y;
  //遍历参数初位
  uint8_t param_start=Menu_ParamListSM.Crrent_Page*Menu_ParamList.Total_Row;
  //遍历最大值
  uint8_t param_end=param_start+Menu_ParamList.Total_Row;
  if(param_end>Menu_ParamList.Total_Param)param_end=Menu_ParamList.Total_Param;
  //循环遍历更新 计算屏幕上需更新参数
  for(uint8_t i=param_start;i<param_end;i++)
  {
		param_y=Menu_ParamList.Y_Start+(i%Menu_ParamList.Total_Row)*Menu_ParamList.RowInterval;
    if(MenuParamItems[i].type==Menu_TYPE_Int16t){ParamShow(Menu_ParamList.X_Start,param_y,MenuParamItems[i].format,*(int16_t*)MenuParamItems[i].value);}
    if(MenuParamItems[i].type==Menu_TYPE_Uint16t){ParamShow(Menu_ParamList.X_Start,param_y,MenuParamItems[i].format,*(uint16_t*)MenuParamItems[i].value);}
    if(MenuParamItems[i].type==Menu_TYPE_Float){ParamShow(Menu_ParamList.X_Start,param_y,MenuParamItems[i].format,*(float*)MenuParamItems[i].value);}
  }
  //显示页面
	ParamShow(Menu_ParamList.X_Start,Menu_ParamList.Y_END-Menu_ParamList.RowInterval,"<Page:%d/%d>",Menu_ParamListSM.Crrent_Page+1,Menu_ParamList.Total_Page);
  //显示状态
	ParamShow(Menu_ParamList.X_END-Menu_ParamList.font_weight*8,Menu_ParamList.Y_Start-Menu_ParamList.font_height,"%s ",StatuStr[Menu_ParamListSM.Statu]);
} 

//参数列表菜单选中UI
void ParamList_ChoseUI(uint8_t last_loc,bool is_turn_page)
{
  //翻页清空
  if(is_turn_page){MenuClearArea(Menu_ParamList.X_Start,Menu_ParamList.Y_Start,Menu_ParamList.X_END,Menu_ParamList.Y_END);}

  //计算撤销位置
  int16_t clearPX=Menu_ParamList.X_Start-Menu_ParamList.RowInterval;
  int16_t clearPY=Menu_ParamList.Y_Start+Menu_ParamList.RowInterval*last_loc;
  //清空上次选中区域
  MenuClearArea(clearPX,clearPY,Menu_ParamList.RowInterval,Menu_ParamList.RowInterval);

  //计算选中坐标
  int16_t px0=Menu_ParamList.X_Start-Menu_ParamList.RowInterval;
  int16_t py0=Menu_ParamList.Y_Start+(Menu_ParamList.RowInterval/2)+Menu_ParamList.RowInterval*Menu_ParamListSM.Crrent_Param;
  int16_t px1=Menu_ParamList.X_Start-1;
  int16_t py1=Menu_ParamList.Y_Start+Menu_ParamList.RowInterval*Menu_ParamListSM.Crrent_Param;
  int16_t px2=Menu_ParamList.X_Start-1;
  int16_t py2=Menu_ParamList.Y_Start+Menu_ParamList.RowInterval*(Menu_ParamListSM.Crrent_Param+1)-1;
  //选中当前参数
  MenuDrawTriangle(px0,py0,px1,py1,px2,py2,1);
  //MenuDrawTriangle(0,19,8,15,8,23,1);

	
  //若为可变参数则划线
  uint8_t num = Menu_ParamListSM.Crrent_Param + Menu_ParamListSM.Crrent_Page*Menu_ParamList.Total_Row;
	//计算移位
	uint8_t l_shift=Menu_ParamList.RowInterval-Menu_ParamList.font_height;
  //只针对可修改变量进行划线
  if(MenuParamItems[num].is_modify==Menu_EN_Modify)
  {
    //下滑线y坐标
    uint16_t line_y = Menu_ParamList.Y_Start+Menu_ParamList.RowInterval*(Menu_ParamListSM.Crrent_Param+1)-1;
    //标中下划线 等比例缩放
    for(uint8_t i = 0;i<l_shift;i++)
    {
      //移位
      MenuDrawLine(Menu_ParamList.X_Start,line_y-i,Menu_ParamList.X_END,line_y-i);
    }
  }
	//计算撤销划线Y坐标
	int16_t clearLY=Menu_ParamList.Y_Start+Menu_ParamList.RowInterval*(last_loc+1)-l_shift;
	//撤销上次选中划线
	MenuClearArea(Menu_ParamList.X_Start,clearLY,Menu_ParamList.X_END-Menu_ParamList.X_Start,l_shift);


}

/**
 * @brief: 菜单标出选中参数
 * @param loc 选中回退或前进
 */
void ParamList_ParamChose(H_APP_Menu_ParamModify_T loc)
{
  uint8_t last_loc = Menu_ParamListSM.Crrent_Param;
  uint8_t last_page = Menu_ParamListSM.Crrent_Page;

  //计算当前全局绝对索引
  int16_t abs_index = Menu_ParamListSM.Crrent_Page * Menu_ParamList.Total_Row+ Menu_ParamListSM.Crrent_Param;
  // 更新索引
  abs_index += loc;   

  //处理全局越界(翻页)
  if (abs_index < 0) 
  {
    // 翻到最后一页
    Menu_ParamListSM.Crrent_Page = Menu_ParamList.Total_Page - 1;
    abs_index = Menu_ParamList.Total_Param - 1;
  }
  else if (abs_index >= Menu_ParamList.Total_Param)
  {
    // 翻到第一页
    Menu_ParamListSM.Crrent_Page = 0;
    abs_index = 0;
  }
  //更新当前页面及选中参数
  Menu_ParamListSM.Crrent_Page  = abs_index / Menu_ParamList.Total_Row;
  Menu_ParamListSM.Crrent_Param = abs_index % Menu_ParamList.Total_Row;
  //如果新页面与旧页面页数不符则翻页
  bool turn_page = (Menu_ParamListSM.Crrent_Page != last_page );
  //更新 UI
  ParamList_ChoseUI(last_loc, turn_page);
}

/**
 * @brief 修改参数
 * @param modify 修改操作 加或减
 * @return 0 正常修改 -1 参数为非可变参数
 */
int8_t ParamList_Modify(H_APP_Menu_ParamModify_T modify)
{
  uint8_t num = Menu_ParamListSM.Crrent_Param + Menu_ParamListSM.Crrent_Page*Menu_ParamList.Total_Row;
  H_APP_Menu_ParamItem_T *p= &MenuParamItems[num];

  if(p->is_modify == Menu_NO_Modify){return -1;}
  //类型分别判断
  if (p->type == Menu_TYPE_Uint16t)
  {
    uint16_t *val = (uint16_t*)p->value;
    *val += (p->step * modify);
    if (*val > (uint16_t)p->max) *val = (uint16_t)p->max;
    if (*val < (uint16_t)p->min) *val = (uint16_t)p->min;
  }
  else if (p->type == Menu_TYPE_Float)
  {
    float *val = (float*)p->value;
    *val += p->step * modify;
    if (*val > p->max) *val = p->max;
    if (*val < p->min) *val = p->min;
  }
  return 0;
}

//参数列表菜单清除
void ParamList_Clear()
{
	//清空区域
	int16_t ClearY=Menu_ParamList.Y_Start+Menu_ParamListSM.Crrent_Param*Menu_ParamList.RowInterval;
	//画线移位
	uint16_t l_shift = Menu_ParamList.RowInterval-Menu_ParamList.font_height;
  //清除选中
  MenuClearArea(Menu_ParamList.X_Start-Menu_ParamList.RowInterval,ClearY,Menu_ParamList.RowInterval,Menu_ParamList.RowInterval);
	MenuClearArea(Menu_ParamList.X_Start,ClearY+Menu_ParamList.RowInterval-l_shift,Menu_ParamList.X_END-Menu_ParamList.X_Start,l_shift);
  //重设参数
  Menu_ParamListSM.Crrent_Param=Menu_ParamList.Total_Param;
}

//////////////////////////////////////

typedef H_APP_MenuSM_Status (*MenuActionFn)(void);

/*事件触发动作处理函数*/

//无动作
H_APP_MenuSM_Status Act_None()
{return MenuSM_Statu_Idle;}
//空闲状态确认
H_APP_MenuSM_Status Act_Idle_Confirm()
{return MenuSM_Statu_Chose;}

//选择状态向上
H_APP_MenuSM_Status Act_Chose_Up()
{ParamList_ParamChose(Menu_ParamAdd);return Menu_ParamListSM.Statu;}
//选择状态向下
H_APP_MenuSM_Status Act_Chose_Down()
{ParamList_ParamChose(Menu_ParamSub);return Menu_ParamListSM.Statu;}
//选择状态确认
H_APP_MenuSM_Status Act_Chose_Confirm()
{return MenuSM_Statu_Modify;}
//选择状态回退
H_APP_MenuSM_Status Act_Chose_Return()
{ParamList_Clear();return MenuSM_Statu_Idle;}

//修改状态向上
H_APP_MenuSM_Status Act_Modify_Up()
{ParamList_Modify(Menu_ParamAdd);return Menu_ParamListSM.Statu;}
//修改状态向下
H_APP_MenuSM_Status Act_Modify_Down()
{ParamList_Modify(Menu_ParamSub);return Menu_ParamListSM.Statu;}
//修改状态确认
H_APP_MenuSM_Status Act_Modify_Confirm()
{ParamList_Clear();H_APP_MenuSetCall();return MenuSM_Statu_Idle;}
//修改状态回退
H_APP_MenuSM_Status Act_Modify_Return()
{return MenuSM_Statu_Chose;}

//状态机表
MenuActionFn ParamList_MenuSM_Table[MenuSM_Statu_MAX][MenuSM_Event_MAX] =
{
    // Idle 状态
    [MenuSM_Statu_Idle] =
    {
        [MenuSM_Event_Up] = Act_None,
        [MenuSM_Event_Down] = Act_None,
        [MenuSM_Event_Confirm] = Act_Idle_Confirm,
        [MenuSM_Event_Return]  = Act_None,
    },

    // Chose 状态
    [MenuSM_Statu_Chose] =
    {
        [MenuSM_Event_Up] = Act_Chose_Up,
        [MenuSM_Event_Down] = Act_Chose_Down,
        [MenuSM_Event_Confirm] = Act_Chose_Confirm,
        [MenuSM_Event_Return]  = Act_Chose_Return,
    },

    // Modify 状态
    [MenuSM_Statu_Modify] =
    {
      [MenuSM_Event_Up] = Act_Modify_Up,
      [MenuSM_Event_Down] = Act_Modify_Down,
      [MenuSM_Event_Confirm] = Act_Modify_Confirm,
      [MenuSM_Event_Return]  = Act_Modify_Return,
    },
};

//菜单运行状态机
void Menu_Task_SM()
{
  //若无事件，则直接退出
  if(Menu_ParamListSM.Event == MenuSM_Event_Idle)return;
  //获取当前事件对应状态
  MenuActionFn action = ParamList_MenuSM_Table[Menu_ParamListSM.Statu][Menu_ParamListSM.Event];

  if(action)
  {
    // 执行动作，并得到下一状态
    H_APP_MenuSM_Status next = action();
    Menu_ParamListSM.Statu=next;
  }

  Menu_ParamListSM.Event=MenuSM_Event_Idle;
	
}

/////////////////////////////////////////////外部调用///////////////////////////////////////////

//菜单初始化
void H_APP_Menu_Init()
{
	//驱动显示接口
  #ifdef Use6x8_OeldShow
	OLED_Init();
	#endif
	
  #ifdef Use12x24_TFT_Show
	H_APP_TFT_Init();
	#endif

  #ifdef Use_Others_Show
  #endif

  MenuPortInit();
}




//菜单数据更新 放入轮询定时20ms调用
void H_APP_Menu_Update()
{
  //状态机运行
  Menu_Task_SM();
  //参数列表菜单更新
  ParamList_Update();

	//驱动显示接口
  #ifdef Use6x8_OeldShow
  OLED_Update();
	#endif

  #ifdef Use_Others_Show
  #endif
}


//设置完成外部调用  业务处理
void H_APP_MenuSetCall()
{
// //保存参数
// H_MID_ConfigWrite_All();

	H_APP_ConnonCHRG_VoltSet();

}

