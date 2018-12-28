//                      gcc binary.c -lm -o binary;./binary
#include <stdio.h>
#include <string.h>
#include <math.h>
#define LEN 26
#define MAX 100
short int trie[(1 << 15)][26], cnt[(1 << 15)], deleteStack[1000], availableStack[(1 << 15)], deleteTop;
unsigned short int end, availableTop;
FILE *fp, *fpStack, *fpCnt;

int absolute(int x)
{
    return x - 2 * (x < 0) * x;
}
short int nextAvailableNode()
{
    //if stack is empty
    if (availableTop == 0)
        return ++end;
    if (availableStack[availableTop - 1] == 1)
        return availableStack[availableTop -= 2];
    return availableStack[--availableTop];
}

void insertTrie(char *str)
{
    int curr = 1, i, size = ftell(fp);
    short int child;
    for (i = 0; str[i + 1]; i++)
    {
        child = 0;
        fseek(fp, 2 * (curr * LEN + (str[i] - 'a')), SEEK_SET);
        fread(&child, sizeof(short int), 1, fp);
        if (child)
        {
            curr = (child < 0) ? -child : child;
        }
        else
        {
            cnt[curr]++;
            fseek(fp, 2 * (curr * LEN + (str[i] - 'a')), SEEK_SET); //Important step. seek. allocate. write.
            curr = nextAvailableNode();
            fwrite(&curr, sizeof(short int), 1, fp);
        }
    }
    child = 0;
    fseek(fp, 2 * (curr * LEN + (str[i] - 'a')), SEEK_SET);
    fread(&child, sizeof(short int), 1, fp);
    if ((child < 0))
        printf("%s already present\n", str);
    else if ((child > 0))
    {
        child *= -1;
        fseek(fp, 2 * (curr * LEN + (str[i] - 'a')), SEEK_SET);
        fwrite(&child, sizeof(short int), 1, fp);
        printf("%s is inserted\n", str);
    }
    else
    {
        cnt[curr]++;
        child = -nextAvailableNode();
        fseek(fp, 2 * (curr * LEN + (str[i] - 'a')), SEEK_SET);
        fwrite(&child, sizeof(short int), 1, fp);
        printf("%s is inserted\n", str);
    }
}

void searchTrie(char *str)
{
    int curr = 1, i;
    short int child;
    for (i = 0; str[i + 1]; i++)
    {
        child = 0;
        fseek(fp, 2 * (curr * LEN + (str[i] - 'a')), SEEK_SET);
        fread(&child, sizeof(short int), 1, fp);
        if (child)
            curr = (child < 0) * -child + (child >= 0) * child;
        else
        {
            printf("%s not present\n", str);
            return;
        }
    }
    child = 0;
    fseek(fp, 2 * (curr * LEN + (str[i] - 'a')), SEEK_SET);
    fread(&child, sizeof(short int), 1, fp);
    if (child < 0)
        printf("%s is present\n", str);
    else
        printf("%s not present\n", str);
}

void deleteTrie(char *str)
{
    int curr = 1, i;
    short int child;
    deleteTop = 0;
    deleteStack[deleteTop++] = 1;
    for (i = 0; str[i + 1]; i++)
    {
        child = 0;
        fseek(fp, 2 * (absolute(curr) * LEN + (str[i] - 'a')), SEEK_SET);
        fread(&child, sizeof(short int), 1, fp);
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
    fseek(fp, 2 * (absolute(curr) * LEN + (str[i] - 'a')), SEEK_SET);
    fread(&child, sizeof(short int), 1, fp);
    if (child < 0)
    {
        if (cnt[-child] > 0)
        {
            child *= -1;
            fseek(fp, 2 * (absolute(curr) * LEN + (str[i] - 'a')), SEEK_SET);
            fwrite(&child, sizeof(short int), 1, fp);
        }
        else
        {
            availableStack[availableTop++] = -child;
            deleteTop--;
            while (deleteTop >= 0)
            {
                child = 0;
                fseek(fp, 2 * (absolute(curr) * LEN + (str[i] - 'a')), SEEK_SET);
                fwrite(&child, sizeof(short int), 1, fp);
                i--;
                cnt[absolute(curr)]--;
                if (cnt[absolute(curr)] > 0 || curr < 0) //If must not delete, break and stop futher deletions
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
    fread(&end, sizeof(unsigned short int), 1, fp);

    fpStack = fopen("TrieStack", "r+");
    fseek(fpStack, 0, SEEK_SET);
    fread(&availableTop, sizeof(unsigned short int), 1, fpStack);
    fread(availableStack, sizeof(short int) * availableTop, 1, fpStack);

    fpCnt = fopen("TrieCnt", "r+");
    fread(cnt, sizeof(short int) * (1 << 15), 1, fpCnt);




    FILE *fpin;
    char choice, str[MAX];
    //printf("Enter choice:\ninsert: i\nsearch: s\nd\ndelete: d\nexit: e\n");
    while (choice != 'e')
    {
        scanf("%c", &choice);
        switch (choice)
        {
        case 'i':
            scanf("%s", str);
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
            scanf("%s", str);
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
            scanf("%s", str);
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
        scanf("%c", &choice);
    }
    printf("\nend=%d\n", end);
    printf("availableTop=%d\n\n", availableTop);





    fseek(fpCnt, 0, SEEK_SET);
    fwrite(cnt, sizeof(short int) * (1 << 15), 1, fpCnt);
    fclose(fpCnt);

    fseek(fpStack, 0, SEEK_SET);
    fwrite(&availableTop, sizeof(unsigned short int), 1, fpStack);
    fwrite(availableStack, sizeof(short int) * availableTop, 1, fpStack);
    fclose(fpStack);

    fseek(fp, 0, SEEK_SET);
    fwrite(&end, sizeof(unsigned short int), 1, fp);
    fclose(fp);
    return 0;
}
