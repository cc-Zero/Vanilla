// Vanilla标签(控件)
#include "Include.h"


/*绘制边框*/
VanillaVoid OnPaint_Frame(VanillaEdit p)
{
	if (p->p->Window->FocusControl == p->p || p->p->Window->MouseInControl == p->p)
	{
		VanillaDrawRect(p->p->Graphics, p->FireColor, 0, 0, p->p->CRect.Width, p->p->CRect.Height, 1);
	}
	else
	{
		VanillaDrawRect(p->p->Graphics, p->FrameColor, 0, 0, p->p->CRect.Width, p->p->CRect.Height, 1);
	}
}
/*绘制背景*/
VanillaVoid OnPaint_Bkg(VanillaEdit p)
{
	OnPaint_Frame(p);
}
/*绘制字符串*/
VanillaVoid OnPaint_Str(VanillaEdit p)
{
	VanillaDrawString(p->p->Graphics, p->StringFormat, String2Text(p->String), &p->p->CRect);
}

/*标签控件的回调函数*/
VanillaInt VanillaEditProc(VanillaInt ID, VanillaInt Message, VanillaInt Param1, VanillaInt Param2) {
	VanillaControl Control = (VanillaControl)ID;
	VanillaEdit p = (VanillaEdit)Control->ControlData;
	switch (Message) {
	case VM_SETFOCUS:
		VanillaControlRedraw(Control, true);
		break;
	case VM_KILLFOCUS :
		VanillaControlRedraw(Control, true);
		break;
	case VM_MOUSEIN:
		VanillaControlRedraw(Control, true);
		break;
	case VM_MOUSEOUT:
		VanillaControlRedraw(Control, true);
		break;
	case VM_PAINT:
		OnPaint_Bkg(p);
		OnPaint_Str(p);
		break;
	}
	return NULL;
}
VAPI(VanillaControl) VanillaEditCreate(VanillaControl ParentControl, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaText String, VanillaStringFormat StringFormat, VanillaBool Visible, VanillaBool Enabled) {
	VanillaEdit p = (VanillaEdit)malloc(sizeof(VEdit));
	if (p){
		memset(p, 0, sizeof(VEdit));
		VanillaControl Control = VanillaControlCreate(ParentControl, Left, Top, Width, Height, p, NULL, Visible, Enabled, NULL);
		if (Control) {
			p->p = Control;
			Control->CtlProc = &VanillaEditProc;
			p->StringFormat = StringFormat;
			p->String = new VanillaString(String);
			p->FrameColor = ARGB(255, 0, 0, 0);
			p->FireColor = ARGB(255, 155, 155, 0);
			return Control;
		}
	}
	return NULL;
}

VAPI(VanillaVoid) VanillaEditSetStringFormat(VanillaControl Control, VanillaStringFormat StringFormat) {
	((VanillaEdit)(Control->ControlData))->StringFormat = StringFormat;
}
