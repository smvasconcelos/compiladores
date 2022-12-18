#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
	* Produções
	* p1: S → M
	* p2: S -> G M
	* p3: S -> N G M
	* p4: N → n(){ C; r(E); }
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

int WORD_LEN = 100;
int TOKEN_POS = -1;
// File pointer
FILE *file;
char token = '\0';
char WORD[1024];
int LINE = 0;
int COL = 0;
int VALUE[500];
char CLASS[500][500];

void erro(int, char, char *);
int M();
int G();
int N();
int F();
int W();
int O();
int E();
int C();
int X();
void lex();
void table();

int main(int argc, char *argv[])
{

	//
		// Values
		VALUE['1'] = 1;
		VALUE['0'] = 0;
		VALUE['m'] = '-';
		VALUE['n'] = '-';
		VALUE['f'] = '-';
		VALUE['n'] = '-';
		VALUE['x'] = '-';
		VALUE['y'] = '-';
		VALUE['r'] = '-';
		VALUE['('] = '-';
		VALUE[')'] = '-';
		VALUE['{'] = '-';
		VALUE['}'] = '-';
		VALUE[';'] = '-';
		VALUE['='] = '-';
		VALUE['+'] = '-';
		VALUE['-'] = '-';
		VALUE['/'] = '-';
		VALUE['*'] = '-';
		VALUE[';'] = '-';
		VALUE[' '] = '-';
		VALUE['\n'] = '-';
		// Classes
		strcpy(CLASS['1'], "number");
		strcpy(CLASS['0'], "number");
		strcpy(CLASS['m'], "keyword");
		strcpy(CLASS['n'], "keyword");
		strcpy(CLASS['g'], "keyword");
		strcpy(CLASS['f'], "keyword");
		strcpy(CLASS['r'], "keyword");
		strcpy(CLASS['o'], "keyword");
		strcpy(CLASS['w'], "keyword");
    strcpy(CLASS['h'], "varible");
    strcpy(CLASS['i'], "varible");
    strcpy(CLASS['z'], "varible");
    strcpy(CLASS['x'], "varible");
    strcpy(CLASS['y'], "variable");
    strcpy(CLASS['('], "special symbol");
    strcpy(CLASS[')'], "special symbol");
    strcpy(CLASS['{'], "special symbol");
    strcpy(CLASS['}'], "special symbol");
    strcpy(CLASS[';'], "separator");
    strcpy(CLASS[' '], "separator");
    strcpy(CLASS['\n'], "separator");
		strcpy(CLASS['='], "attribution indicator");
		strcpy(CLASS['+'], "operator");
		strcpy(CLASS['-'], "operator");
		strcpy(CLASS['*'], "operator");
		strcpy(CLASS['/'], "operator");
	//
	file = fopen(argv[1], "r");
  // getline(&WORD, &WORD_LEN, file);
  if (!file)
  {
    puts("Insira um caminho de arquivo valido");
    return 0;
  }
  fgets(WORD, WORD_LEN, file);
  lex();
  M();
  // G();
  // N();
  return 0;
}

// p6: M → m(){ C; r(E); }
int M(){
	int err = 0;
	if(token=='m'){ lex();
		if(token=='('){ lex();
			if(token==')'){ lex();
				if(token=='{'){ lex();
					C();
					if(token==';'){ lex();
						if(token=='r'){ lex();
							if(token=='('){ lex();
								E();
								if(token==')'){ lex();
									if(token==';'){ lex();
										if(token=='}'){ lex();}
										else { erro(++err, 'M', "}");}
									}//if-;
									else { erro(++err, 'M', ";");}
								}//if-)
								else { erro(++err, 'M', ")");}
							}//if-(
							else { erro(++err, 'M', "(");}
						}//if-r 
						else { erro(++err, 'M', "r");}
					}//if-;
					else { erro(++err, 'M', ";");}
				}//if-{
				else { erro(++err, 'M', "{");} // não veio {
			}//if-)
			else { erro(++err, 'M', ")");} // não veio )
		}//if-(
		else { erro(++err, 'M', "(");} // não veio (
	}//if-m
	else { erro(++err, 'M', "m"); } // não veio m
}//end-M

// p5: G → g(){ C; r(E); }
int G(){
	int err = 0;
	if(token=='g'){ lex();
		if(token=='('){ lex();
			if(token==')'){ lex();
				if(token=='{'){ lex();
					C();
					if(token==';'){ lex();
						if(token=='r'){ lex();
							if(token=='('){ lex();
								E();
								if(token==')'){ lex();
									if(token==';'){ lex();
										if(token=='}'){ lex();}
										else { erro(++err, 'G', "}");}
									}//if-;
									else { erro(++err, 'M', ";");}
								}//if-)
								else { erro(++err, 'G', ")");}
							}//if-(
							else { erro(++err, 'G', "(");}
						}//if-r
						else { erro(++err, 'G', "r");}
					}//if-;
					else { erro(++err, 'G', ";");}
				}//if-{
				else { erro(++err, 'G', "{");} // não veio {
			}//if-)
			else { erro(++err, 'G', ")");} // não veio )
		}//if-(
		else { erro(++err, 'G', "(");} // não veio (
	}//if-g
	else { erro(++err, 'G', "g"); } // não veio m
}//end-G

