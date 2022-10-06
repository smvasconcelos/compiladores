#include <stdio.h>
#include <stdlib.h>
/*
* p1: S → M
* p2: S -> G M
* p3: S -> F G M
* p4: F → f(){ C; r(E); }
* p5: G → g(){ C; r(E); }
* p6: M → m(){ C; r(E); }
* p7: E → 0
* p7: E -> 1
* p9: E -> x
* p10: E -> y
* p11: E -> (EXE)
* p12: X → +
* p13: X -> -
* p14: X -> *
* p15: X -> /
* p16: C → h=E
* p17: C -> i=E
* p18: C -> j=E
* p19: C -> k=E
* p20: C -> z=E
* p21: C -> (EXE)
* p22: C -> w(E){ C; }
* p23: C -> f(E){ C; }
* p24: C -> o(E; E; E){ C; }
*/
#define STACKSIZE 100 // Size of stack

char * WORD = NULL;
int WORD_LEN = 100;
int TOKEN_POS = 0;
// Top of stack
int TOP = -1;
// Step of the machine
int STEP = 0;
// Production id
short int PI = 0;
// Stack
char STACK[STACKSIZE];
// File pointer
FILE *file;

void push(char);
void pop();
void show();

// Lida com as produções p1 à p5
int p1_p6(char);
// Lida com as produções p6 à p11
int p5_p11(char);
// Lida com as produções p12 à p15
int p12_p15(char);
// Lida com as produções p16 à p24
int p16_p24(char);
// Lida com as transições dos () {} ; =
int consume_extras(char);
// Printa o estado da executação
void print_state(int, char);


/*
 * Um pi com número p0 indica que o char no topo da pilha e o que está sendo lido, irá ser consumido
 * ou seja, representa uma transição x.
 * Para executar o programa é necessário passar o nome do arquivo pelo argv saida.exe input.txt
 * no arquivo presente existem várias palavras, somente a primeira linha será lida.
*/
int main (int argc, char *argv[ ])
{
	char token;
	file = fopen(argv[1], "r");
	char word = getline(&WORD, &WORD_LEN, file);

	if (file) {
		// Initial state
		q0:
			push('S'); print_state(0, '-'); goto q1;
		// Reading state
		q1:
			STEP++;
			token = WORD[TOKEN_POS];

			if(token == EOF || token == '\n'){
				print_state(1, '\0');
				goto fim;
			}

			if(consume_extras(token))goto q1;
			else if(p1_p6(token)) goto q1;
			else if(p5_p11(token)) goto q1;
			else if(p12_p15(token)) goto q1;
			else if(p16_p24(token)) goto q1;
			else goto fim;


		fim:
			fclose(file);
			if(TOP == -1)
				puts("\n A palavra foi consumida totalmente.");
			else{
				puts("*****************************************************************************************************************");
				print_state(-1, token == '\n' ? '\0' : token);
				puts("*****************************************************************************************************************");
				puts("\n A palavra nao pode ser consumida.");
			}

	}else{
		puts("Insira um nome de arquivo válido");
		exit(1);
	}
	return 0;
}

int p1_p6(char token){
	/*
	! p1: S → M
	! p2: S -> G M
	! p3: S -> F G M
	! p4: F → f(){ C; r(E); }
	! p5: G → g(){ C; r(E); }
	! p6: M → m(){ C; r(E); }
	*/

	// Empilha a produção responsável por cada função
	if(token == 'm' && STACK[TOP] == 'S'){
		 PI = 1; print_state(1, token);  pop(); push('M'); return 1;
	}
	else if(token == 'g' && STACK[TOP] == 'S'){
		 PI = 2; print_state(1, token);  pop(); push('G'); return 1;
	}
	else if(token == 'f' && STACK[TOP] == 'S'){
		 PI = 3; print_state(1, token);  pop(); push('F'); return 1;
	}

	// Consome a produção responsável por cada função
	if(token == 'm' && STACK[TOP] == 'M')
	{
  	 PI = 4; print_state(1, token);  pop(); push('}'); push(';'); push(')'); push('E'); push('('); push('r'); push(';'); push('C'); push('{'); push(')'); push('('); push('m');
		return 1;
	}
	else if(token == 'g' && STACK[TOP] == 'G')
	{
		 PI = 5; print_state(1, token);  pop(); push('}'); push(';'); push(')'); push('E'); push('('); push('r'); push(';'); push('C'); push('{'); push(')'); push('('); push('g');
		return 1;
	}
	else if(token == 'f' && STACK[TOP] == 'F'){
		 PI = 6; print_state(1, token);  pop(); push('}'); push(';'); push(')'); push('E'); push('('); push('r'); push(';'); push('C'); push('{'); push(')'); push('('); push('f');
		return 1;
	}

	// Consome os nomes das funções das funções
	if(token == 'f' && STACK[TOP] == 'f'){print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'm' && STACK[TOP] == 'm'){print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'g' && STACK[TOP] == 'g'){print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'r' && STACK[TOP] == 'r'){print_state(1, token);  pop(); TOKEN_POS++; return 1;}

	return 0;
}

