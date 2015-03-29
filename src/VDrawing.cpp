// Vanilla Drawing Functions
#include "stdafx.h"
#include "VDefine.h"
#include "VStruct.h"
#include "VPort.h"
#include "VDrawing.h"
#include "VWindow.h"

#ifdef DRAW_SKIA
	SkMaskFilter* VStringFormat::BlurMaskFilter = SkBlurMaskFilter::Create(SkBlurStyle::kOuter_SkBlurStyle, 3, SkBlurMaskFilter::BlurFlags::kNone_BlurFlag);
#elif defined DRAW_GDI
typedef struct
{
	int GdiplusVersion;
	int DebugEventCallback;
	int SuppressBackgroundThreadc;
	int SuppressExternalCodecs;
}GdiplusStartupInput;
#ifdef __cplusplus
extern "C" {
#endif
int __stdcall GdiplusStartup(int *token, GdiplusStartupInput *input, void *output);
int __stdcall GdipCreateFromHDC(HDC hdc, void **graphics);
int __stdcall GdipDeleteGraphics(void *graphics);
int __stdcall GdipCreateSolidFill(VanillaColor color, void **brush);
int __stdcall GdipFillRectangleI(void *graphics, void *brush, int x, int y, int width, int height);
int __stdcall GdipDeleteBrush(void *brush);
int __stdcall GdipDrawRectangleI(void *graphics, void *pen, int x, int y, int width, int height);
int __stdcall GdipCreatePen1(VanillaColor color, VanillaReal width, int unit, void **pen);
int __stdcall GdipDeletePen(void *pen);
int __stdcall GdipDrawString(void *graphics, VanillaText string, int length, void *font, VanillaRectR layoutRect, void *stringFormat, void *brush);
int __stdcall GdipDrawImageI(void *graphics, void *image, int x, int y);
int __stdcall GdipGraphicsClear(void *graphics, VanillaColor color);
int __stdcall GdipCreateFontFamilyFromName(VanillaText name, void *fontCollection, void **fontFamily);
int __stdcall GdipDeleteFontFamily(void *fontFamily);
int __stdcall GdipCreateFont(void *fontFamily, VanillaReal emSize, int style, int unit, void **font);
int __stdcall GdipCreateStringFormat(int formatAttributes, int language, void **StringFormat);
int __stdcall GdipSetStringFormatAlign(void *format, int align);
int __stdcall GdipSetStringFormatLineAlign(void *format, int align);

int __stdcall GdipDeleteFont(void* font);
int __stdcall GdipLoadImageFromStream(void* stream, void **image);
int __stdcall GdipLoadImageFromFile(VanillaText filename, void **image);
int __stdcall GdipGetImageWidth(void *image, int *width);
int __stdcall GdipGetImageHeight(void *image, int *height);
int __stdcall GdipDisposeImage(void *image);
int __stdcall GdipDrawImageRectRectI(void *graphics, void *image, int dstx, int dsty, int dstwidth, int dstheight, int srcx, int srcy, int srcwidth, int srcheight, int srcUnit, void* imageAttributes, void *callback, void * callbackData);
int __stdcall CreateStreamOnHGlobal(HGLOBAL hGlobal, BOOL fDeleteOnRelease, void **ppstm);
#ifdef __cplusplus
}
#endif
#endif

VanillaBool VanillaInitDrawing(){
#ifdef DRAW_SKIA

#elif defined DRAW_GDI
	GdiplusStartupInput gdiplusStartupInput;
	int pGdiToken;
	memset(&gdiplusStartupInput, 0, sizeof(GdiplusStartupInput));
	gdiplusStartupInput.GdiplusVersion = 1;
	GdiplusStartup(&pGdiToken, &gdiplusStartupInput, NULL);//初始化GDI+
#endif
	return true;
}

VAPI(VanillaVoid) VanillaFillRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height) {
#ifdef DRAW_SKIA
	SkIRect Rect = SkIRect::MakeXYWH(Top, Left, Width, Height);
	Graphics->Paint.reset();
	Graphics->Paint.setColor(Color);
	Graphics->Paint.setStyle(SkPaint::kFill_Style);
	Graphics->Canvas.drawIRect(Rect, Graphics->Paint);
#elif defined DRAW_GDI
	void *Brush;
	GdipCreateSolidFill(Color, &Brush);
	GdipFillRectangleI(Graphics->graphics, Brush, Left, Top, Width, Height);
	GdipDeleteBrush(Brush);
#endif
}

