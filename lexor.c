#include <stdio.h>
#include <stdlib.h>

#define STACKSIZE 500 // Size of stack

int STACK_TOP = 0;
char STACK[STACKSIZE];

void push(char);
void pop();
void show();

int main (int argc, char *argv[ ])
{
	int r_char;
	FILE *file;
	file = fopen(argv[1], "r");
	if (file) {
			while ((r_char = getc(file)) != EOF){
					// putchar(r_char);
					push(r_char);
			}
			fclose(file);
			show();
	}else{
		puts("Insira um nome de arquivo válido");
		exit(1);
	}
	return 0;
}

void push(char value)
{
	STACK[STACK_TOP++] = value;
}

void pop()
{
	STACK[STACK_TOP--] = '\0';
}

void show()
{
	for(int i = 0; i < STACK_TOP; i++)
	{
		if(STACK[i] == '\n')
			printf("STACK_ITEM[%d] \t = \t[%c]\n", i, '\0');
		else
			printf("STACK_ITEM[%d] \t = \t[%c]\n", i, STACK[i]);
	}
}
