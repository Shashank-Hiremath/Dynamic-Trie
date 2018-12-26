//                      gcc binary.c -lm -o binary;./binary
#include <stdio.h>
#include <string.h>
#include <math.h>
#define LEN 26
short int trie[(1 << 15)][26], cnt[(1 << 15)], deleteStack[1000], availableStack[(1 << 15)], deleteTop;
unsigned short int end, availableTop;
FILE *fp, *fpStack,*fpCnt;

int absolute(int x)
{
    return x - 2*(x<0)*x;
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
    printf("insertTrie:\n");
    int curr = 1, i,size=ftell(fp);
    short int child;
    for (i = 0; str[i + 1]; i++)
    // if (trie[curr][str[i] - 'a'])
    //     curr = (trie[curr][str[i] - 'a'] < 0) * -(trie[curr][str[i] - 'a']) + (trie[curr][str[i] - 'a'] >= 0) * trie[curr][str[i] - 'a'];
    // else
    // {
    //     cnt[curr]++;
    //     curr = trie[curr][str[i] - 'a'] = nextAvailableNode();
    //     printf("%c %d\n", str[i], curr);
    // }
    {
        child=0;
        fseek(fp, 2 * (curr * LEN + (str[i] - 'a')), SEEK_SET);
        //printf("PosRead=%ld ",ftell(fp));
        fread(&child, sizeof(short int), 1, fp);
        //printf("%d: child=%d ",i,child);
        if (child)//&&(2 * (curr * LEN + (str[i] - 'a'))<size))
            {
                curr=(child<0)?-child:child;
                printf("-->");
            }
            //curr = (child < 0) * -child + (child >= 0) * child;
        else
        {
            cnt[curr]++;
            fseek(fp, 2 * (curr * LEN + (str[i] - 'a')), SEEK_SET);     //Important step. seek. allocate. write.
            curr = nextAvailableNode();
            fwrite(&curr, sizeof(short int), 1, fp);
            //printf("str[i]=%c curr=%d pos=%ld\n", str[i], curr,ftell(fp));
        }
            printf("%c %d\n",str[i],curr);
    }
    //printf(" 1last=%d curr=%d  ",child,curr);
    // if (trie[curr][str[i] - 'a'] < 0)
    //     printf("%s already present\n", str);
    // else if (trie[curr][str[i] - 'a'] > 0)
    // {
    //     trie[curr][str[i] - 'a'] *= -1;
    //     printf("%s is inserted\n", str);
    // }
    // else
    // {
    //     cnt[curr]++;
    //     trie[curr][str[i] - 'a'] = -nextAvailableNode();
    //     printf("%c %d\n", str[i], trie[curr][str[i] - 'a']);
    //     printf("%s is inserted\n", str);
    // }
    child=0;
    fseek(fp, 2 * (curr * LEN + (str[i] - 'a')), SEEK_SET);
    fread(&child, sizeof(short int), 1, fp);
    //printf(" curr=%d child=%d ",curr,child);
    if ((child < 0))//&&(2 * (curr * LEN + (str[i] - 'a'))<size))
        printf("%s already present\n", str);
    else if ((child > 0))//&&(2 * (curr * LEN + (str[i] - 'a'))<size))
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
        //printf("nextAvbl=%d\n",curr);
        fseek(fp, 2 * (curr * LEN + (str[i] - 'a')), SEEK_SET);
        fwrite(&child, sizeof(short int), 1, fp);
        printf("%c %d\n", str[i], child);
        printf("%s is inserted\n", str);
    }
}

