#include "define.h"
#include "title.h"
#include "ImageLayer.h"
ImageLayer imageLayer = { NULL, 0, RGB(0,0,0) , NULL, NULL,_initialize, _renderAll, _renderAndFadeIn, _renderAndFadeOut, NULL };
INPUT_RECORD rec;
DWORD dwNOER;
HANDLE CIN = 0;

void printTextWithAngle(HDC hdc, int x, int y, int size, int weight, int angle, COLORREF textColor, int align, char* text) { // �̹��� ���� ���� ���� �Լ�
	if (weight == 0) weight = 900;
	size = (int)(size * RESOLUTION_MULTIPLIER);
	const HFONT font = CreateFont(size, 0, angle, 0, weight, 0, 0, 0, HANGEUL_CHARSET,
		0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("����"));

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

void printText(HDC hdc, int x, int y, int size, int weight, COLORREF textColor, int align, char* text) { // �̹��� ���� ���� ���� �Լ�
	printTextWithAngle(hdc, x, y, size, weight, 0, textColor, align, text);
}

void initLayer() { // �̹������̾� �ʱ�ȭ
	imageLayer.initialize(&imageLayer);
	imageLayer.transparentColor = RGB(0, 255, 0);
}

/********************Ÿ��Ʋ ���*************************/
void showTitle() {
    Sleep(500);

    initLayer();
    Image images[7] = {
        {"resource/background/start_background.bmp", 0, 0}, //{�̹��� �̸�, ���� x��ǥ, ���� y��ǥ, ũ�� ����(���� ������ �⺻���� 16�� ��)}
        {"resource/title/start_button.bmp", 700, 500},
        {"resource/title/title_text.bmp", 250, 50},
    }; //�迭�� ù ���Ұ� ���� �Ʒ� �׷�����.

    imageLayer.imageCount = 4; //images �迭�� ũ�⺸�� �۰ų� ���ƾ� �Ѵ�.
    imageLayer.images = images;

    imageLayer.renderAll(&imageLayer);
    int mouse_x, mouse_y, mouseOn = 0;
    int key;
    while (1) {
        key = getch();
        //mouse_x = rec.Event.MouseEvent.dwMousePosition.X; // X�� �޾ƿ�
        //mouse_y = rec.Event.MouseEvent.dwMousePosition.Y; // Y�� �޾ƿ�
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

/********************�ҷ����� / �����ϱ� ���*************************/
void newNickname(FILE* fp, char* nn, ImageLayer layer) { // ���� �г��� �����

    initLayer();

    Image images[5] = {
        {"resource/background/start_background.bmp", 0, 0}, //{�̹��� �̸�, ���� x��ǥ, ���� y��ǥ, ũ�� ����(���� ������ �⺻���� 16�� ��)}
        {"resource/text/textarea.bmp", 150, 300}
    }; //�迭�� ù ���Ұ� ���� �Ʒ� �׷�����.
    imageLayer.renderAll(&imageLayer);
    int len = 0;
    char pressedKey;
    printText(layer._consoleDC, 300, 450, 60, 0, RGB(0, 0, 0), TA_LEFT, ("����� �̸��� �����ּ���. (8~14���� ����� Ư����ȣ�� �����մϴ�.)"));

    while (3) {
        //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

        initLayer();
        imageLayer.renderAll(&imageLayer);
        printText(layer._consoleDC, 300, 450, 60, 0, RGB(0, 0, 0), TA_LEFT, ("����� �̸��� �����ּ���.(8~14���� ��� �����մϴ�.)"));
        printText(layer._consoleDC, 300, 550, 60, 0, RGB(0, 0, 255), TA_LEFT, ("%s", nn)); // ���ݱ��� �� �г��� ���
        if (len != 0) printText(layer._consoleDC, 300, 650, 60, 0, RGB(0, 0, 0), TA_LEFT, ("�� ���ߴٸ� ���͸� �����ּ���."));


        pressedKey = _getch(); // Ű���� �Է¹ޱ�

        if (pressedKey == 13) {
            if (len == 0) continue; // �Է¹����� ������ �Ѿ�� �ʵ���
            fprintf(fp, "%s\n", nn);
            selectGender(fp);
            break;
        }
        if (pressedKey == '\b') { // �齺���̽� ������ ��������
            if (len == 0) continue;
            len--;
            nn[len] = NULL;
            continue;
        }

        if (len > 13) { // 14���� �Ѿ�� �� �̻� �Ƚ����� �ϱ�
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
        nn[len] = pressedKey; // �迭�� �Է¹��� ���� �߰�
        len++;


    }
}

void UserName(FILE *fp, char *nn) {
    initLayer();
    Image images[5] = { //��� �׸���
        {"resource/background/start_background.bmp", 0, 0}, //{�̹��� �̸�, ���� x��ǥ, ���� y��ǥ, ũ�� ����(���� ������ �⺻���� 16�� ��)}
        {"resource/text/textarea.bmp", 150, 300}
    }; //�迭�� ù ���Ұ� ���� �Ʒ� �׷�����.

    imageLayer.imageCount = 2; //images �迭�� ũ�⺸�� �۰ų� ���ƾ� �Ѵ�.
    imageLayer.images = images;

    imageLayer.renderAll(&imageLayer);

    char ch;
    int cnt = 0;
    while (fscanf(fp, "%c", &ch) != EOF) // ���� data ���Ͽ� �̸��� �ִ��� (�ҷ��� �����Ͱ� �ִ��� Ȯ��)
        cnt++;
    if (cnt == 0) {
        newNickname(fp, nn, imageLayer); //����� �����Ͱ� ���� ������ �����ϱ�
    }
    else {
        printText(imageLayer._consoleDC, 300, 500, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT("�����ϱ� : 1"));
        printText(imageLayer._consoleDC, 300, 600, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT("�ҷ����� : ������"));
        if (getch() == '1') {//�����ϱ⸦ ������ ���� ������������ ����� ���� ������ �����.
            fp = freopen("data/user.txt","w",fp);
            fp = freopen("data/user.txt","r+",fp);
            newNickname(fp, nn, imageLayer);
        }
        else {

        }
    }
}
/*********************����������� *********************************/
void selectGender(FILE *fp) {
    initLayer();
    Image images[4] = {
        {"resource/background/start_background.bmp", 0, 0}, //{�̹��� �̸�, ���� x��ǥ, ���� y��ǥ, ũ�� ����(���� ������ �⺻���� 16�� ��)}
        {"resource/gender/character_girl.bmp", 296-150, 290+50},
        {"resource/gender/character_boy.bmp", 792, 290+50},
        //{"resource/gender/character_girl_selected.bmp", 1288+150, 290+50},
        {"resource/gender/character_girl_selected.bmp", 296-166, 290+34}
    }; // ��   u    ?        ?   ?     .
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


/**********************���������� �б�******************************/
void readData(FILE *fp, struct information *data) {
    fseek(fp,0,SEEK_SET);
    fscanf(fp,"%s\n",(*data).name);
    fscanf(fp,"%c\n",&(*data).gender);
    fseek(fp,-1,SEEK_END);
    fscanf(fp,"%c", &(*data).difficultyInformation);
}

/*********************�������� ����*********************************/

void selectStage(struct information *data) {
    initLayer();
    Image images[5] = {
        {"resource/background/start_background.bmp", 0, 0}, //{�̹��� �̸�, ���� x��ǥ, ���� y��ǥ, ũ�� ����(���� ������ �⺻���� 16�� ��)}
        {"resource/difficulty/weekend_day.bmp", 296-150, 290+50},
        {"resource/difficulty/weekend_night.bmp", 792, 290+50},
        {"resource/difficulty/weekday.bmp", 1288+150, 290+50},
        {"resource/difficulty/selected.bmp", 296-166, 290+34}
    }; //�迭�� ù ���Ұ� ���� �Ʒ� �׷�����.

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
                printText(imageLayer._consoleDC, 600, 1000, 60, 0, RGB(0, 0, 0), TA_LEFT, TEXT("���� ������ �� �����ϴ�."));
            }
            else {
                (*data).nowDifficulty == select;  // 0 : easy, 1 : normal, 2 : hard
                break;
            }
        }


    }
}


/**********************���ӽ���**************************************/
/**********************Ŭ����, �ٽ��ϱ�*****************************/
int clear() {
    //fprintf
    initLayer();
    Image images[5] = {
        {"resource/background/start_background.bmp", 0, 0}, //{�̹��� �̸�, ���� x��ǥ, ���� y��ǥ, ũ�� ����(���� ������ �⺻���� 16�� ��)}
        {"resource/clear/quit_button.bmp",1400,500},
        {"resource/clear/quit_button_clicked.bmp",1400,500},
        {"resource/clear/restart_button.bmp",1400,800},
        {"resource/clear/restart_button_clicked.bmp",1400,800}
    }; // ��   u    ?        ?   ?     .
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
            switch(select) {
            case 0:
                printf("quit");
                return 0;
                break;
            case 1:
                printf("restart");
                return 1;
                break;
            }
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

