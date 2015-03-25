// Vanilla Window
#include "stdafx.h"
#include "VDefine.h"
#include "VStruct.h"
#include "VWindow.h"
#include "VControl.h"
#include "VControlBase.h"
#include "VDrawing.h"
#include "VPort.h"
#include "VGlobalFunction.h"
#include "Utils/VTaskQueue.h"

#include "Controls/VanillaLabel.h"


#define VanillaCreateDefaultControl(NewControl, DefaultControl, Window, DefaultControlID) \
	DefaultControl.Control = NewControl; \
	DefaultControl.Control->ID = (VanillaInt)&DefaultControl; \
	DefaultControl.Window = Window; \
	DefaultControl.Control->CtlProc = VanillaWindowDefaultControlsProc; \
	DefaultControl.ID = DefaultControlID;

VAPI(VanillaWindow) VanillaCreateWindow(VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height,
					VanillaInt WindowStyle,
					VanillaText Title,
					VanillaStringFormat StringFormat,
					VanillaImage BackgroundImage,
					VanillaColor BackgroundColor,
					VanillaInt BackgroundType,
					VanillaInt Shape
					) {
	VanillaWindow Window = (VanillaWindow)malloc(sizeof(VWindow));
	memset(Window, 0, sizeof(VWindow));

	VanillaPortWindow PortWindow = VanillaPortCreateWindow(Left, Top, Width, Height, Title, !(WindowStyle & VWS_NOTASKBAR), (WindowStyle & VWS_POSMIDDLE), Window);
	if (NULL == PortWindow) {
		free(Window);
		return NULL;
	}
	Window->PortWindow = PortWindow;	//保存移植层的window对象
	MAKEVRECT(Window->Rect, Left, Top, Width, Height);

	Window->BackgroundColor = BackgroundColor;
	Window->BackgroundImage = BackgroundImage;
	Window->BackgroundType = BackgroundType;

	Window->DragType = WindowStyle & (VWS_DRAG_ANY | VWS_DRAG_NO | VWS_DRAG_TITLE);//任意拖动

	Window->ShadowColor = -1;//阴影颜色 默认无

	VanillaSetWindowShape(Window, Shape);//窗口形状

	Window->TaskQueue = new VTaskQueue(Window);//创建任务列队
	/*创建根控件*/
	//VanillaCreateDefaultControl(VanillaControlCreate((VanillaControl)(- (VanillaInt)Window), "VanillaUI.WindowRootControl", RECT(0, 0, Window->Rect.Width, Window->Rect.Height), NULL, NULL, true, true, NULL), Window->RootControl, Window, VWDC_ROOT);
	Window->RootControl.Control = VanillaControlCreate((VanillaControl)(-(VanillaInt)Window), "VanillaUI.WindowRootControl", 0, 0, Window->Rect.Width, Window->Rect.Height, NULL, NULL, true, true, NULL);
	Window->RootControl.Control->ID = (VanillaInt)&Window->RootControl;
	Window->RootControl.Window = Window;
	Window->RootControl.Control->CtlProc = VanillaWindowDefaultControlsProc;
	Window->RootControl.ID = VWDC_ROOT;
	if (WindowStyle & VWS_TITLE) {
		/*创建标题栏 作为跟控件的子控件*/
		//VanillaCreateDefaultControl(VanillaLabelCreate(VanillaGetWindowRootControl(Window), RECT(13, 11, Window->Rect.Width - 20, 30), Title, StringFormat, true, true), Window->Title, Window, VWDC_TITLE);
		Window->Title.Control = VanillaLabelCreate(VanillaGetWindowRootControl(Window), 13, 11, Window->Rect.Width - 20, 30, Title, StringFormat, true, true);
		Window->Title.Control->ID = (VanillaInt)&Window->Title;
		Window->Title.Window = Window;
		Window->Title.Control->CtlProc = VanillaWindowDefaultControlsProc;
		Window->Title.ID = VWDC_TITLE;
	}
	/*创建窗口相关Graphics对象*/
	VanillaWindowInitGraphics(Window, true);
	return Window;
}

VAPI(VanillaVoid) VanillaDestroyWindow(VanillaWindow Window) {
	/*销毁窗口根控件 与其相关的子控件都会*/
	VanillaControlDestroy(Window->RootControl.Control);
	/*销毁背景图形*/
	VanillaDestroyGraphics(Window->GraphicsBackground);
	/*销毁窗口图形*/
	VanillaDestroyGraphics(Window->GraphicsWindow);
	/*释放内存*/
	free(Window);
}

VAPI(VanillaVoid) VanillaSetWindowEventProc(VanillaWindow Window, VCtlEventProc EventProc) {
	Window->RootControl.Control->EventProc = EventProc;
}