VAPI(VanillaVoid) VanillaDrawFilledRect(VanillaGraphics Graphics, VanillaColor Color, VanillaColor BorderColor, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth) {
#ifdef DRAW_SKIA
	SkIRect Rect = SkIRect::MakeXYWH(Top, Left, Width, Height);
	Graphics->Paint.reset();
	Graphics->Paint.setColor(Color);
	Graphics->Paint.setStyle(SkPaint::kFill_Style);
	Graphics->Canvas.drawIRect(Rect, Graphics->Paint);

	Rect = SkIRect::MakeXYWH(Top, Left, Width, Height);
	Graphics->Paint.setStrokeWidth((VanillaReal)LineWidth);
	Graphics->Paint.setColor(BorderColor);
	Graphics->Paint.setStyle(SkPaint::kStroke_Style);
	Graphics->Canvas.drawIRect(Rect, Graphics->Paint);
#elif defined DRAW_GDI

#endif
}

VAPI(VanillaVoid) VanillaDrawRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth) {
#ifdef DRAW_SKIA
	SkIRect Rect = SkIRect::MakeXYWH(Top, Left, Width, Height);
	Graphics->Paint.reset();
	Graphics->Paint.setStrokeWidth((VanillaReal)LineWidth);
	Graphics->Paint.setColor(Color);
	Graphics->Paint.setStyle(SkPaint::kStroke_Style);
	Graphics->Canvas.drawIRect(Rect, Graphics->Paint);
#elif defined DRAW_GDI
	void *pen;
	GdipCreatePen1(Color, (VanillaReal)LineWidth, 2, &pen);
	GdipDrawRectangleI(Graphics->graphics, pen, Left, Top, Width, Height);
	GdipDeletePen(pen);
#endif
}

#ifdef DRAW_SKIA
VAPI(SkTextBox*) VanillaCreateTextBox(VanillaGraphics Graphics, VanillaStringFormat StringFormat, VanillaText String, VanillaRectR Rect) {

	SkTextBox* TextBox = new SkTextBox;
	TextBox->setText(String, strlen(String), Graphics->Paint);
	TextBox->setSpacingAlign(StringFormat->VAlign);
	TextBox->setBox(Rect->Left, Rect->Top, Rect->Width, Rect->Height);
	TextBox->setMode(StringFormat->LineMode);
	return TextBox;
}
#endif

VAPI(VanillaVoid) VanillaDrawString(VanillaGraphics Graphics, VanillaStringFormat StringFormat, VanillaText String, VanillaRect Rect) {
#ifdef DRAW_SKIA
	VRectR RectD;
	if (Rect == NULL) {
		MAKEVRECT(RectD, 0, 0, (VanillaReal)Graphics->Width, (VanillaReal)Graphics->Height);
	} else {
		MAKEVRECT(RectD, (VanillaReal)Rect->Left, (VanillaReal)Rect->Top, (VanillaReal)Rect->Width - 1, (VanillaReal)Rect->Height);
	}
	Graphics->Paint.reset();
	Graphics->Paint.setTypeface(StringFormat->Typeface);
	Graphics->Paint.setTextSize(StringFormat->Size);
	Graphics->Paint.setAntiAlias(true);
	Graphics->Paint.setLCDRenderText(false);
	Graphics->Paint.setColor(StringFormat->TextColor);
	Graphics->Paint.setTextAlign(StringFormat->Align);
	SkTextBox* TextBox = VanillaCreateTextBox(Graphics, StringFormat, String, &RectD);
	TextBox->draw(&Graphics->Canvas);
	if (StringFormat->Style & VTS_SHADOW) {
		Graphics->Paint.setMaskFilter(StringFormat->BlurMaskFilter);
		Graphics->Paint.setColor(StringFormat->ShadowColor);
		TextBox->draw(&Graphics->Canvas);
	}
	delete TextBox;
#elif defined DRAW_GDI
	VRectR r;
	MAKEVRECT(r, (VanillaReal)Rect->Left, (VanillaReal)Rect->Top, (VanillaReal)Rect->Width, (VanillaReal)Rect->Height);
	GdipDrawString(Graphics->graphics, String, wcslen((wchar_t *)String), StringFormat->font, &r, StringFormat->StringFormat, StringFormat->Brush);
#endif

}
/*
VAPI(VanillaVoid) VanillaCreateRoundRectPath(VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaReal Round, SkPath* Path) {
#ifdef DRAW_SKIA
	Path->addArc(SkRect::MakeXYWH((VanillaReal)x, (VanillaReal)y, Round, Round), 180, 90);
	Path->addArc(SkRect::MakeXYWH((VanillaReal)(x + Width - Round), (VanillaReal)y, Round, Round), 270, 90);
	Path->addArc(SkRect::MakeXYWH((VanillaReal)(x + Width - Round), (VanillaReal)(y + Height - Round), Round, Round), 0, 90);
	Path->addArc(SkRect::MakeXYWH((VanillaReal)x, (VanillaReal)(y + Height - Round), Round, Round), 90, 90);
	Path->close();
#elif defined DRAW_GDI

#endif
}
*/
VAPI(VanillaVoid) VanillaDrawFilledRoundRect(VanillaGraphics Graphics, VanillaColor Color, VanillaColor BorderColor, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth, VanillaReal Round) {
#ifdef DRAW_SKIA
	if (Round <= 0){
		VanillaDrawFilledRect(Graphics, Color, BorderColor, x, y, Width, Height, LineWidth);
		return;
	}

	SkRect Rect = SkRect::MakeXYWH((VanillaReal)(x), (VanillaReal)(y), (VanillaReal)(Width), (VanillaReal)(Height));
	Graphics->Paint.reset();
	Graphics->Paint.setColor(Color);
	Graphics->Paint.setStyle(SkPaint::kFill_Style);
	Graphics->Canvas.drawRoundRect(Rect, Round, Round, Graphics->Paint);

	Graphics->Paint.setStrokeWidth((VanillaReal)LineWidth);
	Graphics->Paint.setColor(BorderColor);
	Graphics->Paint.setStyle(SkPaint::kStroke_Style);
	Graphics->Canvas.drawRoundRect(Rect, Round, Round, Graphics->Paint);
#elif defined DRAW_GDI

#endif
}

