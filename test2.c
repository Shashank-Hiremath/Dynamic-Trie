#include <stdio.h>
#include <string.h>
#include <math.h>

int main()
{
    short int a[10]={1,4,6,8,10,12,14,16,18,20},x=220,b[10]={0};
    FILE *fp = fopen("sample", "r+");
    //fread(b,sizeof(int),1,fp);
    fseek(fp,0,SEEK_END);
    fseek(fp,ftell(fp),SEEK_SET);
    //fwrite(&a,sizeof(a),1,fp);
    // fseek(fp,0,SEEK_SET);
    //fread(b,sizeof(b),1,fp);
    fread(&x,sizeof(short int),1,fp);
    //fseek(fp,10,SEEK_SET);
    printf("%d %ld\n",x,ftell(fp));
    //printf("%d\n",b[3]);
    fclose(fp);
    return 0;
}
