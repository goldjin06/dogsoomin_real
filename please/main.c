#include "define.h"
// ܼ   ȼ        = 2160 * 1280


int main()
{
    struct information data;

    ClearCursor();
    ResizeConsole();
    showTitle();
    FILE *fp = fopen("data/user.txt","r+");
    char nn[15] = {0, };
    //printBound();
    //DisplayStart();
    UserName(fp,nn);

    selectGender(fp);
    selectStage();

    readData(fp,&data);
    selectStage(&data);


    return 0;
}


