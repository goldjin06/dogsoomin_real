#include "define.h"
// 화면비율 == 2160 * 1280


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
        int status = clear(fp, data);
        switch (status) {
        case 0:
            break;
        case 1:
            continue;
        }
    }


    return 0;
}