VAPI(VanillaVoid) VanillaDrawRoundRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth, VanillaReal Round) {
	if (Round <= 0){
		VanillaDrawRect(Graphics, Color, x, y, Width, Height, LineWidth);
		return;
	}
#ifdef DRAW_SKIA
	SkRect Rect = SkRect::MakeXYWH((VanillaReal)(x), (VanillaReal)(y), (VanillaReal)(Width), (VanillaReal)(Height));
	Graphics->Paint.reset();
	Graphics->Paint.setStrokeWidth((VanillaReal)LineWidth);
	Graphics->Paint.setColor(Color);
	Graphics->Paint.setStyle(SkPaint::kStroke_Style);
	Graphics->Canvas.drawRoundRect(Rect, Round, Round, Graphics->Paint);
#elif defined DRAW_GDI

#endif

}

VAPI(VanillaVoid) VanillaFillRoundRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaReal Round) {
#ifdef DRAW_SKIA
	if (Round <= 0){
		VanillaFillRect(Graphics, Color, x, y, Width, Height);
		return;
	}
	SkRect Rect = SkRect::MakeXYWH((VanillaReal)x, (VanillaReal)y, (VanillaReal)Width, (VanillaReal)Height);
	Graphics->Paint.reset();
	Graphics->Paint.setColor(Color);
	Graphics->Paint.setStyle(SkPaint::kFill_Style);
	Graphics->Canvas.drawRoundRect(Rect, Round, Round, Graphics->Paint);
#elif defined DRAW_GDI

#endif
}

VAPI(VanillaVoid) VanillaDrawImage(VanillaGraphics Graphics, VanillaImage Image, VanillaInt x, VanillaInt y) {
	if (!(Graphics && Image)) {
		return;
}
#ifdef DRAW_SKIA
	Graphics->Canvas.drawBitmap(Image->Bitmap, (VanillaReal)(x), (VanillaReal)(y), NULL);
#elif defined DRAW_GDI
	GdipDrawImageI(Graphics->graphics, Image->image, x, y);
#endif

}

