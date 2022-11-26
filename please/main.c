#include "define.h"
// ȭ      == 2160 * 1280


int main()
{

    system("chcp 65001");
    system("cls");

    struct information data;
    int window = 0;
    int perfume = 0;
    int fan = 0;
    int lock = 0;

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
//        int status = maze(data);
//        switch (status) {
//        case 0:
//            return 0;
//        case 1:
//            continue;
//        case 2:
//            break;
//        }
        int prepare = roomFront(&data,window,perfume,fan,lock);  // how much the player prepared of four(window, perfume, fan, lockdoor)
        eatRamen(&data, prepare);
//        status = gameClear(fp, data);
//        switch (status) {
//        case 0:
//            return 0;
//        case 1:
//            continue;
//       }
    }


    return 0;
}