void searchTrie(char *str)
{
    printf("searchTrie:\n");
    int curr = 1, i;
    short int child;
    for (i = 0; str[i + 1]; i++)
    // if (trie[curr][str[i] - 'a'])
    //     curr = (trie[curr][str[i] - 'a'] < 0) * -(trie[curr][str[i] - 'a']) + (trie[curr][str[i] - 'a'] >= 0) * trie[curr][str[i] - 'a'];
    // else
    // {
    //     printf("%s not present\n", str);
    //     return;
    // }
    {
        child=0;
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
    child=0;
    fseek(fp, 2 * (curr * LEN + (str[i] - 'a')), SEEK_SET);
    fread(&child, sizeof(short int), 1, fp);
    if (child < 0)
        printf("%s is present\n", str);
    else
        printf("%s not present\n", str);
}

void deleteTrie(char *str)
{
    printf("deleteTrie:\n");
    int curr = 1, i;
    short int child;
    deleteTop = 0;
    deleteStack[deleteTop++] = 1;
    for (i = 0; str[i + 1]; i++)
    {

        // deleteStack[deleteTop++] = (curr<0)*-curr+(curr>=0)*curr;
        child=0;
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
    // if (trie[curr][str[i] - 'a'] < 0)
    // {
    //     if (cnt[trie[curr][str[i] - 'a']] > 0) //Remove only flag and retain node, for the sake of its other children
    //         trie[curr][str[i] - 'a'] *= -1;
    //     else
    //     {
    //         availableStack[availableTop++] = -trie[curr][str[i] - 'a'];
    //         while (deleteTop >= 0)
    //         {
    //             trie[curr][str[i] - 'a'] = 0;
    //             i--;
    //             cnt[curr]--;
    //             if (cnt[curr] > 0)
    //                 break;
    //             availableStack[availableTop++] = curr;
    //             deleteTop--;
    //             if (deleteTop >= 0)
    //                 curr = deleteStack[deleteTop];
    //         }
    //     }
    //     printf("%s deleted\n", str);
    // }
    //deleteStack[deleteTop++] = curr;
    child=0;
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
            // child = 0;
            // fseek(fp, 2 * (curr * LEN + (str[i] - 'a')), SEEK_SET);
            // fwrite(&child, sizeof(short int), 1, fp);
            deleteTop--;
            while (deleteTop >= 0)
            {
                // curr=deleteStack[deleteTop--];
                // cnt[(curr<0)*-curr+(curr>=0)*curr]--;
                // if ((curr<0) || (cnt[(curr<0)*-curr+(curr>=0)*curr] > 0))
                //     break;
                // fseek(fp, 2 * (curr * LEN + (str[i] - 'a')), SEEK_SET);
                // fwrite(&child, sizeof(short int), 1, fp);

                child = 0;
                fseek(fp, 2 * (absolute(curr) * LEN + (str[i] - 'a')), SEEK_SET);
                fwrite(&child, sizeof(short int), 1, fp);
                i--;
                cnt[absolute(curr)]--;
                if (cnt[absolute(curr)] > 0 || curr<0)          //If must not delete, break and stop futher deletions
                    break;
                availableStack[availableTop++] = absolute(curr);    //Else add to available stack. find location using parent and delete
                deleteTop--;
                if (deleteTop >= 0)
                    curr = deleteStack[deleteTop];
                // fseek(fp, 2 * (curr * LEN + (str[i] - 'a')), SEEK_SET);
                // fread(&child, sizeof(short int), 1, fp);
                // if(child<0)
                //     break;

            }
        }
        printf("%s deleted\n", str);
    }
    else
        printf("%s not present\n", str);
}
void printStack()
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
    printf("%d\n\n", end);
    //end = 1;

    fpStack = fopen("TrieStack", "r+");
    fseek(fpStack, 0, SEEK_SET);
    fread(&availableTop, sizeof(unsigned short int), 1, fpStack);
    printf("%d\n\n", availableTop);
    //availableTop = 0;
    fread(availableStack, sizeof(short int) * availableTop, 1, fpStack);

    fpCnt=fopen("TrieCnt","r+");
    fread(cnt,sizeof(short int)*(1<<15),1,fpCnt);

    //memset(trie, 0, sizeof(trie));
    //memset(cnt, 0, sizeof(cnt));

    // insertTrie("apple");
    // insertTrie("apple");
    // insertTrie("ball");
    // insertTrie("apple");
    // searchTrie("apple");
    // searchTrie("ball");
    // searchTrie("apple1");
    // printStack();
    // deleteTrie("apple");
    // printStack();
    // deleteTrie("ball");
    // printStack();
    // searchTrie("ball");
    // deleteTrie("apple1");
    // insertTrie("apple");
    // printStack();
    // searchTrie("apple");
    // insertTrie("ball");
    // printStack();
    insertTrie("car");
    insertTrie("cars");
    insertTrie("ball");
    insertTrie("balls");
    searchTrie("car");
    searchTrie("cars");
    searchTrie("balls");
    printStack();
    deleteTrie("cars");
    printStack();
    deleteTrie("ball");
    printStack();
    searchTrie("car");
    searchTrie("ball");
    searchTrie("cars");
    searchTrie("balls");
    deleteTrie("balls");
    printStack();
    insertTrie("ball");
    printStack();
    searchTrie("cars");
    insertTrie("ball");
    printStack();

    fseek(fpCnt, 0, SEEK_SET);
    fwrite(cnt,sizeof(short int)*(1<<15),1,fpCnt);
    fclose(fpCnt);
    
    fseek(fpStack, 0, SEEK_SET);
    fwrite(&availableTop, sizeof(unsigned short int), 1, fpStack);
    // printf("%d\n\n", availableTop);
    fwrite(availableStack, sizeof(short int) * availableTop, 1, fpStack);
    fclose(fpStack);

    fseek(fp, 0, SEEK_SET);
    fwrite(&end, sizeof(unsigned short int), 1, fp);
    fclose(fp);
    return 0;
}
