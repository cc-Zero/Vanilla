#ifndef __VANILLA_CONTROLS_BUTTON_H__
#define __VANILLA_CONTROLS_BUTTON_H__
// Vanilla按钮(控件)

#define VBS_METRO 1
#define VBS_IMAGE 2

typedef struct VButton
{
	VanillaString* Title;
	VanillaColor ButtonColor[5];
	VanillaImage Image[5];
	VanillaStringFormat StringFormat;
	VanillaInt Status;
	VanillaInt Style;
} *VanillaButton, _VButton;

VAPI(VanillaControl) VanillaButtonCreate(VanillaControl ParentControl, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaText Title, VanillaStringFormat StringFormat, VanillaBool Visible, VanillaBool Enabled);
VAPI(VanillaVoid) VanillaButtonSetStyle_Metro(VanillaControl Control, VanillaColor ButtonColor [5]);
VAPI(VanillaInt) VanillaButtonGetStyle(VanillaControl Control);
VAPI(VanillaVoid) VanillaButtonSetTitle(VanillaControl Control, VanillaText Title);
VAPI(VanillaVoid) VanillaButtonSetStringFormat(VanillaControl Control, VanillaStringFormat StringFormat);
VAPI(VanillaText) VanillaButtonGetTitle(VanillaControl Control);
#endif	//__VANILLA_CONTROLS_BUTTON_H__

