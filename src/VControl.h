#ifndef __VANILLA_CORE_CONTROL_H__
#define __VANILLA_CORE_CONTROL_H__

// Vanilla控件处理

#define VM_CREATE	1
#define VM_DESTROY	2
#define VM_SETFOCUS	3
#define VM_KILLFOCUS	4
#define VM_SETVISIBLE	5
#define VM_SETENABLED	6
#define VM_PAINT	7
#define VM_SIZE		8
#define VM_MOVE		9
#define VM_TIMER	10
#define VM_CHAR		11
#define VM_KEYDOWN	12
#define VM_KEYUP	13
#define VM_LBUTTONDOWN	14
#define VM_LBUTTONUP	15
#define VM_LBUTTONCLK	16
#define VM_LBUTTONDBCLK	17
#define VM_RBUTTONDOWN	18
#define VM_RBUTTONUP	19
#define VM_RBUTTONCLK	20
#define VM_RBUTTONDBCLK	21
#define VM_MBUTTONDOWN	101
#define VM_MBUTTONUP	102
#define VM_MBUTTONCLK	103
#define VM_MBUTTONDBCLK	104
#define VM_MOUSEWHEEL	22
#define VM_SETPARENT	24
#define VM_MOUSEMOVE	26
#define VM_MOUSEIN	27
#define VM_MOUSEOUT	28
#define VM_REDRAW	29
#define VM_UPDATE	30
#define VM_USER		301
#define VM_NOTIFY	999


#define VE_LBTNCLK	1	// 左键单击
#define VE_LBTNDOWN	2	// 左键按下
#define VE_LBTNUP	3	// 左键弹起
#define VE_LBTNDBCLK	4	// 左键双击
#define VE_RBTNCLK	5	// 右键单击
#define VE_RBTNDOWN	6	// 右键按下
#define VE_RBTNUP	7	// 右键弹起
#define VE_RBTNDBCLK	8	// 右键双击
#define VE_KEYDOWN	9	// 按下某键
#define VE_KEYUP	10	// 放开某键
#define VE_CHAR		11	// 字符输入
#define VE_MOUSEMOVE	12	// 鼠标移动
#define VE_MOUSEIN	13	// 鼠标进入
#define VE_MOUSELEAVE	14	// 鼠标离开
#define VE_MOUSEWHEEL	15	// 滚轮滚动
#define VE_MBTNCLK	16	// 中键单击
#define VE_MBTNDOWN	17	// 中键按下
#define VE_MBTNUP	18	// 中键弹起
#define VE_MBTNDBCLK	19	// 中键双击


typedef struct VControl
{
	// 链表 Begin
	// 同级控件
	VanillaControl LastControl;//上一个
	VanillaControl NextControl;//下一个
	// 子控件
	VanillaControl ChildControlFirst;//第一个
	VanillaControl ChildControlEnd;//最后一个
	// 上级控件
	VanillaControl ParentControl;
	// 链表 End

	// 自定义ID
	VanillaInt ID;

	// 控件属性 Begin
	// 窗口
	VanillaWindow Window;
	// 可视
	VanillaBool Visible;
	// 可用
	VanillaBool Enabled;
	// 父层所有组件中不可用的层数
	VanillaInt DisabledCount;
	// 父层所有组件中不可视的层数
	VanillaInt InvisibleCount;
	// 鼠标穿透
	VanillaBool MousePenetration;
	// 透明度
	VanillaByte Alpha;
	// 控件回调函数(替换Class中的控件回调函数)
	VCtlProc CtlProc;
	// 事件回调函数
	VCtlEventProc EventProc;
	// 绑定控件 此控件可接收到NOTIFY消息
	VanillaControl BindOwner;
	// 控件类
	VanillaControlClass Class;

	VRect Rect;

	// 缓存图形
	VanillaGraphics Graphics;
	// 渐变缓存图形
	VanillaGraphics Graphics_Gradient1;
	VanillaGraphics Graphics_Gradient2;
	// 渐变参数
	VanillaBool Gradienting;
	VanillaReal GradientAlpha;
	VanillaInt GradientUserData;
	VanillaBool GradientType;
	// 控件属性 End

	VanillaAny ControlData;
} *VanillaControl, _VControl;

typedef struct VControlClass
{
	VanillaString ClassName;
	VCtlProc CtlProc;
	VanillaBool Focusable;
	VanillaBool Virtual;
} *VanillaControlClass, _VControlClass;