VAPI(VanillaVoid) VanillaDrawImageEx(VanillaGraphics Graphics, VanillaImage Image, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaInt xSrc, VanillaInt ySrc, VanillaInt WidthSrc, VanillaInt HeightSrc, VanillaByte Alpha) {
	if (!(Graphics && Image)) {
		return;
	}

	if (Width <= 0) {
		Width = (VanillaInt)(Image->Width - xSrc);
	}
	if (Height <= 0) {
		Height = (VanillaInt)(Image->Height - ySrc);
	}


	if (WidthSrc <= 0 || WidthSrc + xSrc > Image->Width) {
		WidthSrc = (VanillaInt)(Image->Width - xSrc);
	}
	if (HeightSrc <= 0 || HeightSrc + ySrc > Image->Height) {
		HeightSrc = (VanillaInt)(Image->Height - ySrc);
	}
#ifdef DRAW_SKIA
	Graphics->Paint.reset();
	Graphics->Paint.setAlpha(Alpha);
	SkRect Src = SkRect::MakeXYWH((VanillaReal)xSrc, (VanillaReal)ySrc, (VanillaReal)WidthSrc, (VanillaReal)HeightSrc);
	SkRect Dest = SkRect::MakeXYWH((VanillaReal)(x), (VanillaReal)(y), (VanillaReal)(Width), (VanillaReal)(Height));
	Graphics->Canvas.drawBitmapRectToRect(Image->Bitmap, &Src, Dest, &Graphics->Paint);
#elif defined DRAW_GDI
	GdipDrawImageRectRectI(Graphics->graphics, Image->image, x, y, Width, Height, xSrc, ySrc, WidthSrc, HeightSrc, 2, NULL, NULL, NULL);
#endif

}

VAPI(VanillaVoid) VanillaAlphaBlendEx(VanillaGraphics Dest, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaGraphics Src, VanillaInt xSrc, VanillaInt ySrc, VanillaInt WidthSrc, VanillaInt HeightSrc, VanillaByte Alpha) {
#ifdef DRAW_SKIA
	Dest->Paint.reset();
	Dest->Paint.setAlpha(Alpha);
	SkIRect Rect = SkIRect::MakeXYWH(xSrc, ySrc, WidthSrc, HeightSrc);
	Dest->Canvas.drawBitmapRect(Src->Bitmap, &Rect, SkRect::MakeXYWH((VanillaReal)x, (VanillaReal)y, (VanillaReal)Width, (VanillaReal)Height), &Dest->Paint);
#elif defined DRAW_GDI
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 1;
	bf.SourceConstantAlpha = Alpha;
	AlphaBlend(Dest->hdc, x, y, Width, Height, Src->hdc, xSrc, ySrc, WidthSrc, HeightSrc, bf);
#endif

}

VAPI(VanillaVoid) VanillaAlphaBlend(VanillaGraphics Dest, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaGraphics Src, VanillaInt xSrc, VanillaInt ySrc, VanillaByte Alpha) {
#ifdef DRAW_SKIA
	Dest->Paint.reset();
	Dest->Paint.setAlpha(Alpha);
	SkIRect Rect = SkIRect::MakeXYWH(xSrc, ySrc, Width, Height);
	Dest->Canvas.drawBitmapRect(Src->Bitmap, &Rect, SkRect::MakeXYWH((VanillaReal)x, (VanillaReal)y, (VanillaReal)Width, (VanillaReal)Height), &Dest->Paint);
#elif defined DRAW_GDI
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;     
	bf.BlendFlags = 0;
	bf.AlphaFormat = 1;
	bf.SourceConstantAlpha = Alpha;
	BOOL a=AlphaBlend(Dest->hdc, x, y, Width, Height, Src->hdc, xSrc, ySrc, Width, Height, bf);
#endif

}

VAPI(VanillaVoid) VanillaGraphicsClear(VanillaGraphics Graphics, VanillaColor Color) {
#ifdef DRAW_SKIA
	Graphics->Bitmap.eraseColor(Color);
#elif defined DRAW_GDI
	GdipGraphicsClear(Graphics->graphics, Color);
#endif

}

