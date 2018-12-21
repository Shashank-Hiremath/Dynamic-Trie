#include <stdio.h>
#include <string.h>
#include <math.h>
short int trie[(1 << 15)][26], cnt[(1 << 15)];
unsigned short int end;

short int nextAvailableNode()
{
    //if stack is empty
    //if(top==0)
    end++;
    return end;
}

void insertTrie(char *str)
{
    //printf("%c\n",str[1]);
    int curr = 1, i;
    for (i = 0; str[i + 1]; i++)
        if (trie[curr][str[i] - '0'])
            curr = (trie[curr][str[i] - '0'] < 0) * -(trie[curr][str[i] - '0']) + (trie[curr][str[i] - '0'] >= 0) * trie[curr][str[i] - '0'];
        else
            curr = trie[curr][str[i] - '0'] = nextAvailableNode();
    if (trie[curr][str[i] - '0'] < 0)
        printf("%s already present\n", str);
    else if (trie[curr][str[i] - '0'] > 0)
        {
            trie[curr][str[i] - '0'] *= -1;
            cnt[curr]++;
            printf("%s is inserted\n", str);
        }
    else
        {
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
    for (i = 0; str[i + 1]; i++)
        if (trie[curr][str[i] - '0'])
            curr = (trie[curr][str[i] - '0'] < 0) * -(trie[curr][str[i] - '0']) + (trie[curr][str[i] - '0'] >= 0) * trie[curr][str[i] - '0'];
        else
        {
            printf("%s not present\n", str);
            return;
        }
    if (trie[curr][str[i] - '0'] < 0)
    {
        trie[curr][str[i] - '0'] *= -1;
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
    //length=atoi(fread(fp))
    end = 1;
    insertTrie("apple");
    insertTrie("apple");
    insertTrie("apple");
    insertTrie("apple");
    searchTrie("apple");
    searchTrie("apple1");
    deleteTrie("apple");
    deleteTrie("apple1");
    searchTrie("apple");
    //fwrite(fp,length);
    fclose(fp);
    return 0;
}
