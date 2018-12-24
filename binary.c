#include <stdio.h>
#include <string.h>
#include <math.h>
short int trie[(1 << 15)][26], cnt[(1 << 15)], deleteStack[1000], availableStack[(1 << 15)], deleteTop;
unsigned short int end, availableTop;
FILE *fp, *fpStack;

short int nextAvailableNode()
{
    //if stack is empty
    if (availableTop == 0)
        return ++end;
    if(availableStack[availableTop-1]==1)
        return availableStack[availableTop-=2];
    return availableStack[--availableTop];
}

void insertTrie(char *str)
{
    int curr = 1, i;
    for (i = 0; str[i + 1]; i++)
        if (trie[curr][str[i] - '0'])
            curr = (trie[curr][str[i] - '0'] < 0) * -(trie[curr][str[i] - '0']) + (trie[curr][str[i] - '0'] >= 0) * trie[curr][str[i] - '0'];
        else
        {
            cnt[curr]++;
            curr = trie[curr][str[i] - '0'] = nextAvailableNode();
            printf("%c %d\n", str[i], curr);
        }
    if (trie[curr][str[i] - '0'] < 0)
        printf("%s already present\n", str);
    else if (trie[curr][str[i] - '0'] > 0)
    {
        trie[curr][str[i] - '0'] *= -1;
        printf("%s is inserted\n", str);
    }
    else
    {
        cnt[curr]++;
        trie[curr][str[i] - '0'] = -nextAvailableNode();
        printf("%c %d\n", str[i], trie[curr][str[i] - '0']);
        printf("%s is inserted\n", str);
    }
}

void searchTrie(char *str)
{
    int curr = 1, i;
    for (i = 0; str[i + 1]; i++)
        if (trie[curr][str[i] - '0'])
            curr = (trie[curr][str[i] - '0'] < 0) * -(trie[curr][str[i] - '0']) + (trie[curr][str[i] - '0'] >= 0) * trie[curr][str[i] - '0'];
        else
        {
            printf("%s not present\n", str);
            return;
        }
    if (trie[curr][str[i] - '0'] < 0)
        printf("%s present\n", str);
    else
        printf("%s not present\n", str);
}

void deleteTrie(char *str)
{
    int curr = 1, i;
    deleteTop = 0;
    for (i = 0; str[i + 1]; i++)
    {

        deleteStack[deleteTop++] = curr;

        if (trie[curr][str[i] - '0'])
            curr = (trie[curr][str[i] - '0'] < 0) * -(trie[curr][str[i] - '0']) + (trie[curr][str[i] - '0'] >= 0) * trie[curr][str[i] - '0'];
        else
        {
            printf("%s not present\n", str);
            return;
        }
    }
    if (trie[curr][str[i] - '0'] < 0)
    {

        if (cnt[trie[curr][str[i] - '0']] > 0) //Remove only flag and retain node, for the sake of its other children
            trie[curr][str[i] - '0'] *= -1;
        else
        {
            availableStack[availableTop++] = -trie[curr][str[i] - '0'];
            while (deleteTop >= 0)
            {
                trie[curr][str[i] - '0'] = 0;
                i--;
                cnt[curr]--;
                if(cnt[curr]>0)
                    break;
                availableStack[availableTop++] = curr;
                deleteTop--;
                if (deleteTop>=0)
                    curr = deleteStack[deleteTop];
            }
        }
        printf("%s deleted\n", str);
    }
    else
        printf("%s not present\n", str);
}
void printStack()
{
    for(int i=0;i<availableTop;i++)
        printf("%d ",availableStack[i]);
    printf("---------\n");
}
int main()
{
    fp = fopen("TrieBinary", "r+");
    fread(&end, sizeof(unsigned short int), 1, fp);
    printf("%d\n\n", end);
    //end = 1;
    fpStack = fopen("TrieStack", "r+");
    fread(&availableTop, sizeof(unsigned short int), 1, fpStack);
    printf("%d\n\n", availableTop);
    //availableTop = 0;
    fread(&availableStack, sizeof(short int) * availableTop, 1, fpStack);
    memset(trie, 0, sizeof(trie));
    memset(cnt, 0, sizeof(cnt));

    insertTrie("apple");
    insertTrie("apple");
    insertTrie("ball");
    insertTrie("apple");
    searchTrie("apple");
    searchTrie("ball");
    searchTrie("apple1");
    printStack();
    deleteTrie("apple");
    printStack();
    deleteTrie("ball");
    printStack();
    searchTrie("ball");
    deleteTrie("apple1");
    insertTrie("apple");
    printStack();
    searchTrie("apple");
    insertTrie("ball");
    printStack();

    fseek(fpStack, 0, SEEK_SET);
    fwrite(&availableTop, sizeof(unsigned short int), 1, fpStack);
    printf("%d\n\n", availableTop);
    fwrite(&availableStack, sizeof(short int) * availableTop, 1, fpStack);
    fseek(fp, 0, SEEK_SET);
    fwrite(&end, sizeof(unsigned short int), 1, fp);
    fclose(fp);
    return 0;
}
