#include <stdio.h>
#include <stdlib.h>
#include "mbxus.h"

node_t *head = NULL;

node_t* findMbx(const char *name)
{
	int size = 0;

	node_t *it = head;

	int t = 1;
	int end = 1;

	while (t != 0)
	{
		end = 1;
		for (int i = 0; i < 64; i++)
		{
			//since the cur->name is set to 64, after '\0' the array will be same which will set the same flag
			//therefor, once '\0' is found, we dont check anymore
			if (name[i] != '\0' && end)
			{
				//have to check one character at a time since it stores "a" as 0x0stuff"a" and therefor
				//curr->name and name will be different before the "a" and wont ever be equal
				if (it->name[i] == name[i])
				{
					t = 0;
				}
				else
				{
					t = 1;
				}
			}
			else
			{
				end = 0;
			}
		}
		//since prev is already set to head and curr is prev->next, in the first iteration prev and curr do not have to change
		if (t != 0)
		{
			it = it->next;
		}
	}

	return it;
}

void printList()
{
	node_t *it;
	it = head;
	while(it != NULL)
	{
		printf("mbx name: %s, ", it->name);
		printf("data size: %d bytes, ", it->dataSize);
		printf("data in mbx: ");

		//this allows for null terminators to not affect printing what is after the null terminator
		for (int i = 0; i < it->dataSize; i++)
		{
			printf("%c", it->data[i]);
		}
		printf("\n");

		it = it->next;
	}
	printf("\n");
}

long create(const char *name, unsigned long size)
{
	//create the new mailbox
	node_t *current = malloc(sizeof(node_t));

	for(int i = 0; i <= 64; i++)
	{
		current->name[i] = name[i];
	}

    current->data = (char*)malloc(size);
	current->dataSize = size;

	//0 the array out so that there is not random stuff in the array at start
	for (unsigned int i = 0; i < size; i++)
	{
		current->data[i] = 0;
	}

    current->next = NULL;

	//put it in the list
	//case where list is empty
	if(head == NULL)
	{
		head = current;
	}else
	{
	        node_t *it;
	        it = head;

		while(it->next != NULL)
		{
			it = it->next;
		}
		it->next = current;
	}

	return 0;
}

long resize(const char *name, unsigned long size)
{
	node_t *it;
	it = findMbx(name);

	it->data = (char*)realloc(it->data, size);
	it->dataSize = size;

	return 0;
}

long deleteMbx(const char *name)
{
	node_t *curr, *prev = NULL;
	prev = head;

	//if (prev->next != NULL)
	//{
	curr = prev->next;
	//}

	int iteration = 0;
	int t = 1;
	int end = 1;

	//look for the correct mailbox
	while (t != 0)
	{
		end = 1;
		for (int i = 0; i < 64; i++)
		{
			//since the cur->name is set to 64, after '\0' the array will be same which will set the same flag
			//therefor, once '\0' is found, we dont check anymore
			if (name[i] != '\0' && end)
			{
				//since we set prev to head, the first iteration we compare with prev but after that
				//we compare with curr. after the first iteration it will be set to 1
				if (iteration == 0)
				{
					//have to check one character at a time since it stores "a" as 0x0stuff"a" and therefor
					//curr->name and name will be different before the "a" and wont ever be equal
					if (prev->name[i] == name[i])
					{
						t = 0;
					}
				}
				else
				{
					if (curr->name[i] == name[i])
					{
						t = 0;
					}
				}
			}
			else
			{
				end = 0;
			}
		}
		//since prev is already set to head and curr is prev->next, in the first iteration prev and curr do not have to change
		if (t != 0 && iteration > 0)
		{
			prev = prev->next;
			curr = curr->next;
		}
		iteration++;
	}

	if(iteration == 1)
	{
		head = curr;

	}else if(curr->next == NULL)
	{
		prev->next = NULL;

	}else
	{
		prev->next = curr->next;
	}
	if (iteration == 1)
	{
		free(prev->data);
		free(prev);
	}
	else
	{
		free(curr->data);
		free(curr);
	}

	return 0;
}

long write(const char *name, const unsigned char *data, unsigned long offset, unsigned long n)
{
	node_t *it;
	it = findMbx(name);

	if ((offset + n) <= it->dataSize)
	{
		for (unsigned int i = 0; i < n; i++)
		{
			it->data[i + offset] = data[i];
		}
	}

	return 0;
}

long read(const char *name, unsigned char *data, unsigned long offset, unsigned long n)
{
	node_t *it;
	it = findMbx(name);

	for (unsigned int i = 0; i <= n; i++)
	{
		if (i != n)
		{
			data[i] = it->data[i + offset];
		}
		else
		{
			data[i] = 0;
		}
	}

	return 0;
}

long size(const char *name)
{
	int size = 0;

	node_t *it;
	it = findMbx(name);

	size = sizeof(it->name) + it->dataSize;

	return size;
}

long list(char **list, unsigned long max_entries)
{
	node_t *it;
	it = head;

	for (unsigned int i = 0; i < max_entries; i++)
	{
		list[i] = it->name;
		it = it->next;
	}

	return 0;
}

long count()
{
	node_t *it;
	it = head;
	int count = 0;

	while (it != NULL)
	{
		it = it->next;
		count++;
	}

	return count;
}