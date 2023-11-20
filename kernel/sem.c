/*
 *  linux/kernel/sem.c
 *
 *  (C) 2023 nitromelon
 *   hwc: lab6 sem
 */
#define __LIBRARY__
#include <asm/segment.h>
#include <asm/system.h>
#include <errno.h>
#include <linux/sched.h>
#include <linux/sem.h>
#include <string.h>
#include <unistd.h>

_syscall2(int, sem_open, const char *, name, unsigned int, value);
_syscall1(int, sem_wait, int, sem);
_syscall1(int, sem_post, int, sem);
_syscall1(int, sem_unlink, int, sem);

sem_t semtable[SEM_TABLE_LEN];

int sys_sem_open(const char * name, unsigned int value)
{
    char kern_name[32];
    char c;
    sem_t * sem = NULL;
    int name_size = 0;
    int i;
    int first_empty = -1;

    while ((c = get_fs_byte(name + name_size)) != '\0')
    {
        kern_name[name_size] = c;
        ++name_size;
        if (name_size >= SEM_NAME_LEN)
        {
            return NULL;
        }
    }
    kern_name[name_size] = '\0';

    for (i = 0; i < SEM_TABLE_LEN; ++i)
    {
        if (first_empty < 0 && semtable[i].name[0] == '\0')
        {
            first_empty = i;
            continue;
        }
        if (strcmp(kern_name, semtable[i].name) == 0)
        {
            return i;
        }
    }

    if (first_empty < 0)
    {
        return NULL;
    }
    sem = &semtable[first_empty];
    for (i = 0; i <= name_size; ++i) // also copy last '\0'
    {
        sem->name[i] = kern_name[i];
    }
    sem->value = value;
    return first_empty;
}

int sys_sem_wait(int sem)
{
    sem_t * s = &semtable[sem];
    cli();
    while (s->value <= 0)
        sleep_on(&s->queue);
    --s->value;
    sti();
    return 0;
}

int sys_sem_post(int sem)
{
    sem_t * s = &semtable[sem];
    cli();
    if (++s->value <= 1)
    {
        wake_up(&s->queue);
    }
    sti();
    return 0;
}

int sys_sem_unlink(int sem)
{
    semtable[sem].name[0] = '\0';
    return 0;
}
