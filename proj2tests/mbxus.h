#include <stdlib.h>
#include <stdio.h>

struct mbx
{
	char name[64];
	char *data;
	int dataSize;
	struct mbx *next;
};
typedef struct mbx node_t;

node_t* findMbx(const char *name);
void printList();
long create(const char *name, unsigned long size);
long resize(const char *name, unsigned long size);
long deleteMbx(const char *name);
long write(const char *name, const unsigned char *data, unsigned long offset, unsigned long n);
long read(const char *name, unsigned char *data, unsigned long offset, unsigned long n);
long size(const char *name);
long list(char **list, unsigned long max_entries);
long count();