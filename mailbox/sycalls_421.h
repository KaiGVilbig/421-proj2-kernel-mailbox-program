#ifndef SYSCALLS_421_H
#define SYSCALLS_421_H

#include "syscall.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/kernel.h>

#define __x64_sys_create_mbox_421 436
#define __x64_sys_resize_mbx_421 437
#define __x64_sys_delete_mbx_421 438
#define __x64_sys_write_mbx_421 439
#define __x64_sys_read_mbx_421 440
#define __x64_sys_size_mbx_421 441
#define __x64_sys_list_mbx_421 442
#define __x64_sys_count_mbx_421 443

long create_mbx_421(const char *name, unsigned long size){return syscall(__x64_sys_create_mbox_421, name, size);}
long resize_mbx_421(const char *name, unsigned long size){return syscall(__x64_sys_resize_mbx_421, name, size);}
long delete_mbx_421(const char *name){return syscall(__x64_sys_delete_mbx_421, name);}
long write_mbx_421(const char *name, const unsigned char *data, long offset, long n){return syscall(__x64_sys_write_mbx_421,name,data,offset,n);}
long read_mbx_421(const char *name, unsigned char *data, long offset, long n){return syscall(__x64_sys_read_mbx_421, name, data, offset, n);}
long size_mbx_421(const char *name){return syscall(__x64_sys_size_mbx_421, name);}
long list_mbx_421(char  **list, unsigned long max_entries){return syscall(__x64_sys_list_mbx_421, list, max_entries);}
long count_mbx_421(void){return syscall(__x64_sys_count_mbx_421);}

#endif