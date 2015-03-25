#ifndef __VANILLA_CONTROLS_LABEL_H__
#define __VANILLA_CONTROLS_LABEL_H__
// Vanilla Label

#define VBS_METRO 1

typedef class VLabel *VanillaLabel;

class CLASS_IMPORT VLabel : public VControlBase
{
private:
	VanillaString* Title;
	VanillaStringFormat StringFormat;
public:
	VanillaBaseConstruct(VLabel)

	VanillaBool Init(VanillaControl ParentControl, VanillaRect Rect, VanillaBool Visible, VanillaBool Enabled);

	static VanillaControlClass Register();

	VanillaLabel Create(VanillaText Title, VanillaStringFormat StringFormat);
	VanillaLabel SetTitle(VanillaText Title);
	VanillaLabel SetStringFormat(VanillaStringFormat StringFormat);
	VanillaLabel SetStyle_Metro(VanillaColor LabelColor[5]);

	VanillaText GetTitle();
	VanillaInt GetStyle();
private:
	virtual VanillaText OnCreate();
	virtual VanillaVoid OnDestroy();
	virtual VanillaVoid OnPaint(VanillaGraphics Graphics);
};


VAPI(VanillaControl) VanillaLabelCreate(VanillaControl ParentControl, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaText Title, VanillaStringFormat StringFormat, VanillaBool Visible, VanillaBool Enabled);
VAPI(VanillaVoid) VanillaLabelSetTitle(VanillaControl Control, VanillaText Title);
VAPI(VanillaVoid) VanillaLabelSetStringFormat(VanillaControl Control, VanillaStringFormat StringFormat);
VAPI(VanillaText) VanillaLabelGetTitle(VanillaControl Control);
#endif	//__VANILLA_CONTROLS_LABEL_H__
