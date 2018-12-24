#include <stdio.h>
#include <string.h>
#include <math.h>

int main()
{
    short int a[10]={1,2,3,4,5,6,7,8,9,0},x=0,b[10]={0};
    unsigned short int end=1;
    FILE *fp = fopen("TrieBinary", "w+");
    FILE *fpStack = fopen("TrieStack","w+");
    FILE *fpCnt = fopen("TrieCnt","w+");
    //fread(b,sizeof(int),1,fp);
    //fseek(fp,10000,SEEK_SET);
    //fwrite(a,sizeof(a),1,fp);
    // fseek(fp,0,SEEK_SET);
    //fread(b,sizeof(b),1,fp);
    //printf("%d\n",b[3]);
    fseek(fp, 0, SEEK_SET);
    fwrite(&end, sizeof(unsigned short int), 1, fp);
    fclose(fpCnt);
    fclose(fpStack);
    fclose(fp);
    return 0;
}
