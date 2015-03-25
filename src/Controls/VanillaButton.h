#ifndef __VANILLA_CONTROLS_BUTTON_H__
#define __VANILLA_CONTROLS_BUTTON_H__
// Vanilla按钮(控件)

#define VBS_METRO 1
#define VBS_IMAGE 2


typedef class VButton *VanillaButton;

class CLASS_IMPORT VButton : public VControlBase
{
private:
	VanillaString* Title;
	VanillaColor ButtonColor[5];
	VanillaImage Image[5];
	VanillaStringFormat StringFormat;
	VanillaInt Status;
	VanillaInt Style;
public:
	VanillaBaseConstruct(VButton);

	static VanillaControlClass Register();

	VanillaButton Create(VanillaText Title, VanillaStringFormat StringFormat);
	VanillaButton SetTitle(VanillaText Title);
	VanillaButton SetStringFormat(VanillaStringFormat StringFormat);
	VanillaButton SetStyle_Metro(VanillaColor ButtonColor[5]);
	VanillaButton SetStyle_Image(VanillaImage Image[5]);

	VanillaText GetTitle();
	VanillaInt GetStyle();
private:
	virtual VanillaText OnCreate();
	virtual VanillaVoid OnDestroy();
	virtual VanillaVoid OnMouseLDown(VanillaPoint Point);
	virtual VanillaVoid OnMouseLUp(VanillaPoint Point);
	virtual VanillaVoid OnMouseIn(VanillaControl OldControl, VanillaPoint Point);
	virtual VanillaVoid OnMouseOut(VanillaControl NewControl);
	virtual VanillaVoid OnSetFocus(VanillaControl OldControl);
	virtual VanillaVoid OnKillFocus(VanillaControl NewControl);
	virtual VanillaVoid OnPaint(VanillaGraphics Graphics);
};


VAPI(VanillaControl) VanillaButtonCreate(VanillaControl ParentControl, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaText Title, VanillaStringFormat StringFormat, VanillaBool Visible, VanillaBool Enabled);
VAPI(VanillaVoid) VanillaButtonSetStyle_Metro(VanillaControl Control, VanillaColor ButtonColor [5]);
VAPI(VanillaInt) VanillaButtonGetStyle(VanillaControl Control);
VAPI(VanillaVoid) VanillaButtonSetTitle(VanillaControl Control, VanillaText Title);
VAPI(VanillaVoid) VanillaButtonSetStringFormat(VanillaControl Control, VanillaStringFormat StringFormat);
VAPI(VanillaText) VanillaButtonGetTitle(VanillaControl Control);
#endif	//__VANILLA_CONTROLS_BUTTON_H__
