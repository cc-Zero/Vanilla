// Vanilla Normal Global Functions
#include "stdafx.h"
#include "VDefine.h"
#include "VStruct.h"
#include "VWindow.h"
#include "VControl.h"
#include "VPort.h"
#include "VGlobalFunction.h"
#include "Controls/VControlRegister.h"

VanillaBool VInit() {
	if (!VInit_ControlClass()) {
		return false;
	}
	if (!VanillaPortInitializeX()) {
		return false;
	}
	return true;
}

VanillaBool VInit_ControlClass()
{
    VanillaRegisterControls();
    VanillaRegisterControlClass("VanillaUI.WindowRootControl", &VanillaWindowDefaultControlsProc, false, true);
	return true;
}

VAPI(VanillaInt) VanillaMsgLoop() {
	return VanillaPortMessageLoop();
}

VAPI(VanillaBool) VanillaInit() {
	return VInit();
}

VAPI(VanillaBin) VanillaReadFileToBin(VanillaText FileName) {
	FILE* File = fopen(FileName, "rb");
	fseek(File, 0, SEEK_END);
	int Size = ftell(File);
	fseek(File, 0, SEEK_SET);
	VanillaByte* Buffer = new VanillaByte[Size];
	fread(Buffer, 1, Size, File);
	fclose(File);
	VanillaBin b = MALLOCVBIN();
	MAKEVBINP(b, Buffer, Size);
	return b;
}

VAPI(VanillaBool) VanillaIsPointInRect(VanillaInt x, VanillaInt y, VanillaRect Rect) {
	if (x <= Rect->Left) {
		return false;
	}
	if (x >= Rect->Width + Rect->Left) {
		return false;
	}

	if (y <= Rect->Top) {
		return false;
	}
	if (y >= Rect->Height + Rect->Top) {
		return false;
	}

	return true;
}

VAPI(VanillaBool) VanillaIntersectRect(VanillaRect Rect1, VanillaRect Rect2, VanillaRect RectResult) {
	RectResult->Left = std::max(Rect1->Left, Rect2->Left);
	RectResult->Top = std::max(Rect1->Top, Rect2->Top);
	RectResult->Width = std::min(Rect1->Left + Rect1->Width, Rect2->Left + Rect2->Width) - RectResult->Left;
	RectResult->Height = std::min(Rect1->Top + Rect1->Height, Rect2->Top + Rect2->Height) - RectResult->Top;
	return (RectResult->Width > 0) && (RectResult->Height > 0);
}