VAPI(VCtlEventProc) VanillaGetWindowEventProc(VanillaWindow Window) {
	return Window->RootControl.Control->EventProc;
}

VAPI(VanillaVoid) VanillaSetWindowVisible(VanillaWindow Window, VanillaBool Visible) {
	if (Visible && Window->FirstShow == false) {
		Window->FirstShow = true;
	}
	VanillaPortSetWindowVisible(Window->PortWindow, Visible);
}

VAPI(VanillaBool) VanillaGetWindowVisible(VanillaWindow Window) {
	return VanillaPortGetWindowVisible(Window->PortWindow);
}

VAPI(VanillaControl) VanillaGetWindowRootControl(VanillaWindow Window) {
	return Window->RootControl.Control;
}

VAPI(VanillaVoid) VanillaSetWindowBkg(VanillaWindow Window, VanillaImage BackgroundImage, VanillaColor BackgroundColor, VanillaInt BackgroundType) {
	Window->BackgroundColor = BackgroundColor;
	Window->BackgroundImage = BackgroundImage;
	Window->BackgroundType = BackgroundType;
	VanillaWindowInitGraphics(Window, false);
}

VAPI(VanillaVoid) VanillaSetWindowShape(VanillaWindow Window, VanillaInt Shape) {
	Window->Shape = Shape;
	/*
	HRGN Rgn;
	switch (Shape) {
	case V_WndShape_Rect:
		Rgn = CreateRoundRectRgn(7, 7, Window->Rect.Width - 7 + 2, Window->Rect.Height - 7 + 2, 0, 0);
		Window->ShapeRegion = Region::FromHRGN(Rgn);
		DeleteObject((HGDIOBJ)Rgn);
		SetWindowRgn(Window->PortWindow->hWnd, NULL, false);
		break;
	case V_WndShape_RoundRect:
		Rgn = CreateRoundRectRgn(7, 7, Window->Rect.Width - 7 + 2, Window->Rect.Height - 7 + 2, 3, 3);
		Window->ShapeRegion = Region::FromHRGN(Rgn);
		if (!Window->Layered) {
			SetWindowRgn(Window->PortWindow->hWnd, Rgn, true);
		}
		DeleteObject((HGDIOBJ)Rgn);
		break;
	case V_WndShape_Custom:
		Window->ShapeRegion->MakeEmpty();
		SetWindowRgn(Window->PortWindow->hWnd, NULL, false);
		break;
	}
	*/
}

VAPI(VanillaInt) VanillaGetWindowShape(VanillaWindow Window) {
	return Window->Shape;
}

VAPI(VanillaVoid) VanillaSetWindowShadowColor(VanillaWindow Window, VanillaInt ShadowColor) {
	Window->ShadowColor = ShadowColor;
}

VAPI(VanillaInt) VanillaGetWindowShadowColor(VanillaWindow Window) {
	return Window->ShadowColor;
}

VAPI(VanillaVoid) VanillaRedrawWindowGraphics(VanillaWindow Window) {
	VanillaWindowInitGraphics(Window, false);
}

VAPI(VanillaVoid) VanillaRedrawWindow(VanillaWindow Window) {
	VanillaRedrawWindowGraphics(Window);
	VanillaFlashWindow(Window);
}

VAPI(VanillaVoid) VanillaFlashWindow(VanillaWindow Window) {
	VanillaWindowUpdate(Window, NULL);
}

VAPI(VanillaVoid) VanillaSetWindowComposite(VanillaWindow Window, VanillaBool Composite) {
	VanillaPortSetWindowComposite(Window->PortWindow, Composite);
}

VAPI(VanillaBool) VanillaGetWindowComposite(VanillaWindow Window) {
	return VanillaPortGetWindowComposite(Window->PortWindow);
}

VAPI(VanillaVoid) VanillaSetWindowAlpha(VanillaWindow Window, VanillaByte Alpha) {
	Window->Alpha = Alpha;
}

VAPI(VanillaByte) VanillaGetWindowAlpha(VanillaWindow Window) {
	return Window->Alpha;
}

VanillaVoid VanillaWindowInitGraphics(VanillaWindow Window, VanillaBool ForceRecreate) {
	if (ForceRecreate) {
		/*强制重建*/
		VanillaDestroyGraphicsOfWindow(Window->GraphicsBackground);
		VanillaDestroyGraphicsOfWindow(Window->GraphicsWindow);
		Window->GraphicsBackground = VanillaCreateGraphicsOfWindow(Window);
		Window->GraphicsWindow = VanillaCreateGraphicsOfWindow(Window);
	} else {
		VanillaGraphicsClear(Window->GraphicsBackground, 0);
		VanillaGraphicsClear(Window->GraphicsWindow, 0);
	}
	/*重画背景*/
	VanillaWindowDrawBackground(Window, Window->GraphicsBackground);
	/*重画窗口*/
	VanillaWindowDrawWindow(Window, Window->GraphicsWindow);
}

