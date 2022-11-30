#include "define.h"
/**
project name : Vision Passion Ramen
simple explanation : Let's eat ramen in domitary and don't be caught by teacher.
*/
int main()
{
    srand(time(NULL));

    system("chcp 65001"); // to save hanguel
    system("cls");

    struct information data;
    int window = 0;
    int perfume = 0;
    int fan = 0;
    int lock = 0;

    ClearCursor();
    ResizeConsole();
    showTitle();
    FILE *fp = fopen("data/user.txt","r+"); // save player's data
    char nn[15] = {0, };
    UserName(fp,nn);

    while (1) {
        readData(fp,&data);
        selectStage(&data);
        int status = maze(data);
        switch (status) {
        case 0:
            return 0;   // quit
        case 1:
            continue;   // menu(restart)
        case 2:
            break;      // go to next screen
        }

        int prepare = roomFront(&data,window,perfume,fan,lock);  // how much the player prepared of four(window, perfume, fan, lockdoor)

        status = eatRamen(&data, prepare);

        switch (status) {
        case 0:
            return 0;   // quit
        case 1:
            continue;   // menu(restart)
        case 2:
            break;      // stage clear
        }
        status = gameClear(fp, data);
        switch (status) {
        case 0:
            return 0;   // quit game
        case 1:
            continue;   // menu
       }
    }


    return 0;
}


