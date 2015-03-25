// Vanilla标签(控件)
#include "Include.h"

#define LABEL_CLASS "Vanilla.Label"


VanillaText VLabel::OnCreate() {
	this->StringFormat = NULL;
	this->Title = NULL;
	return LABEL_CLASS;
}

VanillaLabel VLabel::Create(VanillaText Title, VanillaStringFormat StringFormat) {
	this
		->SetTitle(Title)
		->SetStringFormat(StringFormat);
	return this;
}

VanillaLabel VLabel::SetTitle(VanillaText Title) {
	if (this->Title) {
		delete this->Title;
	}
	this->Title = new VanillaString(Title);
	return this;
}

VanillaLabel VLabel::SetStringFormat(VanillaStringFormat StringFormat) {
	this->StringFormat = StringFormat;
	return this;
}

VanillaText VLabel::GetTitle() {
	return String2Text(this->Title);
}

VanillaVoid VLabel::OnPaint(VanillaGraphics Graphics) {
	VRect Rect;
	this->GetFrameRect(&Rect);
	VanillaDrawString(Graphics, this->StringFormat, String2Text(this->Title), &Rect);
}

VanillaVoid VLabel::OnDestroy() {
	if (this->Title) {
		delete this->Title;
		this->Title = NULL;
	}
	this->StringFormat = NULL;
}

VanillaControlClass VLabel::Register() {
	return BaseRegister(LABEL_CLASS, true);
}


VAPI(VanillaControl) VanillaLabelCreate(VanillaControl ParentControl, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaText Title, VanillaStringFormat StringFormat, VanillaBool Visible, VanillaBool Enabled) {
	VanillaLabel Label = new VLabel(ParentControl, Left,  Top, Width, Height, Visible, Enabled);
	Label->Create(Title, StringFormat);
	return Label->Control;
}

VAPI(VanillaVoid) VanillaLabelSetTitle(VanillaControl Control, VanillaText Title) {
	((VanillaLabel)Control->ControlData)->SetTitle(Title);
}

VAPI(VanillaVoid) VanillaLabelSetStringFormat(VanillaControl Control, VanillaStringFormat StringFormat) {
	((VanillaLabel)Control->ControlData)->SetStringFormat(StringFormat);
}

VAPI(VanillaText) VanillaLabelGetTitle(VanillaControl Control) {
	return ((VanillaLabel)Control->ControlData)->GetTitle();
}

