// Vanilla Controls
#include "stdafx.h"
#include "VDefine.h"
#include "VStruct.h"
#include "VWindow.h"
#include "VControl.h"
#include "VDrawing.h"
#include "VPort.h"
#include "VGlobalFunction.h"


VCtlEventProc DefEventProc = NULL;//全局默认回调函数


VAPI(VanillaControl) VanillaControlCreate(VanillaControl ParentControl, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaAny ControlData, VanillaInt CustomID, VanillaBool Visible, VanillaBool Enabled, VanillaAny CreateStruct) {
	if (ParentControl == NULL) {
		return NULL;
	}
	bool RootControl;
	if ((VanillaInt)ParentControl < 0) {
		RootControl = true;
	} else {
		RootControl = false;
	}

	VanillaControl Control = (VanillaControl)malloc(sizeof(VControl));
	memset(Control, 0, sizeof(VControl));
	Control->Focusable = true;
	Control->Virtual = false;

	Control->Alpha = 255;			//透明度
	Control->BindOwner = NULL;
	Control->Window = RootControl ? ((VanillaWindow)(- (VanillaInt)ParentControl)) : ParentControl->Window;	//所在的窗口
	Control->CtlProc = NULL;		//内核回调函数
	Control->Enabled = Enabled;		//可用
	Control->EventProc = NULL;		//用户回调
									//渐变参数
	Control->GradientAlpha = 0;
	Control->Gradienting = false;
	Control->GradientType = false;
	Control->GradientUserData = NULL;

	Control->ControlData = ControlData;//继承指针
	Control->ID = (CustomID == 0) ? (VanillaInt)Control : CustomID;
									//同级控件
	Control->LastControl = NULL;
	Control->NextControl = NULL;
										//子控件
	Control->ChildControlFirst = NULL;
	Control->ChildControlEnd = NULL;

	Control->Graphics = NULL;			//缓存图形
	Control->Graphics_Gradient1 = NULL;	//渐变缓存
	Control->Graphics_Gradient2 = NULL;
	Control->Visible = Visible;			//可视
	Control->MousePenetration = false;	//鼠标穿透
										//保存控件区域
	MAKEVRECT(Control->Rect, Left, Top, Width, Height);
	MAKEVRECT(Control->CRect, 0, 0, Width - 1, Height - 1);
	Control->ParentControl = RootControl ? NULL : ParentControl;
	if (!RootControl) {
		if (Control->ParentControl->ChildControlEnd != NULL) {
			Control->ParentControl->ChildControlEnd->NextControl = Control;
			Control->LastControl = Control->ParentControl->ChildControlEnd;
		}
		Control->ParentControl->ChildControlEnd = Control;
		if (Control->ParentControl->ChildControlFirst == NULL) {
			Control->ParentControl->ChildControlFirst = Control;
		}
	}
	Control->DisabledCount = Control->ParentControl ? (Control->ParentControl->DisabledCount + ((Control->ParentControl->Enabled) ? 0 : 1)) : 0;
	Control->InvisibleCount = Control->ParentControl ? (Control->ParentControl->InvisibleCount + ((Control->ParentControl->Visible) ? 0 : 1)) : 0;
	if (!(RootControl || Control->Virtual)) {
		Control->Graphics = VanillaCreateGraphicsInMemory(Width, Height);
	} else {
		Control->Graphics = NULL;
	}
	VanillaControlSendMessage(Control, VM_CREATE, NULL, (VanillaInt)CreateStruct);
	return Control;
}

