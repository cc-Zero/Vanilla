﻿#ifndef __VANILLA_CORE_DRAWING_H__
#define __VANILLA_CORE_DRAWING_H__
// Vanilla Drawing Functions

#define VTS_BOLD 1				//粗体
#define VTS_ITALIC 2			//斜体
#define VTS_ALIGN_LEFT 0		//水平居左
#define VTS_ALIGN_CENTER 4		//水平居中
#define VTS_ALIGN_RIGHT 8		//水平居右
#define VTS_VALIGN_TOP 0		//垂直顶部对齐
#define VTS_VALIGN_MIDDLE 16	//垂直居中对齐
#define VTS_VALIGN_BOTTOM 32	//垂直底部对齐
#define VTS_SHADOW 64

/**
* 此函数用作填充一个矩形区域.
* @param Graphics VanillaGraphics对象
* @param Color 用作填充的颜色
* @param Left 左边
* @param Top 顶边
* @param Width 宽度
* @param Height 高度
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaFillRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height);
/**
* 此函数用作.
* @param Graphics VanillaGraphics对象
* @param Color 用作填充的颜色
* @param BorderColor
* @param Left 左边
* @param Top 顶边
* @param Width 宽度
* @param Height 高度
* @param LineWidth 线宽
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDrawFilledRect(VanillaGraphics Graphics, VanillaColor Color, VanillaColor BorderColor, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth);
/**
* 此函数用作.
* @param Graphics VanillaGraphics对象
* @param Color 颜色
* @param Left 左边
* @param Top 顶边
* @param Width 宽度
* @param Height 高度
* @param LineWidth 线宽
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDrawRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth);
/**
* 此函数用作绘制文本.
* @param Graphics VanillaGraphics对象
* @param StringFormat 文本格式
* @param String 文本
* @param Rect 矩形区域
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDrawString(VanillaGraphics Graphics, VanillaStringFormat StringFormat, VanillaText String, VanillaRect Rect);
VAPI(VanillaVoid) VanillaCreateRoundRectPath(VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaReal Round, SkPath* Path);
VAPI(VanillaVoid) VanillaDrawFilledRoundRect(VanillaGraphics Graphics, VanillaColor Color, VanillaColor BorderColor, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth, VanillaReal Round);
/**
* 此函数用作绘制圆角矩形.
* @param Graphics VanillaGraphics对象
* @param Color
* @param x
* @param y
* @param Width
* @param Height
* @param LineWidth
* @param Round
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDrawRoundRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth, VanillaReal Round);

VAPI(VanillaVoid) VanillaFillRoundRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaReal Round);
/**
* 此函数用作绘制图像.
* @param Graphics VanillaGraphics对象
* @param Image VanillaImage对象
* @param x
* @param y
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDrawImage(VanillaGraphics Graphics, VanillaImage Image, VanillaInt x, VanillaInt y);
/**
* 此函数用作绘制图像(扩展).
* @param Graphics VanillaGraphics对象
* @param Image VanillaImage对象
* @param x 左边
* @param y 顶边
* @param Width 宽度
* @param Height 高度
* @param xSrc 源坐标
* @param ySrc 源顶边
* @param WidthSrc 源宽度
* @param HeightSrc 源高度
* @param Alpha 透明度
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDrawImageEx(VanillaGraphics Graphics, VanillaImage Image, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaInt xSrc, VanillaInt ySrc, VanillaInt WidthSrc, VanillaInt HeightSrc, VanillaByte Alpha);
VAPI(VanillaVoid) VanillaAlphaBlendEx(VanillaGraphics Dest, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaGraphics Src, VanillaInt xSrc, VanillaInt ySrc, VanillaInt WidthSrc, VanillaInt HeightSrc, VanillaByte Alpha);
VAPI(VanillaVoid) VanillaAlphaBlend(VanillaGraphics Dest, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaGraphics Src, VanillaInt xSrc, VanillaInt ySrc, VanillaByte Alpha);
/**
* 此函数用作使用指定颜色清空VanillaGraphics对象.
* @param Graphics VanillaGraphics对象
* @param Color 颜色
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaGraphicsClear(VanillaGraphics Graphics, VanillaColor Color);
/**
* 此函数用作创建VanillaStringFormat(文本格式)对象.
* @param FontName 字体名称
* @param FontSize 字体大小
* @param TextColor 字体颜色
* @param ShadowColor 阴影颜色
* @param StringStyle 字体风格 VTS_开头的常量
* @param Returns 成功返回VanillaStringFormat对象.
*/
VAPI(VanillaStringFormat) VanillaCreateStringFormat(VanillaText FontName, VanillaReal FontSize, VanillaColor TextColor, VanillaColor ShadowColor, VanillaInt StringStyle);
/**
* 此函数用作销毁VanillaStringFormat对象.
* @param StringFormat VanillaStringFormat对象
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDestroyStringFormat(VanillaStringFormat StringFormat);
VAPI(VanillaGraphics) VanillaCreateGraphicsOfWindow(VanillaWindow Window);
VAPI(VanillaVoid) VanillaDestroyGraphicsOfWindow(VanillaGraphics Graphcis);

VAPI(VanillaGraphics) VanillaCreateGraphicsInMemory(VanillaInt Width, VanillaInt Height);
/**
* 此函数用作销毁VanillaGraphics对象.
* @param Graphics VanillaGraphics对象
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDestroyGraphics(VanillaGraphics Graphics);
/**
* 此函数用作从文件载入图像.
* @param Graphics VanillaGraphics对象
* @param FileName 路径
* @param Returns 成功返回VanillaImage对象.
*/
VAPI(VanillaImage) VanillaLoadImageFromFile(VanillaText FileName);
/**
* 此函数用作从字节集载入图像.
* @param Graphics VanillaGraphics对象
* @param Binary 字节集
* @param Returns 成功返回VanillaImage对象.
*/
VAPI(VanillaImage) VanillaLoadImageFromBinary(VanillaBin Binary);
/**
* 此函数用作销毁VanillaImage对象.
* @param Image VanillaImage对象
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDestroyImage(VanillaImage Image);
#endif	//__VANILLA_CORE_DRAWING_H__
