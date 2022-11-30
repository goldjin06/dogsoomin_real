#include "define.h"
#include "ImageLayer.h"
#include "mazefield.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

ImageLayer imageLayer = { NULL, 0, RGB(0,0,0) , NULL, NULL,_initialize, _renderAll, _renderAndFadeIn, _renderAndFadeOut, NULL };
INPUT_RECORD rec;
DWORD dwNOER;
HANDLE CIN = 0;

/*****************************>>>>> GAME SETING <<<<<<*********************************/


void printTextWithAngle(HDC hdc, int x, int y, int size, int weight, int angle, COLORREF textColor, int align, char* text) {
   if (weight == 0) weight = 900;
   size = (int)(size * RESOLUTION_MULTIPLIER);
   const HFONT font = CreateFont(size, 0, angle, 0, weight, 0, 0, 0, HANGEUL_CHARSET,
      0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("    "));

   SelectObject(hdc, font);
   SetBkMode(hdc, TRANSPARENT);
   SetTextColor(hdc, textColor);
   SetTextAlign(hdc, align);

   x = (int)(x * RESOLUTION_MULTIPLIER);
   y = (int)(y * RESOLUTION_MULTIPLIER);
   TextOut(hdc, x, y, text, lstrlen(text));

   PAINTSTRUCT paint;
   EndPaint(WINDOW_HANDLE, &paint);

   DeleteObject(font);
}

void printText(HDC hdc, int x, int y, int size, int weight, COLORREF textColor, int align, char* text) {
   printTextWithAngle(hdc, x, y, size, weight, 0, textColor, align, text);
}

/** @brief initalize imagelayer
*/
void initLayer() {
   imageLayer.initialize(&imageLayer);
   imageLayer.transparentColor = RGB(0, 255, 0);
}

/******************* TITLE *************************/

/** @brief start page
* @return none
* @param none
*/
void showTitle() {
    Sleep(500);

    initLayer();
    Image images[7] = {
        {"resource/background/start_background.bmp", 0, 0},
        {"resource/title/start_button.bmp", 700, 500},
        {"resource/title/title_text.bmp", 250, 50},
    }; // first element is at the bottom
    imageLayer.imageCount = 4;
    imageLayer.images = images;

    imageLayer.renderAll(&imageLayer);
    int on = 0;
    int key;
    while (1) {
        key = getch();
        if (key == 13 && on == 1) { // second ENTER -> go to next page
            break;
        }
        if (key == 13){
            images[1].fileName = "resource/title/start_button_clicked.bmp"; // first ENTER -> change color
            imageLayer.renderAll(&imageLayer);
            on = 1;
            Sleep(300);
            key = 0;
        }
    }
}

/******************** NEW / LOAD *************************/
/** @brief make new nickname
* @return none
* @param file pointer and char array for write new nickname, ImageLayer for control image
*/
void newNickname(FILE* fp, char* nn, ImageLayer layer) {

    initLayer();

    Image images[5] = {
        {"resource/background/start_background.bmp", 0, 0},
        {"resource/text/textarea.bmp", 150, 300}
    }; // first element is at the bottom
    imageLayer.renderAll(&imageLayer);
    int len = 0;
    char pressedKey;
    printText(layer._consoleDC, 300, 450, 60, 0, RGB(0, 0, 0), TA_LEFT, (" ̸     Է  ϼ   (8~14                 մϴ .)"));

    while (3) {
        initLayer();
        imageLayer.renderAll(&imageLayer);
        printText(layer._consoleDC, 300, 450, 60, 0, RGB(0, 0, 0), TA_LEFT, (" ̸     Է  ϼ   (8~14                 մϴ .)"));
        printText(layer._consoleDC, 300, 550, 60, 0, RGB(0, 0, 255), TA_LEFT, ("%s", nn)); //    ?
        if (len != 0) printText(layer._consoleDC, 300, 650, 60, 0, RGB(0, 0, 0), TA_LEFT, ("    Է  ߴٸ     ͸       ּ   "));


        pressedKey = _getch();

        if (pressedKey == 13) { // ENTER -> go to next page
            if (len == 0) continue; // if nothing on nickname, not go to next page
            fprintf(fp, "%s\n", nn);
            selectGender(fp); // next page : select gender
            break;
        }
        if (pressedKey == '\b') { // backspace -> delete character
            if (len == 0) continue; // if there is nothing on nickname, do nothing
            len--;
            nn[len] = NULL;
            continue;
        }

        if (len > 13) { // 14 character exceed
            printText(layer._consoleDC, 100, 100, 60, 0, RGB(0, 0, 0), TA_LEFT, ("14    ʰ  Դϴ ."));
            while (3) {
                pressedKey = _getch();
                if (pressedKey == '\b') {
                    break;
                }
            }
            continue;
        }
        nn[len] = pressedKey;
        len++;


    }
}