VAPI(VanillaStringFormat) VanillaCreateStringFormat(VanillaText FontName, VanillaReal FontSize, VanillaColor TextColor, VanillaColor ShadowColor, VanillaInt StringStyle) {
#ifdef DRAW_SKIA
	VanillaStringFormat StringFormat = new VStringFormat;
	StringFormat->TextColor = TextColor;
	StringFormat->Style = StringStyle;
	StringFormat->ShadowColor = ShadowColor;
	if (StringStyle & VTS_VALIGN_MIDDLE) {
		StringFormat->VAlign = SkTextBox::kCenter_SpacingAlign;
	}
	else if (StringStyle & VTS_VALIGN_BOTTOM) {
		StringFormat->VAlign = SkTextBox::kEnd_SpacingAlign;
	}
	else {
		StringFormat->VAlign = SkTextBox::kStart_SpacingAlign;
	}

	if (StringStyle & VTS_ALIGN_CENTER) {
		StringFormat->Align = SkPaint::kCenter_Align;
	}
	else if (StringStyle & VTS_ALIGN_RIGHT) {
		StringFormat->Align = SkPaint::kRight_Align;
	}
	else {
		StringFormat->Align = SkPaint::kLeft_Align;
	}
	StringFormat->LineMode = SkTextBox::kLineBreak_Mode;
	StringFormat->Size = FontSize;
	StringFormat->Typeface = VanillaPortCreateSkTypeface(FontName, (StringStyle & VTS_ITALIC) ? SkTypeface::kItalic : SkTypeface::kNormal + (StringStyle & VTS_BOLD) ? SkTypeface::kBold : SkTypeface::kNormal);
	return StringFormat;
#elif defined DRAW_GDI
	int i = 0;
	VanillaStringFormat StringFormat = (VanillaStringFormat)malloc(sizeof(VStringFormat));
	if (!StringFormat){ return NULL; }
	memset(StringFormat, 0, sizeof(VStringFormat));

	GdipCreateStringFormat(0, 0, &StringFormat->StringFormat);//创建字体格式
	
	if (StringStyle & VTS_ALIGN_LEFT){
		i = 0;
	}
	else if (StringStyle & VTS_ALIGN_CENTER){
		i = 1;
	}
	else if (StringStyle & VTS_ALIGN_RIGHT)
	{
		i = 2;
	}
	GdipSetStringFormatAlign(StringFormat->StringFormat, i);//设置水平对齐方式
	if (StringStyle & VTS_VALIGN_TOP){
		i = 0;
	}
	else if (StringStyle & VTS_VALIGN_MIDDLE){
		i = 1;
	}
	else if (StringStyle & VTS_VALIGN_BOTTOM)
	{
		i = 2;
	}
	GdipSetStringFormatLineAlign(StringFormat->StringFormat, i);//设置纵向对齐方式
	i = 0;
	if (StringStyle & VTS_BOLD){
		i = 1;
	}
	else if (StringStyle & VTS_ITALIC){
		i += 2;
	}
	
	GdipCreateFontFamilyFromName(FontName, NULL, &StringFormat->FontFamily);
	GdipCreateFont(StringFormat->FontFamily, FontSize, i, 2, &StringFormat->font);
	GdipDeleteFontFamily(StringFormat->FontFamily);
	if (StringStyle & VTS_SHADOW)
	{
		GdipCreateSolidFill(ShadowColor, &StringFormat->BrushShadow);
	}
	GdipCreateSolidFill(TextColor, &StringFormat->Brush);
	StringFormat->TextColor = TextColor;
	return StringFormat;
#endif
}

VAPI(VanillaVoid) VanillaDestroyStringFormat(VanillaStringFormat StringFormat) {
#ifdef DRAW_SKIA
	delete StringFormat;
#elif defined DRAW_GDI
	GdipDeleteFontFamily(StringFormat->FontFamily);
	GdipDeleteFont(StringFormat->font);
	free(StringFormat);
#endif
}

VAPI(VanillaGraphics) VanillaCreateGraphicsInMemory(VanillaInt Width, VanillaInt Height) {
#ifdef DRAW_SKIA
	VanillaGraphics Graphics = new VGraphics;
	Graphics->Bitmap.allocPixels(SkImageInfo::Make(Width, Height, kBGRA_8888_SkColorType, kPremul_SkAlphaType));
	new (&Graphics->Canvas) SkCanvas(Graphics->Bitmap);
	Graphics->Width = Width;
	Graphics->Height = Height;
	return Graphics;
#elif defined DRAW_GDI
	VanillaGraphics Graphics = (VanillaGraphics)malloc(sizeof(VGraphics));
	if (!Graphics){ return NULL; }
	memset(Graphics, 0, sizeof(VGraphics));
	
	Graphics->hdc = CreateCompatibleDC(NULL);
	BITMAPINFO BitmapInfo;
	memset(&BitmapInfo, 0, sizeof(BitmapInfo));
	BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
	BitmapInfo.bmiHeader.biBitCount = 32;
	BitmapInfo.bmiHeader.biWidth = Width;
	BitmapInfo.bmiHeader.biHeight = Height;
	BitmapInfo.bmiHeader.biPlanes = 1;
	VanillaAny Bits;
	Graphics->HBitmap = CreateDIBSection(0, &BitmapInfo, 0, &Bits, 0, 0);
	if (!Graphics->HBitmap) {
		DeleteDC(Graphics->hdc);
		return NULL;
	}
	HBITMAP OldBitmap = (HBITMAP)SelectObject(Graphics->hdc, (HGDIOBJ)Graphics->HBitmap);

	GdipCreateFromHDC(Graphics->hdc, &Graphics->graphics);
	return Graphics;
#endif

}

