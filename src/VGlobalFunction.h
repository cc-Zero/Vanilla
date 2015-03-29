#ifndef __VANILLA_CORE_GLOBALFUNCTION_H__
#define __VANILLA_CORE_GLOBALFUNCTION_H__
// Normal Global Functions

VanillaBool VInit ();

VanillaBool VInit_WndMsg ();
VanillaBool VInit_WindowClass();
VanillaBool VInit_ControlClass();

/**
* 此函数用作Vanilla消息循环.
* @param Returns int.
*/
VAPI(VanillaInt) VanillaMsgLoop();
/**
* 此函数用作Vanilla初始化.
* @param Returns 返回true初始化成功.
*/
VAPI(VanillaBool) VanillaInit();
/**
* 此函数用作读取一个二进制文件的数据到VanillaBin结构中.
* @param FileName 文件名
* @param Returns 成功返回VanillaBin对象.
*/
VAPI(VanillaBin) VanillaReadFileToBin(VanillaText FileName);
/**
* 此函数用作检测某点是否存在与一个矩形之内.
* @param x 欲检测的点
* @param y
* @param Rect 用作检测的矩形
* @param Returns 在或不在.
*/
VAPI(VanillaBool) VanillaIsPointInRect(VanillaInt x, VanillaInt y, VanillaRect Rect);
/**
* 此函数用作计算两个矩形的交.
* @param Rect1 源矩形
* @param Rect2 源矩形
* @param RectResult 两个源矩形相交的矩形
* @param Returns 如果交不为空，则返回true；否则，如果交为空则返回false.
*/
VAPI(VanillaBool) VanillaIntersectRect(VanillaRect Rect1, VanillaRect Rect2, VanillaRect RectResult);
#endif	//__VANILLA_CORE_GLOBALFUNCTION_H__
