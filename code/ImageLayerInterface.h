#ifndef IMAGE_LAYER_INTERFACE_H
#define IMAGE_LAYER_INTERFACE_H

#include <windows.h>


typedef struct {
	char* fileName;
	int x, y;
	double scale;
	int isHide;
}Image;

typedef struct _ImageLayer {
	//¸â¹ö º¯¼ö
	Image* images;
	int imageCount;
	UINT transparentColor;

	HWND _windowHandle;
	HDC _consoleDC;

	//ÇÔ¼ö
	void (*initialize)(struct _ImageLayer*);
	void (*renderAll)(struct _ImageLayer*);
	void (*fadeIn)(struct _ImageLayer*, void(*applyToBackDC)(HDC));
	void (*fadeOut)(struct _ImageLayer*, void(*applyToBackDC)(HDC));
	void (*applyToDC)(HDC);
}ImageLayer;

#endif