int p5_p11(char token)
{
	/*
		! p7: E → 0
		! p7: E -> 1
		! p9: E -> x
		! p10: E -> y
		! p11: E -> (EXE)
	*/


	// Empilha
	if(token == '0' && STACK[TOP] == 'E') { PI = 7; print_state(1, token);  pop(); push('0'); return 1;}
	else if(token == '1' && STACK[TOP] == 'E') { PI = 8; print_state(1, token);  pop(); push('1'); return 1;}
	else if(token == 'x' && STACK[TOP] == 'E') { PI = 9; print_state(1, token);  pop(); push('x'); return 1;}
	else if(token == 'y' && STACK[TOP] == 'E') { PI = 10; print_state(1, token);  pop(); push('y'); return 1;}
	else if((token == '(' && STACK[TOP] == 'E')) { PI = 11; print_state(1, token);  pop(); push(')'); push('E'); push('X'); push('E'); push('('); return 1;}
	// Consume
	else if(token == '0' && STACK[TOP] == '0') {print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == '1' && STACK[TOP] == '1') {print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'x' && STACK[TOP] == 'x') {print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'y' && STACK[TOP] == 'y') {print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	return 0;
}

int p12_p15(char token)
{
	/*
	! p12: X → +
	! p13: X -> -
	! p14: X -> *
	! p15: X -> /
	*/
	// Empilha
	if(token == '+' && STACK[TOP] == 'X') { PI = 12; print_state(1, token); pop(); push('+'); return 1;}
	else if(token == '-' && STACK[TOP] == 'X') { PI = 13; print_state(1, token); pop(); push('-'); return 1;}
	else if(token == '*' && STACK[TOP] == 'X') { PI = 14; print_state(1, token); pop(); push('*'); return 1;}
	else if(token == '/' && STACK[TOP] == 'X') { PI = 15; print_state(1, token); pop(); push('/'); return 1;}
	// Consome
	else if(token == '+' && STACK[TOP] == '+') {print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == '-' && STACK[TOP] == '-') {print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == '*' && STACK[TOP] == '*') {print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == '/' && STACK[TOP] == '/') {print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	return 0;
}

int p16_p24(char token)
{
	/*
	! p16: C → h=E
	! p17: C -> i=E
	! p18: C -> j=E
	! p19: C -> k=E
	! p20: C -> z=E
	! p21: C -> (EXE)
	! p22: C -> w(E){ C; }
	! p23: C -> f(E){ C; }
	! p24: C -> o(E; E; E){ C; }
	*/
	// Empilha
	if(token == 'h' && STACK[TOP] == 'C') { PI = 16; print_state(1, token); pop(); push('E');  push('='); push('h'); return 1;}
	else if(token == 'j' && STACK[TOP] == 'C') { PI = 18; print_state(1, token); pop(); push('E');  push('='); push('j'); return 1;}
	else if(token == 'i' && STACK[TOP] == 'C') { PI = 17; print_state(1, token); pop(); push('E');  push('='); push('i'); return 1;}
	else if(token == 'k' && STACK[TOP] == 'C') { PI = 19; print_state(1, token); pop(); push('E');  push('='); push('k'); return 1;}
	else if(token == 'z' && STACK[TOP] == 'C') { PI = 20; print_state(1, token); pop(); push('E');  push('='); push('z'); return 1;}
	// p21-p24
	else if(token == '(' && STACK[TOP] == 'C') { PI = 21; print_state(1, token); pop(); push(')');  push('E'); push('X'); push('E'); push('('); return 1;}
	else if(token == 'w' && STACK[TOP] == 'C') { PI = 22; print_state(1, token); pop(); push('}'); push(';'); push('C'); push('{'); push(')'); push('E'); push('('); push('w'); return 1;}
	else if(token == 'f' && STACK[TOP] == 'C') { PI = 23; print_state(1, token); pop(); push('}'); push(';'); push('C'); push('{'); push(')'); push('E'); push('('); push('f'); return 1;}
	else if(token == 'o' && STACK[TOP] == 'C') { PI = 24; print_state(1, token); pop(); push('}'); push(';'); push('C'); push('{'); push(')'); push('E'); push(';'); push('E'); push(';'); push('E'); push('('); push('o'); return 1;}
	// Consome
	else if(token == 'w' && STACK[TOP] == 'w') {print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'o' && STACK[TOP] == 'o') {print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'f' && STACK[TOP] == 'f') {print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'h' && STACK[TOP] == 'h') {print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'j' && STACK[TOP] == 'j') {print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'i' && STACK[TOP] == 'i') {print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'k' && STACK[TOP] == 'k') {print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'z' && STACK[TOP] == 'z') {print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	return 0;
}

int consume_extras(char token)
{
	/*
		! (
		! )
		! {
		! }
		! ;
		! =
	*/

	if(token == '(' && STACK[TOP] == '(') { 	print_state(1, token);  TOKEN_POS++; pop(); return 1;}
	else if(token == ')' && STACK[TOP] == ')') { 	print_state(1, token);  TOKEN_POS++; pop(); return 1;}
	else if(token == '{' && STACK[TOP] == '{') { 	print_state(1, token);  TOKEN_POS++; pop(); return 1;}
	else if(token == '}' && STACK[TOP] == '}') { 	print_state(1, token);  TOKEN_POS++; pop(); return 1;}
	else if(token == 'r' && STACK[TOP] == 'r') { 	print_state(1, token);  TOKEN_POS++; pop(); return 1;}
	else if(token == '=' && STACK[TOP] == '=') { 	print_state(1, token);  TOKEN_POS++; pop(); return 1;}
	else if(token == ';' && STACK[TOP] == ';') { 	print_state(1, token);  TOKEN_POS++; pop(); return 1;}

	return 0;
}

void push(char value)
{
	STACK[++TOP] = value;
}

void pop()
{
	STACK[TOP--] = '\0';
}

void print_state(int state, char token)
{
	printf("Step: %2d \t Estado: q%3d \t Token: %5c \t Stack: %20s \t\t pi: p%2d \t Word: %s", STEP, state,token, STACK, PI, WORD);
	PI = 0;
}

void show()
{
	for(int i = 0; i < TOP; i++)
	{
		printf("Index: %1d \t Stack: %1c\n", i, STACK[i]);
	}
}