/** @brief check data for load, and ask user for load or new
* @return none
* @param file pointer and char array for write new nickname
*/
void UserName(FILE *fp, char *nn) {
    initLayer();
    Image images[5] = {
        {"resource/background/start_background.bmp", 0, 0},
        {"resource/text/textarea.bmp", 150, 300}
    }; // first element is at the bottom

    imageLayer.imageCount = 2;
    imageLayer.images = images;

    imageLayer.renderAll(&imageLayer);

    char ch;
    int cnt = 0;
    while (fscanf(fp, "%c", &ch) != EOF) cnt++; // check if there is data for load (cnt : length of character in data file)
    if (cnt == 0) { // if there is nothing on data file
        newNickname(fp, nn, imageLayer);
    }
    else {
        printText(imageLayer._consoleDC, 300, 500, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT("     ϱ  : 1"));
        printText(imageLayer._consoleDC, 300, 600, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT(" ҷ      : 2"));
        if (getch() == '1') {//     ??                                                 .
            fp = freopen("data/user.txt","w",fp); // delete all data
            fp = freopen("data/user.txt","r+",fp);
            newNickname(fp, nn, imageLayer);
        }
        else {

        }
    }
}
/********************* SELECT GENDER *********************************/

/** @brief page of selecting character's gender
* @return none
* @param fp : input character's gender in text file
*/
void selectGender(FILE *fp) {
    initLayer();
    Image images[4] = {
        {"resource/background/start_background.bmp", 0, 0},
        {"resource/gender/character_girl.bmp", 656, 272},
        {"resource/gender/character_boy.bmp",  1136, 272},
        {"resource/gender/character_girl_selected.bmp", 640, 272-16}
    }; // first element is at the bottom
    imageLayer.imageCount = 4;
    imageLayer.images = images;
    imageLayer.renderAll(&imageLayer);
	printText(imageLayer._consoleDC, 300, 100, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT("            ϼ   "));
    int key, select = 0;
    while(1) {
        key = getch(); // move selecter by left key and right key
        if (key == RIGHT || key == LEFT) {
            select++;
            select %= 2;
        }
        else if(key == 13) {
            switch(select) { // ENTER -> select and write gender information in "data.txt"
            case 0:
                fprintf(fp, "f\ne");
                break;
            case 1:
                fprintf(fp, "m\ne");
                break;
            }
            break;
        }

        switch (select) {
        case 0: // move selecter
            images[3].x = 640;
            images[3].fileName = "resource/gender/character_girl_selected.bmp";
            imageLayer.renderAll(&imageLayer);
            break;
        case 1:
            images[3].x = 1120;
            images[3].fileName = "resource/gender/character_boy_selected.bmp";
            imageLayer.renderAll(&imageLayer);
            break;
        }
        printText(imageLayer._consoleDC, 300, 100, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT("            ϼ   "));

    }
}


/********************** READDATA (in file)  ******************************/
/** @brief read name,gender,difficulty data in "data.txt"
*  @param file pointer, struct information for submit
*/
void readData(FILE *fp, struct information *data) {
    fseek(fp,0,SEEK_SET); // go to first line
    fscanf(fp,"%s\n",(*data).name); // first line -> name
    fscanf(fp,"%c\n",&(*data).gender); // second line -> gender (m / f)
    fseek(fp,-1,SEEK_END); // last character -> highest level the user ever clear
    fscanf(fp,"%c", &(*data).difficultyInformation);
    fseek(fp,0,SEEK_END);
}

/********************* SELECT STAGE *********************************/
/** @brief page : selecting stage
*  @param struct information for load data
*/
void selectStage(struct information *data) {
    initLayer();
    Image images[7] = {
        {"resource/background/start_background.bmp", 0, 0},
        {"resource/difficulty/weekend_day.bmp", 296-150, 290+100},
        {"resource/difficulty/weekend_night.bmp", 792, 290+100},
        {"resource/difficulty/weekday.bmp", 1288+150, 290+100},
        {"resource/difficulty/selected.bmp", 296-166, 290-16+100}
    }; // first element is at the bottom
    switch((*data).difficultyInformation) { // lock the level which user can't play yet
    case 'e':
        images[2].fileName = "resource/difficulty/weekend_night_locked.bmp";
    case 'n':
        images[3].fileName = "resource/difficulty/weekday_locked.bmp";
        break;
    }


    imageLayer.imageCount = 5;
    imageLayer.images = images;
    imageLayer.renderAll(&imageLayer);
    printText(imageLayer._consoleDC, 300, 100, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT("   ̵         ϼ   "));
    int key, select = 0;
    while(1) { // move selecter by left key and right key
        key = getch();
        if (key == RIGHT) {
            select++;
        }
        else if (key == LEFT) {
            select += 2;
        }
        select %= 3;

        switch (select) {
        case 0: // move selector
            images[4].x = 296-166;
            imageLayer.renderAll(&imageLayer);
            printText(imageLayer._consoleDC, 300, 100, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT("   ̵         ϼ   "));
            break;
        case 1:
            images[4].x = 792-16;
            imageLayer.renderAll(&imageLayer);
            printText(imageLayer._consoleDC, 300, 100, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT("   ̵         ϼ   "));
            break;
        case 2:
            images[4].x = 1288+150-16;
            imageLayer.renderAll(&imageLayer);
            printText(imageLayer._consoleDC, 300, 100, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT("   ̵         ϼ   "));
            break;
        }

        if (key == ENTER) {
            if (images[select+1].fileName == "resource/difficulty/weekend_night_locked.bmp" ||images[select+1].fileName == "resource/difficulty/weekday_locked.bmp") {
                printText(imageLayer._consoleDC, 600, 1000, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT("                    ϴ  ."));
                printText(imageLayer._consoleDC, 300, 100, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT("   ̵         ϼ   "));
            }
            else {
                (*data).nowDifficulty = select;  // nowdifficculty : the level which user is going to play now / 0 : easy, 1 : normal, 2 : hard
                break;
            }
        }


    }
}