VanillaVoid VanillaWindowDrawWindow(VanillaWindow Window, VanillaGraphics Graphics) {
	//VanillaWindowDrawBackground(Window, Graphics);
	VanillaWindowDrawControl(Window);
}

VanillaVoid VanillaWindowDrawBackground(VanillaWindow Window, VanillaGraphics Graphics) {
	VanillaWindowDrawBackgroundImage(Window, Graphics);

	//HICON Icon = (HICON)SendMessageW (Window->PortWindow->hWnd, WM_GETICON, ICON_SMALL, NULL);
	//if (0 != Icon) {
	//	VanillaDrawIcon(Graphics, Icon, 7 + 8, 6 + 8, 16, 16);
	//	Window->TitleRect->Top = 3 + 8;
	//	Window->TitleRect->Left = 28 + 8;
	//} else {
	//	Window->TitleRect.Top = 3 + 8;
	//	Window->TitleRect.Left = 5 + 8;
	//}
	//VanillaDrawString(Graphics, Window->TitleStringFormat, STR(Window->Title), &Window->TitleRect);
}

VanillaVoid VanillaWindowDrawBackgroundImage(VanillaWindow Window, VanillaGraphics Graphics) {
	if (!(Window->BackgroundType & VBT_NOCOLORFILL)) {
		VanillaFillRect(Graphics, Window->BackgroundColor, 0, 0, Window->Rect.Width, Window->Rect.Height);
	}
	int ShadowColor = Window->ShadowColor;

    //if(Window->BackgroundImage) VanillaDrawImageEx(Window->GraphicsBackground, Window->BackgroundImage, 0, 0, 0, 0, 0, 0, 0, 0, 255);
    //DEBUG_PUTPNG(Window->GraphicsBackground->Bitmap, "CREATE.png");

	if (Window->BackgroundImage) {
		/*绘制背景图片*/
		if (Window->BackgroundType & VBT_LEFTTOP) {
			VanillaDrawImage(Graphics, Window->BackgroundImage, (ShadowColor == -1) ? 0 : 5, (ShadowColor == -1) ? 0 : 5);
		} else {
			VanillaDrawImageEx(Window->GraphicsBackground, Window->BackgroundImage, (ShadowColor == -1) ? 0 : 5, (ShadowColor == -1) ? 0 : 5, Window->Rect.Width - ((ShadowColor == -1) ? 0 : 10), Window->Rect.Height - ((ShadowColor == -1) ? 0 : 10), 0, 0, 0, 0, 255);
			//VanillaDrawImageEx(Graphics, Window->BackgroundImage, 5, 5, Window->Rect.Width - 10, Window->Rect.Height - 10, 0, 0, 0, 0, 255);
		}
	}
	if (Window->Shape == VWFS_RECT) {
		if (ShadowColor != -1) {
			/*双层边框*/
			VanillaDrawRect(Graphics, ARGB(204, 96, 96, 96), 5, 5, Window->Rect.Width - 10, Window->Rect.Height - 10, 1);
			VanillaDrawRect(Graphics, ARGB(102, 255, 255, 255), 6, 6, Window->Rect.Width - 12, Window->Rect.Height - 12, 1);

			/*绘制阴影*/
			VanillaDrawRoundRect(Graphics, RGB2ARGB(ShadowColor, 45), 5, 5, Window->Rect.Width - 10, Window->Rect.Height - 10, 1, 0);
			VanillaDrawRoundRect(Graphics, RGB2ARGB(ShadowColor, 25), 4, 4, Window->Rect.Width - 8, Window->Rect.Height - 8, 1, 0);
			VanillaDrawRoundRect(Graphics, RGB2ARGB(ShadowColor, 10), 3, 3, Window->Rect.Width - 6, Window->Rect.Height - 6, 1, 0);
			VanillaDrawRoundRect(Graphics, RGB2ARGB(ShadowColor, 5), 2, 2, Window->Rect.Width - 4, Window->Rect.Height - 4, 1, 0);
		}
		else{
			/*双层边框*/
			VanillaDrawRect(Graphics, ARGB(204, 96, 96, 96), 0, 0, Window->Rect.Width - 1, Window->Rect.Height - 1, 1);
			VanillaDrawRect(Graphics, ARGB(102, 255, 255, 255), 1, 1, Window->Rect.Width - 3, Window->Rect.Height - 3, 1);
		}
	}
}

VanillaVoid VanillaWindowDrawControl(VanillaWindow Window) {
	VanillaWindowUpdateGraphicsRect(Window, NULL, true, false);
}

