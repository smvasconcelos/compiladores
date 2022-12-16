#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define MAX_TREE_SIZE 10000
/*
 * Produções
 * p1: S → M
 * p2: S -> G M
 * p3: S -> N G M
 * p4: N → n(){ A; r(E); }
 * p5: G → g(){ A; r(E); }
 * p6: M → m(){ A; r(E); }
 * p7: A → CB
 * p8: B -> .
 * p9: B -> ;CB
 * p10: E → 0
 * p11: E -> 1
 * p12: E -> x
 * p13: E -> y
 * p14: E -> (EXE)
 * p15: X → +
 * p16: X -> -
 * p17: X -> *
 * p18: X -> /
 * p19: C → h=g()
 * p20: C -> i=n()
 * p21: C -> j=E
 * p22: C -> k=E
 * p23: C -> z=E
 * p24: C -> (EXE)
 * p25: C -> w(E){ CD
 * p26: C -> f(E){ CD
 * p27: C -> o(E; E; E){ CD
 * p28: D -> } | ;CD
 * Transições:
 * t0: (e, e, S)
 * t1: (e, S, M)
 * t2: (e, S, GM)
 * t3: (e, S, NGM)
 * t4: (e, F, n(){ A; r(E); })
 * t5: (e, G, g(){ A; r(E); })
 * t6: (e, M, m(){ A; r(E); })
 * t7: (e, A, CB)
 * t8: (e, b, .)
 * t9: (e, b, ;CB)
 * t10: (e, E, 0)
 * t11: (e, E, 1)
 * t12: (e, E, x)
 * t13: (e, E, y)
 * t14: (e, E, (EXE))
 * t15: (e, X, +)
 * t16: (e, X, -)
 * t17: (e, X, *)
 * t18: (e, X, /)
 * t19: (e, C, h=E)
 * t20: (e, C, i=E)
 * t21: (e, C, j=E)
 * t22: (e, C, k=E)
 * t23: (e, C, z=E)
 * t24: (e, C, (EXE))
 * t25: (e, C, W(E){ CD)
 * t26: (e, C, f(E){ CD)
 * t27: (e, C, o(E; E; E){ CD)
 * t28: (e, D, })
 * t29: (e, D, ;CD)
 * t30: (0, 0, e)
 * t31: (1, 1, e)
 * t32: (x, x, e)
 * t33: (y, y, e)
 * t34: ((, (, e)
 * t35: (), ), e)
 * t36: (+, +, e)
 * t37: (-, -, e)
 * t38: (*, *, e)
 * t39: (/, /, e)
 * t40: (h, h, e)
 * t41: (i, i, e)
 * t42: (j, j, e)
 * t43: (k, k, e)
 * t44: (z, z, e)
 * t45: (=, =, e)
 * t46: (w, w, e)
 * t47: ({, {, e)
 * t48: (;, ;, e)
 * t49: (}, }, e)
 * t50: (o, o, e)
 * t51: (f, f, e)
 * t52: (g, g, e)
 * t53: (m, m, e)
 * t54: (r, r, e)
 */
#define STACKSIZE 100 // Size of stack
typedef struct parse_tree
{
  int id;
  char value;
} parse_tree_element;

parse_tree_element TREE[MAX_TREE_SIZE];
int TREE_N = 0;
int TREE_MAX_SIZE = 12;
int PROD_COUNT = 0;
int PROD_INDEX = 0;
int PROD_STACK[MAX_TREE_SIZE];

char WORD[100];
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
// Preenche a arvore de análise
void set_tree_state(char *);
// Transforma a arvore em uma string para ser aplicada em : http://mshang.ca/syntree/
void parse_tree();
// Insere um elemento na pilha de produçoes para a arvore
void prod_push(int);
// Remove um elemento na pilha de produçoes para a arvore
void prod_pop();
// Mostra a arvore n-ária
void show_tree();
void update_prods(int);
void record_tree();

/*
 * Para executar o programa é necessário passar o nome do arquivo pelo argv saida.exe input.txt
 * no arquivo presente existem várias palavras, somente a primeira linha será lida.
 */
