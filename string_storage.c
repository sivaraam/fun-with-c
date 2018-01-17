#include<stdio.h>


char * getString(){
	char *str = "Hello";

	/* Segmentation fault as string is stored in
	 * read-only memory and thus we're trying to modify
	 * read-only memory.
	 */
//	str[0] = 'W';

	printf("getString: %s\n", str);

	/* String is returned without issues as the memory is
	 * allocated at run-time mostly in a read-only block
	 * (generally in data segment) that is shared among
	 * functions.
	 */
	return str;
}


char * getStringArray(){
	char str[] = {'H', 'e', 'l' ,'l', 'o', '\0'};

	/* No segmentation fault as the string is an array which
	 * is present only locally.
	 */
	str[0] = 'W';

	printf("getStringArray: %s\n", str);

	/* String is not returned correctly as it is present in
	 * local storage and may not be present after function
	 * returns.
	 * This even gives a compiler warning,
	 *
	 *	warning: function returns address of local variable [-Wreturn-local-addr]
	 *	   return str;
	 *	          ^~~
	 */
	return str;
}

char * getStringArrayCast(){
	char *str = (char [6]){'H', 'e', 'l' ,'l', 'o'};

	/* No segmentation fault possibly as the string is an array in
	 * local storage.
	 */
	str[0] = 'W';

	printf("getStringArrayCast: %s\n", str);

	/* Value returned is grabage.
	 * Reason is not so clear yet.
	 * Maybe, "Casting is evil"
	 */
	return str;
}

int main(void) {
	printf("%s\n",getString() );
//	printf("%s\n",getStringArray() );
	printf("%s\n",getStringArrayCast() );
	return 0;
}
