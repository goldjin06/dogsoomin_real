#include "define.h"
#include "title.h"
#include "ImageLayer.h"
ImageLayer imageLayer = { NULL, 0, RGB(0,0,0) , NULL, NULL,_initialize, _renderAll, _renderAndFadeIn, _renderAndFadeOut, NULL };
INPUT_RECORD rec;
DWORD dwNOER;
HANDLE CIN = 0;

void printTextWithAngle(HDC hdc, int x, int y, int size, int weight, int angle, COLORREF textColor, int align, char* text) { // 이미지 위에 글자 띄우는 함수
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

void printText(HDC hdc, int x, int y, int size, int weight, COLORREF textColor, int align, char* text) { // 이미지 위에 글자 띄우는 함수
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

    imageLayer.imageCount = 4; //images 배열의 크기보다 작거나 같아야 한다.
    imageLayer.images = images;

    imageLayer.renderAll(&imageLayer);
    int mouse_x, mouse_y, mouseOn = 0;
    int key;
    while (1) {
        key = getch();
        //mouse_x = rec.Event.MouseEvent.dwMousePosition.X; // X값 받아옴
        //mouse_y = rec.Event.MouseEvent.dwMousePosition.Y; // Y값 받아옴
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

/********************불러오기 / 새로하기 장면*************************/
void newNickname(FILE* fp, char* nn, ImageLayer layer) { // 새로 닉네임 만들기

    initLayer();

    Image images[5] = {
        {"resource/background/start_background.bmp", 0, 0}, //{이미지 이름, 시작 x좌표, 시작 y좌표, 크기 배율(쓰지 않으면 기본값인 16이 들어감)}
        {"resource/text/textarea.bmp", 150, 300}
    }; //배열의 첫 원소가 가장 아래 그려진다.
    imageLayer.renderAll(&imageLayer);
    int len = 0;
    char pressedKey;
    printText(layer._consoleDC, 300, 450, 60, 0, RGB(0, 0, 0), TA_LEFT, ("당신의 이름을 정해주세요. (8~14자의 영어와 특수기호만 가능합니다.)"));

    while (3) {
        //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

        initLayer();
        imageLayer.renderAll(&imageLayer);
        printText(layer._consoleDC, 300, 450, 60, 0, RGB(0, 0, 0), TA_LEFT, ("당신의 이름을 정해주세요.(8~14자의 영어만 가능합니다.)"));
        printText(layer._consoleDC, 300, 550, 60, 0, RGB(0, 0, 255), TA_LEFT, ("%s", nn)); // 지금까지 쓴 닉네임 출력
        if (len != 0) printText(layer._consoleDC, 300, 650, 60, 0, RGB(0, 0, 0), TA_LEFT, ("다 정했다면 엔터를 눌러주세요."));


        pressedKey = _getch(); // 키보드 입력받기

        if (pressedKey == 13) {
            if (len == 0) continue; // 입력받은게 없으면 넘어가지 않도록
            fprintf(fp, "%s\n", nn);
            selectGender(fp);
            break;
        }
        if (pressedKey == '\b') { // 백스페이스 누르면 지워지기
            if (len == 0) continue;
            len--;
            nn[len] = NULL;
            continue;
        }

        if (len > 13) { // 14글자 넘어가면 더 이상 안써지게 하기
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
        nn[len] = pressedKey; // 배열에 입력받은 문자 추가
        len++;


    }
}

void UserName(FILE *fp, char *nn) {
    initLayer();
    Image images[5] = { //배경 그리기
        {"resource/background/start_background.bmp", 0, 0}, //{이미지 이름, 시작 x좌표, 시작 y좌표, 크기 배율(쓰지 않으면 기본값인 16이 들어감)}
        {"resource/text/textarea.bmp", 150, 300}
    }; //배열의 첫 원소가 가장 아래 그려진다.

    imageLayer.imageCount = 2; //images 배열의 크기보다 작거나 같아야 한다.
    imageLayer.images = images;

    imageLayer.renderAll(&imageLayer);

    char ch;
    int cnt = 0;
    while (fscanf(fp, "%c", &ch) != EOF) // 지금 data 파일에 이름이 있는지 (불러올 데이터가 있는지 확인)
        cnt++;
    if (cnt == 0) {
        newNickname(fp, nn, imageLayer); //저장된 데이터가 없어 무조건 새로하기
    }
    else {
        printText(imageLayer._consoleDC, 300, 500, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT("새로하기 : 1"));
        printText(imageLayer._consoleDC, 300, 600, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT("불러오기 : 나머지"));
        if (getch() == '1') {//새로하기를 누르면 현재 적혀진내용을 지우고 새로 내용을 만든다.
            fp = freopen("data/user.txt","w",fp);
            fp = freopen("data/user.txt","r+",fp);
            newNickname(fp, nn, imageLayer);
        }
        else {

        }
    }
}
/*********************성별선택장면 *********************************/
void selectGender(FILE *fp) {
    initLayer();
    Image images[4] = {
        {"resource/background/start_background.bmp", 0, 0}, //{이미지 이름, 시작 x좌표, 시작 y좌표, 크기 배율(쓰지 않으면 기본값인 16이 들어감)}
        {"resource/gender/character_girl.bmp", 296-150, 290+50},
        {"resource/gender/character_boy.bmp", 792, 290+50},
        //{"resource/gender/character_girl_selected.bmp", 1288+150, 290+50},
        {"resource/gender/character_girl_selected.bmp", 296-166, 290+34}
    }; // 迭   ù    Ұ        Ʒ   ׷     .
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
            images[3].x = 296-166;
            images[3].fileName = "resource/gender/character_girl_selected.bmp";
            imageLayer.renderAll(&imageLayer);
            break;
        case 1:
            images[3].x = 792-16;
            images[3].fileName = "resource/gender/character_boy_selected.bmp";
            imageLayer.renderAll(&imageLayer);
            break;
        }

    }
}


/**********************데이터파일 읽기******************************/
void readData(FILE *fp, struct information *data) {
    fseek(fp,0,SEEK_SET);
    fscanf(fp,"%s\n",(*data).name);
    fscanf(fp,"%c\n",&(*data).gender);
    fseek(fp,-1,SEEK_END);
    fscanf(fp,"%c", &(*data).difficultyInformation);
}

/*********************스태이지 선택*********************************/

void selectStage(struct information *data) {
    initLayer();
    Image images[5] = {
        {"resource/background/start_background.bmp", 0, 0}, //{이미지 이름, 시작 x좌표, 시작 y좌표, 크기 배율(쓰지 않으면 기본값인 16이 들어감)}
        {"resource/difficulty/weekend_day.bmp", 296-150, 290+50},
        {"resource/difficulty/weekend_night.bmp", 792, 290+50},
        {"resource/difficulty/weekday.bmp", 1288+150, 290+50},
        {"resource/difficulty/selected.bmp", 296-166, 290+34}
    }; //배열의 첫 원소가 가장 아래 그려진다.

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
            select += 3;
            select--;
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
                printText(imageLayer._consoleDC, 600, 1000, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT("아직 선택할 수 없습니다."));
            }
            else {
                (*data).nowDifficulty == select;  // 0 : easy, 1 : normal, 2 : hard
                break;
            }
        }


    }
}


/**********************게임시작**************************************/