int main(int argc, char *argv[])
{
  char token;
  file = fopen(argv[1], "r");
  fgets(WORD, WORD_LEN, file);
  // strcpy(WORD, "m(){f((1+1)){h=0;};r(1);}\n");

  if (file)
  {
  // Initial state
  q0:
    push('S');
    print_state(0, '-');
    TREE[TREE_N].value = 'S';
    TREE[TREE_N++].id = 0;
    prod_push(0);
    goto q1;

  // Reading state
  q1:
    STEP++;
    token = WORD[TOKEN_POS];
    if (token == EOF || token == '\n')
    {
      // print_state(1, '\0');
      goto fim;
    }

    if (consume_extras(token))
      goto q1;
    else if (p1_p6(token))
      goto q1;
    else if (p5_p11(token))
      goto q1;
    else if (p12_p15(token))
      goto q1;
    else if (p16_p24(token))
      goto q1;
    else
      goto fim;

  fim:
    // Mostra a arvore n-ária
    // show_tree();
    fclose(file);
    if (TOP == -1)
    {
      puts("\n A palavra foi consumida totalmente.");
      TREE[TREE_N - 1].id = 12 * PROD_STACK[2] + 1;
      show_tree();
      record_tree();
    }
    else
    {
      puts("*****************************************************************************************************************");
      print_state(-1, token == '\n' ? '\0' : token);
      puts("*****************************************************************************************************************");
      puts("\n A palavra nao pode ser consumida.");
    }
  }
  else
  {
    puts("Insira um nome de arquivo válido");
    exit(1);
  }
  return 0;
}

