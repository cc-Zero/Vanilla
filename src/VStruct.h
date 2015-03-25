#ifndef __VANILLA_CORE_STRUCT_H__
#define __VANILLA_CORE_STRUCT_H__
// Vanilla Structs' Define

typedef struct VRect
{
	VanillaInt Left;//左边
	VanillaInt Top;//顶边
	VanillaInt Width;//宽度
	VanillaInt Height;//高度
} *VanillaRect, _VRect;
#define MAKEVRECT(r, L, T, W, H) \
	r.Left = L; \
	r.Top = T;\
	r.Width = W;\
	r.Height = H;
#define MAKEVRECTP(r, L, T, W, H) \
	r->Left = L; \
	r->Top = T;\
	r->Width = W;\
	r->Height = H;
typedef struct VPoint
{
	VanillaInt x;
	VanillaInt y;
} *VanillaPoint, _VPoint;
#define MAKEVPOINT(p, x1, y1) \
	p.x = x1; \
	p.y = y1;
#define MAKEVPOINTP(p, x1, y1) \
	p->x = x1; \
	p->y = y1;
typedef struct VSize
{
	VanillaInt Width;//宽度
	VanillaInt Height;//高度
} *VanillaSize, _VSize;
#define MAKEVSIZE(s, W, H) \
	s.Width = W; \
	s.Height = H;
#define MAKEVSIZEP(s, W, H) \
	s->Width = W; \
	s->Height = H;
typedef struct VRectR
{
	VanillaReal Left;
	VanillaReal Top;
	VanillaReal Width;
	VanillaReal Height;
} *VanillaRectR, _VRectR;

typedef struct VPointR
{
	VanillaReal x;
	VanillaReal y;
} *VanillaPointR, _VPointR;

typedef struct VSizeR
{
	VanillaReal Width;
	VanillaReal Height;
} *VanillaSizeR, _VSizeR;


typedef struct VBin
{
	VanillaByte* Address;
	VanillaInt Length;
} *VanillaBin, _VBin;
#define MAKEVBIN(b, ptr, len) \
	b.Address = ptr; \
	b.Length = len;
#define MAKEVBINP(b, ptr, len) \
	b->Address = ptr; \
	b->Length = len; 
#define MALLOCVBIN() ((VanillaBin)malloc(sizeof(VBin)))
#define FREEVBIN(b) \
	free(b);
#endif	//__VANILLA_CORE_STRUCT_H__
