//                      gcc binary.c -o binary;./binary
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#define LEN 26
#define MAX 100
#define TYPE int
#define NODE_NUM 10000000
int deleteStack[MAX], availableStack[NODE_NUM];
unsigned TYPE end, availableTop, deleteTop;
FILE *fp, *fpStack, *fpCnt;

int absolute(int x)
{
    return x - 2 * (x < 0) * x;
}
TYPE nextAvailableNode()
{
    //if stack is empty
    if(end>=NODE_NUM)
        {
            printf("Trie memory full\n");
            exit(0);
        }
    if (availableTop == 0)
        return ++end;
    if (availableStack[availableTop - 1] == 1)
        return availableStack[availableTop -= 2];
    return availableStack[--availableTop];
}

void insertTrie(char *str)
{
    int curr = 1, i;
    TYPE child;
    for (i = 0; str[i + 1]; i++)
    {
        child = 0;
        fseek(fp, sizeof(TYPE) * (curr * LEN + (str[i] - 'a')), SEEK_SET);
        fread(&child, sizeof(TYPE), 1, fp);
        if (child)
        {
            curr = (child < 0) ? -child : child;
        }
        else
        {
            fseek(fpCnt, sizeof(int)*curr, SEEK_SET);
            fread(&child, sizeof(int),1,fpCnt);
            child++;
            fseek(fpCnt, sizeof(int)*curr, SEEK_SET);
            fwrite(&child,sizeof(int),1,fpCnt);
            //cnt[curr]++;
            fseek(fp, sizeof(TYPE) * (curr * LEN + (str[i] - 'a')), SEEK_SET); //Important step. seek. allocate. write.
            curr = nextAvailableNode();
            fwrite(&curr, sizeof(TYPE), 1, fp);
        }
    }
    child = 0;
    fseek(fp, sizeof(TYPE) * (curr * LEN + (str[i] - 'a')), SEEK_SET);
    fread(&child, sizeof(TYPE), 1, fp);
    if ((child < 0))
        printf("%s already present\n", str);
    else if ((child > 0))
    {
        child *= -1;
        fseek(fp, sizeof(TYPE) * (curr * LEN + (str[i] - 'a')), SEEK_SET);
        fwrite(&child, sizeof(TYPE), 1, fp);
        printf("%s is inserted\n", str);
    }
    else
    {
        fseek(fpCnt, sizeof(int)*curr, SEEK_SET);
        fread(&child, sizeof(int),1,fpCnt);
        child++;
        fseek(fpCnt, sizeof(int)*curr, SEEK_SET);
        fwrite(&child,sizeof(int),1,fpCnt);
        //cnt[curr]++;
        child = -nextAvailableNode();
        fseek(fp, sizeof(TYPE) * (curr * LEN + (str[i] - 'a')), SEEK_SET);
        fwrite(&child, sizeof(TYPE), 1, fp);
        printf("%s is inserted\n", str);
    }
}

void searchTrie(char *str)
{
    int curr = 1, i;
    TYPE child;
    for (i = 0; str[i + 1]; i++)
    {
        child = 0;
        fseek(fp, sizeof(TYPE) * (curr * LEN + (str[i] - 'a')), SEEK_SET);
        fread(&child, sizeof(TYPE), 1, fp);
        if (child)
            curr = (child < 0) * -child + (child >= 0) * child;
        else
        {
            printf("%s not present\n", str);
            return;
        }
    }
    child = 0;
    fseek(fp, 4 * (curr * LEN + (str[i] - 'a')), SEEK_SET);
    fread(&child, sizeof(TYPE), 1, fp);
    if (child < 0)
        printf("%s is present\n", str);
    else
        printf("%s not present\n", str);
}