/********************** >>>>>> GAME START <<<<<<<<< **************************************/

/******************** MAZE ********************************/
void printTextMaze() { //print maze in text under image.

    for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 135; j++) printf("%c",mazefield[i][j]);
            if (i != 39)printf("\n");
    }
}

/** @brief change position of bottle picture by keyboard input
*  @param position of bottle (nowX, nowY) image pointer for change position
*/
int moveCharacter(int * nowX, int *nowY, Image *images) {
    int key = getch();

    switch (key){
        case LEFT: // if there is no wall, change position
            if (mazefield[*nowY/32][(*nowX-16)/16]<0 || mazefield[*nowY/32+1][(*nowX-16)/16]<0 || mazefield[*nowY/32+2][(*nowX-16)/16]<0 || mazefield[(*nowY+80)/32][(*nowX-16)/16]<0 || mazefield[*nowY/32][(*nowX-16)/16] == 'x' || mazefield[*nowY/32+1][(*nowX-16)/16] == 'x' || mazefield[*nowY/32+2][(*nowX-16)/16] == 'x'|| mazefield[(*nowY+80)/32][(*nowX-16)/16] == 'x') break;
            images[1].x -= 16;
            *nowX -= 16;
            break;
        case RIGHT:
            if (mazefield[*nowY/32][(*nowX+96)/16]>135 || mazefield[*nowY/32][(*nowX+96)/16] == 'x' || mazefield[*nowY/32+1][(*nowX+96)/16] == 'x' || mazefield[*nowY/32+2][(*nowX+96)/16] == 'x'|| mazefield[(*nowY+80)/32][(*nowX+96)/16] == 'x') break;
            images[1].x += 16;
            *nowX += 16;
            break;
        case UP:
            if (mazefield[(*nowY-16)/32][*nowX/16] =='x'||mazefield[(*nowY-16)/32][*nowX/16+1] =='x'||mazefield[(*nowY-16)/32][*nowX/16+2] =='x'||mazefield[(*nowY-16)/32][*nowX/16+3] =='x'||mazefield[(*nowY-16)/32][*nowX/16+4] =='x'||mazefield[(*nowY-16)/32][*nowX/16+5] =='x') break;
            images[1].y -= 16;
            *nowY -= 16;
            break;
        case DOWN:
            if (mazefield[(*nowY+96)/32][*nowX/16] =='x'||mazefield[(*nowY+96)/32][*nowX/16+1] =='x'||mazefield[(*nowY+96)/32][*nowX/16+2] =='x'||mazefield[(*nowY+96)/32][*nowX/16+3] =='x'||mazefield[(*nowY+96)/32][*nowX/16+4] =='x'||mazefield[(*nowY+96)/32][*nowX/16+5] =='x') break;
            images[1].y += 16;
            *nowY += 16;
            break;
    }
}

/** @brief maze with no second grade
*  @param image pointer for control image
*  @return 0 - gamefail and exit / 1 - gamefail and replay / 2 - maze clear
*/
int easyMaze(Image *images) {
    initLayer();
    imageLayer.imageCount = 10;
    imageLayer.images = images;
    images[7].isHide = 1; // hide second grade (blue necktie)
    images[8].isHide = 1;
	printText(imageLayer._consoleDC, 300, 100, 60, 0, RGB(255, 255, 255), TA_LEFT, TEXT("      ߷     !"));
    imageLayer.renderAll(&imageLayer);


    int key, nowX = 0, nowY = 192; // now position in pixel
    float Xcoord, Ycoord; // now position in array
    while (1) {

        moveCharacter(&nowX, &nowY,images);
        imageLayer.renderAll(&imageLayer);
        Xcoord = nowX/16.0; Ycoord = nowY/32.0;
        if ((Ycoord >= 5 && Ycoord <= 7 && (Xcoord == 26||Xcoord == 103))||(Ycoord >= 26 && Ycoord <= 28 && Xcoord == 55)||(Ycoord >= 19 && Ycoord <= 21 && Xcoord == 74)) {

            return gameFail(); //fail if the bottle reach to cap, glasses
        }
        if ( (Xcoord>= 121 && Xcoord <= 125 && Ycoord == 9.5)) {

            return 2; // clear if the bottle reach to water purifier
        }
    }

}

/** @brief maze with second grade
*  @param image pointer for control image
*  @return 0 - gamefail and exit / 1 - gamefail and replay / 2 - maze clear
*/
int normalMaze(Image *images) { // almost same with easyMaze
    initLayer();
    imageLayer.imageCount = 10;
    imageLayer.images = images;
    printText(imageLayer._consoleDC, 300, 100, 60, 0, RGB(255, 255, 255), TA_LEFT, TEXT("      ߷     !"));
    imageLayer.renderAll(&imageLayer);

    int key, nowX = 0, nowY = 192; // now position in pixel
    float Xcoord, Ycoord; // now position in array
    while (1) {

        moveCharacter(&nowX, &nowY,images);
        imageLayer.renderAll(&imageLayer);
        Xcoord = nowX/16.0; Ycoord = nowY/32.0;
        if ((Ycoord >= 5 && Ycoord <= 7 && (Xcoord == 26||Xcoord == 103))||(Ycoord >= 26 && Ycoord <= 28 && Xcoord == 55)||(Ycoord >= 19 && Ycoord <= 21 && Xcoord == 74)) {

            return gameFail();
        }
        if ((Xcoord >= 53 && Xcoord <= 56 && Ycoord > 9 && Ycoord <= 21)||(Xcoord >= 122 && Xcoord <= 125 && Ycoord >= 19 && Ycoord <= 28)) {

            return gameFail();
        }
        if (Xcoord >= 121 && Xcoord <= 125 && Ycoord == 9.5) {

            return 2; // clear
        }
    }

}

