#include <stdio.h>
#include <string.h>
#include <math.h>

int main()
{
    short int a[10]={1,4,6,8,10,12,14,16,18,20},x=0,b[10]={0};
    FILE *fp = fopen("sample", "r+");
    //fread(b,sizeof(int),1,fp);
    fseek(fp,0,SEEK_SET);
    fwrite(&a,sizeof(a),1,fp);
    // fseek(fp,0,SEEK_SET);
    //fread(b,sizeof(b),1,fp);
    //printf("%d\n",b[3]);
    fclose(fp);
    return 0;
}
