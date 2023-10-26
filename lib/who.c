/*
 *  linux/kernel/who.c
 *
 *  (C) 2023 nitromelon
 *   hwc: lab3 syscall
 */
#define __LIBRARY__
#include <errno.h>
#include <unistd.h>

_syscall1(int,iam,const char *,name)
_syscall2(int,whoami,char *,buf,int,len)

char s_name[32];
int sys_iam(const char * name)
{
    int i;
    if (!name)
    {
        return 0;
    }
    for (i = 0; i < sizeof(name) - 1; ++i)
    {
        if (name[i] == '\0')
        {
            return 0;
        }
        s_name[i] = name[i];
    }
    return 0;
}

int sys_whoami(char * buf, int len)
{
    int i;
    for (i = 0; i < len; ++i)
    {
        buf[i] = s_name[i];
        if (s_name[i] == '\0')
        {
            return 0;
        }
    }
    return 0;
}