VAPI(VanillaGraphics) VanillaCreateGraphicsOfWindow(VanillaWindow Window) {
	if (Window) {
#ifdef WIN32
		VanillaGraphics Graphics = (VanillaGraphics)malloc(sizeof(VGraphics));
		if (!Graphics){ return NULL; }
		memset(Graphics, 0, sizeof(VGraphics));

		HDC MemoryDC = CreateCompatibleDC(NULL);
		if (!MemoryDC) {
			return NULL;
		}
		BITMAPINFO BitmapInfo;
		memset(&BitmapInfo, 0, sizeof(BitmapInfo));
		BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
		BitmapInfo.bmiHeader.biBitCount = 32;
		BitmapInfo.bmiHeader.biWidth = Window->Rect.Width;
		BitmapInfo.bmiHeader.biHeight = -Window->Rect.Height;
		BitmapInfo.bmiHeader.biPlanes = 1;
		VanillaAny Bits;
		HBITMAP HBitmap = CreateDIBSection(0, &BitmapInfo, 0, &Bits, 0, 0);
		if (!HBitmap) {
			DeleteDC(MemoryDC);
			return NULL;
		}

		HBITMAP OldBitmap = (HBITMAP)SelectObject(MemoryDC, (HGDIOBJ)HBitmap);

#ifdef DRAW_SKIA
		Graphics->Bitmap.installPixels(SkImageInfo::MakeN32Premul(Window->Rect.Width, Window->Rect.Height), Bits, ((Window->Rect.Width * 32 + 15) / 16) * 2);
		new (&Graphics->Canvas) SkCanvas(Graphics->Bitmap);
		Graphics->PortGraphics = new VPortGraphics;
		Graphics->PortGraphics->MemoryDC = MemoryDC;
		Graphics->PortGraphics->CurrentBitmap = HBitmap;
		Graphics->Width = Window->Rect.Width;
		Graphics->Height = Window->Rect.Height;
		Graphics->PortGraphics->OldBitmap = OldBitmap;
#elif defined DRAW_GDI
		Graphics->hdc = MemoryDC;
		Graphics->HBitmap = HBitmap;
		GdipCreateFromHDC(Graphics->hdc, &Graphics->graphics);
		Graphics->PortGraphics = (VPortGraphics*)malloc(sizeof(VPortGraphics));
		Graphics->PortGraphics->MemoryDC = MemoryDC;
		Graphics->PortGraphics->CurrentBitmap = HBitmap;
		Graphics->PortGraphics->OldBitmap = OldBitmap;
#endif
		return Graphics;
#elif defined LINUX
		//SkBitmap Bitmap;
		//Bitmap.allocPixels(SkImageInfo::Make(Window->Rect.Width, Window->Rect.Height, kBGRA_8888_SkColorType, kPremul_SkAlphaType));
		VanillaGraphics Graphics = VanillaCreateGraphicsInMemory(Window->Rect.Width, Window->Rect.Height);
		//Graphics->Width = Window->Rect.Width;
		//Graphics->Height = Window->Rect.Height;
		Graphics->PortGraphics = new VPortGraphics;
		Graphics->PortGraphics->cairo_surface = cairo_image_surface_create_for_data((unsigned char *)Graphics->Bitmap.getPixels(), CAIRO_FORMAT_ARGB32, Window->Rect.Width, Window->Rect.Height, ((Window->Rect.Width * 32 + 15) / 16) * 2);
		//Graphics->image = xcb_image_create_from_bitmap_data((uint8_t*)Bitmap->getPixels(), Window->Rect.Width, Window->Rect.Height);
		return Graphics;
#endif
	}
	return NULL;
}
VAPI(VanillaVoid) VanillaDestroyGraphicsOfWindow(VanillaGraphics Graphcis) {
	if (Graphcis) {
#ifdef WIN32
#ifdef DRAW_SKIA
		Graphcis->Bitmap.reset();
		DeleteObject((HBITMAP)SelectObject(Graphcis->PortGraphics->MemoryDC, (HGDIOBJ)Graphcis->PortGraphics->OldBitmap));
		DeleteDC(Graphcis->PortGraphics->MemoryDC);
		delete Graphcis->PortGraphics;
#elif defined DRAW_GDI
		DeleteObject((HBITMAP)SelectObject(Graphcis->PortGraphics->MemoryDC, (HGDIOBJ)Graphcis->PortGraphics->OldBitmap));
		free(Graphcis->PortGraphics);
#endif
		VanillaDestroyGraphics(Graphcis);
#elif defined LINUX
		cairo_surface_destroy(Graphcis->PortGraphics->cairo_surface);
		delete Graphcis->PortGraphics;
		VanillaDestroyGraphics(Graphcis);
		//xcb_free_pixmap(connection, Graphics->bitmap);
#endif
	}
	return;
}