/** @brief maze with second grade and blackout
*  @param image pointer for control image
*  @return 0 - gamefail and exit / 1 - gamefail and replay / 2 - maze clear
*/
int hardMaze(Image *images) {  // almost same with easyMaze
    initLayer();
    imageLayer.imageCount = 10;
    imageLayer.images = images;
    printText(imageLayer._consoleDC, 300, 100, 60, 0, RGB(255, 255, 255), TA_LEFT, TEXT("      ߷     !"));
    imageLayer.renderAll(&imageLayer);

    int key, nowX = 0, nowY = 192; // now position in pixel
    int hardset = (rand()%10)*2+20 , hard = 0; // hardset => random light on
    float Xcoord, Ycoord; // now position in array
    while (1) {

        if (hard == 0) {
            for (int i = 0;  i < 9; i++) {if(i != 1)images[i].isHide = 1;}
        }
        else if (hard == hardset-2) {
            for (int i = 0;  i < 9; i++) images[i].isHide = 0;
            hardset = (rand()%10)*2+20;
            hard = -2;
        }

        hard++;
        moveCharacter(&nowX, &nowY,images);
        imageLayer.renderAll(&imageLayer);
        Xcoord = nowX/16.0; Ycoord = nowY/32.0;
        if ((Ycoord >= 5 && Ycoord <= 7 && (Xcoord == 26||Xcoord == 103))||(Ycoord >= 26 && Ycoord <= 28 && Xcoord == 55)||(Ycoord >= 19 && Ycoord <= 21 && Xcoord == 74)) {

            return gameFail();
        }
        if ((Xcoord >= 53 && Xcoord <= 56 && Ycoord > 9 && Ycoord <= 21)||(Xcoord >= 122 && Xcoord <= 125 && Ycoord >= 19 && Ycoord <= 28)) {

            return gameFail();
        }
        if (Xcoord >= 121 && Xcoord <= 125 && Ycoord == 9.5) {

            return 2; // clear
        }


    }

}

/** @brief select maze difficulty by nowDifficulty
*  @param image pointer for control image
*  @return return value by maze (0 - gamefail and exit / 1 - gamefail and replay / 2 - maze clear)
*/
int maze(struct information data) {
    Image images[10] = {
        {"resource/maze/maze.bmp", 0, 0},
        {"resource/maze/bottle.bmp", 0, 192,4},
        {"resource/maze/red_cap.bmp", 315, 175,6},
        {"resource/maze/yong_glasses.bmp", 780, 845,6},
        {"resource/maze/red_cap.bmp", 1260, 625,6},
        {"resource/maze/yong_glasses.bmp", 1725, 170,6},
        {"resource/maze/water.bmp", 1950, 170,3},
        {"resource/maze/second_grade.bmp", 928, 384,4},
        {"resource/maze/second_grade.bmp", 2032, 608,4}
    }; // first element is at the bottom

    switch (data.nowDifficulty) {
    case 0:
        return easyMaze(images);
        break;
    case 1:
        return normalMaze(images);
        break;
    case 2:
        return hardMaze(images);
        break;
    }
}


/********************** BEFORE EATING *********************/

/** @brief page of setting(preparing) for eating ramen - back part of room
* @return roomFront() : pressed LEFT button, count : to check how much player setted
* @param data : user information, window : 1 - open/ 0 - close, perfume :1 - used, fan : 1 - turn on / 0 - off, lock : 1 - lock / 0 - unlock
*/
int roomBack(struct information *data, int window, int perfume, int fan, int lock) {
    initLayer();
    Image images[5] = {
        {"resource/room_back/view1.bmp", 0, 0},
        {"resource/room_back/left_arrow.bmp", 32, 560},
        {"resource/room_back/window_closed_day.bmp",  1008, 64},
        {"resource/room_back/perfume.bmp", 1072, 912},
        {"resource/room_back/go.bmp", 2080-176, 1040},
    }; // first element is at the bottom

    window %= 2;
    if(window == 1) {
        if((*data).nowDifficulty != 0)
            images[2].fileName = "resource/room_back/window_opened_night.bmp";
        else
            images[2].fileName = "resource/room_back/window_opened_day.bmp";
        }
    else {
        if((*data).nowDifficulty != 0)
            images[2].fileName = "resource/room_back/window_closed_night.bmp";
        else
            images[2].fileName = "resource/room_back/window_closed_day.bmp";
    }

    imageLayer.imageCount = 5;
    imageLayer.images = images;
    imageLayer.renderAll(&imageLayer);
    int key, select = 0, enter = 0;
    int count = 0;
    while(1) {
        key = getch();
        count = window + perfume + fan + lock;
        switch (key) {
        case ENTER:
            enter++;
            if(enter == 1)
                images[4].fileName = "resource/room_back/go_pressed.bmp";
            else
                return count;
            imageLayer.renderAll(&imageLayer);
            break;
        case LEFT:
            return roomFront(data, window, perfume, fan, lock);
            break;
        case WINDOW:
            window++;
            window %= 2;
            if(window == 1) {
                if((*data).nowDifficulty != 0)
                    images[2].fileName = "resource/room_back/window_opened_night.bmp";
                else
                    images[2].fileName = "resource/room_back/window_opened_day.bmp";
                PlaySound("sound/window_open.wav",NULL,SND_FILENAME|SND_ASYNC);
            }
            else {
                if((*data).nowDifficulty != 0)
                    images[2].fileName = "resource/room_back/window_closed_night.bmp";
                else
                    images[2].fileName = "resource/room_back/window_closed_day.bmp";
                PlaySound("sound/window_close.wav",NULL,SND_FILENAME|SND_ASYNC);
            }
            imageLayer.renderAll(&imageLayer);
            Sleep(300);
            break;
        case PERFUME:
            images[3].fileName = "resource/room_back/perfume_selected.bmp";
            imageLayer.renderAll(&imageLayer);
            PlaySound("sound/perfume.wav",NULL,SND_FILENAME|SND_ASYNC);
            Sleep(300);
            images[3].fileName = "resource/room_back/perfume.bmp";
            imageLayer.renderAll(&imageLayer);
            perfume = 1;
            break;
        }
    }
}

