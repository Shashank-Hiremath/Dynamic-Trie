//                  gcc initialize.c -o initialize;./initialize
#include <stdio.h>
#include <string.h>
#include <math.h>
#define NODE_NUM 10000000
#define LEN 26

int main()
{
    unsigned short int end=1;
    FILE *fp = fopen("TrieBinary", "w+");
    FILE *fpStack = fopen("TrieStack","w+");
    FILE *fpCnt = fopen("TrieCnt","w+");
    fseek(fp, 0, SEEK_SET);
    fwrite(&end, sizeof(unsigned short int), 1, fp);
    fseek(fp, NODE_NUM*sizeof(int)*LEN, SEEK_SET);
    end=0;
    fwrite(&end, sizeof(unsigned short int), 1, fp);
    fseek(fpCnt, NODE_NUM*sizeof(int), SEEK_SET);
    fwrite(&end, sizeof(unsigned short int), 1, fpCnt);
    fseek(fpStack, NODE_NUM*sizeof(int), SEEK_SET);
    fwrite(&end, sizeof(unsigned short int), 1, fpStack);
    fclose(fpCnt);
    fclose(fpStack);
    fclose(fp);
    return 0;
}