void deleteTrie(char *str)
{
    int curr = 1, i,cnt1;
    TYPE child;
    deleteTop = 0;
    deleteStack[deleteTop++] = 1;
    for (i = 0; str[i + 1]; i++)
    {
        child = 0;
        fseek(fp, sizeof(TYPE) * (absolute(curr) * LEN + (str[i] - 'a')), SEEK_SET);
        fread(&child, sizeof(TYPE), 1, fp);
        deleteStack[deleteTop++] = child;
        if (child)
            curr = child;
        else
        {
            printf("%s not present\n", str);
            return;
        }
    }
    child = 0;
    fseek(fp, sizeof(TYPE) * (absolute(curr) * LEN + (str[i] - 'a')), SEEK_SET);
    fread(&child, sizeof(TYPE), 1, fp);
    if (child < 0)
    {
        cnt1=0;
        fseek(fpCnt, sizeof(int)*-child, SEEK_SET);
        fread(&cnt1, sizeof(int),1,fpCnt);
        if (cnt1 > 0)
        {
            child *= -1;
            fseek(fp, sizeof(TYPE) * (absolute(curr) * LEN + (str[i] - 'a')), SEEK_SET);
            fwrite(&child, sizeof(TYPE), 1, fp);
        }
        else
        {
            availableStack[availableTop++] = -child;
            deleteTop--;
            while (deleteTop >= 0)
            {
                child = 0;
                fseek(fp, sizeof(TYPE) * (absolute(curr) * LEN + (str[i] - 'a')), SEEK_SET);
                fwrite(&child, sizeof(TYPE), 1, fp);
                i--;
                fseek(fpCnt, sizeof(int)*absolute(curr), SEEK_SET);
                fread(&cnt1, sizeof(int),1,fpCnt);
                cnt1--;
                fseek(fpCnt, sizeof(int)*absolute(curr), SEEK_SET);
                fwrite(&cnt1,sizeof(int),1,fpCnt);
                //cnt[absolute(curr)]--;
                if (cnt1 > 0 || curr < 0) //If must not delete, break and stop futher deletions
                    break;
                availableStack[availableTop++] = absolute(curr); //Else add to available stack. find location using parent and delete
                deleteTop--;
                if (deleteTop >= 0)
                    curr = deleteStack[deleteTop];
            }
        }
        printf("%s deleted\n", str);
    }
    else
        printf("%s not present\n", str);
}
void printStack()       //To use if necessary
{
    for (int i = 0; i < availableTop; i++)
        printf("%d ", availableStack[i]);
    printf("---------> Available Stack\n");
}
int main()
{
    fp = fopen("TrieBinary", "r+");
    fseek(fp, 0, SEEK_SET);
    fread(&end, sizeof(unsigned TYPE), 1, fp);

    fpStack = fopen("TrieStack", "r+");
    fseek(fpStack, 0, SEEK_SET);
    fread(&availableTop, sizeof(unsigned TYPE), 1, fpStack);
    fread(availableStack, sizeof(TYPE) * availableTop, 1, fpStack);

    fpCnt = fopen("TrieCnt", "r+");
    //fread(cnt, sizeof(TYPE) * NODE_NUM, 1, fpCnt);




    FILE *fpin;
    char choice, str[MAX];
    //printf("Enter choice:\ninsert: i\nsearch: s\nd\ndelete: d\nexit: e\n");
    while (choice != 'e')
    {
        scanf("%c", &choice);
        switch (choice)
        {
        case 'i':
            scanf("%s\n", str);
            if (str[0] == '#')
            {
                fpin = fopen(str + 1, "r");
                while (fscanf(fpin, "%s", str) != -1)
                    insertTrie(str);
                fclose(fpin);
            }
            else
                insertTrie(str);
            break;
        case 's':
            scanf("%s\n", str);
            if (str[0] == '#')
            {
                fpin = fopen(str + 1, "r");
                while (fscanf(fpin, "%s", str) != -1)
                    searchTrie(str);
                fclose(fpin);
            }
            else
                searchTrie(str);
            break;
        case 'd':
            scanf("%s\n", str);
            if (str[0] == '#')
            {
                fpin = fopen(str + 1, "r");
                while (fscanf(fpin, "%s", str) != -1)
                    deleteTrie(str);
                fclose(fpin);
            }
            else
                deleteTrie(str);
            break;
        case 'e':
            break;
        default:
            printf("Invalid choice\n");
        }
        //scanf("%c", &choice);
        //printf("\n--%c\n",choice);
    }
    printf("\nend=%d\n", end);
    printf("availableTop=%d\n\n", availableTop);





    //fseek(fpCnt, 0, SEEK_SET);
    //fwrite(cnt, sizeof(TYPE) * NODE_NUM, 1, fpCnt);
    fclose(fpCnt);

    fseek(fpStack, 0, SEEK_SET);
    fwrite(&availableTop, sizeof(unsigned TYPE), 1, fpStack);
    fwrite(availableStack, sizeof(TYPE) * availableTop, 1, fpStack);
    fclose(fpStack);

    fseek(fp, 0, SEEK_SET);
    fwrite(&end, sizeof(unsigned TYPE), 1, fp);
    fclose(fp);
    return 0;
}
