#ifndef _SEM_H
#define _SEM_H

#define SEM_NAME_LEN  32
#define SEM_TABLE_LEN 32

typedef struct semaphore
{
    char * name[32];
    int value;
    struct task_struct * queue;
} sem_t;
extern sem_t semtable[SEM_TABLE_LEN];

#endif
