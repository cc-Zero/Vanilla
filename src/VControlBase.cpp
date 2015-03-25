// Vanilla Control Base Class
#include "stdafx.h"
#include "VDefine.h"
#include "VStruct.h"
#include "VWindow.h"
#include "VControl.h"
#include "VControlBase.h"


VControlBase::~VControlBase() {
	if (this->Control) {
		this->Destroy();
	}
}

VanillaBool VControlBase::Init(VanillaControl ParentControl, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaBool Visible, VanillaBool Enabled) {
	return this->BaseCreate(this->OnCreate(), ParentControl, Left, Top, Width, Height, Visible, Enabled);
}

VanillaControlClass VControlBase::BaseRegister(VanillaText ClassName, VanillaBool Focusable, VanillaBool Virtual) {
	return VanillaRegisterControlClass(ClassName, &VanillaBasedControlProc, Focusable, Virtual);
}

VanillaBool VControlBase::BaseCreate(VanillaText ClassName, VanillaControl ParentControl, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaBool Visible, VanillaBool Enabled) {
	this->Control = ::VanillaControlCreate(ParentControl, ClassName, Left, Top, Width, Height, this, NULL, Visible, Enabled, NULL);
	if (this->Control) {
		this->Control->CtlProc = &VanillaBasedControlProc;
		return true;
	}
	return false;
}

VanillaInt VControlBase::Destroy() {
	VanillaInt Result = ::VanillaControlDestroy(this->Control);
	this->Control = NULL;
	return Result;
}

VanillaVoid VControlBase::Enabled(VanillaBool Enabled) {
	::VanillaControlSetEnable(this->Control, Enabled);
}

VanillaBool VControlBase::Enabled() {
	return ::VanillaControlIsEnable(this->Control);
}

VanillaVoid VControlBase::Visible(VanillaBool Visible) {
	::VanillaControlSetVisible(this->Control, Visible);
}

VanillaBool VControlBase::Visible() {
	return ::VanillaControlIsVisible(this->Control);
}

VanillaVoid VControlBase::Alpha(VanillaByte Alpha) {
	::VanillaControlSetAlpha(this->Control, Alpha);
}

VanillaByte VControlBase::Alpha() {
	return ::VanillaControlGetAlpha(this->Control);
}

VanillaVoid VControlBase::EventProc(VCtlEventProc EventProc) {
	::VanillaControlSetEventProc(this->Control, EventProc);
}

VCtlEventProc VControlBase::EventProc() {
	return ::VanillaControlGetEventProc(this->Control);
}

VanillaInt VControlBase::SendMessage(VanillaInt Message, VanillaInt Param1, VanillaInt Param2) {
	return ::VanillaControlSendMessage(this->Control, Message, Param1, Param2);
}

VanillaVoid VControlBase::SendMessageToChild(VanillaInt Message, VanillaInt Param1, VanillaInt Param2) {
	::VanillaControlSendMessageToChild(this->Control, Message, Param1, Param2);
}

VanillaVoid VControlBase::Move(VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height) {
	::VanillaControlMove(this->Control, Left, Top, Width, Height);
}

VanillaVoid VControlBase::Move(VanillaRect NewRect) {
	::VanillaControlMove(this->Control, NewRect->Left, NewRect->Top, NewRect->Width, NewRect->Height);
}

VanillaVoid VControlBase::Move(VanillaPoint NewPoint) {
	::VanillaControlMove(this->Control, NewPoint->x, NewPoint->y, this->Control->Rect.Width, this->Control->Rect.Height);
}

VanillaVoid VControlBase::Move(VanillaSize NewSize) {
	::VanillaControlMove(this->Control, this->Control->Rect.Left, this->Control->Rect.Top, NewSize->Width, NewSize->Height);
}

VanillaRect VControlBase::GetRect() {
	return &this->Control->Rect;
}

VanillaVoid VControlBase::GetFrameRect(VRect *r) {
	MAKEVRECTP(r, 0, 0, this->Control->Rect.Width - 1, this->Control->Rect.Height - 1);
}

VanillaVoid VControlBase::Left(VanillaInt NewLeft) {
	::VanillaControlMove(this->Control, NewLeft, this->Control->Rect.Top, this->Control->Rect.Width, this->Control->Rect.Height);
}

VanillaInt VControlBase::Left() {
	return this->Control->Rect.Left;
}

VanillaVoid VControlBase::Top(VanillaInt NewTop) {
	::VanillaControlMove(this->Control, this->Control->Rect.Left, NewTop, this->Control->Rect.Width, this->Control->Rect.Height);
}

