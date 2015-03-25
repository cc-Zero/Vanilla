#ifndef __VANILLA_CORE_WINDOW_H__
#define __VANILLA_CORE_WINDOW_H__

#define VWS_POSMIDDLE		1   //居中
#define VWS_CTLBTN_LEFT		256	//
#define VWS_CTLBTN_RIGHT	512	//
#define VWS_CTLBTN_CLOSE	2   //控制按钮_关闭
#define VWS_CATBTN_MAX		4   //控制按钮_最大化
#define VWS_CTLBTN_MIN		8   //控制按钮_最小化
#define VWS_NOTASKBAR		16  //不在任务栏显示
#define VWS_TITLE			32  //标题栏
#define VWS_DRAG_TITLE		0   //拖动标题栏(默认)
#define VWS_DRAG_ANY		128 //拖动任意
#define VWS_DRAG_NO			64  //拖动禁止


#define VBT_LEFTTOP			1	//居左上
#define VBT_TENSILE			2	//拉伸
#define VBT_NOCOLORFILL		4	//无填充颜色

#define VWFS_RECT			0	//矩形
#define VWFS_ROUNDRECT		1	//圆角矩形
#define VWFS_CUSTOM			2	//




typedef struct VWindowDefaultControl
{
	VanillaControl Control;
	VanillaWindow Window;
	VanillaInt ID;
} *VanillaWindowDefaultControl, _VWindowDefaultControl;

#define VWDC_ROOT  0
#define VWDC_TITLE 1
#define VWDC_ICON  2


typedef struct VWindow
{
	VanillaPortWindow PortWindow;			//移植层窗口
	VRect Rect;								//窗口矩形
	VanillaByte Alpha;						//透明度
	VanillaTaskQueue TaskQueue;				//任务列队
	VanillaInt Shape;						//形状
	VanillaInt ShadowColor;					//阴影颜色
	VanillaInt DragType;					//随意拖动

	VanillaGraphics GraphicsBackground;		//背景图形
	VanillaGraphics GraphicsWindow;			//窗口图形

	VanillaImage BackgroundImage;			//背景图片
	VanillaColor BackgroundColor;			//背景颜色
	VanillaInt BackgroundType;				//背景类型

	VWindowDefaultControl RootControl;		//根控件 整个窗口都为根控件
	VWindowDefaultControl Title;			//标题栏控件

	VanillaControl ButtonDownControl [3];	//
	VanillaControl MouseInControl;			//鼠标所在的控件
	VanillaControl FocusControl;			//焦点控件
	VanillaControl CaptureControl;


	VanillaBool FirstShow;					//可视
} *VanillaWindow, _VWindow;
/**
* 此函数用作创建VanillaWindow对象.
* @param WndStyle 窗口风格
* @param Title 窗口标题
* @param StringFormat 文本格式
* @param BackgroundImage 背景图像
* @param BackgroundColor 背景颜色
* @param BackgroundType 背景类型
* @param Shape 形状 VWFS_开头的常量
* @param Returns 成功返回VanillaWindow对象,不成功返回NULL.
*/
VAPI(VanillaWindow) VanillaCreateWindow(VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height,
	VanillaInt WndStyle,
	VanillaText Title,
	VanillaStringFormat StringFormat,
	VanillaImage BackgroundImage,
	VanillaColor BackgroundColor,
	VanillaInt BackgroundType,
	VanillaInt Shape
	);
