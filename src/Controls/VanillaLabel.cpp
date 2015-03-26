// Vanilla标签(控件)
#include "Include.h"

/*标签控件的回调函数*/
VanillaInt VanillaLabelProc(VanillaInt ID, VanillaInt Message, VanillaInt Param1, VanillaInt Param2) {
	VanillaControl Control = (VanillaControl)ID;
	VanillaLabel p = (VanillaLabel)Control->ControlData;
	switch (Message) {
	case VM_PAINT:
		VanillaDrawString(Control->Graphics, p->StringFormat, String2Text(p->Title), &Control->CRect);
		break;
	}
	return NULL;
}
VAPI(VanillaControl) VanillaLabelCreate(VanillaControl ParentControl, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaText Title, VanillaStringFormat StringFormat, VanillaBool Visible, VanillaBool Enabled) {
	VanillaLabel p = (VanillaLabel)malloc(sizeof(VLabel));
	if (p){
		memset(p, 0, sizeof(VLabel));
		VanillaControl Control = VanillaControlCreate(ParentControl, Left, Top, Width, Height, p, NULL, Visible, Enabled, NULL);
		if (Control) {
			Control->CtlProc = &VanillaLabelProc;
			p->StringFormat = StringFormat;
			p->Title = new VanillaString(Title);
			return Control;
		}
	}
	return NULL;
}

VAPI(VanillaVoid) VanillaLabelSetTitle(VanillaControl Control, VanillaText Title) {
	VanillaLabel p = (VanillaLabel)Control->ControlData;
	if (p->Title)
	{
		delete p->Title;
	}
	p->Title = new VanillaString(Title);
}

VAPI(VanillaVoid) VanillaLabelSetStringFormat(VanillaControl Control, VanillaStringFormat StringFormat) {
	((VanillaLabel)(Control->ControlData))->StringFormat = StringFormat;
}

VAPI(VanillaText) VanillaLabelGetTitle(VanillaControl Control) {
	return String2Text(((VanillaLabel)(Control->ControlData))->Title);
}

