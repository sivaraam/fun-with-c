/* Reference: http://www.linuxdevcenter.com/pub/a/linux/2006/11/30/linux-out-of-memory.html */

#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTE 1
#define KILOBYTE 1024
#define MEGABYTE 1024*1024
#define HALF_GIGABYTE 1024*1024*512
#define GIGABYTE 1024*1024*1024

int main(int argc, char * argv[])
{
	void* block=NULL;
	int gb_count=0, hgb_count = 0, mb_count = 0, kb_count = 0, byte_count = 0;
	int alloc_size = GIGABYTE;

        while(1)
	{
		block = (void *) malloc(alloc_size);
		if (!(block))
		{
			if (alloc_size == GIGABYTE) alloc_size = HALF_GIGABYTE;
			else if (alloc_size == HALF_GIGABYTE) alloc_size = MEGABYTE;
			else if (alloc_size == MEGABYTE) alloc_size = KILOBYTE;
			else if (alloc_size == KILOBYTE) alloc_size = BYTE;
			else break;
		}
		switch (alloc_size)
		{
		case GIGABYTE: gb_count++;
			       break;
		case HALF_GIGABYTE: hgb_count++;
				    break;
		case MEGABYTE: mb_count++;
			       break;
		case KILOBYTE: kb_count++;
			       break;
		case BYTE: byte_count++;
			   break;
		}
	}
	printf ("Totally obtained %d GB, %d Half GB, %d MB, %d KB and %d bytes using malloc\n",
	        gb_count, hgb_count, mb_count, kb_count, byte_count);
	getchar();
	return 0;
}
