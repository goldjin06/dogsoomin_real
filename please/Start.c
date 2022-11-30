#include "define.h"

/** @brief makes cursor disappear
* @return none
* @param none
*/
void ClearCursor() {
    CONSOLE_CURSOR_INFO c;
    c.dwSize = 1;
    c.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&c);
}

/** @brief change console size
* @return none
* @param none
*/
void ResizeConsole() {
    char temp[30];
    sprintf(temp,"mode con cols=%d lines=%d",MAX_X,MAX_Y);
    system(temp);
}

/** @brief move x,y coordinate
* @return none
* @param x : x-coordinate, y : y-coordinate
*/
void gotoxy(int x, int y) {
    COORD pos = {x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}
