#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	// Question 3
	int *i = calloc(2, sizeof(int));
	char *c = calloc (8, sizeof(char));

	printf("i: %p, c: %p; *i: %d, *c: %c\n", i, c, *i, *c);

	*c=1;
	i=(int *)c;

	printf("i: %p, c: %p; *i: %d, *c: %c\n", i, c, *i, *c);

	printf("Question: 0x%x 0x%x 0x%x 0x%x\n", *i, *i+1, *c, *(c+1));

	*i='1';
	c=(char*)i;

	printf("i: %p, c: %p; *i: %d, *c: %c\n", i, c, *i, *c);

	printf("Question: 0x%x 0x%x 0x%x 0x%x\n", *i, *(i+1), *c, *c+1);


	// Question 1
	int ii = 0;
	int memory_alloc_fail = 0;

	char **dptr = malloc(5*sizeof(char*));
	if (!dptr)
	{
		return 1;
	}

	while (ii<5)
	{
		dptr[ii] = NULL;
		dptr[ii] = malloc(20*sizeof(char));
		if (!dptr[ii])
		{
			memory_alloc_fail = 1;
			break;
		}

		*((int*)dptr[ii]) = 0x00 | (0x41+ii*2) << 16 | (0x42+ii*2) << 8 | (0x43+ii*2);
		ii++;
	}

	if (!memory_alloc_fail)
	{
		printf("dptr[0]:: 0x%X\n", (0x00 | (0x41)<<16 | 0x42<<8 | 0x43));
		printf("ASCII of 'a': %d\n", 'a');
		printf("ASCII of 'A': %d\n", 'A');
		printf("ASCII of '0': %d\n", '0');
		printf("%c\t %hd\t %p\n", **dptr, **dptr, *dptr);
		printf("%c\n", **dptr+3);
		printf("%c\n", *(*dptr)+1);
		printf("%c\t %hd\t %p\n", (*(*dptr+1)), (*(*dptr+1)), (*dptr+1));
		printf("%c\n", *dptr[2]);
		printf("%c\n", *dptr[1]+2);
		printf("%c\n", **(dptr+2));
	}

	ii = 0;
	while (ii<5)
	{
		if (dptr[ii])
		{
			free(dptr[ii]);
			dptr[ii++] = NULL;
		}
	}
	free(dptr);
}
