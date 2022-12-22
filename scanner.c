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
int p7_p14(char);
// Lida com as produções p12 à p15
int p12_p15(char);
// Lida com as produções p16 à p24
int p19_p28(char);
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
int ASA2ASA();
int ASA2ASAP();
int parser();

/*
 * Para executar o programa é necessário passar o nome do arquivo pelo argv saida.exe input.txt
 * no arquivo presente existem várias palavras, somente a primeira linha será lida.
 */
int main(int argc, char *argv[])
{
  char token;
  file = fopen(argv[1], "r");
  fgets(WORD, WORD_LEN, file);
  // strcpy(WORD, "m(){(y/y);r(y);}\n");

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
      print_state(1, '\0');
      goto fim;
    }

    if (consume_extras(token))
      goto q1;
    else if (p1_p6(token))
      goto q1;
    else if (p7_p14(token))
      goto q1;
    else if (p12_p15(token))
      goto q1;
    else if (p19_p28(token))
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
      show_tree();
      record_tree();
      parser();
      ASA2ASA();
      ASA2ASAP();
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
   * p1: S → M
   * p2: S -> G M
   * p3: S -> N G M
   * p4: N → n(){ A; r(E); }
   * p5: G → g(){ A; r(E); }
   * p6: M → m(){ A; r(E); }
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
  else if (token == 'n' && STACK[TOP] == 'S')
  {
    PI = 3;
    TI = 3;
    print_state(1, token);
    pop();
    push('N');
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
    push('A');
    push('{');
    push(')');
    push('(');
    push('m');
    set_tree_state("m(){A;r(E);}\0");
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
    push('A');
    push('{');
    push(')');
    push('(');
    push('g');
    set_tree_state("g(){A;r(E);}\0");
    return 1;
  }
  else if (token == 'n' && STACK[TOP] == 'N')
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
    push('A');
    push('{');
    push(')');
    push('(');
    push('n');
    set_tree_state("n(){A;r(E);}\0");
    return 1;
  }

  // Consome os nomes das funções das funções
  if (token == 'n' && STACK[TOP] == 'n')
  {
    TI = 55;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'm' && STACK[TOP] == 'm')
  {
    TI = 53;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'g' && STACK[TOP] == 'g')
  {
    TI = 52;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'r' && STACK[TOP] == 'r')
  {
    TI = 54;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }

  return 0;
}

