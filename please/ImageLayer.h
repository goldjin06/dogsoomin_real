#ifndef IMAGE_LAYER_H
#define IMAGE_LAYER_H

#include "ImageLayerInterface.h"
#include "ImageFading.h"


static const ImageLayer DEFAULT_IMAGE_LAYER = { NULL, 0, RGB(0,0,0) , NULL, NULL,_initialize, _renderAll, _renderAndFadeIn, _renderAndFadeOut, NULL };

#endif
