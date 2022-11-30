#include "define.h"

/** @brief
* @return none
* @param none
*/
void ClearCursor() // Ŀ
{
    CONSOLE_CURSOR_INFO c;
    c.dwSize = 1;
    c.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&c);
}

/** @brief page of selecting character's gender
* @return none
* @param none
*/
void ResizeConsole() //  ܼ  ũ
{
    char temp[30];
    sprintf(temp,"mode con cols=%d lines=%d",MAX_X,MAX_Y);
    system(temp);
}

/** @brief page of selecting character's gender
* @return none
* @param x : , y :
*/
void gotoxy(int x, int y) // xy   ǥ  ̵
{
    COORD pos = {x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}
