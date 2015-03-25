// Vanilla按钮(控件)
#include "Include.h"

#define BUTTON_STATUS_NORMAL 0
#define BUTTON_STATUS_HOVER  1
#define BUTTON_STATUS_CLICK  2

#define BUTTON_CLASS "Vanilla.Button"


VanillaText VButton::OnCreate() {
	this->Status = 0;
	this->Style = 0;
	this->StringFormat = NULL;
	this->Title = NULL;
	return BUTTON_CLASS;
}

VanillaButton VButton::Create(VanillaText Title, VanillaStringFormat StringFormat) {
	this
		->SetTitle(Title)
		->SetStringFormat(StringFormat);
	return this;
}

VanillaButton VButton::SetTitle(VanillaText Title) {
	if (this->Title) {
		delete this->Title;
	}
	this->Title = new VanillaString(Title);
	return this;
}

VanillaButton VButton::SetStringFormat(VanillaStringFormat StringFormat) {
	this->StringFormat = StringFormat;
	return this;
}

VanillaButton VButton::SetStyle_Metro(VanillaColor Color [5]) {
	for (VanillaInt i = 0; i < 5; i++) {
		this->ButtonColor[i] = Color [i];
	}
	this->Style = VBS_METRO;
	return this;
}

VanillaButton VButton::SetStyle_Image(VanillaImage Image[5]) {
	for (VanillaInt i = 0; i < 5; i++) {
		if (Image[i]) {
			this->Image[i] = Image[i];
		} else {
			this->Image[i] = Image[0];
		}
	}
	this->Style = VBS_IMAGE;
	return this;
}

VanillaText VButton::GetTitle() {
	return String2Text(this->Title);
}

VanillaInt VButton::GetStyle() {
	return this->Style;
}

VanillaVoid VButton::OnSetFocus(VanillaControl OldControl) {
	this->Redraw(true);
}

VanillaVoid VButton::OnKillFocus(VanillaControl OldControl) {
	this->Redraw(true);
}

VanillaVoid VButton::OnMouseIn(VanillaControl OldControl, VanillaPoint Point) {
	this->Status = BUTTON_STATUS_HOVER;
	this->Redraw(true);
}

VanillaVoid VButton::OnMouseOut(VanillaControl OldControl) {
	this->Status = BUTTON_STATUS_NORMAL;
	this->Redraw(true);
}

VanillaVoid VButton::OnMouseLDown(VanillaPoint Point) {
	this->Status = BUTTON_STATUS_CLICK;
	this->Redraw(true);
}

VanillaVoid VButton::OnMouseLUp(VanillaPoint Point) {
	this->Status = BUTTON_STATUS_HOVER;
	this->Redraw(true);
}

VanillaVoid VButton::OnPaint(VanillaGraphics Graphics) {
	VRect Rect;
	this->GetFrameRect(&Rect);
	switch (this->Style) {
	case VBS_METRO:
		VanillaFillRect(Graphics, this->ButtonColor[VanillaControlIsEnable(Control) ? this->Status : 3], 0, 0, Rect.Width, Rect.Height);
		VanillaDrawString(Graphics, this->StringFormat, String2Text(this->Title), &Rect);
		if (Control->Window->FocusControl == Control) {
			VanillaDrawRect(Graphics, this->ButtonColor[4], 1, 1, Rect.Width - 1, Rect.Height - 1, 2);
		}
		else {
			VanillaDrawRect(Graphics, this->ButtonColor[4], 0, 0, Rect.Width, Rect.Height, 1);
		}
	}
}

VanillaVoid VButton::OnDestroy() {
	if (this->Title) {
		delete this->Title;
		this->Title = NULL;
	}
	this->Status = 0;
	this->Style = 0;
	this->StringFormat = NULL;
	for (VanillaInt i = 0; i < 5; i++) {
		this->ButtonColor [i] = 0;
	}
}

VanillaControlClass VButton::Register() {
	return BaseRegister(BUTTON_CLASS);
}


VAPI(VanillaControl) VanillaButtonCreate(VanillaControl ParentControl, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaText Title, VanillaStringFormat StringFormat, VanillaBool Visible, VanillaBool Enabled) {
	VanillaButton Button = new VButton(ParentControl, Left, Top, Width, Height, Visible, Enabled);
	Button->Create(Title, StringFormat);
	return Button->Control;
}

VAPI(VanillaVoid) VanillaButtonSetStyle_Metro(VanillaControl Control, VanillaColor ButtonColor [5]) {
	((VanillaButton)Control->ControlData)->SetStyle_Metro(ButtonColor);
}

VAPI(VanillaInt) VanillaButtonGetStyle(VanillaControl Control) {
	return ((VanillaButton)Control->ControlData)->GetStyle();
}

VAPI(VanillaVoid) VanillaButtonSetTitle(VanillaControl Control, VanillaText Title) {
	((VanillaButton)Control->ControlData)->SetTitle(Title);
}

VAPI(VanillaVoid) VanillaButtonSetStringFormat(VanillaControl Control, VanillaStringFormat StringFormat) {
	((VanillaButton)Control->ControlData)->SetStringFormat(StringFormat);
}

VAPI(VanillaText) VanillaButtonGetTitle(VanillaControl Control) {
	return ((VanillaButton)Control->ControlData)->GetTitle();
}