VAPI(VanillaInt) VanillaControlDestroy(VanillaControl Control) {
	if (Control == NULL) {
		return 0;
	}
	VanillaControl ThisControl;
	VanillaControl NextControl = Control->ChildControlFirst;
	VanillaInt i = 0;
	while (NextControl != NULL) {
		ThisControl = NextControl;
		NextControl = NextControl->NextControl;
		i += VanillaControlDestroy(ThisControl);
	}
	/*通知控件控件已被销毁*/
	VanillaControlSendMessage(Control, VM_DESTROY, 0, 0);
	/*销毁图形*/
	VanillaDestroyGraphics(Control->Graphics);
	VanillaDestroyGraphics(Control->Graphics_Gradient1);
	VanillaDestroyGraphics(Control->Graphics_Gradient2);
	/*移出链表中自身*/
	if (Control->ParentControl) {
		if (Control->ParentControl->ChildControlFirst == Control) {
			Control->ParentControl->ChildControlFirst = Control->NextControl;
		}
		if (Control->ParentControl->ChildControlEnd == Control) {
			Control->ParentControl->ChildControlEnd = Control->LastControl;
		}
		if (Control->LastControl != NULL) {
			Control->LastControl->NextControl = Control->NextControl;
		}
		if (Control->NextControl != NULL) {
			Control->NextControl->LastControl = Control->LastControl;
		}
	}
	if (Control->Window->ButtonDownControl[0] == Control) {
		Control->Window->ButtonDownControl[0] = NULL;
	}
	if (Control->Window->ButtonDownControl[1] == Control) {
		Control->Window->ButtonDownControl[1] = NULL;
	}
	if (Control->Window->ButtonDownControl[2] == Control) {
		Control->Window->ButtonDownControl[2] = NULL;
	}
	if (Control->Window->CaptureControl == Control) {
		/*当前俘获输入控件是欲销毁的控件*/
		Control->Window->CaptureControl = NULL;
	}
	if (Control->Window->FocusControl == Control) {
		/*当前焦点控件是欲销毁的控件*/
		Control->Window->FocusControl = NULL;
	}
	if (Control->Window->MouseInControl == Control) {
		/*鼠标当前所在控件是欲销毁的控件*/
		Control->Window->MouseInControl = NULL;
	}
	free(Control);
	return i + 1;
}

VAPI(VanillaInt) VanillaControlRedraw(VanillaControl Control, VanillaBool Update) {
	return VanillaControlSendMessage(Control, VM_REDRAW, (VanillaInt)Update, NULL);
}
/*渐变更新*/
VAPI(VanillaVoid) VanillaControlGradient(VanillaControl Control, VanillaInt dwTime, VanillaInt dwGradient, VanillaBool bType)
{//真:在旧的缓存图形上慢慢增加不透明度覆盖绘制新的图像(模拟SyserUI工作方式)|||假:慢慢增加新图像的不透明度,减少就图像的不透明度(模拟Ex_DirectUI2.0工作方式)
	if (dwTime <= 0){ dwTime = 10; }
	if (dwGradient <= 0){ dwGradient = 10; }
	/*第一次渐变 初始化缓冲图形*/
	if (Control->Graphics_Gradient1 == 0){ Control->Graphics_Gradient1 = VanillaCreateGraphicsInMemory(Control->CRect.Width, Control->CRect.Height); }
	if (Control->Graphics_Gradient2 == 0){ Control->Graphics_Gradient2 = VanillaCreateGraphicsInMemory(Control->CRect.Width, Control->CRect.Height); }
	if (Control->Gradienting)
	{
		VanillaAlphaBlend(Control->Graphics, 0, 0, Control->CRect.Width, Control->CRect.Height, Control->Graphics_Gradient1, 0, 0, Control->Alpha);
	}
	/*保存旧图形到缓存2*/
	VanillaAlphaBlend(Control->Graphics_Gradient2, 0, 0, Control->CRect.Width, Control->CRect.Height, Control->Graphics, 0, 0, Control->Alpha);
	/*重绘到渐变缓存1*/
	VanillaGraphicsClear(Control->Graphics_Gradient1, 0);
	Control->CtlProc(Control->ID, VM_PAINT, NULL, (VanillaInt)Control->Graphics_Gradient1);
	/*创建时钟*/
	if (Control->Gradienting)
	{
		//上次渐变未完成
		Control->GradientAlpha = 0;
	}
	else
	{
		Control->GradientTimer = VanillaPortCreateTimer(dwTime, Control, dwGradient);
	}

	Control->Gradienting = true;
}

