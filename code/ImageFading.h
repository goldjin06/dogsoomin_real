
#ifndef IMAGE_FADING_H
#define IMAGE_FADING_H
#include "ImageLayerImpl.h"


#define FADING_DELAY 20

//   ο  BLENDFUNCTION          ȯ
 BLENDFUNCTION getBlendFunction() {
	BLENDFUNCTION bf;
	bf.AlphaFormat = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = 0;
	return bf;
}

 void _fade(HDC consoleDC, HDC backDC, BLENDFUNCTION bf, int alpha) {
	HDC alphaDC = createNewBackDC(consoleDC);
	bf.SourceConstantAlpha = alpha;
	AlphaBlend(alphaDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
		backDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, bf);
	applyToDC(consoleDC, alphaDC);
	DeleteDC(alphaDC);
	Sleep(FADING_DELAY);
}

 void _renderAndFade(ImageLayer* self, void(*applyToBackDC)(HDC), int isFadeIn) {
	const HDC consoleDC = self->_consoleDC;
	const HDC backDC = getRenderedBackDC(self);
	if (applyToBackDC != NULL) applyToBackDC(backDC);
	BLENDFUNCTION bf = getBlendFunction();

	if (isFadeIn) {
		for (int alpha = 0; alpha <= 255; alpha += 17)
			_fade(consoleDC, backDC, bf, alpha);
	}
	else {
		for (int alpha = 255; alpha >= 0; alpha -= 17)
			_fade(consoleDC, backDC, bf, alpha);
	}

	DeleteDC(backDC);
}

// ̹                   ȿ         
 void _renderAndFadeIn(ImageLayer* self, void(*applyToBackDC)(HDC)) {
	_renderAndFade(self, applyToBackDC, 1);
}

// ̹             ο      ȿ         
 void _renderAndFadeOut(ImageLayer* self, void(*applyToBackDC)(HDC)) {
	_renderAndFade(self, applyToBackDC, 0);
}

#endif