/** @brief page of setting(preparing) for eating ramen - front part of room
* @return roomBack() : pressed RIGHT button, count : to check how much player setted
* @param data : user information, window : 1 - open/ 0 - close, perfume :1 - used, fan : 1 - turn on / 0 - off, lock : 1 - lock / 0 - unlock
*/
int roomFront(struct information *data, int window, int perfume, int fan, int lock) {
    initLayer();
    Image images[6] = {
        {"resource/room_front/view2.bmp", 0, 0},
        {"resource/room_front/right_arrow.bmp", 2032, 560},
        {"resource/room_front/fan.bmp", 912, 544},
        {"resource/room_front/perfume.bmp", 1136, 784},
        {"resource/room_front/door_handle.bmp", 736, 560},
        {"resource/room_front/go.bmp", 2080-176, 1040},
    }; // first element is at the bottom

    fan %= 2;
    lock %= 2;

    if(fan == 1)
        images[2].fileName = "resource/room_front/fan_clicked.bmp";


    if(lock == 1)
        images[4].fileName = "resource/room_front/lock_door.bmp";

    imageLayer.imageCount = 6;
    imageLayer.images = images;
    imageLayer.renderAll(&imageLayer);

    int key, select = 0, enter = 0;
    int count = 0;

    while(1) {
        key = getch();
        count = window + perfume + fan + lock;
        switch (key) {
        case 'E':
        case ENTER:
            enter++;
            if(enter == 1)
                images[5].fileName = "resource/room_front/go_pressed.bmp";

            else
                return count;
            imageLayer.renderAll(&imageLayer);
            break;
        case 'R':
        case RIGHT:
            return roomBack(data, window, perfume, fan, lock);
        case FANBUTTON:
            fan++;
            fan %= 2;
            if(fan == 1)
                images[2].fileName = "resource/room_front/fan_clicked.bmp";
            else
                images[2].fileName = "resource/room_front/fan.bmp";
            PlaySound("sound/fan.wav",NULL,SND_FILENAME|SND_ASYNC);
            imageLayer.renderAll(&imageLayer);
            Sleep(300);
            break;
        case PERFUME:
            images[3].fileName = "resource/room_front/perfume_selected.bmp";
            imageLayer.renderAll(&imageLayer);
            PlaySound("sound/perfume.wav",NULL,SND_FILENAME|SND_ASYNC);
            Sleep(300); // sound
            images[3].fileName = "resource/room_front/perfume.bmp";
            imageLayer.renderAll(&imageLayer);
            perfume = 1;
            break;
        case LOCKDOOR:
            lock++;
            lock %= 2;
            if(lock == 1)
                images[4].fileName = "resource/room_front/lock_door.bmp";
            else
                images[4].fileName = "resource/room_front/door_handle.bmp";
            imageLayer.renderAll(&imageLayer);
            PlaySound("sound/door_lock.wav",NULL,SND_FILENAME|SND_ASYNC);
            Sleep(300);
            break;
        }

    }
}


/************************** EAT ****************************/

/** @brief execute correct mode of eating ramen
* @return 0 : quit, 1 : retry, 2 : stage clear
* @param data : user information, prepare : to check how much player setted
*/
int eatRamen(struct information *data, int prepare) {

    int status = 0;

    if((*data).nowDifficulty == 0) // easy
        status = easyEatRamen(&data, prepare);
    else if((*data).nowDifficulty == 1) // normal
        status = normalEatRamen(&data, prepare);
    else if((*data).nowDifficulty == 2) // hard
        status = hardEatRamen(&data, prepare);

    return status;
}

