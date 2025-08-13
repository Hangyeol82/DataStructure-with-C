#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_SLOT 4
#define OK 1
#define FALSE 0

typedef struct Student
{
    int id;
    char name[10];
} Student;

typedef struct Bucket
{
    int local_depth;
    int cnt;
    Student slot[MAX_SLOT];
} Bucket;

typedef struct Directory
{
    int global_depth;
    int size;
    Bucket **address;
} Directory;

Directory ext_hash;

int hash_fun(int n, int k)
{
    return (n & ((1 << k) - 1));
}

int bucket_split(Bucket *cur, int index)
{
    if (cur->local_depth == ext_hash.global_depth)
    {
        int new_size = ext_hash.size * 2;

        Bucket **tmp = (Bucket **)realloc(ext_hash.address, sizeof(Bucket *) * new_size);
        if (tmp == NULL)
        {
            printf("Error in bucket_split()\n");
            exit(1);
        }
        ext_hash.address = tmp;

        for (int i = new_size / 2; i < new_size; i++)
        {
            ext_hash.address[i] = ext_hash.address[i - new_size / 2];
        }

        ext_hash.size = new_size;
        ext_hash.global_depth++;
    }

    int cur_idx = index;
    int new_idx = (1 << cur->local_depth) | index;
    cur->local_depth++;

    Bucket *new = (Bucket *)malloc(sizeof(Bucket));
    new->cnt = 0;
    new->local_depth = cur->local_depth;

    for (int i = new_idx; i < ext_hash.size; i += (1 << new->local_depth) - 1)
        ext_hash.address[i] = new;

    cur->cnt = 0;

    for (int i = 0; i < MAX_SLOT; i++)
    {
        int idx = hash_fun(cur->slot[i].id, cur->local_depth);
        if (idx == cur_idx)
        {
            cur->slot[cur->cnt++] = cur->slot[i];
        }
        else
        {
            new->slot[new->cnt++] = cur->slot[i];
        }
    }
}

int insert(int id, char name[])
{
    while (1)
    {
        int idx = hash_fun(id, ext_hash.global_depth);
        Bucket *cur = ext_hash.address[idx];
        if (cur->cnt >= MAX_SLOT)
        {
            bucket_split(cur, idx);
            continue;
        }

        cur->slot[cur->cnt].id = id;
        strcpy(cur->slot[cur->cnt++].name, name);
        break;
    }
}

int delete(int id)
{
    int idx = hash_fun(id, ext_hash.global_depth);
    Bucket *cur = ext_hash.address[idx];

    int check = 0;
    int i;

    for (i = 0; i < cur->cnt; i++)
    {
        if (cur->slot[i].id == id)
        {
            check = 1;
            break;
        }
    }

    if (check)
    {
        printf("Id: %d Name: %s is deleted\n", id, cur->slot[i].name);
        for (; i < cur->cnt - 1; i++)
        {
            cur->slot[i] = cur->slot[i + 1];
        }
        cur->cnt -= 1;
    }
    else
    {
        printf("There is no student whit such Id\n");
        return FALSE;
    }

    if (cur->cnt == 0) // 병합
    {
        check = 1 ^ (idx >> (cur->local_depth - 1)); // 맨 앞에 비트가 0인지 1인지 확인
        Bucket *live;

        if (!check)
        {
            live = ext_hash.address[idx + (1 << (cur->local_depth - 1))];
        }
        else
        {
            live = ext_hash.address[idx - (1 << (cur->local_depth - 1))];
        }

        for (int i = idx; i >= 0; i -= (1 << (cur->local_depth)))
        {
            ext_hash.address[i] = live;
        }
        live->local_depth--;

        check = 1;
        if (cur->local_depth == ext_hash.global_depth - 1) // global depth 줄일수 있는지 시도
        {
            for (int i = 0; i < ext_hash.size / 2; i++)
            {
                if (ext_hash.address[i] != ext_hash.address[i + ext_hash.size / 2]) // 절반 접어서 동일한지 확인
                {
                    check = 0;
                    break;
                }
            }
        }

        if (check) // global depth 줄이기
        {
            Bucket **tmp = (Bucket **)realloc(ext_hash.address, sizeof(Bucket *) * ext_hash.size);
            ext_hash.global_depth -= 1;
            ext_hash.size /= 2;
        }
    }
    return OK;
}

int retrieve(int id)
{
    int idx = hash_fun(id, ext_hash.global_depth);
    Bucket *cur = ext_hash.address[idx];

    int check = 0;
    int i;

    for (i = 0; i < cur->cnt; i++)
    {
        if (cur->slot[i].id == id)
        {
            check = 1;
            break;
        }
    }

    if (check)
        printf("Name: %s\n", cur->slot[i].name);
    else
        printf("There is no student with such Id\n");
}

static void dump_dir(void)
{
    printf("\n[DIR] g=%d size=%d\n", ext_hash.global_depth, ext_hash.size);
    for (int i = 0; i < ext_hash.size; ++i)
    {
        Bucket *b = ext_hash.address[i];
        printf("[%2d] -> %p", i, (void *)b);
        if (b)
        {
            printf(" (ld=%d,cnt=%d):", b->local_depth, b->cnt);
            for (int k = 0; k < b->cnt; ++k)
                printf(" %d", b->slot[k].id);
        }
        putchar('\n');
    }
    putchar('\n');
}

int main()
{
    ext_hash.global_depth = 1;
    ext_hash.size = 2;
    ext_hash.address = (Bucket **)malloc(sizeof(Bucket *) * ext_hash.size);
    if (ext_hash.address == NULL)
    {
        printf("Error in main()]\n");
        exit(1);
    }
    for (int i = 0; i < ext_hash.size; i++)
    {
        ext_hash.address[i] = (Bucket *)malloc(sizeof(Bucket));
        if (ext_hash.address[i] == NULL)
        {
            printf("Error in main()]\n");
            exit(1);
        }
        ext_hash.address[i]->local_depth = 1;
        ext_hash.address[i]->cnt = 0;
    }

    // int seq1[] = {0, 8, 16, 24, 32, 40, 48, 56, 64}; // 깊은 연속 분할
    // for (int i = 0; i < sizeof(seq1) / sizeof(seq1[0]); ++i)
    // {
    //     char n[10];
    //     snprintf(n, sizeof(n), "N%d", seq1[i]);
    //     insert(seq1[i], n);
    //     dump_dir();
    // }

    // int seq2[] = {1, 9, 17, 25, 33, 2, 10, 18, 26, 34, 3, 11, 19, 27, 35};
    // for (int i = 0; i < sizeof(seq2) / sizeof(seq2[0]); ++i)
    // {
    //     char n[10];
    //     snprintf(n, sizeof(n), "N%d", seq2[i]);
    //     insert(seq2[i], n);
    //     dump_dir();
    // }

    // retrieve(33);
    // retrieve(9999);
}