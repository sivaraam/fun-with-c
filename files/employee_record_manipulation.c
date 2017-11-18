#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
//#include <sys/types.h>

struct emp 
{ 
	char name[40] ; 
	int age; 
	float bs; 
};

int main(void)
{
	FILE *fp;
	unsigned choice;
	struct emp e;
	char empname[40];
	long int recsize;
	fp = fopen("EMP.DAT", "wb+");

	if ( fp == NULL ) 
	{
		puts ("Cannot open file");
		exit(EXIT_FAILURE);
	}

	recsize = sizeof(e);

	while(1)
	{
	printf("\n1. Add record \n2. List Record \n3. Modify Record\n"
				 "4. Delete Record \n5. Exit");

	fflush(stdin);
	printf("\nEnter the choice: ");
	scanf("%d", &choice);

	switch (choice)
	{
		case 1:
			fseek (fp, 0, SEEK_END);
			printf ("\nEnter name, age and basic salary: ");
			scanf ("%s %d %f", e.name, &e.age, &e.bs);
			fwrite (&e, recsize, 1, fp);
			fflush(fp);
			break;

		case 2:
			rewind(fp);
			while (fread(&e, recsize, 1, fp) == 1)
				printf ("\n%s %d %f", e.name, e.age, e.bs);
			break;

		case 3: 
			printf ("\nEnter name of employee to modify: ");
			scanf ("%s", empname);
			rewind (fp);

			while (fread ( &e, recsize, 1, fp ) == 1)
			{
				if (strcmp ( e.name, empname ) == 0)
				{
					printf ("\nEnter new name, age & bs: ");
					scanf ("%s %d %f", e.name, &e.age, &e.bs);
					fseek (fp, -recsize, SEEK_CUR);
					fwrite (&e, recsize, 1, fp);
				break;
				}
			}
			break;

		case 4:
		{
			unsigned record_found = 0;
			printf ("\nEnter name of employee to delete: ");
			scanf ( "%s", empname );
			rewind(fp);

			while (fread(&e, recsize, 1, fp) == 1)
			{
				if (strcmp (e.name, empname) == 0) {
					record_found = 1;
					break;
				}
			}

			/* move all records after found record
			   to a position that is one record position
			   less than its old position */
			if (record_found) {

				while(1) {
					// copy records one 'recsize' position back
					// Note: file pointer should be pointing to the
					// record just next to the record to be overwritten
					if (fread(&e, recsize, 1, fp) == 1) {
						fseek(fp, -(recsize*2), SEEK_CUR);
						fwrite(&e, recsize, 1, fp);
						fseek(fp, recsize, SEEK_CUR);
					}
					else {
						// handle last record
						/* This might be an ugly way (switching between file pointer
						   and file descriptor)[1] but it's the only way for "interactive"
						   applications like this one (or may be they should have used file descriptors
						   from the beginning!)

							 [1]: http://pubs.opengroup.org/onlinepubs/9699919799/functions/V2_chap02.html#tag_15_05_01) */
						fflush(fp);
						ftruncate(fileno(fp), (off_t)ftell(fp)-recsize);
						break;
					}
				}
			}
			break;
		}

		case 5:
			fclose(fp);
			exit(EXIT_SUCCESS) ;
	}
	}

}