// p5: N → n(){ C; r(E); }
int N(){
	int err = 0;
	if(token=='n'){ lex();
		if(token=='('){ lex();
			if(token==')'){ lex();
				if(token=='{'){ lex();
					C();
					if(token==';'){ lex();
						if(token=='r'){ lex();
							if(token=='('){ lex();
								E();
								if(token==')'){ lex();
									if(token==';'){ lex();
										if(token=='}'){ lex();}
										else { erro(++err, 'N', "}");}
									}//if-;
									else { erro(++err, 'M', ";");}
								}//if-)
								else { erro(++err, 'N', ")");}
							}//if-(
							else { erro(++err, 'N', "(");}
						}//if-r
						else { erro(++err, 'N', "R");}
					}//if-;
					else { erro(++err, 'N', ";");}
				}//if-{
				else { erro(++err, 'N', "{");} // não veio {
			}//if-)
			else { erro(++err, 'N', ")");} // não veio )
		}//if-(
		else { erro(++err, 'N', "(");} // não veio (
	}//if-n
	else { erro(++err, 'N', "N"); } // não veio m
}//end-N

// p7: E → 0 | 1 | x | y | (EXE)
int E(){
	if (token=='0'|| token=='1'|| token=='x'|| token=='y'){ lex(); }
	else if (token=='('){
		lex();
		E();
		X();
		E();
		if (token==')'){ lex(); }
		else {erro(1, 'E', ")");}
	}else {erro(2, 'E', "0 | 1 | x | y | (");}
	return(0);
}//end-E

//p12: X → + | - | * | /
int X(){
	if (token=='+'|| token=='-'|| token=='*'|| token=='/'){ lex(); }
	else { erro(1, 'X', "+ | - | * | /"); }
	return(0);
}//end

// p16: C → h=E | i=E | j=E | k=E | z=E | (EXE) | w(E){ C; } | f(E){ C; } | o(E; E; E){ C; }
int C(){
	if (token=='h'|| token=='i'|| token=='j'||token=='k'||token=='z'){ lex();
			if (token=='='){ lex(); E(); }
			else { erro(1, 'C', "="); }
	}
	else if (token=='('){ lex();
			E();
			X();
			E();
			if (token==')'){ lex(); }
			else { erro(2, 'C', ")"); }
	}
	else if (token=='w'){ lex(); W(); }
	else if (token=='f'){ lex(); F(); }
	else if (token=='o'){ lex(); O(); }
	else { erro(3, 'C', "h | i | j | k | z | ( | w | f | o"); }
	return(0);
}

// p22: W →w(E){ C; }
int W(){
	if (token=='('){ lex();
				E();
		if (token==')'){ lex();
			if (token=='{'){ lex();
				C();
				if (token==';'){ lex();
					if (token=='}'){ lex(); }
					else { erro(1, 'W', "}"); }
				} else { erro(2, 'W', ";"); }
			} else { erro(3, 'W', "{"); }
		} else { erro(4, 'W', ")"); }
	} else { erro(5, 'W', "("); }
}//end-W

// p23: F →f(E){ C; }
int F(){
	if (token=='('){ lex();
		E();
		if (token==')'){ lex();
			if (token=='{'){ lex();
				C();
				if (token==';'){ lex();
						if (token=='}'){ lex(); }
						else { erro(1, 'F', "}"); }
				} else { erro(2, 'F', ";"); }
			} else { erro(3, 'F', "{"); }
		} else { erro(4, 'F', ")"); }
	} else { erro(5, 'F', "("); }
}//end-f

// p24: O → o(E; E; E){ C; }
int O(){
	if (token=='('){ lex();
        E();
        if (token==';'){ lex();
           E();
           if (token==';'){ lex();
              E();
              if (token==')'){ lex();
                 if (token=='{'){ lex();
                    C();
                    if (token==';'){ lex();
                       if (token=='}'){ lex(); }
                       else { erro(1, 'O', "}"); }
                    }
                    else { erro(2, 'O', ";"); }
                 }
                 else { erro(3, 'O', "{"); }
              }
              else { erro(4, 'O', ")"); }
           }
           else { erro(5, 'O', ";"); }
        }
        else { erro(6, 'O', ";"); }
	}
	else { erro(7, 'O', "("); }

	return(0);
}

void erro(int err, char where, char * expected)
{
	COL = TOKEN_POS;
	puts("**************************************************************************************************");
	printf("\tErro: [%d] \t Expressao: '%c' \t Linha: [%d] \t Coluna: [%d]\n", err, where, LINE, COL);
	printf("\tEra esperado ['%s'] \t foi lido ['%c'].\n", expected, token);
	puts("**************************************************************************************************");
	exit(1);
}

void lex()
{
	token = WORD[++TOKEN_POS];
	table();
}

void table()
{
	COL = TOKEN_POS;
	printf("L: %2d | \t C: %2d |\t Token: %2c |\t Value: %2c |\t\t Class: %2s\n", LINE, COL, token == '\n'? '-' : token, VALUE[token], CLASS[token]);
}
