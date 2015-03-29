// Vanilla按钮(控件)
#include "Include.h"

#define BUTTON_STATUS_NORMAL 0
#define BUTTON_STATUS_HOVER  1
#define BUTTON_STATUS_CLICK  2

/*按钮控件的回调函数*/
VanillaInt VanillaButtonProc(VanillaInt ID, VanillaInt Message, VanillaInt Param1, VanillaInt Param2) {
	VanillaControl Control = (VanillaControl)ID;
	VanillaButton p = (VanillaButton)Control->ControlData;
	switch (Message) {
	case VM_LBUTTONDOWN:
		p->Status = BUTTON_STATUS_CLICK;
		VanillaControlRedraw(Control, true);
		break;
	case VM_LBUTTONUP:
		p->Status = BUTTON_STATUS_HOVER;
		VanillaControlRedraw(Control, true);
		break;
	case VM_MOUSEIN:
		p->Status = BUTTON_STATUS_HOVER;
		VanillaControlRedraw(Control, true);
		break;
	case VM_MOUSEOUT:
		p->Status = BUTTON_STATUS_NORMAL;
		VanillaControlRedraw(Control, true);
		break;
	case VM_SETFOCUS:
		VanillaControlRedraw(Control, true);
		break;
	case VM_KILLFOCUS:
		VanillaControlRedraw(Control, true);
		break;
	case VM_PAINT:
		switch (p->Style) {
		case VBS_METRO:
			VanillaFillRect(Control->Graphics, p->ButtonColor[VanillaControlIsEnable(Control) ? p->Status : 3], 0, 0, Control->CRect.Width, Control->CRect.Height);
			VanillaDrawString(Control->Graphics, p->StringFormat, String2Text(p->Title), &Control->CRect);
			if (Control->Window->FocusControl == Control) {
				VanillaDrawRect(Control->Graphics, p->ButtonColor[4], 1, 1, Control->CRect.Width - 1, Control->CRect.Height - 1, 2);
			}
			else {
				VanillaDrawRect(Control->Graphics, p->ButtonColor[4], 0, 0, Control->CRect.Width, Control->CRect.Height, 1);
			}
		}
		break;
	}
	return NULL;
}

VAPI(VanillaControl) VanillaButtonCreate(VanillaControl ParentControl, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaText Title, VanillaStringFormat StringFormat, VanillaBool Visible, VanillaBool Enabled) {
	VanillaButton p = (VanillaButton)malloc(sizeof(VButton));
	if (p){
		memset(p, 0, sizeof(VButton));
		VanillaControl Control = VanillaControlCreate(ParentControl, Left, Top, Width, Height, p, NULL, Visible, Enabled, NULL);
		if (Control) {
			Control->CtlProc = &VanillaButtonProc;
			p->Status = 0;
			p->Style = 0;
			p->StringFormat = StringFormat;
			p->Title = new VanillaString(Title);
			Control->Alpha = 100;
			return Control;
		}
	}
	return NULL;
}

VAPI(VanillaVoid) VanillaButtonSetStyle_Metro(VanillaControl Control, VanillaColor ButtonColor [5]) {
	VanillaButton p = (VanillaButton)Control->ControlData;
	for (VanillaInt i = 0; i < 5; i++) {
		p->ButtonColor[i] = ButtonColor[i];
	}
	p->Style = VBS_METRO;
}

VAPI(VanillaInt) VanillaButtonGetStyle(VanillaControl Control) {
	return ((VanillaButton)(Control->ControlData))->Style;
}

VAPI(VanillaVoid) VanillaButtonSetTitle(VanillaControl Control, VanillaText Title) {
	VanillaButton p = (VanillaButton)Control->ControlData;
	if (p->Title)
	{
		delete p->Title;
	}
	p->Title = new VanillaString(Title);
}

VAPI(VanillaVoid) VanillaButtonSetStringFormat(VanillaControl Control, VanillaStringFormat StringFormat) {
	((VanillaButton)(Control->ControlData))->StringFormat = StringFormat;
}

VAPI(VanillaText) VanillaButtonGetTitle(VanillaControl Control) {
	return String2Text(((VanillaButton)(Control->ControlData))->Title);
}

