#include <stdio.h>
#include <stdlib.h>
/*
* Produções
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
* Transições:
* t0: (e, e, S)
* t1: (e, S, M)
* t2: (e, S, GM)
* t3: (e, S, FGM)
* t4: (e, F, f(){ C; r(E); })
* t5: (e, G, g(){ C; r(E); })
* t6: (e, M, m(){ C; r(E); })
* t7: (e, E, 0)
* t8: (e, E, 1)
* t9: (e, E, x)
* t10: (e, E, y)
* t11: (e, E, (EXE))
* t12: (e, X, +)
* t13: (e, X, -)
* t14: (e, X, *)
* t15: (e, X, /)
* t16: (e, C, h=E)
* t17: (e, C, i=E)
* t18: (e, C, j=E)
* t19: (e, C, k=E)
* t20: (e, C, z=E)
* t21: (e, C, (EXE))
* t22: (e, C, W(E){ C;})
* t23: (e, C, f(E){ C; })
* t24: (e, C, o(E; E; E){ C; })
* t25: (0, 0, e)
* t26: (1, 1, e)
* t27: (x, x, e)
* t28: (y, y, e)
* t29: ((, (, e)
* t30: (), ), e)
* t31: (+, +, e)
* t32: (-, -, e)
* t33: (*, *, e)
* t34: (/, /, e)
* t35: (h, h, e)
* t36: (i, i, e)
* t37: (j, j, e)
* t38: (k, k, e)
* t39: (z, z, e)
* t40: (=, =, e)
* t41: (w, w, e)
* t42: ({, {, e)
* t43: (;, ;, e)
* t44: (}, }, e)
* t45: (o, o, e)
* t46: (f, f, e)
* t47: (g, g, e)
* t48: (m, m, e)
* t49: (r, r, e)
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
// Transiction id
short int TI = 0;
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
		 PI = 1; TI = 1; print_state(1, token);  pop(); push('M'); return 1;
	}
	else if(token == 'g' && STACK[TOP] == 'S'){
		 PI = 2; TI = 2; print_state(1, token);  pop(); push('G'); return 1;
	}
	else if(token == 'f' && STACK[TOP] == 'S'){
		 PI = 3; TI = 3; print_state(1, token);  pop(); push('F'); return 1;
	}

	// Consome a produção responsável por cada função
	if(token == 'm' && STACK[TOP] == 'M')
	{
  	 PI = 6; TI = 6; print_state(1, token);  pop(); push('}'); push(';'); push(')'); push('E'); push('('); push('r'); push(';'); push('C'); push('{'); push(')'); push('('); push('m');
		return 1;
	}
	else if(token == 'g' && STACK[TOP] == 'G')
	{
		 PI = 5; TI = 5; print_state(1, token);  pop(); push('}'); push(';'); push(')'); push('E'); push('('); push('r'); push(';'); push('C'); push('{'); push(')'); push('('); push('g');
		return 1;
	}
	else if(token == 'f' && STACK[TOP] == 'F'){
		 PI = 4; TI = 4; print_state(1, token);  pop(); push('}'); push(';'); push(')'); push('E'); push('('); push('r'); push(';'); push('C'); push('{'); push(')'); push('('); push('f');
		return 1;
	}

	// Consome os nomes das funções das funções
	if(token == 'f' && STACK[TOP] == 'f'){TI = 46; print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'm' && STACK[TOP] == 'm'){TI = 48; print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'g' && STACK[TOP] == 'g'){TI = 47; print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'r' && STACK[TOP] == 'r'){TI = 49; print_state(1, token);  pop(); TOKEN_POS++; return 1;}

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
	if(token == '0' && STACK[TOP] == 'E') {TI = 7; PI = 7; print_state(1, token);  pop(); push('0'); return 1;}
	else if(token == '1' && STACK[TOP] == 'E') {TI = 8; PI = 8; print_state(1, token);  pop(); push('1'); return 1;}
	else if(token == 'x' && STACK[TOP] == 'E') {TI = 9; PI = 9; print_state(1, token);  pop(); push('x'); return 1;}
	else if(token == 'y' && STACK[TOP] == 'E') {TI = 10; PI = 10; print_state(1, token);  pop(); push('y'); return 1;}
	else if((token == '(' && STACK[TOP] == 'E')) {TI = 11; PI = 11; print_state(1, token);  pop(); push(')'); push('E'); push('X'); push('E'); push('('); return 1;}
	// Consume
	else if(token == '0' && STACK[TOP] == '0') {TI = 25; print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == '1' && STACK[TOP] == '1') {TI = 26; print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'x' && STACK[TOP] == 'x') {TI = 27; print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'y' && STACK[TOP] == 'y') {TI = 28; print_state(1, token);  pop(); TOKEN_POS++; return 1;}
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
	if(token == '+' && STACK[TOP] == 'X') { PI = 12; TI = 12;  print_state(1, token); pop(); push('+'); return 1;}
	else if(token == '-' && STACK[TOP] == 'X') { PI = 13;  TI = 13; print_state(1, token); pop(); push('-'); return 1;}
	else if(token == '*' && STACK[TOP] == 'X') { PI = 14;  TI = 14; print_state(1, token); pop(); push('*'); return 1;}
	else if(token == '/' && STACK[TOP] == 'X') { PI = 15;  TI = 15; print_state(1, token); pop(); push('/'); return 1;}
	// Consome
	else if(token == '+' && STACK[TOP] == '+') { TI = 31; print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == '-' && STACK[TOP] == '-') { TI = 32; print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == '*' && STACK[TOP] == '*') { TI = 33; print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == '/' && STACK[TOP] == '/') { TI = 34; print_state(1, token);  pop(); TOKEN_POS++; return 1;}
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
	if(token == 'h' && STACK[TOP] == 'C') { PI = 16; TI = 16; print_state(1, token); pop(); push('E');  push('='); push('h'); return 1;}
	else if(token == 'j' && STACK[TOP] == 'C') { PI = 18; TI = 18; print_state(1, token); pop(); push('E');  push('='); push('j'); return 1;}
	else if(token == 'i' && STACK[TOP] == 'C') { PI = 17; TI = 17; print_state(1, token); pop(); push('E');  push('='); push('i'); return 1;}
	else if(token == 'k' && STACK[TOP] == 'C') { PI = 19; TI = 19; print_state(1, token); pop(); push('E');  push('='); push('k'); return 1;}
	else if(token == 'z' && STACK[TOP] == 'C') { PI = 20; TI = 20; print_state(1, token); pop(); push('E');  push('='); push('z'); return 1;}
	// p21-p24
	else if(token == '(' && STACK[TOP] == 'C') { PI = 21; TI = 21; print_state(1, token); pop(); push(')');  push('E'); push('X'); push('E'); push('('); return 1;}
	else if(token == 'w' && STACK[TOP] == 'C') { PI = 22; TI = 22; print_state(1, token); pop(); push('}'); push(';'); push('C'); push('{'); push(')'); push('E'); push('('); push('w'); return 1;}
	else if(token == 'f' && STACK[TOP] == 'C') { PI = 23; TI = 23; print_state(1, token); pop(); push('}'); push(';'); push('C'); push('{'); push(')'); push('E'); push('('); push('f'); return 1;}
	else if(token == 'o' && STACK[TOP] == 'C') { PI = 24; TI = 24; print_state(1, token); pop(); push('}'); push(';'); push('C'); push('{'); push(')'); push('E'); push(';'); push('E'); push(';'); push('E'); push('('); push('o'); return 1;}
	// Consome
	else if(token == 'w' && STACK[TOP] == 'w') {TI = 41; print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'o' && STACK[TOP] == 'o') {TI = 45; print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'f' && STACK[TOP] == 'f') {TI = 46; print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'h' && STACK[TOP] == 'h') {TI = 35; print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'j' && STACK[TOP] == 'j') {TI = 37; print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'i' && STACK[TOP] == 'i') {TI = 36; print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'k' && STACK[TOP] == 'k') {TI = 38; print_state(1, token);  pop(); TOKEN_POS++; return 1;}
	else if(token == 'z' && STACK[TOP] == 'z') {TI = 39; print_state(1, token);  pop(); TOKEN_POS++; return 1;}
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

	if(token == '(' && STACK[TOP] == '(') {TI = 29; print_state(1, token);  TOKEN_POS++; pop(); return 1;}
	else if(token == ')' && STACK[TOP] == ')') {TI = 30; print_state(1, token);  TOKEN_POS++; pop(); return 1;}
	else if(token == '{' && STACK[TOP] == '{') {TI = 42; print_state(1, token);  TOKEN_POS++; pop(); return 1;}
	else if(token == '}' && STACK[TOP] == '}') {TI = 44; print_state(1, token);  TOKEN_POS++; pop(); return 1;}
	else if(token == 'r' && STACK[TOP] == 'r') {TI = 49; print_state(1, token);  TOKEN_POS++; pop(); return 1;}
	else if(token == '=' && STACK[TOP] == '=') {TI = 40; print_state(1, token);  TOKEN_POS++; pop(); return 1;}
	else if(token == ';' && STACK[TOP] == ';') {TI = 43; print_state(1, token);  TOKEN_POS++; pop(); return 1;}

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
	if(PI == 0 && TI == 0)
		printf("Step: %2d \t Estado: q%3d \t Token: %5c \t Stack: %20s \t\t pi: - %2 \t ti: - %2 \t Word: %s", STEP, state,token, STACK, WORD);
	else if(PI == 0 && TI > 0)
		printf("Step: %2d \t Estado: q%3d \t Token: %5c \t Stack: %20s \t\t pi: - %2 \t ti: t%2d \t Word: %s", STEP, state,token, STACK, TI, WORD);
	else if(TI == 0 && PI > 0)
		printf("Step: %2d \t Estado: q%3d \t Token: %5c \t Stack: %20s \t\t pi: p%2d \t ti: - %2 \t Word: %s", STEP, state,token, STACK, PI, WORD);
	else
		printf("Step: %2d \t Estado: q%3d \t Token: %5c \t Stack: %20s \t\t pi: p%2d \t ti: t%2d \t Word: %s", STEP, state,token, STACK, PI, TI, WORD);

	PI = 0;
	TI = 0;
}

void show()
{
	for(int i = 0; i < TOP; i++)
	{
		printf("Index: %1d \t Stack: %1c\n", i, STACK[i]);
	}
}
