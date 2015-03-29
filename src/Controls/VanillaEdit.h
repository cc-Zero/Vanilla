#ifndef __VANILLA_CONTROLS_EDIT_H__
#define __VANILLA_CONTROLS_EDIT_H__
// Vanilla Label

#define VBS_METRO 1

typedef struct VEdit
{
	VanillaControl p;//控件指针
	/*控件数据*/
	VanillaString* String;//文本
	VanillaStringFormat StringFormat;//字体格式
	VanillaColor FrameColor;//边框颜色
	VanillaColor FireColor;//点燃颜色

	VanillaTimer GradientTimer;		//光标时钟
} *VanillaEdit, _VEdit;

VAPI(VanillaControl) VanillaEditCreate(VanillaControl ParentControl, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaText String, VanillaStringFormat StringFormat, VanillaBool Visible, VanillaBool Enabled);
VAPI(VanillaVoid) VanillaEditSetStringFormat(VanillaControl Control, VanillaStringFormat StringFormat);
#endif	//__VANILLA_CONTROLS_EDIT_H__
