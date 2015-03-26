#ifndef __VANILLA_CONTROLS_LABEL_H__
#define __VANILLA_CONTROLS_LABEL_H__
// Vanilla Label

#define VBS_METRO 1

typedef struct VLabel
{
	VanillaString* Title;
	VanillaStringFormat StringFormat;
} *VanillaLabel, _VLabel;

VAPI(VanillaControl) VanillaLabelCreate(VanillaControl ParentControl, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaText Title, VanillaStringFormat StringFormat, VanillaBool Visible, VanillaBool Enabled);
VAPI(VanillaVoid) VanillaLabelSetTitle(VanillaControl Control, VanillaText Title);
VAPI(VanillaVoid) VanillaLabelSetStringFormat(VanillaControl Control, VanillaStringFormat StringFormat);
VAPI(VanillaText) VanillaLabelGetTitle(VanillaControl Control);
#endif	//__VANILLA_CONTROLS_LABEL_H__
