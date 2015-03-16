#ifndef __VGLOBALFUNCTION_H__
#define __VGLOBALFUNCTION_H__
// Normal Global Functions

VanillaBool VInit ();
VanillaBool VInit_WndMsg ();
VanillaBool VInit_WindowClass();
VanillaBool VInit_ControlClass();

VAPI(VanillaInt) VanillaMsgLoop();
VAPI(VanillaBool) VanillaInit();
VAPI(VanillaBin) VanillaReadFileToBin(VanillaText FileName);
VAPI(VanillaBool) VanillaIsPointInRect(VanillaPoint pt, VanillaRect Rect);
VAPI(VanillaBool) VanillaIntersectRect(VanillaRect Rect1, VanillaRect Rect2, VanillaRect RectResult);
#endif	//__VGLOBALFUNCTION_H__