/** @brief page of eating ramen in easy mode
* @return 0 : quit, 1 : retry, 2 : stage clear
* @param data : user information, prepare : to check how much player setted(At least one must be done)
*/
int easyEatRamen(struct information *data, int prepare) {
    initLayer();
    Sleep(1000);
    Image images[7] = {
        {"resource/eat_ramen/view2.bmp", 0, 0},
        {"resource/eat_ramen/hunger_gauge.bmp", 1888, 256},
        {"resource/eat_ramen/eat.bmp", 1648, 560},
        {"resource/eat_ramen/hide.bmp", 1648, 800},
        {"resource/eat_ramen/eating1.bmp", 800, 720},
        {},
        {"resource/eat_ramen/knock.bmp",785,140,8}
    }; // first element is at the bottom
    images[6].isHide = 1;

    imageLayer.imageCount = 7;
    imageLayer.images = images;
    imageLayer.renderAll(&imageLayer);
    int key;
    int count = 0, domi_teacher;
    int isHide = 0; // 1 : hid the ramen, 0 : didn't
    int teacher = 1; // 1 : not comming, 2 : knocking, 3 : coming

    srand(time(NULL));
    if(prepare == 0){ // fail -> At least one must be done
        PlaySound("sound/knocking.wav",NULL,SND_FILENAME|SND_ASYNC);
        Sleep(700);
        images[5].fileName = "resource/eat_ramen/view2_dooropen.bmp";
        images[5].x = 512;
        images[5].y = 288;
        imageLayer.renderAll(&imageLayer);
        Sleep(800);
        return gameFail();
    }

    while(1) {


        domi_teacher = rand() % 10; // cycle that teacher comes in(random)

        if (teacher == 1 && count%10 == domi_teacher) {
                teacher = 2;
                PlaySound("sound/knocking.wav",NULL,SND_FILENAME|SND_ASYNC);
                images[6].isHide = 0;
                imageLayer.renderAll(&imageLayer);
        } // knocking
        else if (teacher == 2) {
                teacher = 3;
        }
        if (teacher == 3 && isHide == 0) return gameFail(); // door open but, didn't hide
        if (teacher == 3) {
            images[5].fileName = "resource/eat_ramen/view2_dooropen.bmp";
            images[6].isHide = 1;
            images[5].x = 512;
            images[5].y = 288;
            imageLayer.renderAll(&imageLayer);
            Sleep(1300);
            images[5].fileName = '\0';

            imageLayer.renderAll(&imageLayer);
            teacher = 1;
        } // door open and hid the ramen

        key = getch();
        switch(key) {
        case 'E':
        case EAT:
            if(isHide >= 1){
                images[4].x = 800;
                images[4].y = 720;
                imageLayer.renderAll(&imageLayer);
            }
            PlaySound("sound/noodle.wav",NULL,SND_FILENAME|SND_ASYNC);
            isHide = 0;
            count++;
            if(count == 1) {
                images[4].fileName = "resource/eat_ramen/eating2.bmp";
            }
            else if(count == 5) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge15.bmp";
                images[4].fileName = "resource/eat_ramen/eating2.bmp";
            }
            else if(count == 11) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge30.bmp";
                images[4].fileName = "resource/eat_ramen/eating2.bmp";
            }
            else if(count == 18) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge45.bmp";
                images[4].fileName = "resource/eat_ramen/eating3.bmp";
            }
            else if(count == 25) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge60.bmp";
                images[4].fileName = "resource/eat_ramen/eating4.bmp";
            }
            else if(count == 35) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge75.bmp";
                images[4].fileName = "resource/eat_ramen/eating4.bmp";
            }
            else if(count == 48) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge90.bmp";
                images[4].fileName = "resource/eat_ramen/eating5.bmp";
            }
            else if(count == 60) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge95.bmp";
                images[4].fileName = "resource/eat_ramen/eating5.bmp";
            }
            else if(count == 68) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge100.bmp";
                images[4].fileName = "resource/eat_ramen/eating6.bmp";
            }
            imageLayer.renderAll(&imageLayer);
            break;
        case 'H':
        case HIDE:
            isHide++;
            images[4].x = 2081;
            images[4].y = 1281;
            imageLayer.renderAll(&imageLayer);
            break;
        }
        if (count >= 68) return 2; // stage clear
    }
}

