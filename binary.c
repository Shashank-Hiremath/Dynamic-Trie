//                      gcc binary.c -o binary;./binary
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#define LEN 26
#define MAX 100
#define TYPE int
#define NODE_NUM 10000000
int deleteStack[MAX],deleteTop;
unsigned TYPE end, availableTop;
FILE *fp, *fpStack, *fpCnt;
int trie_fd,cnt_fd,stack_fd;
int *trie,*cnt,*stack;

int absolute(int x)
{
    return x - 2 * (x < 0) * x;
}
TYPE nextAvailableNode()
{
    //if stack is empty
    if(end>=NODE_NUM)
        {
            ;//printf("Trie memory full\n");
            exit(0);
        }
    if (availableTop == 0)
        return ++end;
    TYPE val;
    val = stack[availableTop-1+1];
    
    if(val==1)
        {
            val = stack[availableTop-2+1];
            availableTop-=2;
            return val;
        }
    availableTop--;
    return val;
}

void insertTrie(char *str)
{
    int curr = 1, i;
    TYPE child;
    for (i = 0; str[i + 1]; i++)
    {   
        child = trie[curr*LEN + (str[i]-'a')];
        if (child)
        {
            curr = (child < 0) ? -child : child;
        }
        else
        {
            cnt[curr]++;
            curr = trie[curr*LEN + (str[i]-'a')] = nextAvailableNode();
        }
    }
    child = trie[curr*LEN + (str[i]-'a')];
    if ((child < 0))
        ;//printf("%s already present\n", str);
    else if ((child > 0))
    {
        trie[curr*LEN + (str[i]-'a')]*=-1;
        ;//printf("%s is inserted\n", str);
    }
    else
    {
        cnt[curr]++;
        trie[curr*LEN + (str[i]-'a')] = -nextAvailableNode();
        ;//printf("%s is inserted\n", str);
    }
}

void searchTrie(char *str)
{
    int curr = 1, i;
    TYPE child;
    for (i = 0; str[i + 1]; i++)
    {
        child = trie[curr*LEN + (str[i]-'a')];

        if (child)
            curr = (child < 0) * -child + (child >= 0) * child;
        else
        {
            ;//printf("%s not present\n", str);
            return;
        }
    }
    child = trie[curr*LEN + (str[i]-'a')];
    if (child < 0)
        ;//printf("%s is present\n", str);
    else
        ;//printf("%s not present\n", str);
}

void deleteTrie(char *str)
{
    int curr = 1, i,cnt1;
    TYPE child;
    deleteTop = 0;
    deleteStack[deleteTop++] = 1;
    for (i = 0; str[i + 1]; i++)
    {
        child=trie[absolute(curr)*LEN + (str[i]-'a')];
        deleteStack[deleteTop++] = child;
        if (child)
            curr = child;
        else
        {
            ;//printf("%s not present\n", str);
            return;
        }
    }
    child=trie[absolute(curr)*LEN + (str[i]-'a')];
    if (child < 0)
    {
        cnt1=cnt[-child];
        if (cnt1 > 0)
        {
            trie[absolute(curr) * LEN + (str[i] - 'a')]=-child;
        }
        else
        {
            stack[availableTop+1]=child;
            availableTop++;
            deleteTop--;
            while (deleteTop >= 0)
            {
                trie[absolute(curr) * LEN + (str[i] - 'a')]=0;
                i--;
                cnt[absolute(curr)]--;
                cnt1=cnt[absolute(curr)];
                if (cnt1 > 0 || curr < 0)       //If must not delete, break and stop futher deletions
                    break;
                stack[availableTop+1]=curr;     //Else add to available stack. find location using parent and delete
                availableTop++;
                deleteTop--;
                if (deleteTop >= 0)
                    curr = deleteStack[deleteTop];
            }
        }
        ;//printf("%s deleted\n", str);
    }
    else
        ;//printf("%s not present\n", str);
}
int main()
{
    trie_fd = open("TrieBinary",O_RDWR);
     if (trie_fd == -1) {
	perror("Error opening file for writing");
	exit(EXIT_FAILURE);
    }

    trie = mmap(0, NODE_NUM*sizeof(TYPE)*LEN, PROT_READ | PROT_WRITE, MAP_SHARED, trie_fd, 0);
    if (trie == MAP_FAILED) {
	close(trie_fd);
	perror("Error mmapping the file");
	exit(EXIT_FAILURE);
    }

    stack_fd = open("TrieStack",O_RDWR);
    if (stack_fd == -1) {
	perror("Error opening file for writing");
	exit(EXIT_FAILURE);
    }

    stack = mmap(0, NODE_NUM*sizeof(TYPE), PROT_READ | PROT_WRITE, MAP_SHARED, stack_fd, 0);
    if (stack == MAP_FAILED) {
	close(stack_fd);
	perror("Error mmapping the file");
	exit(EXIT_FAILURE);
    }
    cnt_fd = open("TrieCnt",O_RDWR);
    if (cnt_fd == -1) {
	perror("Error opening file for writing");
	exit(EXIT_FAILURE);
    }

    cnt = mmap(0, NODE_NUM*sizeof(TYPE), PROT_READ | PROT_WRITE, MAP_SHARED, cnt_fd, 0);
    if (cnt == MAP_FAILED) {
	close(cnt_fd);
	perror("Error mmapping the file");
	exit(EXIT_FAILURE);
    }



    FILE *fpin;
    char choice='a', str[MAX];
    //;//printf("Enter choice:\ninsert: i\nsearch: s\nd\ndelete: d\nexit: e\n");
    while (choice != 'e')
    {
        scanf("%c", &choice);
        switch (choice)
        {
        case 'i':
            scanf(" %s\n", str);
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
            scanf(" %s\n", str);
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
            scanf(" %s\n", str);
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
            ;//printf("Invalid choice\n");
        }
    }
    ;//printf("\nend=%d\n", end);
    ;//printf("availableTop=%d\n\n", availableTop);





    if (munmap(trie, NODE_NUM*sizeof(TYPE)*LEN) == -1) {
	perror("Error un-mmapping the file");
    }
    close(trie_fd);

    if (munmap(stack, NODE_NUM*sizeof(TYPE)) == -1) {
	perror("Error un-mmapping the file");
    }
    close(stack_fd);

    if (munmap(cnt, NODE_NUM*sizeof(TYPE)) == -1) {
	perror("Error un-mmapping the file");
    }
    close(cnt_fd);

    return 0;
}