VAPI(VanillaControlClass) VanillaRegisterControlClass(VanillaText ClassName, VCtlProc CtlProc, VanillaBool Focusable, VanillaBool Virtual);
/**
* 此函数用作创建VanillaControl(控件)对象.
* @param ParentControl 上级控件
* @param ClassName 类名
* @param Rect 控件矩形
* @param ControlData
* @param CustomID
* @param VanillaBool
* @param Enabled
* @param CreateStruct
* @param Returns 成功返回VanillaControl对象,不成功返回NULL.
*/
VAPI(VanillaControl) VanillaControlCreate(VanillaControl ParentControl, VanillaText ClassName, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaAny ControlData, VanillaInt CustomID, VanillaBool Visible, VanillaBool Enabled, VanillaAny CreateStruct);
/**
* 此函数用作销毁VanillaControl(控件)对象.
* @param Control VanillaControl对象
* @param Returns 返回释放的控件总数.
*/
VAPI(VanillaInt) VanillaControlDestroy(VanillaControl Control);
/**
* 此函数用作重画VanillaControl(控件)对象.
* @param Control VanillaControl对象
* @param Update 是否刷新
* @param Returns 返回控件处理重绘事件后的返回值(一般为0).
*/
VAPI(VanillaInt) VanillaControlRedraw(VanillaControl Control, VanillaBool Update);
/**
* 此函数用作设置Control可用状态.
* @param Control VanillaControl对象
* @param Visible 是否可用
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaControlSetEnable(VanillaControl Control, VanillaBool Enabled);
/**
* 此函数用作获取Control可用状态.
* @param Control VanillaControl对象
* @param Returns 返回Control可用状态.
*/
VAPI(VanillaBool) VanillaControlIsEnable(VanillaControl Control);
/**
* 此函数用作设置Control可视状态.
* @param Control VanillaControl对象
* @param Visible 是否可视
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaControlSetVisible(VanillaControl Control, VanillaBool Visible);
/**
* 此函数用作获取Control可视状态.
* @param Control VanillaControl对象
* @param Returns 返回Control是否可视.
*/
VAPI(VanillaBool) VanillaControlIsVisible(VanillaControl Control);
/**
* 此函数用作设置Control透明度.
* @param Control VanillaControl对象
* @param Alpha 透明度
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaControlSetAlpha(VanillaControl Control, VanillaByte Alpha);
/**
* 此函数用作获取Control透明度.
* @param Control VanillaControl对象
* @param Returns 返回Control透明度.
*/
VAPI(VanillaByte) VanillaControlGetAlpha(VanillaControl Control);
/**
* 此函数用作移动Control.
* @param Control VanillaControl对象
* @param Left 左边
* @param Top 顶边
* @param Width 宽度
* @param Height 高度
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaControlMove(VanillaControl Control, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height);
/**
* 此函数用作获取Control相对于窗口根控件(RootControl)的区域.
* @param Control VanillaControl对象
* @param Rect 用来存放结果的Rect对象
* @param Returns 返回&Rect
*/
VAPI(VanillaRect) VanillaControlGetRectOfWindow(VanillaControl Control, VanillaRect Rect);
/**
* 此函数用作获取Control矩形区域.
* @param Control VanillaControl对象
* @param Returns 返回Control矩形
*/
VAPI(VanillaRect) VanillaControlGetRect(VanillaControl Control);
/**
* 此函数用作设置Control事件回调函数.
* @param Control VanillaControl对象
* @param EventProc 回调函数
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaControlSetEventProc(VanillaControl Control, VCtlEventProc EventProc);
/**
* 此函数用作获取Control事件回调函数指针.
* @param Control VanillaControl对象
* @param Returns 返回回调函数指针
*/
VAPI(VCtlEventProc) VanillaControlGetEventProc(VanillaControl Control);
/**
* 此函数用作设置全局默认事件回调函数.
* @param EventProc 全局默认事件回调函数
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaControlSetDefaultEventProc(VCtlEventProc EventProc);
/**
* 此函数用作触发Control的事件.
* @param Control VanillaControl对象
* @param Event 事件类型
* @param Returns 返回用户处理事件后的返回值
*/
VAPI(VanillaInt) VanillaControlTriggerEvent(VanillaControl Control, VanillaInt Event, VanillaInt Param1, VanillaInt Param2, VanillaInt Param3);
VAPI(VanillaInt) VanillaControlSendMessage(VanillaControl Control, VanillaInt Message, VanillaInt Param1, VanillaInt Param2);
VAPI(VanillaVoid) VanillaControlSendMessageToChild(VanillaControl ParentControl, VanillaInt Message, VanillaInt Param1, VanillaInt Param2);
VAPI(VanillaVoid) VanillaControlSendMessageToChildOfWindow(VanillaWindow Window, VanillaInt Message, VanillaInt Param1, VanillaInt Param2);
/**
* 此函数用作根据点在一个窗口内寻找另外一个控件.
* @param Window VanillaWindow对象
* @param x
* @param y
* @param x1 Out 相对坐标
* @param y1 Out
* @param Returns 返回寻找到的Control对象
*/
VAPI(VanillaControl) VanillaFindControlInWindow(VanillaWindow Window, VanillaInt x, VanillaInt y, VanillaInt *x1, VanillaInt *y1);
/**
* 此函数用作根据点在一个控件内寻找另外一个控件.
* @param ParentControl VanillaControl对象
* @param x
* @param y
* @param x1 Out 相对坐标
* @param y1 Out
* @param Returns 返回寻找到的Control对象
*/
VAPI(VanillaControl) VanillaFindControlInControl(VanillaControl ParentControl, VanillaInt x, VanillaInt y, VanillaInt *x1, VanillaInt *y1);
/**控件的默认处理函数
*/
VanillaInt VanillaDefaultControlProc(VanillaControl Control, VanillaInt Message, VanillaInt Param1, VanillaInt Param2);

extern std::map<VanillaString, VanillaControlClass> ControlClasses;
#endif	//__VANILLA_CORE_CONTROL_H__