int p7_p14(char token)
{
  /*
   * p7: A → CB
   * p8: B -> .
   * p9: B -> ;CB
   * p10: E → 0
   * p11: E -> 1
   * p12: E -> x
   * p13: E -> y
   * p14: E -> (EXE)
   */

  /*
     if A -> EMPILHA CB
     IF . -> B push . POP
     if ; -> CB
  */
  // Empilha
  if (STACK[TOP] == 'A')
  {

    TI = 7;
    PI = 7;
    print_state(1, token);
    pop();
    push('B');
    push('C');
    set_tree_state("CB\0");
  }

  if (token == '.' && STACK[TOP] == 'B')
  {
    TI = 8;
    PI = 8;
    print_state(1, token);
    pop();
    push('.');
    set_tree_state(".\0");
    return 1;
  }
  else if (token == ';' && STACK[TOP] == 'B')
  {
    TI = 9;
    PI = 9;
    print_state(1, token);
    pop();
    push('B');
    push('C');
    push(';');
    set_tree_state(";CB\0");
    return 1;
  }
  else if (token == '0' && STACK[TOP] == 'E')
  {
    TI = 10;
    PI = 10;
    print_state(1, token);
    pop();
    push('0');
    set_tree_state("0\0");
    return 1;
  }
  else if (token == '1' && STACK[TOP] == 'E')
  {
    TI = 11;
    PI = 11;
    print_state(1, token);
    pop();
    push('1');
    set_tree_state("1\0");
    return 1;
  }
  else if (token == 'x' && STACK[TOP] == 'E')
  {
    TI = 12;
    PI = 12;
    print_state(1, token);
    pop();
    push('x');
    set_tree_state("x\0");
    return 1;
  }
  else if (token == 'y' && STACK[TOP] == 'E')
  {
    TI = 13;
    PI = 13;
    print_state(1, token);
    pop();
    push('y');
    set_tree_state("y\0");
    return 1;
  }
  else if ((token == '(' && STACK[TOP] == 'E'))
  {
    TI = 14;
    PI = 14;
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
    TI = 30;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == '1' && STACK[TOP] == '1')
  {
    TI = 31;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'x' && STACK[TOP] == 'x')
  {
    TI = 32;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'y' && STACK[TOP] == 'y')
  {
    TI = 33;
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
   * p12: X → +
   * p13: X -> -
   * p14: X -> *
   * p15: X -> /
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

int p19_p28(char token)
{
  /*
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
   */

  // Empilha
  if (token == 'h' && STACK[TOP] == 'C')
  {
    PI = 19;
    TI = 19;
    print_state(1, token);
    pop();
    push(')');
    push('(');
    push('g');
    push('=');
    push('h');
    set_tree_state("h=g()\0");
    return 1;
  }
  else if (token == 'j' && STACK[TOP] == 'C')
  {
    PI = 21;
    TI = 21;
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
    PI = 20;
    TI = 20;
    print_state(1, token);
    pop();
    push(')');
    push('(');
    push('n');
    push('=');
    push('i');
    set_tree_state("i=n()\0");
    return 1;
  }
  else if (token == 'k' && STACK[TOP] == 'C')
  {
    PI = 22;
    TI = 22;
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
    PI = 23;
    TI = 23;
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
    PI = 24;
    TI = 24;
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
    PI = 25;
    TI = 25;
    print_state(1, token);
    pop();
    push('D');
    push('C');
    push('{');
    push(')');
    push('E');
    push('(');
    push('w');
    set_tree_state("w(E){CD\0");
    return 1;
  }
  else if (token == 'f' && STACK[TOP] == 'C')
  {
    PI = 26;
    TI = 26;
    print_state(1, token);
    pop();
    push('D');
    push('C');
    push('{');
    push(')');
    push('E');
    push('(');
    push('f');
    set_tree_state("f(E){CD\0");
    return 1;
  }
  else if (token == 'o' && STACK[TOP] == 'C')
  {
    PI = 27;
    TI = 27;
    print_state(1, token);
    pop();
    push('D');
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
    set_tree_state("o(E;E;E){CD\0");
    return 1;
  }
  else if (token == '}' && STACK[TOP] == 'D')
  {
    PI = 28;
    TI = 28;
    print_state(1, token);
    pop();
    push('}');
    set_tree_state("}\0");
    return 1;
  }
  else if (token == ';' && STACK[TOP] == 'D')
  {
    PI = 29;
    TI = 29;
    print_state(1, token);
    pop();
    push('D');
    push('C');
    push(';');
    set_tree_state(";CD\0");
    return 1;
  }
  // Consome
  else if (token == 'w' && STACK[TOP] == 'w')
  {
    TI = 46;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'o' && STACK[TOP] == 'o')
  {
    TI = 50;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'f' && STACK[TOP] == 'f')
  {
    TI = 51;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'h' && STACK[TOP] == 'h')
  {
    TI = 40;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'j' && STACK[TOP] == 'j')
  {
    TI = 42;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'i' && STACK[TOP] == 'i')
  {
    TI = 41;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'k' && STACK[TOP] == 'k')
  {
    TI = 43;
    print_state(1, token);
    pop();
    TOKEN_POS++;
    return 1;
  }
  else if (token == 'z' && STACK[TOP] == 'z')
  {
    TI = 44;
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
   * (
   * )
   * {
   * }
   * ;
   * =
   * .
   */

  if (token == '(' && STACK[TOP] == '(')
  {
    TI = 34;
    print_state(1, token);
    TOKEN_POS++;
    pop();
    return 1;
  }
  else if (token == ')' && STACK[TOP] == ')')
  {
    TI = 35;
    print_state(1, token);
    TOKEN_POS++;
    pop();
    return 1;
  }
  else if (token == '{' && STACK[TOP] == '{')
  {
    TI = 47;
    print_state(1, token);
    TOKEN_POS++;
    pop();
    return 1;
  }
  else if (token == '}' && STACK[TOP] == '}')
  {
    TI = 49;
    print_state(1, token);
    TOKEN_POS++;
    pop();
    return 1;
  }
  else if (token == 'r' && STACK[TOP] == 'r')
  {
    TI = 54;
    print_state(1, token);
    TOKEN_POS++;
    pop();
    return 1;
  }
  else if (token == '=' && STACK[TOP] == '=')
  {
    TI = 45;
    print_state(1, token);
    TOKEN_POS++;
    pop();
    return 1;
  }
  else if (token == ';' && STACK[TOP] == ';')
  {
    TI = 48;
    print_state(1, token);
    TOKEN_POS++;
    pop();
    return 1;
  }
  else if (token == '.' && STACK[TOP] == '.')
  {
    TI = 8;
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
  if (PI == 0 && TI == 0)
    printf("Step: %2d \t Estado: q%3d \t Token: %5c \t Stack: %20s \t\t pi: - \t\t ti: - \t\t Word: %s", STEP, state, token, STACK, WORD);
  else if (PI == 0 && TI > 0)
    printf("Step: %2d \t Estado: q%3d \t Token: %5c \t Stack: %20s \t\t pi: - \t\t ti: t%2d \t Word: %s", STEP, state, token, STACK, TI, WORD);
  else if (TI == 0 && PI > 0)
    printf("Step: %2d \t Estado: q%3d \t Token: %5c \t Stack: %20s \t\t pi: p%2d \t ti: - \t\t Word: %s", STEP, state, token, STACK, PI, WORD);
  else
    printf("Step: %2d \t Estado: q%3d \t Token: %5c \t Stack: %20s \t\t pi: p%2d \t ti: t%2d \t Word: %s", STEP, state, token, STACK, PI, TI, WORD);

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
  int local_stack[100];
  int index = PROD_STACK[PROD_INDEX];
  int i = 0;
  int add_prod = 0;
  // for (int i = 0; i <= PROD_INDEX; i++)
  // {
  //   printf("Prod {%d} Prod Index {%d} Index {%d} Word {%s}\n", PROD_STACK[i], i, index, word);
  // }
  // puts("--------------------------------------------------------------------------------");
  prod_pop();

  // printf("Producao {%d} Prod Index {%d} Word {%s}\n", index, PROD_INDEX, word);
  for (; word[i] != '\0'; i++)
  {
    TREE[TREE_N].value = word[i];
    TREE[TREE_N++].id = 12 * index + i + 1;
    if (isupper(word[i]))
    {
      add_prod = 1;
      local_stack[i] = TREE[TREE_N - 1].id;
    }
    else
      local_stack[i] = -1;
  }

  if (word[1] == '\0' || !add_prod)
  {
    return;
  }

  for (; i >= 0; i--)
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
