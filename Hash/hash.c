#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TABLE_SIZE 101
#define OK 1
#define FAIL 0

typedef struct Entry
{
    int id;
    char name[10];
    struct Entry *next;
} Entry;

Entry *hash_table[MAX_TABLE_SIZE];

int hash_fun(int key)
{
    return key % MAX_TABLE_SIZE;
}

int insert(int id, char name[])
{
    int idx = hash_fun(id);

    Entry *new_student = (Entry *)malloc(sizeof(Entry));
    new_student->id = id;
    strcpy(new_student->name, name);

    if (hash_table[idx] == NULL)
    {
        hash_table[idx] = new_student;
    }
    else
    {
        Entry *cur;
        for (cur = hash_table[idx]->next; cur != NULL; cur = cur->next)
            ;

        cur = new_student;
    }

    return OK;
}

int delete(int id)
{
    int idx = hash_fun(id);

    Entry *cur;
    Entry *pre = NULL;
    for (cur = hash_table[idx]; cur != NULL; pre = cur, cur = cur->next)
    {
        if (cur->id == id)
        {
            break;
        }
    }

    if (cur == NULL)
    {
        printf("There is no student with that Id\n");
        return FAIL;
    }

    if (pre == NULL)
    {
        hash_table[idx] = cur->next;
        free(cur);
    }
    else
    {
        pre->next = cur->next;
        free(cur);
    }

    return OK;
}

int retrieve(int id)
{
    int idx = hash_fun(id);

    Entry *cur;
    for (cur = hash_table[idx]; cur != NULL; cur = cur->next)
    {
        if (cur->id == id)
        {
            break;
        }
    }

    if (cur == NULL)
    {
        printf("There is no student with that Id\n");
        return FAIL;
    }
    else
    {
        printf("Id: %d\n", cur->id);
        printf("Name: %s\n", cur->name);
    }

    return OK;
}

int main()
{
    insert(101, "A");
    retrieve(101);
    delete(101);
    retrieve(101);
}