VAPI(VanillaVoid) VanillaDestroyGraphics(VanillaGraphics Graphics) {
#ifdef DRAW_SKIA
	if (Graphics) {
		delete Graphics;
	}
#elif defined DRAW_GDI
	GdipDeleteGraphics(Graphics);
	DeleteDC(Graphics->hdc);
	DeleteObject(Graphics->HBitmap);
	free(Graphics);
#endif

}

VAPI(VanillaImage) VanillaLoadImageFromFile(VanillaText FileName) {
	VanillaImage Image = (VanillaImage)malloc(sizeof(VImage));
	if (!Image){ return NULL; }
	memset(Image, 0, sizeof(VImage));
#ifdef DRAW_SKIA
	if (!SkImageDecoder::DecodeFile(FileName, &Image->Bitmap)) {
		free(Image);
		return NULL;
	}
	Image->Width = Image->Bitmap.width();
	Image->Height = Image->Bitmap.height();
#elif defined DRAW_GDI
	GdipGetImageHeight(Image->image, &Image->Height);
	GdipGetImageWidth(Image->image, &Image->Width);
#endif
	return Image;
}

VAPI(VanillaImage) VanillaLoadImageFromBinary(VanillaBin Binary) {
	VanillaImage Image = (VanillaImage)malloc(sizeof(VImage));
	if (!Image){ return NULL; }
	memset(Image, 0, sizeof(VImage));
#ifdef DRAW_SKIA
	if (!Binary) {
		return NULL;
	}
	if (!SkImageDecoder::DecodeMemory(Binary->Address, Binary->Length, &Image->Bitmap)) {
		free(Image);
		return NULL;
	}
	Image->Width = Image->Bitmap.width();
	Image->Height = Image->Bitmap.height();
#elif defined DRAW_GDI
	HGLOBAL hMem = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE, Binary->Length);
	void *str;
	CreateStreamOnHGlobal(hMem, 0, &str);
	void * mem = GlobalLock(hMem);
	memcpy(mem, Binary->Address, Binary->Length);
	GlobalUnlock(hMem);
	GdipLoadImageFromStream(str, &Image->image);
	GdipGetImageHeight(Image->image, &Image->Height);
	GdipGetImageWidth(Image->image, &Image->Width);
#endif
	return Image;
}

VAPI(VanillaVoid) VanillaDestroyImage(VanillaImage Image) {
#ifdef DRAW_SKIA
	if (Image) {
		free(Image);
	}
#elif defined DRAW_GDI
	GdipDisposeImage(Image->image);
	free(Image);
#endif

}
#ifdef DRAW_SKIA
SkTypeface* VanillaPortCreateSkTypeface(VanillaText FontName, SkTypeface::Style Style) {
#ifdef WIN32
	VanillaPort_ICONV;
	const wchar_t* Unicode;
	Unicode = VanillaPort_U2W(FontName).c_str();
	int LengthOld = wcslen(Unicode);
	int LengthNew = LengthOld * 2 + 1;
	char* Ansi = (char*)alloca(LengthNew);
	LengthNew = WideCharToMultiByte(CP_ACP, NULL, Unicode, LengthOld, Ansi, LengthNew, NULL, NULL);
	Ansi[LengthNew] = 0;
	return SkTypeface::CreateFromName(Ansi, Style);
#elif defined LINUX
	return SkTypeface::CreateFromName(FontName, Style);
#endif
	return NULL;
}
#endif