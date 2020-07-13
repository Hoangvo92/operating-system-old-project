#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
	int number;
	char array[10];
	scanf("%d", &number);
	FILE *tty = fopen("/dev/tty", "r");	
	if (!tty) {
   		perror("/dev/tty");
    		exit(1);
	}
	printf("Type your name: ");
    	fgets(array, sizeof(array), tty);
	array[strlen(array)-1]='\0';
	printf("Name = %s, Number= %d\n", array, number);
	fclose(tty);
}
