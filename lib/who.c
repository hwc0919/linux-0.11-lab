/*
 *  linux/kernel/who.c
 *
 *  (C) 2023 nitromelon
 *   hwc: lab3 syscall
 */
#define __LIBRARY__
#include <errno.h>
#include <unistd.h>
#include <asm/segment.h>

_syscall1(int,iam,const char *,name)
_syscall2(int,whoami,char *,buf,int,len)

char s_name[24];
int sys_iam(const char * name)
{
    int i;
    char c;
    if (!name)
    {
        return 0;
    }
    for (i = 0;;++i)
    {
        if (i >= sizeof(s_name))
        {
            return -EINVAL;
        }
        c = get_fs_byte(name++);
        s_name[i] = c;
        if (c == '\0')
        {
            return i;
        }
    }
    return i;
}

int sys_whoami(char * buf, int len)
{
    int i;
    char c;
    for (i = 0; i < len; ++i)
    {
        put_fs_byte(s_name[i], buf + i);
        if (s_name[i] == '\0')
        {
            return i;
        }
    }
    return -1;
}
