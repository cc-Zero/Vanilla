// Vanilla Control Base Class
#include "stdafx.h"
#include "VDefine.h"
#include "VStruct.h"
#include "VWindow.h"
#include "VControl.h"
#include "VControlBase.h"


VanillaBool VControlBase::Init(VanillaControl ParentControl, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaBool Visible, VanillaBool Enabled) {
	return this->BaseCreate(ParentControl, Left, Top, Width, Height, Visible, Enabled);
}

VanillaBool VControlBase::BaseCreate(VanillaControl ParentControl, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaBool Visible, VanillaBool Enabled) {
	this->Control = ::VanillaControlCreate(ParentControl, Left, Top, Width, Height, this, NULL, Visible, Enabled, NULL);
	if (this->Control) {
		this->Control->CtlProc = &VanillaBasedControlProc;
		return true;
	}
	return false;
}

VanillaVoid VControlBase::GetFrameRect(VRect *r) {
	MAKEVRECTP(r, 0, 0, this->Control->Rect.Width - 1, this->Control->Rect.Height - 1);
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
