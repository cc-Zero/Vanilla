#ifndef __VANILLA_CORE_DEFINE_H__
#define __VANILLA_CORE_DEFINE_H__
// Vanilla Defines

#include <stdint.h>
#define DEBUG_BORDER 0
#define DEBUG_PUTPNG(BITMAP, FILE) SkASSERT(SkImageEncoder::EncodeFile(FILE, BITMAP, SkImageEncoder::kPNG_Type, 100));


/*编译配置*/
//渲染后端
#define DRAW_SKIA
//#define DRAW_AGG
//#define DRAW_GDI
//#define DRAW_CAIRO


#ifdef WIN32

#define VAPI(Type) extern "C" __declspec(dllexport) Type __stdcall
#define CLASS_IMPORT __declspec(dllexport)

#else

#define VAPI(Type) extern "C" Type __attribute__((__stdcall__))
#define CLASS_IMPORT

#endif

#define String2Text(String) (VanillaText)String->c_str()

#define ARGB(A,R,G,B) ((uint32_t)((((A)&0xff)<<24)|(((R)&0xff)<<16)|(((G)&0xff)<<8)|((B)&0xff)))
#define RGBA(R,G,B,A) ARGB(A,R,G,B)
#define RGB2ARGB(COLOR,A) RGBA(((COLOR) >> 16 & 0xFF), ((COLOR) >> 8 & 0xFF), ((COLOR) & 0xFF), (A))
#ifdef LINUX
#define RGB(r,g,b) ((VanillaInt)(((VanillaByte)(r)|((short)((VanillaByte)(g))<<8))|(((VanillaInt)(VanillaByte)(b))<<16)))
#endif


typedef const char* VanillaText;

typedef std::string VanillaString;

typedef float VanillaReal;

typedef int32_t VanillaInt;

typedef bool VanillaBool;

typedef void VanillaVoid;

typedef void* VanillaAny;

typedef unsigned VanillaByte;

typedef struct VRect *VanillaRect, _VRect;

typedef struct VPoint *VanillaPoint, _VPoint;

typedef struct VSize *VanillaSize, _VSize;

typedef struct VRectR *VanillaRectR, _VRectR;

typedef struct VPointR *VanillaPointR, _VPointR;

typedef struct VSizeR *VanillaSizeR, _VSizeR;

typedef struct VImage *VanillaImage, _VImage;

typedef uint32_t VanillaColor;

typedef struct VGraphics  *VanillaGraphics, _VGraphics;

typedef struct VStringFormat *VanillaStringFormat, _VStringFromat;

typedef struct VControl  *VanillaControl, _VControl;

typedef struct VSkinDB *VanillaSkinDB, _VSkinDB;

typedef struct VWindow *VanillaWindow, _VWindow;

typedef struct VPortWindow *VanillaPortWindow, _VPortWindow;

typedef UINT_PTR VanillaTimer;

typedef struct VPortGraphics *VanillaPortGraphics, _VPortGraphics;

typedef struct VBin *VanillaBin, _VBin;

typedef struct VHashTableItem *VanillaHashTableItem, _VHashTableItem;

typedef std::map<VanillaString, VanillaBin> VHashTable;

typedef std::map<VanillaString, VanillaBin>* VanillaHashTable;

typedef struct VHashTableTraveler *VanillaHashTableTraveler, _VHashTableTraveler;

typedef struct VTask *VanillaTask, _VTask;

typedef struct VTaskQueue *VanillaTaskQueue, _VTaskQueue;

typedef int(*VCtlProc)(VanillaInt ID, VanillaInt Message, VanillaInt Param1, VanillaInt Param2);

typedef int(*VCtlEventProc)(VanillaWindow Window, VanillaControl Control, VanillaInt Event, VanillaInt Param1, VanillaInt Param2, VanillaInt Param3);
#endif	//__VANILLA_CORE_DEFINE_H__
