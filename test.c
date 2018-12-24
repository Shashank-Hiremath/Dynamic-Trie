#include <stdio.h>
#include <string.h>
#include <math.h>
short int trie[(1 << 15)][26], cnt[(1 << 15)];
unsigned short int end;
int deleteStack[1000],deleteTop,availableStack[100000],availableTop;


int main()
{
    short int a[10]={1,2,3,4,5,6,7,8,9,0},x=0,b[10]={0};
    FILE *fp = fopen("TrieBinary", "r+");
    fread(b,sizeof(int),1,fp);
    // fwrite(a,sizeof(a),1,fp);
    // fseek(fp,0,SEEK_SET);
    // fread(b,sizeof(b),1,fp);
    printf("%d\n",b[0]);
    fclose(fp);
    return 0;
}
