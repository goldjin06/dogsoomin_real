#include "define.h"
#include "title.h"
#include "ImageLayer.h"
ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;

void printTextWithAngle(HDC hdc, int x, int y, int size, int weight, int angle, COLORREF textColor, int align, char* text) {
	if (weight == 0) weight = 900;
	size = (int)(size * RESOLUTION_MULTIPLIER);
	const HFONT font = CreateFont(size, 0, angle, 0, weight, 0, 0, 0, HANGEUL_CHARSET,
		0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("굴림"));

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

void initLayer() { // 이미지레이어 초기화
	imageLayer.initialize(&imageLayer);
	imageLayer.transparentColor = RGB(0, 255, 0);
}

/********************타이틀 장면*************************/
void showTitle() {
    Sleep(500);

    initLayer();
    Image images[7] = {
        {"resource/background/start_background.bmp", 0, 0}, //{이미지 이름, 시작 x좌표, 시작 y좌표, 크기 배율(쓰지 않으면 기본값인 16이 들어감)}
        {"resource/title/start_button.bmp", 700, 500},
        {"resource/title/title_text.bmp", 250, 50},
    }; //배열의 첫 원소가 가장 아래 그려진다.

    images[3].isHide = TRUE;
    imageLayer.imageCount = 4; //images 배열의 크기보다 작거나 같아야 한다.
    imageLayer.images = images;

    imageLayer.renderAll(&imageLayer);

    while (1) {
        if(getch() == 13) break;
    }
    images[1].fileName = "resource/title/start_button_clicked.bmp";
    imageLayer.renderAll(&imageLayer);
    while (1) {
        if(getch() == 13) break;
    }
}

/********************불러오기 / 새로하기 장면*************************/
void newNickname(FILE* fp, char* nn, ImageLayer layer) {
    initLayer();

    Image images[5] = {
        {"resource/background/start_background.bmp", 0, 0}, //{이미지 이름, 시작 x좌표, 시작 y좌표, 크기 배율(쓰지 않으면 기본값인 16이 들어감)}
        {"resource/text/textarea.bmp", 150, 300}
    }; //배열의 첫 원소가 가장 아래 그려진다.
    imageLayer.renderAll(&imageLayer);
    int len = 0;
    char pressedKey;
    printText(layer._consoleDC, 300, 450, 60, 0, RGB(0, 0, 0), TA_LEFT, ("당신의 이름을 정해주세요.(8~14자의 영어만 가능합니다.)"));

    while (3) {
        //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

        initLayer();
        imageLayer.renderAll(&imageLayer);
        printText(layer._consoleDC, 300, 450, 60, 0, RGB(0, 0, 0), TA_LEFT, ("당신의 이름을 정해주세요.(8~14자의 영어만 가능합니다.)"));
        printText(layer._consoleDC, 300, 550, 60, 0, RGB(0, 0, 255), TA_LEFT, ("%s", nn));
        if (len != 0) printText(layer._consoleDC, 300, 650, 60, 0, RGB(0, 0, 0), TA_LEFT, ("다 정했다면 엔터를 눌러주세요."));


        pressedKey = _getch();

        if (pressedKey == 13) {
            fprintf(fp, "%s\n", nn);
            break;
        }
        if (pressedKey == '\b') {
            if (len == 0) continue;
            len--;
            nn[len] = NULL;
            continue;
        }

        if (len > 13) {
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
        nn[len] = pressedKey;
        len++;


    }
}

void UserName(FILE *fp, char *nn) {
    initLayer();
    Image images[5] = {
        {"resource/background/start_background.bmp", 0, 0}, //{이미지 이름, 시작 x좌표, 시작 y좌표, 크기 배율(쓰지 않으면 기본값인 16이 들어감)}
        {"resource/text/textarea.bmp", 150, 300}
    }; //배열의 첫 원소가 가장 아래 그려진다.

    imageLayer.imageCount = 2; //images 배열의 크기보다 작거나 같아야 한다.
    imageLayer.images = images;

    imageLayer.renderAll(&imageLayer);

    char ch;
    int cnt = 0;
    while (fscanf(fp, "%c", &ch) != EOF)
        cnt++;
    if (cnt == 0) {
        newNickname(fp, nn, imageLayer);
    }
    else {
        printText(imageLayer._consoleDC, 300, 500, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT("새로하기 : 1"));
        printText(imageLayer._consoleDC, 300, 600, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT("불러오기 : 나머지"));
        if (getch() == '1') {
            fp = freopen("data/user.txt","w",fp);
            fp = freopen("data/user.txt","r+",fp);
            newNickname(fp, nn, imageLayer);
        }
        else {

        }
    }
}



