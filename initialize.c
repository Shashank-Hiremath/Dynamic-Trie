//                  gcc initialize.c -o initialize;./initialize
#include <stdio.h>
#include <string.h>
#include <math.h>

int main()
{
    unsigned short int end=1;
    FILE *fp = fopen("TrieBinary", "w+");
    FILE *fpStack = fopen("TrieStack","w+");
    FILE *fpCnt = fopen("TrieCnt","w+");
    fseek(fp, 0, SEEK_SET);
    fwrite(&end, sizeof(unsigned short int), 1, fp);
    fseek(fpCnt, 100000000, SEEK_SET);
    end=0;
    fwrite(&end, sizeof(unsigned short int), 1, fp);
    fclose(fpCnt);
    fclose(fpStack);
    fclose(fp);
    return 0;
}
