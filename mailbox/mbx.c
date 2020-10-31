#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/errno.h>
#include "mbx.h"

mbx *head = NULL;

mbx* findMbx(const char *name)
{
	int t, end, i;
	mbx *it = head;
	t = 1;
	end = 1;
	i = 0;

	while (t != 0)
	{
		end = 1;
		while(i < 64)
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
			i++;
		}
		//since prev is already set to head and curr is prev->next, in the first iteration prev and curr do not have to change
		if (t != 0)
		{
			it = it->next;
		}
	}

	return it;
}

//void printList()
//{
//	mbx *it;
//	it = head;
//	int i = 0;
//
//	while(it != NULL)
//	{
//		printk("mbx name: %s, ", it->name);
//		printk("data size: %d bytes, ", it->dataSize);
//		printk("data in mbx: ");
//
//		//this allows for null terminators to not affect printing what is after the null terminator
//		while (i < it->dataSize)
//		{
//			printk("%c", it->data[i]);
//			i++;
//		}
//		printk("\n");
//
//		it = it->next;
//	}
//	printk("\n");
//}

SYSCALL_DEFINE2(create_mbx_421, const char __user*, name, unsigned long, size)
{
	int i;
	mbx *it;
	mbx *curr;
	i = 0;
	//create the new mailbox
	curr = (mbx*)kmalloc(sizeof(mbx), GFP_KERNEL);

	memcpy(curr->name, name, sizeof(curr->name));
//	while(i <= 64)
//	{
//		memcpy(current->name[i], name[i], sizeof(char));
//		//current->name[i] = name[i];
//		i++;
//	}

    	curr->data = (char*)kmalloc(size, GFP_KERNEL);
	curr->dataSize = size;

	//0 the array out so that there is not random stuff in the array at start
	i = 0;
	memset(curr->data, 0, curr->dataSize);
//	while(i < size)
//	{
//		memcpy(current->data[i], 0, sizeof(int));
//		//current->data[i] = 0;
//		i++;
//	}

    	curr->next = NULL;

	//put it in the list
	//case where list is empty
	if(head == NULL)
	{
		head = curr;
	}else
	{
	        it = head;

		while(it->next != NULL)
		{
			it = it->next;
		}
		it->next = curr;
	}

	return 0;
}

SYSCALL_DEFINE2(resize_mbx_421, const char __user*, name, unsigned long, size)
{
	mbx *it;
	it = findMbx(name);

	it->data = (char*)krealloc(it->data, size, GFP_KERNEL);
	it->dataSize = size;

	return 0;
}

SYSCALL_DEFINE1(delete_mbx_421, const char __user*, name)
{
	int i, iteration, t, end;
	mbx *curr, *prev = NULL;
	i = 0;
	iteration = 0;
	t = 1;
	end = 1;

	prev = head;

	//if (prev->next != NULL)
	//{
	curr = prev->next;
	//}

	//look for the correct mailbox
	while (t != 0)
	{
		end = 1;
		while (i < 64)
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
			i++;
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
		kfree(prev->data);
		kfree(prev);
	}
	else
	{
		kfree(curr->data);
		kfree(curr);
	}

	return 0;
}

SYSCALL_DEFINE4(write_mbx_421, const char __user*, name, const unsigned char __user*, data, unsigned long, offset, unsigned long, n)
{
	int i;
	mbx *it;
	i = 0;

	it = findMbx(name);

	if ((offset + n) <= it->dataSize)
	{
		while(i < n)
		{
			it->data[i + offset] = data[i];
			i++;
		}
	}

	return 0;
}

SYSCALL_DEFINE4(read_mbx_421, const char __user*, name, unsigned char __user*, data, unsigned long, offset, unsigned long, n)
{
	int i;
	mbx *it;
	i = 0;

	it = findMbx(name);

	while(i <= n)
	{
		if (i != n)
		{
			memcpy(&data[i], &it->data[i + offset], sizeof(char));
			//data[i] = it->data[i + offset];
		}
		else
		{
			data[i] = 0;
		}
		i++;
	}

	return 0;
}

SYSCALL_DEFINE1(size_mbx_421, const char __user*, name)
{
	int size;
	mbx *it;
	size = 0;

	it = findMbx(name);

	size = sizeof(it->name) + it->dataSize;

	return size;
}

SYSCALL_DEFINE2(list_mbx_421, char __user**, list, unsigned long, max_entries)
{
	int i;
	mbx *it;
	i = 0;

	it = head;

	while(i < max_entries)
	{
		list[i] = it->name;
		it = it->next;
		i++;
	}

	return 0;
}

SYSCALL_DEFINE0(count_mbx_421)
{
	int count;
	mbx *it;
	count = 0;

	it = head;

	while (it != NULL)
	{
		it = it->next;
		count++;
	}

	return count;
}