int p1_p6(char token)
{
  /*
  ! p1: S → M
  ! p2: S -> G M
  ! p3: S -> N G M
  ! p4: N → n(){ C; r(E); }
  ! p5: G → g(){ C; r(E); }
  ! p6: M → m(){ C; r(E); }
  */

  // Empilha a produção responsável por cada função
  if (token == 'm' && STACK[TOP] == 'S')
  {
    PI = 1;
    TI = 1;
    print_state(1, token);
    pop();
    push('M');
    return 1;
  }
  else if (token == 'g' && STACK[TOP] == 'S')
  {
    PI = 2;
    TI = 2;
    print_state(1, token);
    pop();
    push('G');
    return 1;
  }
  else if (token == 'f' && STACK[TOP] == 'S')
  {
    PI = 3;
    TI = 3;
    print_state(1, token);
    pop();
    push('F');
    return 1;
  }

  // Consome a produção responsável por cada função
  if (token == 'm' && STACK[TOP] == 'M')
  {
    PI = 6;
    TI = 6;
    print_state(1, token);
    pop();
    push('}');
    push(';');
    push(')');
    push('E');
    push('(');
    push('r');
    push(';');
    push('C');
    push('{');
    push(')');
    push('(');
    push('m');
    set_tree_state("m(){C;r(E);}\0");
    return 1;
  }
  else if (token == 'g' && STACK[TOP] == 'G')
  {
    PI = 5;
    TI = 5;
    print_state(1, token);
    pop();
    push('}');
    push(';');
    push(')');
    push('E');
    push('(');
    push('r');
    push(';');
    push('C');
    push('{');
    push(')');
    push('(');
    push('g');
    set_tree_state("g(){C;r(E);}\0");
    return 1;
  }
  else if (token == 'f' && STACK[TOP] == 'F')
  {
    PI = 4;
    TI = 4;
    print_state(1, token);
    pop();
    push('}');
    push(';');
    push(')');
    push('E');
    push('(');
    push('r');
    push(';');
    push('C');
    push('{');
    push(')');
    push('(');
    push('f');
    set_tree_state("f(){C;r(E);}\0");
    return 1;
  }

  // Consome os nomes das funções das funções
  if (token == 'f' && STACK[TOP] == 'f')
  {
    TI = 46;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'm' && STACK[TOP] == 'm')
  {
    TI = 48;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'g' && STACK[TOP] == 'g')
  {
    TI = 47;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'r' && STACK[TOP] == 'r')
  {
    TI = 49;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }

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
  if (token == '0' && STACK[TOP] == 'E')
  {
    TI = 7;
    PI = 7;
    print_state(1, token);
    pop();
    push('0');
    set_tree_state("0\0");
    return 1;
  }
  else if (token == '1' && STACK[TOP] == 'E')
  {
    TI = 8;
    PI = 8;
    print_state(1, token);
    pop();
    push('1');
    set_tree_state("1\0");
    return 1;
  }
  else if (token == 'x' && STACK[TOP] == 'E')
  {
    TI = 9;
    PI = 9;
    print_state(1, token);
    pop();
    push('x');
    set_tree_state("x\0");
    return 1;
  }
  else if (token == 'y' && STACK[TOP] == 'E')
  {
    TI = 10;
    PI = 10;
    print_state(1, token);
    pop();
    push('y');
    set_tree_state("y\0");
    return 1;
  }
  else if ((token == '(' && STACK[TOP] == 'E'))
  {
    TI = 11;
    PI = 11;
    print_state(1, token);
    pop();
    push(')');
    push('E');
    push('X');
    push('E');
    push('(');
    set_tree_state("(EXE)\0");
    return 1;
  }
  // Consume
  else if (token == '0' && STACK[TOP] == '0')
  {
    TI = 25;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == '1' && STACK[TOP] == '1')
  {
    TI = 26;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'x' && STACK[TOP] == 'x')
  {
    TI = 27;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'y' && STACK[TOP] == 'y')
  {
    TI = 28;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
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
  if (token == '+' && STACK[TOP] == 'X')
  {
    PI = 12;
    TI = 12;
    print_state(1, token);
    pop();
    push('+');
    set_tree_state("+\0");
    return 1;
  }
  else if (token == '-' && STACK[TOP] == 'X')
  {
    PI = 13;
    TI = 13;
    print_state(1, token);
    pop();
    push('-');
    set_tree_state("-\0");
    return 1;
  }
  else if (token == '*' && STACK[TOP] == 'X')
  {
    PI = 14;
    TI = 14;
    print_state(1, token);
    pop();
    push('*');
    set_tree_state("*\0");
    return 1;
  }
  else if (token == '/' && STACK[TOP] == 'X')
  {
    PI = 15;
    TI = 15;
    print_state(1, token);
    pop();
    push('/');
    set_tree_state("/\0");
    return 1;
  }
  // Consome
  else if (token == '+' && STACK[TOP] == '+')
  {
    TI = 31;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == '-' && STACK[TOP] == '-')
  {
    TI = 32;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == '*' && STACK[TOP] == '*')
  {
    TI = 33;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == '/' && STACK[TOP] == '/')
  {
    TI = 34;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
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
  if (token == 'h' && STACK[TOP] == 'C')
  {
    PI = 16;
    TI = 16;
    print_state(1, token);
    pop();
    push('E');
    push('=');
    push('h');
    set_tree_state("h=E\0");
    return 1;
  }
  else if (token == 'j' && STACK[TOP] == 'C')
  {
    PI = 18;
    TI = 18;
    print_state(1, token);
    pop();
    push('E');
    push('=');
    push('j');
    set_tree_state("j=E\0");
    return 1;
  }
  else if (token == 'i' && STACK[TOP] == 'C')
  {
    PI = 17;
    TI = 17;
    print_state(1, token);
    pop();
    push('E');
    push('=');
    push('i');
    set_tree_state("i=E\0");
    return 1;
  }
  else if (token == 'k' && STACK[TOP] == 'C')
  {
    PI = 19;
    TI = 19;
    print_state(1, token);
    pop();
    push('E');
    push('=');
    push('k');
    set_tree_state("k=E\0");
    return 1;
  }
  else if (token == 'z' && STACK[TOP] == 'C')
  {
    PI = 20;
    TI = 20;
    print_state(1, token);
    pop();
    push('E');
    push('=');
    push('z');
    set_tree_state("z=E\0");
    return 1;
  }
  // p21-p24
  else if (token == '(' && STACK[TOP] == 'C')
  {
    PI = 21;
    TI = 21;
    print_state(1, token);
    pop();
    push(')');
    push('E');
    push('X');
    push('E');
    push('(');
    set_tree_state("(EXE)\0");
    return 1;
  }
  else if (token == 'w' && STACK[TOP] == 'C')
  {
    PI = 22;
    TI = 22;
    print_state(1, token);
    pop();
    push('}');
    push(';');
    push('C');
    push('{');
    push(')');
    push('E');
    push('(');
    push('w');
    set_tree_state("w(E){C;}\0");
    return 1;
  }
  else if (token == 'f' && STACK[TOP] == 'C')
  {
    PI = 23;
    TI = 23;
    print_state(1, token);
    pop();
    push('}');
    push(';');
    push('C');
    push('{');
    push(')');
    push('E');
    push('(');
    push('f');
    set_tree_state("f(E){C;}\0");
    return 1;
  }
  else if (token == 'o' && STACK[TOP] == 'C')
  {
    PI = 24;
    TI = 24;
    print_state(1, token);
    pop();
    push('}');
    push(';');
    push('C');
    push('{');
    push(')');
    push('E');
    push(';');
    push('E');
    push(';');
    push('E');
    push('(');
    push('o');
    set_tree_state("o(E;E;E){C;}\0");
    return 1;
  }
  // Consome
  else if (token == 'w' && STACK[TOP] == 'w')
  {
    TI = 41;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'o' && STACK[TOP] == 'o')
  {
    TI = 45;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'f' && STACK[TOP] == 'f')
  {
    TI = 46;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'h' && STACK[TOP] == 'h')
  {
    TI = 35;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'j' && STACK[TOP] == 'j')
  {
    TI = 37;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'i' && STACK[TOP] == 'i')
  {
    TI = 36;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'k' && STACK[TOP] == 'k')
  {
    TI = 38;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'z' && STACK[TOP] == 'z')
  {
    TI = 39;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
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

  if (token == '(' && STACK[TOP] == '(')
  {
    TI = 29;
    print_state(1, token);
    TOKEN_POS++;
    pop();
    return 1;
  }
  else if (token == ')' && STACK[TOP] == ')')
  {
    TI = 30;
    print_state(1, token);
    TOKEN_POS++;
    pop();
    return 1;
  }
  else if (token == '{' && STACK[TOP] == '{')
  {
    TI = 42;
    print_state(1, token);
    TOKEN_POS++;
    pop();
    return 1;
  }
  else if (token == '}' && STACK[TOP] == '}')
  {
    TI = 44;
    print_state(1, token);
    TOKEN_POS++;
    pop();
    return 1;
  }
  else if (token == 'r' && STACK[TOP] == 'r')
  {
    TI = 49;
    print_state(1, token);
    TOKEN_POS++;
    pop();
    return 1;
  }
  else if (token == '=' && STACK[TOP] == '=')
  {
    TI = 40;
    print_state(1, token);
    TOKEN_POS++;
    pop();
    return 1;
  }
  else if (token == ';' && STACK[TOP] == ';')
  {
    TI = 43;
    print_state(1, token);
    TOKEN_POS++;
    pop();
    return 1;
  }

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
  // if (PI == 0 && TI == 0)
  //   printf("Step: %2d \t Estado: q%3d \t Token: %5c \t Stack: %20s \t\t pi: - \t\t ti: - \t\t Word: %s", STEP, state, token, STACK, WORD);
  // else if (PI == 0 && TI > 0)
  //   printf("Step: %2d \t Estado: q%3d \t Token: %5c \t Stack: %20s \t\t pi: - \t\t ti: t%2d \t Word: %s", STEP, state, token, STACK, TI, WORD);
  // else if (TI == 0 && PI > 0)
  //   printf("Step: %2d \t Estado: q%3d \t Token: %5c \t Stack: %20s \t\t pi: p%2d \t ti: - \t\t Word: %s", STEP, state, token, STACK, PI, WORD);
  // else
  //   printf("Step: %2d \t Estado: q%3d \t Token: %5c \t Stack: %20s \t\t pi: p%2d \t ti: t%2d \t Word: %s", STEP, state, token, STACK, PI, TI, WORD);

  PI = 0;
  TI = 0;
}

void show()
{
  for (int i = 0; i < TOP; i++)
  {
    printf("Index: %1d \t Stack: %1c\n", i, STACK[i]);
  }
}

void set_tree_state(char *word)
{
  int local_stack[20];
  int index = PROD_STACK[PROD_INDEX];
  int i = 0;

  prod_pop();
  for (i; word[i] != '\0'; i++)
  {
    fflush(stdout);
    setbuf(stdout, NULL);
    TREE[TREE_N].value = word[i];
    TREE[TREE_N++].id = 12 * index + i + 1;
    if (isupper(word[i]))
      local_stack[i] = TREE[TREE_N - 1].id;
    else
      local_stack[i] = -1;
  }

  if (word[1] == '\0')
  {
    return;
  }

  for (i; i >= 0; i--)
  {
    if (local_stack[i] != -1)
    {
      prod_push(local_stack[i]);
    }
  }
}

void prod_push(int val)
{
  PROD_STACK[++PROD_INDEX] = val;
}

void prod_pop()
{
  PROD_STACK[PROD_INDEX--] = -1;
}

void show_tree()
{
  for (int i = 0; i < TREE_N; i++)
  {
    printf("T[%d] = %2c \t ID = %2d\n", i, TREE[i].value, TREE[i].id);
  }
}

void record_tree()
{
  FILE *output = fopen("tree.txt", "w+");
  for (int i = 0; i < TREE_N; i++)
  {
    fprintf(output, "%c, %d \n", TREE[i].value, TREE[i].id);
  }
  fclose(output);
}