VAPI(VanillaVoid) VanillaControlSetEnable(VanillaControl Control, VanillaBool Enabled) {
	if (Control->Enabled != Enabled) {
		VanillaControlSendMessageToChild(Control, VM_SETENABLED, NULL, Enabled ? -1 : 1);
		Control->Enabled = Enabled;
	}
}

VAPI(VanillaBool) VanillaControlIsEnable(VanillaControl Control) {
	return Control->DisabledCount == 0 && Control->Enabled;
}

VAPI(VanillaVoid) VanillaControlSetVisible(VanillaControl Control, VanillaBool Visible) {
	if (Control->Visible != Visible) {
		VanillaControlSendMessageToChild(Control, VM_SETVISIBLE, NULL, Visible ? -1 : 1);
		Control->Visible = Visible;
	}
}

VAPI(VanillaBool) VanillaControlIsVisible(VanillaControl Control) {
	return Control->InvisibleCount == 0 && Control->Visible;
}

VAPI(VanillaVoid) VanillaControlSetAlpha(VanillaControl Control, VanillaByte Alpha) {
	Control->Alpha = Alpha;
}

VAPI(VanillaByte) VanillaControlGetAlpha(VanillaControl Control) {
	return Control->Alpha;
}

VAPI(VanillaVoid) VanillaControlSetFocus(VanillaControl Control)
{
	if (Control->Focusable){
		VanillaControl OldControl = Control->Window->FocusControl;
		Control->Window->FocusControl = Control;
		/*向旧控件发送失去焦点的消息*/
		if (OldControl)
		{
			VanillaDefaultControlProc(OldControl, VM_KILLFOCUS, NULL, (VanillaInt)Control);
		}
		/*向新控件发送得到焦点的消息*/
		VanillaDefaultControlProc(Control, VM_SETFOCUS, NULL, (VanillaInt)OldControl);
	}
}

VAPI(VanillaBool) VanillaControlGetFocus(VanillaControl Control)
{
	return Control->Window->FocusControl == Control;
}

VAPI(VanillaVoid) VanillaControlSetDefaultEventProc(VCtlEventProc EventProc) {
	DefEventProc = EventProc;
}

VAPI(VanillaVoid) VanillaControlSetCtlProc(VanillaControl Control, VCtlProc CtlProc) {
	Control->CtlProc = CtlProc;
}

VAPI(VCtlProc) VanillaControlGetCtlProc(VanillaControl Control) {
	return Control->CtlProc;
}

VAPI(VanillaVoid) VanillaControlSetEventProc(VanillaControl Control, VCtlEventProc EventProc) {
	Control->EventProc = EventProc;
}

VAPI(VCtlEventProc) VanillaControlGetEventProc(VanillaControl Control) {
	return Control->EventProc;
}

VAPI(VanillaInt) VanillaControlTriggerEvent(VanillaControl Control, VanillaInt Event, VanillaInt Param1, VanillaInt Param2, VanillaInt Param3) {
	if (Control == NULL) {
		return NULL;
	}
	if (Control->EventProc != NULL) {
		return Control->EventProc(Control->Window, Control, Event, Param1, Param2, Param3);
	} else if (DefEventProc != NULL) {
		return DefEventProc(Control->Window, Control, Event, Param1, Param2, Param3);
	}
	return NULL;
}

VAPI(VanillaInt) VanillaControlSendMessage(VanillaControl Control, VanillaInt Message, VanillaInt Param1, VanillaInt Param2) {
	if (Control == NULL) {
		return NULL;
	}
	return VanillaDefaultControlProc(Control, Message, Param1, Param2);
}

