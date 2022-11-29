#include "define.h"

void ClearCursor() // Ŀ       
{
    CONSOLE_CURSOR_INFO c;
    c.dwSize = 1;
    c.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&c);
}
void ViewCursor() // Ŀ       
{
    CONSOLE_CURSOR_INFO c;
    c.dwSize = 1;
    c.bVisible = TRUE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&c);
}
void ResizeConsole() //  ܼ  ũ       
{
    char temp[30];
    sprintf(temp,"mode con cols=%d lines=%d",MAX_X,MAX_Y);
    system(temp);
}
void gotoxy(int x, int y) // xy   ǥ  ̵ 
{
    COORD pos = {x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}
