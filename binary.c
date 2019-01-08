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
int deleteStack[MAX], availableStack[NODE_NUM],deleteTop;
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
            printf("Trie memory full\n");
            exit(0);
        }
    if (availableTop == 0)
        return ++end;
    TYPE val;
    // fseek(fpStack, sizeof(TYPE)*(availableTop-1+1), SEEK_SET);
    // fread(&val, sizeof(TYPE),1,fpStack);
    val = stack[availableTop-1+1];
    
    if(val==1)
        {
            // fseek(fpStack, sizeof(TYPE)*(availableTop-2+1), SEEK_SET);
            // fread(&val, sizeof(TYPE),1,fpStack);
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
        // fseek(fp, sizeof(TYPE) * (curr * LEN + (str[i] - 'a')), SEEK_SET);
        // fread(&child, sizeof(TYPE), 1, fp);
        child = trie[curr*LEN + (str[i]-'a')];
        if (child)
        {
            curr = (child < 0) ? -child : child;
        }
        else
        {
            // fseek(fpCnt, sizeof(int)*curr, SEEK_SET);
            // fread(&child, sizeof(int),1,fpCnt);
            // child++;
            // fseek(fpCnt, sizeof(int)*curr, SEEK_SET);
            // fwrite(&child,sizeof(int),1,fpCnt);
            cnt[curr]++;
            // fseek(fp, sizeof(TYPE) * (curr * LEN + (str[i] - 'a')), SEEK_SET); //Important step. seek. allocate. write.
            // curr = nextAvailableNode();
            // fwrite(&curr, sizeof(TYPE), 1, fp);
            curr = trie[curr*LEN + (str[i]-'a')] = nextAvailableNode();
        }
    }
    // child = 0;
    // fseek(fp, sizeof(TYPE) * (curr * LEN + (str[i] - 'a')), SEEK_SET);
    // fread(&child, sizeof(TYPE), 1, fp);
    child = trie[curr*LEN + (str[i]-'a')];
    if ((child < 0))
        ;//printf("%s already present\n", str);
    else if ((child > 0))
    {
        //child *= -1;
        // fseek(fp, sizeof(TYPE) * (curr * LEN + (str[i] - 'a')), SEEK_SET);
        // fwrite(&child, sizeof(TYPE), 1, fp);
        trie[curr*LEN + (str[i]-'a')]*=-1;
        ;//printf("%s is inserted\n", str);
    }
    else
    {
        // fseek(fpCnt, sizeof(int)*curr, SEEK_SET);
        // fread(&child, sizeof(int),1,fpCnt);
        // child++;
        // fseek(fpCnt, sizeof(int)*curr, SEEK_SET);
        // fwrite(&child,sizeof(int),1,fpCnt);
        cnt[curr]++;
        // child = -nextAvailableNode();
        // fseek(fp, sizeof(TYPE) * (curr * LEN + (str[i] - 'a')), SEEK_SET);
        // fwrite(&child, sizeof(TYPE), 1, fp);
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
        // child = 0;
        // fseek(fp, sizeof(TYPE) * (curr * LEN + (str[i] - 'a')), SEEK_SET);
        // fread(&child, sizeof(TYPE), 1, fp);
        child = trie[curr*LEN + (str[i]-'a')];

        if (child)
            curr = (child < 0) * -child + (child >= 0) * child;
        else
        {
            ;//printf("%s not present\n", str);
            return;
        }
    }
    // child = 0;
    // fseek(fp, 4 * (curr * LEN + (str[i] - 'a')), SEEK_SET);
    // fread(&child, sizeof(TYPE), 1, fp);
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
        // child = 0;
        // fseek(fp, sizeof(TYPE) * (absolute(curr) * LEN + (str[i] - 'a')), SEEK_SET);
        // fread(&child, sizeof(TYPE), 1, fp);
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
    // child = 0;
    // fseek(fp, sizeof(TYPE) * (absolute(curr) * LEN + (str[i] - 'a')), SEEK_SET);
    // fread(&child, sizeof(TYPE), 1, fp);
    child=trie[absolute(curr)*LEN + (str[i]-'a')];
    if (child < 0)
    {
        // cnt1=0;
        // fseek(fpCnt, sizeof(int)*-child, SEEK_SET);
        // fread(&cnt1, sizeof(int),1,fpCnt);
        cnt1=cnt[-child];
        if (cnt1 > 0)
        {
            // child *= -1;
            // fseek(fp, sizeof(TYPE) * (absolute(curr) * LEN + (str[i] - 'a')), SEEK_SET);
            // fwrite(&child, sizeof(TYPE), 1, fp);
            trie[absolute(curr) * LEN + (str[i] - 'a')]=-child;
        }
        else
        {
            // child*=-1;
            // fseek(fpStack, sizeof(TYPE)*(availableTop+1), SEEK_SET);
            // fwrite(&child, sizeof(TYPE),1,fpStack);
            stack[availableTop+1]=child;
            availableTop++;
            deleteTop--;
            while (deleteTop >= 0)
            {
                // child = 0;
                // fseek(fp, sizeof(TYPE) * (absolute(curr) * LEN + (str[i] - 'a')), SEEK_SET);
                // fwrite(&child, sizeof(TYPE), 1, fp);
                trie[absolute(curr) * LEN + (str[i] - 'a')]=0;
                i--;
                // fseek(fpCnt, sizeof(int)*absolute(curr), SEEK_SET);
                // fread(&cnt1, sizeof(int),1,fpCnt);
                // cnt1--;
                // fseek(fpCnt, sizeof(int)*absolute(curr), SEEK_SET);
                // fwrite(&cnt1,sizeof(int),1,fpCnt);
                cnt[absolute(curr)]--;
                cnt1=cnt[absolute(curr)];
                if (cnt1 > 0 || curr < 0) //If must not delete, break and stop futher deletions
                    break;
                //fseek(fpStack, sizeof(TYPE)*(availableTop+1), SEEK_SET);    
                //fwrite(&curr, sizeof(TYPE),1,fpStack);
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
void printStack()       //To use if necessary
{
    for (int i = 0; i < availableTop; i++)
        printf("%d ", availableStack[i]);
    printf("---------> Available Stack\n");
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

    
    // fp = fopen("TrieBinary", "r+");
    // fseek(fp, 0, SEEK_SET);
    // fread(&end, sizeof(unsigned TYPE), 1, fp);

    // fpStack = fopen("TrieStack", "r+");
    // fseek(fpStack, 0, SEEK_SET);
    // fread(&availableTop, sizeof(unsigned TYPE), 1, fpStack);
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

    // fpCnt = fopen("TrieCnt", "r+");
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

    // for (int i = 1; i <=10; ++i) {
	// trie[i] = 2 * i; 
    // }


    FILE *fpin;
    char choice='a', str[MAX];
    //printf("Enter choice:\ninsert: i\nsearch: s\nd\ndelete: d\nexit: e\n");
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
            printf("Invalid choice\n");
        }
    }
    printf("\nend=%d\n", end);
    printf("availableTop=%d\n\n", availableTop);





    // fclose(fpCnt);
    if (munmap(trie, NODE_NUM*sizeof(TYPE)*LEN) == -1) {
	perror("Error un-mmapping the file");
    }
    close(trie_fd);

    // fseek(fpStack, 0, SEEK_SET);
    // fwrite(&availableTop, sizeof(unsigned TYPE), 1, fpStack);
    // fclose(fpStack);

    if (munmap(stack, NODE_NUM*sizeof(TYPE)) == -1) {
	perror("Error un-mmapping the file");
    }
    close(stack_fd);

    if (munmap(cnt, NODE_NUM*sizeof(TYPE)) == -1) {
	perror("Error un-mmapping the file");
    }
    close(cnt_fd);
    
    // fseek(fp, 0, SEEK_SET);
    // fwrite(&end, sizeof(unsigned TYPE), 1, fp);
    // fclose(fp);
    return 0;
}
