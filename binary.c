#include <stdio.h>
#include <string.h>
#include <math.h>
short int trie[(1 << 15)][26], cnt[(1 << 15)];
unsigned short int end;
int deleteStack[1000], deleteTop, availableStack[100000], availableTop;

short int nextAvailableNode()
{
    //if stack is empty
    if(availableTop==0)
        return ++end;
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
        while (deleteTop>=0)
        {
            if (cnt[curr] > 1) //Remove only flag and retain node, for the sake of its other children
            {
                trie[curr][str[i] - '0'] *= -1;
                break;
            }
            else
            {
                trie[curr][str[i] - '0'] = 0;
                availableStack[availableTop++] = curr;
                deleteTop--;
                if(deleteTop)
                    curr = deleteStack[deleteTop];
            }
        }
        printf("%s deleted\n", str);
    }
    else
        printf("%s not present\n", str);
}

int main()
{
    FILE *fp = fopen("TrieBinary", "w");
    //printf("%d\n",sizeof(trie[0][0]));
    memset(trie, 0, sizeof(trie));
    memset(cnt, 0, sizeof(cnt));
    availableTop=0;
    //length=atoi(fread(fp))
    end = 1;
    insertTrie("apple");
    insertTrie("apple");
    insertTrie("ball");
    insertTrie("apple");
    searchTrie("apple");
    searchTrie("ball");
    searchTrie("apple1");
    deleteTrie("apple");
    printf("%d\n",availableTop);
    deleteTrie("ball");
    printf("%d\n",availableTop);
    searchTrie("ball");
    deleteTrie("apple1");
    insertTrie("apple");
    searchTrie("apple");
    printf("%d\n",availableTop);
    //fwrite(fp,length);
    fclose(fp);
    return 0;
}
