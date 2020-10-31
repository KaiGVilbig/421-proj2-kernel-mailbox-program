#include "mbxus.h"

int main()
{
	long a;

	//test create
	printf("Test the create function\n");
	int c;
	int countt;

	printf("Input mbx name: ");
	//get character from user input
	c = getchar();
	char* input = (char*)malloc(sizeof(char));
	int nextSize = sizeof(char);
	countt = 0;

	while (c != 0x0A)
	{
		if (count > 0)
		{
			nextSize += sizeof(char);
			input = (char*)realloc(input, nextSize);
		}
		input[countt] = c;
		countt++;
		c = getchar();
	}
	printf("\n");

	char *naem = malloc(64);

	for (int i = 0; i < 64; i++)
	{
		if (i < countt && i < 63)
		{
			naem[i] = input[i];
		}
		else
		{
			naem[i] = 0;
		}
	}
	free(input);

	int sizeI = 30;
	int resizeN = sizeI / 2;
	a = create(naem, sizeI);

	printList();

	//test resize
	printf("Test the resize function\n");
	a = resize(naem, resizeN);
	printf("mbx ");
	printf(naem);
	printf(" now has %d bytes", resizeN);
	printf(" and the rest still have %d bytes\n", sizeI);
	printList();

	//test count
	printf("Test the count function\n");
	a = count();
	printf("There are %d mbxs\n\n", a);

	//test size
	printf("Test the size function\n");
	a = size(naem);
	printf("Size of mbx ");
	printf(naem);
	printf(": %d bytes\n\n", a);

	//test write
	printf("Test the write function\n");

	printf("Input mbx data: ");
	//get character from user input
	c = getchar();
	char* input1 = (char*)malloc(sizeof(char));
	countt = 0;

	while (c != 0x0A)
	{
		if (count > 0)
		{
			nextSize += sizeof(char);
			input1 = (char*)realloc(input1, nextSize);
		}
		input1[countt] = c;
		countt++;
		c = getchar();
	}

	int offset = 0;

	char *data = malloc(countt);

	for (int i = 0; i < countt; i++)
	{
		data[i] = input1[i];
	}

	free(input1);

	a = write(naem, data, offset, countt);
	printList();

	free(data);

	//test read
	char *data1 = malloc(countt + offset + 1);

	for (int i = 0; i < (countt + offset); i++)
	{
		data1[i] = 0;
	}

	printf("Test the read function\n");
	printf("here is what is in mbx %s: ", naem);
	a = read(naem, data1, offset, countt);
	printf(data1);
	printf("\n");
	free(data1);

	//test list
	int max_entries = 1;
	char **lists = malloc(128);
	printf("\nTest the list function\n");
	a = list(&*lists, max_entries);

	for (int i = 0; i < max_entries; i++)
	{
		printf("%d: %s\n", i + 1, lists[i]);
	}
	free(lists);

	//test delete
	printf("Test the delete function\n");
	a = deleteMbx(naem);

	free(naem);

	return 0;
}