/** @brief page of eating ramen in normal mode
* @return 0 : quit, 1 : retry, 2 : stage clear
* @param data : user information, prepare : to check how much player setted(At least two must be done)
*/
int normalEatRamen(struct information *data, int prepare) { // page of eating ramen in normal mode
    initLayer();
    Sleep(1000);
    Image images[7] = {
        {"resource/eat_ramen/view2.bmp", 0, 0},
        {"resource/eat_ramen/hunger_gauge.bmp", 1888, 256},
        {"resource/eat_ramen/eat.bmp", 1648, 560},
        {"resource/eat_ramen/hide.bmp", 1648, 800},
        {"resource/eat_ramen/eating1.bmp", 800, 720},
        {},
        {"resource/eat_ramen/knock.bmp",785,140,8}
    }; // first element is at the bottom
    images[6].isHide = 1;

    imageLayer.imageCount = 7;
    imageLayer.images = images;
    imageLayer.renderAll(&imageLayer);
    int key;
    int count = 0, domi_teacher;
    int isHide = 0; // 1 : hid the ramen, 0 : didn't
    int teacher = 1; // 1 : not comming, 2 : knocking, 3 : coming

    srand(time(NULL));
    if(prepare < 2){ // fail -> At least two must be done
        PlaySound("sound/knocking.wav",NULL,SND_FILENAME|SND_ASYNC);
        Sleep(700);
        images[5].fileName = "resource/eat_ramen/view2_dooropen.bmp";
        images[5].x = 512;
        images[5].y = 288;
        imageLayer.renderAll(&imageLayer);
        Sleep(800);
        return gameFail();
    }

    while(1) {

        domi_teacher = rand() % 8; // cycle that teacher comes in(random)

        if (teacher == 1 && count%8 == domi_teacher) {
                teacher = 2;
                PlaySound("sound/knocking.wav",NULL,SND_FILENAME|SND_ASYNC);
                images[6].isHide = 0;
                imageLayer.renderAll(&imageLayer);
        } // knocking
        else if (teacher == 2) {
                teacher = 3;

        }
        if (teacher == 3 && isHide == 0) return gameFail(); // door open but, didn't hide
        if (teacher == 3) {
            images[5].fileName = "resource/eat_ramen/view2_dooropen.bmp";
            images[5].x = 512;
            images[5].y = 288;
            images[6].isHide = 1;
            imageLayer.renderAll(&imageLayer);
            Sleep(1300);
            images[5].fileName = '\0';

            imageLayer.renderAll(&imageLayer);
            teacher = 1;
        } // door open and hid the ramen

        key = getch();
        switch(key) {
        case 'E':
        case EAT:
            if(isHide >= 1){
                images[4].x = 800;
                images[4].y = 720;
                imageLayer.renderAll(&imageLayer);
            }
            PlaySound("sound/noodle.wav",NULL,SND_FILENAME|SND_ASYNC);
            isHide = 0;
            count++;
            if(count == 1) {
                images[4].fileName = "resource/eat_ramen/eating2.bmp";
            }
            else if(count == 8) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge15.bmp";
                images[4].fileName = "resource/eat_ramen/eating2.bmp";
            }
            else if(count == 15) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge30.bmp";
                images[4].fileName = "resource/eat_ramen/eating2.bmp";
            }
            else if(count == 23) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge45.bmp";
                images[4].fileName = "resource/eat_ramen/eating3.bmp";
            }
            else if(count == 30) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge60.bmp";
                images[4].fileName = "resource/eat_ramen/eating4.bmp";
            }
            else if(count == 47) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge75.bmp";
                images[4].fileName = "resource/eat_ramen/eating4.bmp";
            }
            else if(count == 60) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge90.bmp";
                images[4].fileName = "resource/eat_ramen/eating5.bmp";
            }
            else if(count == 73) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge95.bmp";
                images[4].fileName = "resource/eat_ramen/eating5.bmp";
            }
            else if(count == 80) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge100.bmp";
                images[4].fileName = "resource/eat_ramen/eating6.bmp";
            }
            imageLayer.renderAll(&imageLayer);
            break;
        case 'H':
        case HIDE:
            isHide++;
            images[4].x = 2081;
            images[4].y = 1281;
            imageLayer.renderAll(&imageLayer);
            break;
        }
        if (count >= 80) return 2; // stage clear
    }
}

/** @brief page of eating ramen in hard mode
* @return 0 : quit, 1 : retry, 2 : stage clear
* @param data : user information, prepare : to check how much player setted(At least four must be done)
*/
int hardEatRamen(struct information *data, int prepare) {
    initLayer();
    Sleep(1000);
    Image images[7] = {
        {"resource/eat_ramen/view2.bmp", 0, 0},
        {"resource/eat_ramen/hunger_gauge.bmp", 1888, 256},
        {"resource/eat_ramen/eat.bmp", 1648, 560},
        {"resource/eat_ramen/hide.bmp", 1648, 800},
        {"resource/eat_ramen/eating1.bmp", 800, 720},
        {},
        {"resource/eat_ramen/knock.bmp",785,140,8}
    }; // first element is at the bottom
    images[6].isHide = 1;

    imageLayer.imageCount = 7;
    imageLayer.images = images;
    imageLayer.renderAll(&imageLayer);
    int key;
    int count = 0, domi_teacher;
    int isHide = 0; // 1 : hid the ramen, 0 : didn't
    int teacher = 1; // 1 : not comming, 2 : knocking, 3 : coming

    srand(time(NULL));
    if(prepare < 4){ // fail -> At least four must be done
        PlaySound("sound/knocking.wav",NULL,SND_FILENAME|SND_ASYNC);
        Sleep(700);
        images[5].fileName = "resource/eat_ramen/view2_dooropen.bmp";
        images[5].x = 512;
        images[5].y = 288;
        imageLayer.renderAll(&imageLayer);
        Sleep(800);
        return gameFail();
    }

    while(1) {

        domi_teacher = rand() % 5; // cycle that teacher comes in(random)

        if (teacher == 1 && count%5 == domi_teacher) {
                teacher = 2;
                PlaySound("sound/knocking.wav",NULL,SND_FILENAME|SND_ASYNC);
                images[6].isHide = 0;
                imageLayer.renderAll(&imageLayer);
        } // knocking
        else if (teacher == 2) {
                teacher = 3;
        }
        if (teacher == 3 && isHide == 0) return gameFail(); // door open but, didn't hide
        if (teacher == 3) {
            images[5].fileName = "resource/eat_ramen/view2_dooropen.bmp";
            images[5].x = 512;
            images[5].y = 288;
            images[6].isHide = 1;
            imageLayer.renderAll(&imageLayer);
            Sleep(1300);
            images[5].fileName = '\0';

            imageLayer.renderAll(&imageLayer);
            teacher = 1;
        }  // door open and hid the ramen

        key = getch();
        switch(key) {
        case 'E':
        case EAT:
            if(isHide >= 1){
                images[4].x = 800;
                images[4].y = 720;
                imageLayer.renderAll(&imageLayer);
            }
            PlaySound("sound/noodle.wav",NULL,SND_FILENAME|SND_ASYNC);
            isHide = 0;
            count++;
            if(count == 1) {
                images[4].fileName = "resource/eat_ramen/eating2.bmp";
            }
            else if(count == 15) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge15.bmp";
                images[4].fileName = "resource/eat_ramen/eating2.bmp";
            }
            else if(count == 30) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge30.bmp";
                images[4].fileName = "resource/eat_ramen/eating2.bmp";
            }
            else if(count == 45) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge45.bmp";
                images[4].fileName = "resource/eat_ramen/eating3.bmp";
            }
            else if(count == 60) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge60.bmp";
                images[4].fileName = "resource/eat_ramen/eating4.bmp";
            }
            else if(count == 75) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge75.bmp";
                images[4].fileName = "resource/eat_ramen/eating4.bmp";
            }
            else if(count == 88) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge90.bmp";
                images[4].fileName = "resource/eat_ramen/eating5.bmp";
            }
            else if(count == 95) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge95.bmp";
                images[4].fileName = "resource/eat_ramen/eating5.bmp";
            }
            else if(count == 100) {
                images[1].fileName = "resource/eat_ramen/hunger_gauge100.bmp";
                images[4].fileName = "resource/eat_ramen/eating6.bmp";
            }
            imageLayer.renderAll(&imageLayer);
            break;
        case 'H':
        case HIDE:
            isHide++;
            images[4].x = 2081;
            images[4].y = 1281;
            imageLayer.renderAll(&imageLayer);
            break;
        }
        if (count >= 100) return 2;  // stage clear
    }
}

