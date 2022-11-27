#include "define.h"

void ClearCursor() // 커서 삭제
{
    CONSOLE_CURSOR_INFO c;
    c.dwSize = 1;
    c.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&c);
}
void ViewCursor() // 커서 보임
{
    CONSOLE_CURSOR_INFO c;
    c.dwSize = 1;
    c.bVisible = TRUE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&c);
}
void ResizeConsole() // 콘솔 크기 조정
{
    char temp[30];
    sprintf(temp,"mode con cols=%d lines=%d",MAX_X,MAX_Y);
    system(temp);
}
void gotoxy(int x, int y) // xy 좌표 이동
{
    COORD pos = {x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}
