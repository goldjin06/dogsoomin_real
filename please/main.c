#include "define.h"
//콘솔 픽셀사이즈 = 2160 * 1280


int main()
{
    ClearCursor();
    ResizeConsole();
    showTitle();
    FILE *fp = fopen("data/user.txt","r+");
    char nn[15] = {0, };
    //printBound();
    //DisplayStart();
    UserName(fp,nn);


    return 0;
}


