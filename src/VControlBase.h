#ifndef __VANILLA_CORE_CONTROLBASE_H__
#define __VANILLA_CORE_CONTROLBASE_H__
// Vanilla Control Base Class

#ifdef SendMessage
#undef SendMessage
#endif

#define VanillaBaseConstruct(Class) \
	Class() { \
		; \
	} \
	Class(VanillaControl ParentControl, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaBool Visible = true, VanillaBool Enabled = true) { \
		VControlBase::Init(ParentControl, Left, Top, Width, Height, Visible, Enabled); \
	} \
	Class(VanillaWindow ParentWindow, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaBool Visible = true, VanillaBool Enabled = true) {\
		VControlBase::Init(ParentWindow->RootControl.Control, Left, Top, Width, Height, Visible, Enabled); \
	}

typedef class VControlBase *VanillaControlBase;

class CLASS_IMPORT VControlBase
{
	friend VanillaInt VanillaBasedControlProc(VanillaInt ID, VanillaInt Message, VanillaInt Param1, VanillaInt Param2);
private:
	/*回调函数*/
	VanillaInt CtlProc(VanillaInt Message, VanillaInt Param1, VanillaInt Param2);
	VanillaBool BaseCreate(VanillaText ClassName, VanillaControl ParentControl, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaBool Visible = true, VanillaBool Enabled = true);
protected:
	static VanillaControlClass BaseRegister(VanillaText ClassName, VanillaBool Focusable = true, VanillaBool Virtual = false);
public:
	VanillaControl Control;
	~VControlBase();
	VanillaBool Init(VanillaControl ParentControl, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaBool Visible = true, VanillaBool Enabled = true);
	VanillaInt Destroy();
	VanillaVoid Enabled(VanillaBool Enabled);
	VanillaBool Enabled();
	VanillaVoid Visible(VanillaBool Visible);
	VanillaBool Visible();
	VanillaVoid Alpha(VanillaByte Alpha);
	VanillaByte Alpha();
	VanillaVoid EventProc(VCtlEventProc EventProc);
	VCtlEventProc EventProc();
	VanillaInt SendMessage(VanillaInt Message, VanillaInt Param1 = NULL, VanillaInt Param2 = NULL);
	VanillaVoid SendMessageToChild(VanillaInt Message, VanillaInt Param1 = NULL, VanillaInt Param2 = NULL);
	VanillaVoid Move(VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height);
	VanillaVoid Move(VanillaRect NewRect);
	VanillaVoid Move(VanillaPoint NewPoint);
	VanillaVoid Move(VanillaSize NewSize);
	VanillaVoid GetFrameRect(VRect *r);
	VanillaRect GetRect();
	VanillaVoid Left(VanillaInt NewLeft);
	VanillaInt Left();
	VanillaVoid Top(VanillaInt NewTop);
	VanillaInt Top();
	VanillaVoid Width(VanillaInt NewWidth);
	VanillaInt Width();
	VanillaVoid Height(VanillaInt NewHeight);
	VanillaInt Height();
	VanillaVoid Redraw(VanillaBool Update = false);
	operator VanillaControl();
private:
	virtual VanillaText OnCreate() = 0;
	virtual VanillaVoid OnDestroy(){}

	virtual VanillaVoid OnMouseMove(VanillaPoint Point){}
	virtual VanillaVoid OnMouseMDown(VanillaPoint Point){}
	virtual VanillaVoid OnMouseMUp(VanillaPoint Point){}
	virtual VanillaVoid OnMouseMDbClick(VanillaPoint Point){}
	virtual VanillaVoid OnMouseMClick(VanillaPoint Point){}
	virtual VanillaVoid OnMouseLDown(VanillaPoint Point){}
	virtual VanillaVoid OnMouseLUp(VanillaPoint Point){}
	virtual VanillaVoid OnMouseLDbClick(VanillaPoint Point){}
	virtual VanillaVoid OnMouseLClick(VanillaPoint Point){}
	virtual VanillaVoid OnMouseRDown(VanillaPoint Point){}
	virtual VanillaVoid OnMouseRUp(VanillaPoint Point){}
	virtual VanillaVoid OnMouseRDbClick(VanillaPoint Point){}
	virtual VanillaVoid OnMouseRClick(VanillaPoint Point){}
	virtual VanillaVoid OnMouseIn(VanillaControl OldControl, VanillaPoint Point){}
	virtual VanillaVoid OnMouseOut(VanillaControl NewControl){}
	virtual VanillaVoid OnMouseWheel(VanillaInt Wheel){}

	virtual VanillaVoid OnSetParent(VanillaControl NewParent){}
	virtual VanillaVoid OnSetFocus(VanillaControl OldControl){}
	virtual VanillaVoid OnKillFocus(VanillaControl NewControl){}
	virtual VanillaVoid OnSetEnabled(VanillaBool Enabled){}
	virtual VanillaVoid OnSetVisible(VanillaBool Visible){}
	virtual VanillaVoid OnMove(VanillaPoint OldPoint){}
	virtual VanillaVoid OnSize(VanillaSize OldSize){}

	virtual VanillaVoid OnChar(VanillaText Char){}
	virtual VanillaVoid OnKeyDown(VanillaByte KeyCode){}
	virtual VanillaVoid OnKeyUp(VanillaByte KeyCode){}

	virtual VanillaVoid OnPaint(VanillaGraphics Graphics){}
};

VanillaInt VanillaBasedControlProc(VanillaInt ID, VanillaInt Message, VanillaInt Param1, VanillaInt Param2);
#endif	//__VANILLA_CORE_CONTROLBASE_H__
