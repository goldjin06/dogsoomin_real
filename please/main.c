#include "define.h"
// ȭ      == 2160 * 1280


int main()
{

    system("chcp 65001");
    system("cls");

    struct information data;

    ClearCursor();
    ResizeConsole();
    showTitle();
    FILE *fp = fopen("data/user.txt","r+");
    char nn[15] = {0, };
    //printBound();
    //DisplayStart();
    UserName(fp,nn);

    while (1) {
        readData(fp,&data);
        selectStage(&data);
        int status = maze(data);
        switch (status) {
        case 0:
            return 0;
        case 1:
            continue;
        case 2:
            break;
        }
        roomFront(&data);
        status = gameClear(fp, data);
        switch (status) {
        case 0:
            return 0;
        case 1:
            continue;
       }
    }


    return 0;
}


