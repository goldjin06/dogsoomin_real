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
//void DisplayStart()
//{
//    while(1)
//    {
//        if(_kbhit())
//        {
//            _getch();
//            break;
//        }
//        gotoxy(MAX_X/2-3,MAX_Y/2);
//        printf("tetris");
//        Sleep(500);
//        gotoxy(MAX_X/2-3,MAX_Y/2);
//        printf("      ");
//        Sleep(500);
//
//    }
//}
//void printBound(){
//
//    gotoxy(0, 0);
//    for(int y = 0; y < MAX_Y; y++)
//    {
//        for(int x = 0; x < MAX_X; x+=2)
//        {
//            if(x == 0 || x == MAX_X-2 || y == 0 ||y == MAX_Y-1) {
//                printf("??");
//            }
//            else
//            {
//                printf("  ");
//            }
//
//        }
//    }
//}

