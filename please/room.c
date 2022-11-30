//#include "define.h"
//#include "title.h"
//#include "ImageLayer.h"
////#include "mazefield.h"
//
//ImageLayer imageLayer = { NULL, 0, RGB(0,0,0) , NULL, NULL,_initialize, _renderAll, _renderAndFadeIn, _renderAndFadeOut, NULL };
//
//int count = 0; // 향수 뿌리기, 문 잠그기, 환풍기 틀기, 창문열기 중 최소 4개 해야함 아니면 탈락
//
//void roomBack(struct information *data) {
//    initLayer();
//    Image images[3] = {
//        {"resource/room_back/view1.bmp", 0, 0}, //{ ̹     ̸ ,      x  ǥ,      y  ǥ, ũ       (             ⺻     16     )}
//        {"resource/room_back/left_arrow.bmp", 16, 560},
//        {"resource/room_back/window_closed_day.bmp",  1008, 48},
//    }; //      u    ?        ?   ?     .
//
//    if((*data).difficultyInformation != 'e') {
//        images[2].fileName = "resource/room_back/window_closed_night.bmp";
//        imageLayer.renderAll(&imageLayer);
//    }
//
//    imageLayer.imageCount = 3;
//    imageLayer.images = images;
//    imageLayer.renderAll(&imageLayer);
//    int key, select = 0;
//
//    while(1) {
//        key = getch();
//        if (key == LEFT) {
//            //roomFront(&data);
//        }
//
//        switch (key) {
//        case WINDOW:
//            if((*data).difficultyInformation != 'e')
//                images[2].fileName = "resource/room_back/window_opened_night.bmp";
//            else
//                images[2].fileName = "resource/room_back/window_opened_day.bmp";
//            imageLayer.renderAll(&imageLayer);
//            count++;
//            break;
//        case PERFUME:
//            count++;
//            break;
//        }
//
//        if (key == ENTER) {
//            if (images[select+1].fileName == "resource/difficulty/weekend_night_locked.bmp" ||images[select+1].fileName == "resource/difficulty/weekday_locked.bmp") {
//                printText(imageLayer._consoleDC, 600, 1000, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT("                    ϴ ."));
//            }
//            else {
//                (*data).nowDifficulty = select;  // 0 : easy, 1 : normal, 2 : hard
//                break;
//            }
//        }
//
//
//    }
//}
