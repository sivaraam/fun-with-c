/* Reference: http://www.linuxdevcenter.com/pub/a/linux/2006/11/30/linux-out-of-memory.html */

#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEGABYTE 1024*1024
#define HALF_GIGABYTE 1024*1024*512
#define GIGABYTE 1024*1024*1024

int main(int argc, char * argv[])
{
	void* block=NULL;
	int count=0;
	int alloc_size = GIGABYTE;

        while(1)
	{
		block = (void *) malloc(alloc_size);
		if (!(block))
		{
			if (alloc_size == GIGABYTE) { alloc_size = HALF_GIGABYTE; count = 0; }
			else if (alloc_size == HALF_GIGABYTE) { alloc_size = MEGABYTE; count = 0; }
			else if (alloc_size == MEGABYTE) break;
		}
		memset(block,1,alloc_size);
		printf("Currently allocating %d*%d bytes\n", ++count, alloc_size);
	}
	getchar();
	return 0;
}