VAPI(VanillaVoid) VanillaControlSendMessageToChild(VanillaControl ParentControl, VanillaInt Message, VanillaInt Param1, VanillaInt Param2) {
	VanillaControl Control = ParentControl->ChildControlEnd;
	while (Control != NULL) {
		VanillaControlSendMessage(Control, Message, Param1, Param2);
		VanillaControlSendMessageToChild(Control, Message, Param1, Param2);
		Control = Control->LastControl;
	}
}

VAPI(VanillaVoid) VanillaControlSendMessageToChildOfWindow(VanillaWindow Window, VanillaInt Message, VanillaInt Param1, VanillaInt Param2) {
	VanillaControl Control = VanillaGetWindowRootControl(Window)->ChildControlEnd;
	while (Control != NULL) {
		VanillaControlSendMessage(Control, Message, Param1, Param2);
		VanillaControlSendMessageToChild(Control, Message, Param1, Param2);
		Control = Control->LastControl;
	}
}

VAPI(VanillaVoid) VanillaControlMove(VanillaControl Control, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height) {
	/*是否移动或改变大小*/
	VanillaBool Moved = false, Sized = false;
	if (Control->Rect.Left != Left || Control->Rect.Top != Top) {
		Moved = true;
	}
	if (Control->Rect.Width != Width || Control->Rect.Height != Height) {
		Sized = true;
	}
	if (Width < 0 || Height < 0 ){
		return;
	}
	/*以前的控件矩形*/
	VRect OldRect = Control->Rect;
	VRect OldRectOfWindow;
	VanillaControlGetRectOfWindow(Control, &OldRectOfWindow);
	/*更新窗口矩形*/
	MAKEVRECT(Control->Rect, Left, Top, Width, Height);
	MAKEVRECT(Control->CRect, 0, 0, Width - 1, Height - 1);
	if (Moved) {
		/*通知控件位置被移动*/
		VanillaControlSendMessage(Control, VM_MOVE, NULL, (VanillaInt)&OldRect.Left);
	}
	if (Sized) {
		/*大小被更改*/
		if (!Control->Virtual) {
			/*销毁掉控件当前的图形*/
			VanillaDestroyGraphics(Control->Graphics);
			/*重新为控件创建图形*/
			Control->Graphics = VanillaCreateGraphicsInMemory(Width, Height);
			/*通知控件重绘*/
			VanillaControlSendMessage(Control, VM_REDRAW, NULL, NULL);
			VanillaWindowUpdateGraphicsRect(Control->Window, &OldRectOfWindow, false, false);
			VRect NewRectOfWindow;
			VanillaControlGetRectOfWindow(Control, &OldRectOfWindow);
			VanillaWindowUpdateGraphicsRect(Control->Window, &NewRectOfWindow, false, false);
		}
		/*通知控件大小被改变*/
		VanillaControlSendMessage(Control, VM_SIZE, NULL, (VanillaInt)OldRect.Width);
	}
}

VAPI(VanillaRect) VanillaControlGetRect(VanillaControl Control) {
	return &Control->Rect;
}

VAPI(VanillaRect) VanillaControlGetRectOfWindow(VanillaControl Control, VanillaRect Rect) {
	*Rect = Control->Rect;
	VanillaControl ParentControl = Control->ParentControl;
	while (ParentControl) {
		Rect->Left += ParentControl->Rect.Left;
		Rect->Top += ParentControl->Rect.Top;
		ParentControl = ParentControl->ParentControl;
	}
	return Rect;
}

VAPI(VanillaControl) VanillaFindControlInWindow(VanillaWindow Window, VanillaInt x, VanillaInt y, VanillaInt *x1, VanillaInt *y1) {
	VanillaControl Control = VanillaFindControlInControl(VanillaGetWindowRootControl(Window), x, y, x1,y1);
	if (!Control) {
		*x1 = x;
		*y1 = y;
		return VanillaGetWindowRootControl(Window);
	}
	return Control;
}