/***********************GAME FAIL****************************************/
/** @brief when the user fail to clear, select if the user play again or quit
*  @param none
*  @return 0 - exit / 1 - replay
*/
int gameFail() {
    initLayer();
    Image images[6] = {
        {"resource/background/start_background.bmp", 0, 0},
        {"resource/clear/quit_button.bmp",1400,500},
        {"resource/clear/quit_button_clicked.bmp",1400,500},
        {"resource/clear/restart_button.bmp",1400,800},
        {"resource/clear/restart_button_clicked.bmp",1400,800},
        {"resource/fail/report.bmp",160,50}
    }; // first element is at the bottom
    imageLayer.imageCount = 6;
    imageLayer.images = images;
    int key, select = 0;
    images[2].isHide = 0;
    images[4].isHide = 1;
    imageLayer.renderAll(&imageLayer);
    while(1) {

        if (key == RIGHT || key == LEFT) { // change clicked button by right key and left key
            select++;
            select %= 2;
        }
        else if(key == 13) { // press enter to select
            return select;
            break;
        }

        switch (select) {
        case 0:
            images[1].isHide = 1;
            images[2].isHide = 0;
            images[3].isHide = 0;
            images[4].isHide = 1;
            imageLayer.renderAll(&imageLayer);
            break;
        case 1:
            images[1].isHide = 0;
            images[2].isHide = 1;
            images[3].isHide = 1;
            images[4].isHide = 0;
            imageLayer.renderAll(&imageLayer);
            break;
        }
        key = getch();

    }
}


/********************** CLEAR *****************************/
/** @brief when the user clear the game, select if the user play again or quit
*  @param none
*  @return 0 - exit / 1 - replay
*/
int gameClear(FILE *fp, struct information data) { // almost same with gameFail
    if (data.difficultyInformation == 'e') fprintf_s(fp, "n");
    else if (data.difficultyInformation == 'n' && data.nowDifficulty == 1) fprintf_s(fp,"h");
    initLayer();
    Image images[6] = {
        {"resource/background/start_background.bmp", 0, 0},
        {"resource/clear/quit_button.bmp",1400,500},
        {"resource/clear/quit_button_clicked.bmp",1400,500},
        {"resource/clear/restart_button.bmp",1400,800},
        {"resource/clear/restart_button_clicked.bmp",1400,800},
        {"resource/jmt.bmp",300,300,3},
    }; // first element is at the bottom
    imageLayer.imageCount = 6;
    imageLayer.images = images;
    int key, select = 0;
    images[2].isHide = 0;
    images[4].isHide = 1;
    imageLayer.renderAll(&imageLayer);
    while(1) {

        if (key == RIGHT || key == LEFT) {
            select++;
            select %= 2;
        }
        else if(key == 13) {
            return select;
            break;
        }

        switch (select) {
        case 0:
            images[1].isHide = 1;
            images[2].isHide = 0;
            images[3].isHide = 0;
            images[4].isHide = 1;
            imageLayer.renderAll(&imageLayer);
            break;
        case 1:
            images[1].isHide = 0;
            images[2].isHide = 1;
            images[3].isHide = 1;
            images[4].isHide = 0;
            imageLayer.renderAll(&imageLayer);
            break;
        }
        key = getch();

    }
}