VanillaInt VControlBase::Top() {
	return this->Control->Rect.Top;
}

VanillaVoid VControlBase::Width(VanillaInt NewWidth) {
	::VanillaControlMove(this->Control, this->Control->Rect.Left, this->Control->Rect.Top, NewWidth, this->Control->Rect.Height);
}

VanillaInt VControlBase::Width() {
	return this->Control->Rect.Width;
}

VanillaVoid VControlBase::Height(VanillaInt NewHeight) {
	::VanillaControlMove(this->Control, this->Control->Rect.Left, this->Control->Rect.Top, this->Control->Rect.Width, NewHeight);
}

VanillaInt VControlBase::Height() {
	return this->Control->Rect.Height;
}

VanillaVoid VControlBase::Redraw(VanillaBool Update) {
	::VanillaControlRedraw(this->Control, Update);
}

VControlBase::operator VanillaControl() {
	return this->Control;
}

VanillaInt VControlBase::CtlProc(VanillaInt Message, VanillaInt Param1, VanillaInt Param2) {
	switch (Message) {
	case VM_LBUTTONDOWN:
		this->OnMouseLDown((VanillaPoint)Param2);
		break;
	case VM_LBUTTONUP:
		this->OnMouseLUp((VanillaPoint)Param2);
		break;
	case VM_LBUTTONDBCLK:
		this->OnMouseLDbClick((VanillaPoint)Param2);
		break;
	case VM_LBUTTONCLK:
		this->OnMouseLClick((VanillaPoint)Param2);
		break;
	case VM_RBUTTONDOWN:
		this->OnMouseRDown((VanillaPoint)Param2);
		break;
	case VM_RBUTTONUP:
		this->OnMouseRUp((VanillaPoint)Param2);
		break;
	case VM_RBUTTONDBCLK:
		this->OnMouseRDbClick((VanillaPoint)Param2);
		break;
	case VM_RBUTTONCLK:
		this->OnMouseRClick((VanillaPoint)Param2);
		break;
	case VM_MBUTTONDOWN:
		this->OnMouseMDown((VanillaPoint)Param2);
		break;
	case VM_MBUTTONUP:
		this->OnMouseMUp((VanillaPoint)Param2);
		break;
	case VM_MBUTTONDBCLK:
		this->OnMouseMDbClick((VanillaPoint)Param2);
		break;
	case VM_MBUTTONCLK:
		this->OnMouseMClick((VanillaPoint)Param2);
		break;
	case VM_MOUSEMOVE:
		this->OnMouseMove((VanillaPoint)Param2);
		break;
	case VM_MOUSEIN:
		this->OnMouseIn((VanillaControl)Param1, (VanillaPoint)Param2);
		break;
	case VM_MOUSEOUT:
		this->OnMouseOut((VanillaControl)Param1);
		break;
	case VM_MOUSEWHEEL:
		this->OnMouseWheel(Param1);
		break;
	case VM_SETENABLED:
		this->OnSetEnabled(Param1 == 1);
		break;
	case VM_SETVISIBLE:
		this->OnSetVisible(Param1 == 1);
		break;
	case VM_MOVE:
		this->OnMove((VanillaPoint)Param2);
		break;
	case VM_SIZE:
		this->OnSize((VanillaSize)Param2);
		break;
	case VM_SETPARENT:
		this->OnSetParent((VanillaControl)Param2);
		break;
	case VM_SETFOCUS:
		this->OnSetFocus((VanillaControl)Param2);
		break;
	case VM_KILLFOCUS:
		this->OnKillFocus((VanillaControl)Param2);
		break;
	case VM_PAINT:
		this->OnPaint((VanillaGraphics)Param2);
		break;
	case VM_CHAR:
		this->OnChar((VanillaText)Param2);
		break;
	case VM_KEYDOWN:
		this->OnKeyDown((VanillaByte)Param1);
		break;
	case VM_KEYUP:
		this->OnKeyUp((VanillaByte)Param1);
		break;
	}
	return NULL;
}
/**
* 控件基类回调
*/
VanillaInt VanillaBasedControlProc(VanillaInt ID, VanillaInt Message, VanillaInt Param1, VanillaInt Param2) {
	VanillaControl Control = (VanillaControl)ID;
	VanillaControlBase Object = (VanillaControlBase)Control->ControlData;
	return Object->CtlProc(Message, Param1, Param2);
}
