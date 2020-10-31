typedef struct mbx
{
	char name[64];
	char *data;
	int dataSize;
	struct mbx *next;
}mbx;

mbx* findMbx(const char *name);
//void printList();
//long SYSCALL_DEFINE2(create_mbx_421, const char __user*, name, unsigned long, size);
//long SYSCALL_DEFINE2(resize_mbx_421, const char *name, unsigned long size);
//long SYSCALL_DEFINE1(delete_mbx_421, const char *name);
//long SYSCALL_DEFINE4(write_mbx_421, const char *name, const unsigned char *data, unsigned long offset, unsigned long n);
//long SYSCALL_DEFINE4(read_mbx_421, const char *name, unsigned char *data, unsigned long offset, unsigned long n);
//long SYSCALL_DEFINE1(size_mbx_421, const char *name);
//long SYSCALL_DEFINE2(list_mbx_421, char **list, unsigned long max_entries);
//long SYSCALL_DEFINE0(count_mbx_421);