/**
* 此函数用作销毁VanillaWindow对象.
* @param Window VanillaWindow对象
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDestroyWindow(VanillaWindow Window);
/**
& 此函数用作设置窗口可视状态.
* @param Window VanillaWindow对象
* @param Visible 是否可视
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaSetWindowVisible(VanillaWindow Window, VanillaBool Visible);
/**
* 此函数用作获取窗口当前的可视状态.
* @param Window VanillaWindow对象
* @param Returns 当前的可视状态.
*/
VAPI(VanillaBool) VanillaGetWindowVisible(VanillaWindow Window);
/**
* 此函数用作获取窗口的根控件.
* @param Window VanillaWindow对象
* @param Returns 该窗口的根控件.
*/
VAPI(VanillaControl) VanillaGetWindowRootControl(VanillaWindow Window);
/**
* 此函数用作设置窗口背景.
* @param Window VanillaWindow对象
* @param BackkgroundImage 背景图片
* @param BackkgroundColor 背景颜色
* @param BackkgroundType 背景类型
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaSetWindowBkg(VanillaWindow Window, VanillaImage BackkgroundImage, VanillaColor BackkgroundColor, VanillaInt BackkgroundType);
/**
* 此函数用作设置窗口形状.
* @param Window VanillaWindow对象
* @param Shape 形状 VWFS_开头的常量
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaSetWindowShape(VanillaWindow Window, VanillaInt Shape);
/**
* 此函数用作获取窗口当前形状.
* @param Window VanillaWindow对象
* @param Returns 返回当前形状.
*/
VAPI(VanillaInt) VanillaGetWindowShape(VanillaWindow Window);
/**
* 此函数用作设置窗口阴影颜色.
* 使用函数请先设置分层窗口
* @param Window VanillaWindow对象
* @param ShadowColor  阴影颜色
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaSetWindowShadowColor(VanillaWindow Window, VanillaInt ShadowColor);
/**
* 此函数用作获取窗口阴影颜色.
* @param Window VanillaWindow对象
* @param Returns 阴影颜色
*/
VAPI(VanillaInt) VanillaGetWindowShadowColor(VanillaWindow Window);
/**
* 此函数用作重画并刷新窗口缓存图形.
* @param Window VanillaWindow对象
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaRedrawWindowGraphics(VanillaWindow Window);
/**
* 此函数用作重画并刷新窗口.
* @param Window VanillaWindow对象
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaRedrawWindow(VanillaWindow Window);
/**
* 此函数用作刷新窗口.
* @param Window VanillaWindow对象
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaFlashWindow(VanillaWindow Window);
/**
* 此函数用作设置窗口开启混合(半透明)效果.
* 此函数在Linux下无效,因为Linux的窗口总是自动开启混合效果
* @param Window VanillaWindow对象
* @param Composite 是否开启混合
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaSetWindowComposite(VanillaWindow Window, VanillaBool Composite);
/**
* 此函数用作活取窗口是否开启混合(半透明)效果.
* 此函数在Linux下无效,返回值将总为true,因为Linux的窗口总是自动开启混合效果
* @param Window VanillaWindow对象
* @param Returns 当前混合状态
*/
VAPI(VanillaBool) VanillaGetWindowComposite(VanillaWindow Window);
/**
* 此函数用作设置窗口透明度.
* @param Window VanillaWindow对象
* @param Alpha 透明度 0-255
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaSetWindowAlpha(VanillaWindow Window, VanillaByte Alpha);
/**
* 此函数用作获取窗口当前透明度.
* @param Window VanillaWindow对象
* @param Returns 返回当前透明度.
*/
VAPI(VanillaByte) VanillaGetWindowAlpha(VanillaWindow Window);



VanillaVoid VanillaWindowInitGraphics(VanillaWindow Window, VanillaBool ForceRecreate);
VanillaVoid VanillaWindowDrawWindow(VanillaWindow Window, VanillaGraphics Graphics);
VanillaVoid VanillaWindowDrawBackground(VanillaWindow Window, VanillaGraphics Graphics);
VanillaVoid VanillaWindowDrawBackgroundImage(VanillaWindow Window, VanillaGraphics Graphics);
VanillaVoid VanillaWindowDrawControl(VanillaWindow Window);
VanillaVoid VanillaWindowUpdate(VanillaWindow Window, VanillaRect UpdateRect);
VanillaVoid VanillaWindowUpdateGraphicsRect(VanillaWindow Window, VanillaRect UpdateRect, VanillaBool ForceRedraw, VanillaBool Flash);

VanillaInt VanillaWindowDefaultControlsProc(VanillaInt ID, VanillaInt Message, VanillaInt Param1, VanillaInt Param2);
#endif	//__VANILLA_CORE_WINDOW_H__