VanillaVoid VanillaWindowUpdate(VanillaWindow Window, VanillaRect UpdateRect) {
	VanillaPortUpdateWindow(Window, UpdateRect);
}

VanillaVoid VanillaWindowUpdateGraphicsRect(VanillaWindow Window, VanillaRect UpdateRect, VanillaBool ForceRedraw, VanillaBool Flash) {
	if (!UpdateRect) {
		VRect r;
		/*更新矩形为空的话则更新整个窗口*/
		UpdateRect = &r;
		MAKEVRECTP(UpdateRect, 0, 0, Window->Rect.Width, Window->Rect.Height);
	}
	/*复制更新区域的背景到窗口*/
	VanillaAlphaBlend(Window->GraphicsWindow, UpdateRect->Left, UpdateRect->Top, UpdateRect->Width, UpdateRect->Height, Window->GraphicsBackground, UpdateRect->Left, UpdateRect->Top, 255);
	
	VanillaControl Control = Window->RootControl.Control->ChildControlFirst;
	VPoint OffsetPoint;
	MAKEVPOINT(OffsetPoint, 0, 0);
	VanillaByte Alpha = 255;
	while (Control) {
		VRect Intersect;
		if (!VanillaControlIsVisible(Control)) {
			/*控件不可视 更新下一个同级控件*/
			Control = Control->NextControl;
			/*到循环尾*/
			continue;
		}
		VRect RectOfClient;
		MAKEVRECT(RectOfClient, Control->Rect.Left + OffsetPoint.x, Control->Rect.Top + OffsetPoint.y, Control->Rect.Width, Control->Rect.Height);
		if (!VanillaIntersectRect(&RectOfClient, UpdateRect, &Intersect)) {
			/*控件不在更新范围内 更新下一个同级控件*/
			Control = Control->NextControl;
			/*到循环尾*/
			continue;
		}
		if (!Control->Class->Virtual) {
			if (ForceRedraw) {
				VanillaControlSendMessage(Control, VM_REDRAW, 0, 0);
			}
			VanillaAlphaBlend(Window->GraphicsWindow, Intersect.Left, Intersect.Top, Intersect.Width, Intersect.Height, Control->Graphics, 0, 0, Alpha * Control->Alpha / 255);
		}

		if (Control->ChildControlFirst != NULL) {
			OffsetPoint.x = OffsetPoint.x + Control->Rect.Left;
			OffsetPoint.y = OffsetPoint.y + Control->Rect.Top;
			Control = Control->ChildControlFirst;
			Alpha = Alpha * Control->Alpha / 255;
		} else if (Control->NextControl != NULL) {
			/*更新下一个同级控件*/
			Control = Control->NextControl;
		} else {
			/*已经最后一个控件了*/
			while (Control->NextControl == NULL) {
				if (Control->ParentControl == NULL) {
					if (Flash) {
						VanillaWindowUpdate(Window, UpdateRect);
					}
					return;
				}
				OffsetPoint.x = OffsetPoint.x - Control->ParentControl->Rect.Left;
				OffsetPoint.y = OffsetPoint.y - Control->ParentControl->Rect.Top;
				Alpha = Alpha / Control->Alpha * 255;
				Control = Control->ParentControl;
			}
			/**/
			Control = Control->NextControl;
		}
	}
	if (Flash) {
		VanillaWindowUpdate(Window, UpdateRect);
	}
}
/**
* window类的消息回调
*/
VanillaInt VanillaWindowDefaultControlsProc(VanillaInt ID, VanillaInt Message, VanillaInt Param1, VanillaInt Param2) {
	VanillaWindowDefaultControl ControlInfo = (VanillaWindowDefaultControl)ID;
	VanillaControl Control = ControlInfo->Control;
	if (ControlInfo->ID == VWDC_ROOT) {
		/*窗口消息*/
		switch (Message) {
		case VM_LBUTTONDOWN:
			if (ControlInfo->Window->DragType & VWS_DRAG_ANY) {
				/*任意移动窗口*/
				VanillaPortDragWindow(ControlInfo->Window->PortWindow);
			}
			break;
		case VM_SIZE:
			//if ()
			break;
		}
		return NULL;
	}
	else if (ControlInfo->ID == VWDC_TITLE) {
		/*标题区消息*/
		switch (Message) {
		case VM_LBUTTONDOWN:
			if (!(ControlInfo->Window->DragType & VWS_DRAG_NO)) {
				/*任意移动窗口*/
				VanillaPortDragWindow(ControlInfo->Window->PortWindow);
			}
			break;
		}
		return Control->Class->CtlProc((VanillaInt)Control, Message, Param1, Param2);
	}
	return NULL;
}
