#include "define.h"
#include "ImageLayer.h"
#include "mazefield.h"
ImageLayer imageLayer = { NULL, 0, RGB(0,0,0) , NULL, NULL,_initialize, _renderAll, _renderAndFadeIn, _renderAndFadeOut, NULL };
INPUT_RECORD rec;
DWORD dwNOER;
HANDLE CIN = 0;

void printTextWithAngle(HDC hdc, int x, int y, int size, int weight, int angle, COLORREF textColor, int align, char* text) { //  ̹                    Լ
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

void printText(HDC hdc, int x, int y, int size, int weight, COLORREF textColor, int align, char* text) { //  ̹                    Լ
	printTextWithAngle(hdc, x, y, size, weight, 0, textColor, align, text);
}

void initLayer() { //  ̹      ̾   ʱ ȭ
	imageLayer.initialize(&imageLayer);
	imageLayer.transparentColor = RGB(0, 255, 0);
}

/********************Ÿ  Ʋ    *************************/
void showTitle() {
    Sleep(500);

    initLayer();
    Image images[7] = {
        {"resource/background/start_background.bmp", 0, 0}, //{ ̹     ̸ ,      x  ǥ,      y  ǥ, ũ       (             ⺻     16     )}
        {"resource/title/start_button.bmp", 700, 500},
        {"resource/title/title_text.bmp", 250, 50},
    }; // 迭   ù    Ұ        Ʒ   ׷     .

    imageLayer.imageCount = 4; //images  迭   ũ ⺸    ۰ų     ƾ   Ѵ .
    imageLayer.images = images;

    imageLayer.renderAll(&imageLayer);
    int mouse_x, mouse_y, mouseOn = 0;
    int key;
    while (1) {
        key = getch();
        //mouse_x = rec.Event.MouseEvent.dwMousePosition.X; // X    ޾ƿ
        //mouse_y = rec.Event.MouseEvent.dwMousePosition.Y; // Y    ޾ƿ
        //if (mouse_x >= 200/8 && mouse_x <= 235/8 && mouse_y >= 500/16 && mouse_y <= 535/16 && mouseOn == 0) {
        if (key == 13 && mouseOn == 1) {
            //images[1].fileName = "resource/title/start_button.bmp";
            //imageLayer.renderAll(&imageLayer);
            break;
        }
        if (key == 13){
            images[1].fileName = "resource/title/start_button_clicked.bmp";
            imageLayer.renderAll(&imageLayer);
            mouseOn = 1;
            Sleep(300);
            key = 0;
        }
        //else if (!(mouse_x >= 200/8 && mouse_x <= 235/8 && mouse_y >= 500/16 && mouse_y <= 535/16) && mouseOn == 1) {
    }
}

/******************** ҷ      /      ϱ     *************************/
void newNickname(FILE* fp, char* nn, ImageLayer layer) { //       г

    initLayer();

    Image images[5] = {
        {"resource/background/start_background.bmp", 0, 0}, //{ ̹     ̸ ,      x  ǥ,      y  ǥ, ũ       (             ⺻     16     )}
        {"resource/text/textarea.bmp", 150, 300}
    }; // 迭   ù    Ұ        Ʒ   ׷     .
    imageLayer.renderAll(&imageLayer);
    int len = 0;
    char pressedKey;
    printText(layer._consoleDC, 300, 450, 60, 0, RGB(0, 0, 0), TA_LEFT, ("       ̸         ּ   . (8~14           Ư    ȣ        մϴ .)"));

    while (3) {
        //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

        initLayer();
        imageLayer.renderAll(&imageLayer);
        printText(layer._consoleDC, 300, 450, 60, 0, RGB(0, 0, 0), TA_LEFT, ("       ̸         ּ   .(8~14              մϴ .)"));
        printText(layer._consoleDC, 300, 550, 60, 0, RGB(0, 0, 255), TA_LEFT, ("%s", nn)); //    ݱ        г
        if (len != 0) printText(layer._consoleDC, 300, 650, 60, 0, RGB(0, 0, 0), TA_LEFT, ("      ߴٸ     ͸       ּ   ."));


        pressedKey = _getch(); // Ű      Է¹ޱ

        if (pressedKey == 13) {
            if (len == 0) continue; //  Է¹              Ѿ    ʵ
            fprintf(fp, "%s\n", nn);
            selectGender(fp);
            break;
        }
        if (pressedKey == '\b') { //  齺   ̽
            if (len == 0) continue;
            len--;
            nn[len] = NULL;
            continue;
        }

        if (len > 13) { // 14      Ѿ       ̻   Ƚ       ϱ
            //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
            printText(layer._consoleDC, 100, 100, 60, 0, RGB(0, 0, 0), TA_LEFT, ("Exceeded 14 character."));
            while (3) {
                pressedKey = _getch();
                if (pressedKey == '\b') {
                    gotoxy(MAX_X / 3 + 5, MAX_Y / 3 * 2 - 2);
                    printf("                      ");
                    gotoxy(MAX_X / 2 - 6 + len, MAX_Y / 2);
                    break;
                }
            }
            continue;
        }
        nn[len] = pressedKey; //  迭    Է¹          ߰
        len++;


    }
}

void UserName(FILE *fp, char *nn) {
    initLayer();
    Image images[5] = { //     ׸
        {"resource/background/start_background.bmp", 0, 0}, //{ ̹     ̸ ,      x  ǥ,      y  ǥ, ũ       (             ⺻     16     )}
        {"resource/text/textarea.bmp", 150, 300}
    }; // 迭   ù    Ұ        Ʒ   ׷     .

    imageLayer.imageCount = 2; //images  迭   ũ ⺸    ۰ų     ƾ   Ѵ .
    imageLayer.images = images;

    imageLayer.renderAll(&imageLayer);

    char ch;
    int cnt = 0;
    while (fscanf(fp, "%c", &ch) != EOF) //      data    Ͽ   ̸     ִ    ( ҷ         Ͱ   ִ    Ȯ  )
        cnt++;
    if (cnt == 0) {
        newNickname(fp, nn, imageLayer); //           Ͱ                   ϱ
    }
    else {
        printText(imageLayer._consoleDC, 300, 500, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT("     ϱ  : 1"));
        printText(imageLayer._consoleDC, 300, 600, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT(" ҷ      :       "));
        if (getch() == '1') {//     ϱ⸦                                                 .
            fp = freopen("data/user.txt","w",fp);
            fp = freopen("data/user.txt","r+",fp);
            newNickname(fp, nn, imageLayer);
        }
        else {

        }
    }
}
/*********************            *********************************/
void selectGender(FILE *fp) {
    initLayer();
    Image images[4] = {
        {"resource/background/start_background.bmp", 0, 0}, //{ ̹     ̸ ,      x  ǥ,      y  ǥ, ũ       (             ⺻     16     )}
        {"resource/gender/character_girl.bmp", 656, 272},
        {"resource/gender/character_boy.bmp",  1136, 272},
        //{"resource/gender/character_girl_selected.bmp", 1288+150, 290+50},
        {"resource/gender/character_girl_selected.bmp", 640, 272-16}
    }; //      u    ?        ?   ?     .
    imageLayer.imageCount = 4;
    imageLayer.images = images;
    imageLayer.renderAll(&imageLayer);
    int key, select = 0;
    while(1) {
        key = getch();
        if (key == RIGHT || key == LEFT) {
            select++;
            select %= 2;
        }
        else if(key == 13) {
            switch(select) {
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
        case 0:
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

    }
}


/**********************            б ******************************/
void readData(FILE *fp, struct information *data) {
    fseek(fp,0,SEEK_SET);
    fscanf(fp,"%s\n",(*data).name);
    fscanf(fp,"%c\n",&(*data).gender);
    fseek(fp,-1,SEEK_END);
    fscanf(fp,"%c", &(*data).difficultyInformation);
    fseek(fp,0,SEEK_END);
}

/*********************             *********************************/

void selectStage(struct information *data) {
    initLayer();
    Image images[7] = {
        {"resource/background/start_background.bmp", 0, 0}, //{ ̹     ̸ ,      x  ǥ,      y  ǥ, ũ       (             ⺻     16     )}
        {"resource/difficulty/weekend_day.bmp", 296-150, 290+100},
        {"resource/difficulty/weekend_night.bmp", 792, 290+100},
        {"resource/difficulty/weekday.bmp", 1288+150, 290+100},
        {"resource/difficulty/selected.bmp", 296-166, 290-16+100}
    }; // 迭   ù    Ұ        Ʒ   ׷     .

    switch((*data).difficultyInformation) {
    case 'e':
        images[2].fileName = "resource/difficulty/weekend_night_locked.bmp";
    case 'n':
        images[3].fileName = "resource/difficulty/weekday_locked.bmp";
        break;
    }


    imageLayer.imageCount = 5;
    imageLayer.images = images;
    imageLayer.renderAll(&imageLayer);
    int key, select = 0;
    while(1) {
        key = getch();
        if (key == RIGHT) {
            select++;
        }
        else if (key == LEFT) {
            select += 2;
        }
        select %= 3;

        switch (select) {
        case 0:
            images[4].x = 296-166;
            imageLayer.renderAll(&imageLayer);
            break;
        case 1:
            images[4].x = 792-16;
            imageLayer.renderAll(&imageLayer);
            break;
        case 2:
            images[4].x = 1288+150-16;
            imageLayer.renderAll(&imageLayer);
            break;
        }

        if (key == ENTER) {
            if (images[select+1].fileName == "resource/difficulty/weekend_night_locked.bmp" ||images[select+1].fileName == "resource/difficulty/weekday_locked.bmp") {
                printText(imageLayer._consoleDC, 600, 1000, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT("                    ϴ ."));
            }
            else {
                (*data).nowDifficulty = select;  // 0 : easy, 1 : normal, 2 : hard
                break;
            }
        }


    }
}


/**********************   ӽ   **************************************/

/********************* ̷  (   ߱ )********************************/
void printTextMaze() { //         ؽ Ʈ ̷  (              )

    for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 135; j++) printf("%c",mazefield[i][j]);
            if (i != 39)printf("\n");
    }
}
int easyMaze() {
    initLayer();

    Image images[10] = {
        {"resource/maze/maze.bmp", 0, 0}, //{ ̹     ̸ ,      x  ǥ,      y  ǥ, ũ       (             ⺻     16     )}
        {"resource/maze/bottle.bmp", 0, 192,4},
        {"resource/maze/red_cap.bmp", 315, 175,6},
        {"resource/maze/yong_glasses.bmp", 780, 845,6},
        {"resource/maze/red_cap.bmp", 1260, 625,6},
        {"resource/maze/yong_glasses.bmp", 1725, 170,6},
        {"resource/maze/water.bmp", 1950, 170,3},
    }; // 迭   ù    Ұ        Ʒ   ׷     .

    imageLayer.imageCount = 10; //images  迭   ũ ⺸    ۰ų     ƾ   Ѵ .
    imageLayer.images = images;

    float Xcoord, Ycoord;

    int key, nowX = 0, nowY = 192;
    while (1) {

        key = getch();


        switch (key){
        case LEFT:
            if (mazefield[nowY/32][(nowX-16)/16]<0 || mazefield[nowY/32+1][(nowX-16)/16]<0 || mazefield[nowY/32+2][(nowX-16)/16]<0 || mazefield[(nowY+80)/32][(nowX-16)/16]<0 || mazefield[nowY/32][(nowX-16)/16] == 'x' || mazefield[nowY/32+1][(nowX-16)/16] == 'x' || mazefield[nowY/32+2][(nowX-16)/16] == 'x'|| mazefield[(nowY+80)/32][(nowX-16)/16] == 'x') break;
            images[1].x -= 16;
            nowX -= 16;
            break;
        case RIGHT:
            if (mazefield[nowY/32][(nowX+96)/16]>135 || mazefield[nowY/32][(nowX+96)/16] == 'x' || mazefield[nowY/32+1][(nowX+96)/16] == 'x' || mazefield[nowY/32+2][(nowX+96)/16] == 'x'|| mazefield[(nowY+80)/32][(nowX+96)/16] == 'x') break;
            images[1].x += 16;
            nowX += 16;
            break;
        case UP:
            if (mazefield[(nowY-16)/32][nowX/16] =='x'||mazefield[(nowY-16)/32][nowX/16+1] =='x'||mazefield[(nowY-16)/32][nowX/16+2] =='x'||mazefield[(nowY-16)/32][nowX/16+3] =='x'||mazefield[(nowY-16)/32][nowX/16+4] =='x'||mazefield[(nowY-16)/32][nowX/16+5] =='x') break;
            images[1].y -= 16;
            nowY -= 16;
            break;
        case DOWN:
            if (mazefield[(nowY+96)/32][nowX/16] =='x'||mazefield[(nowY+96)/32][nowX/16+1] =='x'||mazefield[(nowY+96)/32][nowX/16+2] =='x'||mazefield[(nowY+96)/32][nowX/16+3] =='x'||mazefield[(nowY+96)/32][nowX/16+4] =='x'||mazefield[(nowY+96)/32][nowX/16+5] =='x') break;
            images[1].y += 16;
            nowY += 16;
            break;
        }
        imageLayer.renderAll(&imageLayer);
        gotoxy(0,0);
        printf("%3.1f %3.1f",(float)nowX/16, (float)nowY/32);

        Xcoord = nowX/16.0; Ycoord = nowY/32.0;
        if ((Ycoord >= 5 && Ycoord <= 7 && (Xcoord == 26||Xcoord == 103))||(Ycoord >= 26 && Ycoord <= 28 && Xcoord == 55)||(Ycoord >= 19 && Ycoord <= 21 && Xcoord == 74)) {

            return gameFail();
        }
        if (Xcoord>= 121 && Xcoord <= 125 && Ycoord == 9.5) {

            return 2;
        }
    }

}
int maze(struct information data) {
    printf("%d",data.nowDifficulty);
    getch();
    switch (data.nowDifficulty) {
    case 0:
        return easyMaze();
        break;
    case 1:
     //   return normalMaze();
        break;
    case 2:
      //  return hardMaze();
        break;
    }
    return 2;
}
/*********************      (   Ա ) ***************************/




/***********************    ****************************************/

int gameFail() {
    system("cls");
    initLayer();
    Image images[6] = {
        {"resource/background/start_background.bmp", 0, 0}, //{ ̹     ̸ ,      x  ǥ,      y  ǥ, ũ       (             ⺻     16     )}
        {"resource/clear/quit_button.bmp",1400,500},
        {"resource/clear/quit_button_clicked.bmp",1400,500},
        {"resource/clear/restart_button.bmp",1400,800},
        {"resource/clear/restart_button_clicked.bmp",1400,800},
        {"resource/fail/report.bmp",160,50}
    }; //      u    ?        ?   ?     .
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



/**********************Ŭ    ,  ٽ  ϱ *****************************/
int gameClear(FILE *fp, struct information data) {
    if (data.difficultyInformation == 'e') fprintf_s(fp, "n");
    else if (data.difficultyInformation == 'n' && data.nowDifficulty == 1) fprintf_s(fp,"h");
    initLayer();
    Image images[5] = {
        {"resource/background/start_background.bmp", 0, 0}, //{ ̹     ̸ ,      x  ǥ,      y  ǥ, ũ       (             ⺻     16     )}
        {"resource/clear/quit_button.bmp",1400,500},
        {"resource/clear/quit_button_clicked.bmp",1400,500},
        {"resource/clear/restart_button.bmp",1400,800},
        {"resource/clear/restart_button_clicked.bmp",1400,800}
    }; //      u    ?        ?   ?     .
    imageLayer.imageCount = 5;
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

/**********************기숙사방*********************/

int count = 0; // 향수 뿌리기, 문 잠그기, 환풍기 틀기, 창문열기 중 최소 4개 해야함 아니면 탈락
int window = 0;
int perfume = 0;
int fan = 0;
int lock = 0;

void roomBack(struct information *data) {
    initLayer();
    Image images[4] = {
        {"resource/room_back/view1.bmp", 0, 0}, //{ ̹     ̸ ,      x  ǥ,      y  ǥ, ũ       (             ⺻     16     )}
        {"resource/room_back/left_arrow.bmp", 32, 560},
        {"resource/room_back/window_closed_day.bmp",  1008, 64},
        {"resource/room_back/perfume.bmp", 1072, 912},
    }; //      u    ?        ?   ?     .

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

    imageLayer.imageCount = 4;
    imageLayer.images = images;
    imageLayer.renderAll(&imageLayer);
    int key, select = 0;

    while(1) {
        key = getch();
        if (key == LEFT) {
            roomFront(&data);
        }

        switch (key) {
        case WINDOW:
            window %= 2;
            if(window == 0) {
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
            imageLayer.renderAll(&imageLayer);
            window++;
            break;
        case PERFUME:
            images[3].fileName = "resource/room_back/perfume_selected.bmp";
            imageLayer.renderAll(&imageLayer);
            Sleep(300); // sound
            images[3].fileName = "resource/room_back/perfume.bmp";
            imageLayer.renderAll(&imageLayer);
            perfume = 1;
            break;
        }
    }
}

void roomFront(struct information *data) {
    initLayer();
    Image images[5] = {
        {"resource/room_front/view2.bmp", 0, 0}, //{ ̹     ̸ ,      x  ǥ,      y  ǥ, ũ       (             ⺻     16     )}
        {"resource/room_front/right_arrow.bmp", 2036, 560},
        {"resource/room_front/fan.bmp", 912, 544},
        {"resource/room_front/perfume.bmp", 1136, 784},
        {"resource/room_front/door_handle.bmp", 736, 560},
    }; //      u    ?        ?   ?     .

    fan %= 2;
    if(fan == 1)
        images[2].fileName = "resource/room_front/fan_clicked.bmp";
    else
        images[2].fileName = "resource/room_front/fan.bmp";

    lock %= 2;
    if(lock == 1)
        images[4].fileName = "resource/room_front/lock_door.bmp";
    else
        images[4].fileName = "resource/room_front/door_handle.bmp";

    imageLayer.imageCount = 5;
    imageLayer.images = images;
    imageLayer.renderAll(&imageLayer);
    int key, select = 0;

    while(1) {
        key = getch();
        if (key == RIGHT) {
            roomBack(&data);
        }
        switch (key) {
        case FANBUTTON:
            fan %= 2;
            if(fan == 0)
                images[2].fileName = "resource/room_front/fan_clicked.bmp";
            else
                images[2].fileName = "resource/room_front/fan.bmp";
            imageLayer.renderAll(&imageLayer);
            fan++;
            break;
        case PERFUME:
            images[3].fileName = "resource/room_front/perfume_selected.bmp";
            imageLayer.renderAll(&imageLayer);
            Sleep(300); // sound
            images[3].fileName = "resource/room_front/perfume.bmp";
            imageLayer.renderAll(&imageLayer);
            perfume = 1;
            break;
        case LOCKDOOR:
            lock %= 2;
            if(lock == 0)
                images[4].fileName = "resource/room_front/lock_door.bmp";
            else
                images[4].fileName = "resource/room_front/door_handle.bmp";
            imageLayer.renderAll(&imageLayer);
            lock++;
            break;
        }

    }
}
