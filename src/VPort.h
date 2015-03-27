#ifndef __VANILLA_CORE_PORT_H__
#define __VANILLA_CORE_PORT_H__


typedef struct VTimer
{
#ifdef WIN32
	UINT_PTR TimerID;//时钟id
#elif defined LINUX

#endif
} *VanillaTimer, _VTimer;

typedef struct VPortWindow
{
	VanillaWindow Window;
#ifdef WIN32
	HWND hWnd;
	HDC hDC;
	bool Layered;
#elif defined LINUX
	cairo_t* cairo;
	cairo_surface_t* cairo_surface;
	bool mapped;
	xcb_window_t xwindow;
	xcb_pixmap_t xpixmap;
	xcb_gcontext_t gc;
	xcb_colormap_t xcolormap;
#endif
} *VanillaPortWindow, _VPortWindow;

/**
* 此函数用作创建定时器.
* @param nElapse 时钟周期
* @param lpTimerFunc 回调函数
* @param Returns 成功返回定时器指针.
*/
VanillaTimer VanillaPortCreateTimer(VanillaInt nElapse, TIMERPROC lpTimerFunc);



#define VanillaPort_ICONV std::string _VanillaPort_lpu;char* _VanillaPort_lpu_;int _VanillaPort_lpu_len;std::wstring _VanillaPort_lpw;wchar_t* _VanillaPort_lpw_;int _VanillaPort_lpw_len;

#define VanillaPort_U2W(VanillaPort_lpu) ((_VanillaPort_lpu = (VanillaPort_lpu),_VanillaPort_lpu_=(char*)(_VanillaPort_lpu.c_str()),_VanillaPort_lpu_len = strlen(_VanillaPort_lpu_),_VanillaPort_lpw_len = VanillaPortUTF8ToUTF16(NULL, NULL, _VanillaPort_lpu_, _VanillaPort_lpu_len) + 1,_VanillaPort_lpw_ = (wchar_t*)malloc(_VanillaPort_lpw_len * 2),VanillaPortUTF8ToUTF16(_VanillaPort_lpw_, _VanillaPort_lpw_len, _VanillaPort_lpu_, _VanillaPort_lpu_len),_VanillaPort_lpw=_VanillaPort_lpw_,free(_VanillaPort_lpw_),_VanillaPort_lpw))
#define VanillaPort_W2U(VanillaPort_lpw) ((_VanillaPort_lpw = (VanillaPort_lpw),_VanillaPort_lpw_=(wchar_t*)(_VanillaPort_lpw.c_str()),_VanillaPort_lpw_len = wcslen(_VanillaPort_lpw_),_VanillaPort_lpu_len = VanillaPortUTF16ToUTF8(NULL, NULL, _VanillaPort_lpw_, _VanillaPort_lpw_len) + 1,_VanillaPort_lpu_ = (char*)malloc(_VanillaPort_lpu_len),VanillaPortUTF16ToUTF8(_VanillaPort_lpu_, _VanillaPort_lpu_len, _VanillaPort_lpw_, _VanillaPort_lpw_len),_VanillaPort_lpu=_VanillaPort_lpu_,free(_VanillaPort_lpu_),_VanillaPort_lpu))

VAPI(int) VanillaPortUTF16ToUTF8(char* Output, int OutputSize, wchar_t* Input, int InputLength);
VAPI(int) VanillaPortUTF8ToUTF16(wchar_t* Output, int OutputSize, char* Input, int Inputsize);
/**
* 此函数用作初始化X-Client连接.
* @param Returns 成功返回true.
*/
VanillaBool VanillaPortInitializeX();
/**
* 此函数用作创建VanillaPortWindow对象.
* @param Rect 窗口矩形
* @param Title 窗口标题
* @param ShowInTaskbar 是否在任务栏显示
* @param PosMiddle 是否居中
* @param Window 框架层VanillaWindow指针
* @param Returns 成功返回VanillaPortWindow对象,不成功返回NULL.
*/
VanillaPortWindow VanillaPortCreateWindow(VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaString Title, VanillaBool ShowInTaskbar, VanillaBool PosMiddle, VanillaWindow Window);
/**
* 此函数用作销毁由VanillaPortCreateWindow创建的对象.
* @param PortWindow VanillaPortWindow对象
* @此函数没有返回值.
*/
VanillaVoid VanillaPortDestroyWindow(VanillaPortWindow PortWindow);
/**
* 此函数用作设置窗口标题.
* @param PortWindow VanillaPortWindow对象
* @param Title 标题
* @此函数没有返回值.
*/
VanillaVoid VanillaPortSetWindowTitle(VanillaPortWindow PortWindow, VanillaString Title);
/**
* 此函数用作获取窗口标题.
* @param PortWindow VanillaPortWindow对象
* @param Returns 当前窗口标题.
*/
VanillaString VanillaPortGetWindowTitle(VanillaPortWindow PortWindow);
/**
* 此函数用作设置窗口可视状态.
* @param PortWindow VanillaPortWindow对象
* @param Visible 可视状态
* @此函数没有返回值.
*/
VanillaVoid VanillaPortSetWindowVisible(VanillaPortWindow PortWindow, VanillaBool Visible);
/**
* 此函数用作获取窗口当前可视状态.
* @param PortWindow VanillaPortWindow对象
* @param Returns 当前窗口可视状态.
*/
VanillaBool VanillaPortGetWindowVisible(VanillaPortWindow PortWindow);
/**
* 此函数用作设置是否开启混合效果.
* @param PortWindow VanillaPortWindow对象
* @param Composite true or false
* @此函数没有返回值.
*/
VanillaVoid VanillaPortSetWindowComposite(VanillaPortWindow PortWindow, VanillaBool Composite);
/**
* 此函数用作获取是否开启混合效果.
* @param PortWindow VanillaPortWindow对象
* @param Returns 当前混合状态.
*/
VanillaBool VanillaPortGetWindowComposite(VanillaPortWindow PortWindow);
/**
* 此函数用作拖动窗口.
* @param PortWindow VanillaPortWindow对象
* @此函数没有返回值.
*/
VanillaVoid VanillaPortDragWindow(VanillaPortWindow PortWindow);
/**
* 此函数用作刷新窗口显示.
* @param PortWindow VanillaPortWindow对象
* @param UpdateRect 刷新区域
* @此函数没有返回值.
*/
VanillaVoid VanillaPortUpdateWindow(VanillaWindow Window, VanillaRect UpdateRect);
/**
* 此函数用作移植层的消息循环.
* @param Returns int.
*/
VanillaInt VanillaPortMessageLoop();
/**
* 此函数用作获取系统启动时间.
* @param Returns int.
*/
VanillaInt VanillaPortGetTickCount();
/**
* 此函数用作处理鼠标消息.
* @param Window 消息对应的Window
* @param Action 消息对应的行为
* @param Button 消息对应的按钮
* @param x
* @param y
* @param Returns int.
*/
VanillaControl VanillaPortDispatchMouseMessage(VanillaWindow Window, VanillaInt Action, VanillaInt Button, VanillaInt x, VanillaInt y);
#endif	//__VANILLA_CORE_PORT_H__