VAPI(VanillaControl) VanillaFindControlInControl(VanillaControl ParentControl, VanillaInt x, VanillaInt y, VanillaInt *x1, VanillaInt *y1) {
	/*从最后一个子控件开始寻找 倒序*/
	VanillaControl Control = ParentControl->ChildControlEnd;
	while (Control != NULL) {
		if (!Control->Visible || !VanillaControlIsEnable(Control) || Control->MousePenetration) {
			/*不满足可被鼠标命中的条件 处理上一个子控件*/
			Control = Control->LastControl;
			continue;
		}
		if (VanillaIsPointInRect(x, y, &Control->Rect)){
			/*当前处理的控件被命中*/
			/*计算客户区坐标*/
			*x1 = x - Control->Rect.Left;
			*y1 = y - Control->Rect.Top;
			/*递归子控件*/
			VanillaControl SubControl = VanillaFindControlInControl(Control, x, y, x1, y1);
			if (SubControl == NULL) {
				/*子控件被未被命中 返回当前控件*/
				return Control;
			} else {
				/*子控件被命中 返回子控件*/
				return SubControl;
			}
		}
		/*处理上一个子控件*/
		Control = Control->LastControl;
	}
	return NULL;
}
/**
*通过VanillaControlSendMessage调用过来
*/
VanillaInt VanillaDefaultControlProc(VanillaControl Control, VanillaInt Message, VanillaInt Param1, VanillaInt Param2) {
	/*处理内核事件*/
	switch (Message) {
		case VM_SIZE: {
			/*大小被改变*/
			if (Control->Virtual) {
				return NULL;
			}
			VanillaDestroyGraphics(Control->Graphics_Gradient1);
			VanillaDestroyGraphics(Control->Graphics_Gradient2);
			VanillaDestroyGraphics(Control->Graphics);
			Control->Graphics = VanillaCreateGraphicsInMemory(Control->Rect.Width, Control->Rect.Height);
			VanillaControlSendMessage(Control, VM_REDRAW, 0, 0);
			break;
		}

		case VM_UPDATE: {
			if (Control->Virtual) {
				return NULL;
			}
			VRect RectOfWindow;
			VanillaControlGetRectOfWindow(Control, &RectOfWindow);
			VanillaWindowUpdateGraphicsRect(Control->Window, &RectOfWindow, false, true);
			break;
		}

		case VM_REDRAW: {
			if (Control->Virtual) {
				return NULL;
			}
			VanillaGraphicsClear(Control->Graphics, 0);
			VanillaInt Result = VanillaControlSendMessage(Control, VM_PAINT, 0, (VanillaInt)Control->Graphics);
#if (defined DEBUG_BORDER) && (DEBUG_BORDER == 1)
				VanillaDrawRect(Control->Graphics, ARGB(255, 0, 0, 0), 0, 0, Control->Rect.Width - 1, Control->Rect.Height - 1, 1);
#endif
			if (Param1) {
				VanillaControlSendMessage(Control, VM_UPDATE, 0, 0);
			}
			return Result;
			break;
		}
		case VM_TIMER:{
			if (Param2 == Control->GradientTimer)
			{
				/*渐变时钟*/
				Control->GradientAlpha += (VanillaReal)(Param1*2.55);//计算本次的透明度
				if (Control->GradientAlpha > 255){ Control->GradientAlpha = 255; }//透明度最多255
				if (Control->GradientType){
					//在旧的缓存图形上慢慢增加不透明度覆盖绘制新的图像(模拟SyserUI工作方式)
					VanillaAlphaBlend(Control->Graphics, 0, 0, Control->CRect.Width, Control->CRect.Height, Control->Graphics_Gradient2, 0, 0, 255);
				}
				else
				{
					//慢慢增加新图像的不透明度,减少就图像的不透明度(模拟Ex_DirectUI2.0工作方式)
					VanillaGraphicsClear(Control->Graphics, 0);
					VanillaAlphaBlend(Control->Graphics, 0, 0, Control->CRect.Width, Control->CRect.Height, Control->Graphics_Gradient2, 0, 0, 255 - (VanillaByte)Control->GradientAlpha);
				}
				VanillaAlphaBlend(Control->Graphics, 0, 0, Control->CRect.Width, Control->CRect.Height, Control->Graphics_Gradient1, 0, 0, (VanillaByte)Control->GradientAlpha);
				/*刷新缓存*/
				/*刷新显示*/
				VanillaDefaultControlProc(Control, VM_UPDATE, 0, 0);
				if (Control->GradientAlpha == 255)
				{
					VanillaAlphaBlend(Control->Graphics, 0, 0, Control->CRect.Width, Control->CRect.Height, Control->Graphics_Gradient1, 0, 0, Control->Alpha);
					VanillaDefaultControlProc(Control, VM_UPDATE, 0, 0);
					/*销毁时钟*/
					VanillaPortestroyTimer(Control->GradientTimer);
					Control->GradientTimer = 0;
					Control->GradientAlpha = 0;
					Control->Gradienting = false;
				}
			}
			break;
		}
		case VM_SETENABLED: {
			if (Param2 != 0) {
				Control->DisabledCount = Control->DisabledCount - Param2;
				return NULL;
			}
			break;
		}

		case VM_SETVISIBLE: {
			if (Param2 != 0) {
				Control->InvisibleCount = Control->InvisibleCount - Param2;
				return NULL;
			}
			break;
		}
	}
	/*处理内核事件结束*/
	/*通知控件处理*/
	VanillaInt Result = 0;
	if (Control->CtlProc != NULL) {
		Result = Control->CtlProc(Control->ID, Message, Param1, Param2);
	}
	/*控件处理结束*/
	/*通知用户处理*/
	switch (Message) {
		case VM_LBUTTONDOWN: {
			VanillaControlTriggerEvent(Control, VE_LBTNDOWN, Param2, NULL, NULL);
			break;
		}

		case VM_MBUTTONDOWN: {
			VanillaControlTriggerEvent(Control, VE_MBTNDOWN, Param2, NULL, NULL);
			break;
		}

		case VM_RBUTTONDOWN: {
			VanillaControlTriggerEvent(Control, VE_RBTNDOWN, Param2, NULL, NULL);
			break;
		}

		case VM_LBUTTONUP: {
			VanillaControlTriggerEvent(Control, VE_LBTNUP, Param2, NULL, NULL);
			break;
		}

		case VM_MBUTTONUP: {
			VanillaControlTriggerEvent(Control, VE_MBTNUP, Param2, NULL, NULL);
			break;
		}

		case VM_RBUTTONUP: {
			VanillaControlTriggerEvent(Control, VE_RBTNUP, Param2, NULL, NULL);
			break;
		}

		case VM_LBUTTONDBCLK: {
			VanillaControlTriggerEvent(Control, VE_LBTNDBCLK, Param2, NULL, NULL);
			break;
		}

		case VM_MBUTTONDBCLK: {
			VanillaControlTriggerEvent(Control, VE_MBTNDBCLK, Param2, NULL, NULL);
			break;
		}

		case VM_RBUTTONDBCLK: {
			VanillaControlTriggerEvent(Control, VE_RBTNDBCLK, Param2, NULL, NULL);
			break;
		}

		case VM_LBUTTONCLK: {
			VanillaControlTriggerEvent(Control, VE_LBTNCLK, Param2, NULL, NULL);
			break;
		}

		case VM_MBUTTONCLK: {
			VanillaControlTriggerEvent(Control, VE_MBTNCLK, Param2, NULL, NULL);
			break;
		}

		case VM_RBUTTONCLK: {
			VanillaControlTriggerEvent(Control, VE_RBTNCLK, Param2, NULL, NULL);
			break;
		}
	}
	/*用户处理结束*/
	return Result;
}
