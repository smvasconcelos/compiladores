#include <stdio.h>
#include <stdlib.h>

#define CXMAX 200 // {size of code array}
#define STACKSIZE 500
#define LIMITE_MAX 20

#define VALUE 3
#define VALUE_2 4

const int amax = 2047; // {maximum address}
const int levmax = 3; // {maximum depth of block nesting}

enum fct {LIT, OPR, LOD, STO, CAL, INT, JMP, JPC, END}; // {Operations type}
typedef struct {
    enum fct f;
    int l;
    int a;
}  Instruction;

Instruction code[CXMAX]; // Instrucions array
int s[STACKSIZE]; // {datastore}
int p, b, t; // {program, base, topstack-registers}

int base(int l){
	// {find base l levels down}
    int b1;
    b1 = b;
    while(l > 0){
        b1 = s[b1];
        l = l-1;
    }
    return b1;
}

void question(int which)
{
	int index = 0;
	switch(which)
	{
		case 0: // Fatorial Recursivo
		{

			// if (n <= 1)
			// 	return 1;
			// return n * fat(n - 1);

			// main(void)
				code[index].f = INT; code[index++].a = 3;   // Allocates memory
				code[index].f = LIT; code[index++].a = 4;   // Starts paramater fat(value)
				code[index].f = STO; code[index++].a = 6;   // Pass as Parameter fat(value)
				code[index].f = CAL; code[index++].a = 6;   // Calls function
				code[index].f = LOD; code[index++].a = 7;   // Result
				code[index].f = OPR; code[index++].a = 0;   // END

			// Fat(value:int)
				int value = 3;
				code[index].f = INT; code[index++].a = 5;   // Allocates memory
				// Loads n and 1 for check : n <= 1 ? return 1 else: calculate fat(n-1)
				code[index].f = LOD; code[index++].a = value;
				code[index].f = LIT; code[index++].a = 1;
				code[index].f = OPR; code[index++].a = 10;
				code[index].f = JPC; code[index++].a = 13;
				// end

				// Case n <= 1 return 1
				code[index].f = STO; code[index++].a = 4;
				code[index].f = OPR; code[index++].a = 0;
				//

				// Case n > 1 , do fat(n-1)
				code[index].f = LOD; code[index++].a = value;
				code[index].f = LIT; code[index++].a = 1;
				code[index].f = OPR; code[index++].a = 3;
				code[index].f = STO; code[index++].a = 8;
				code[index].f = CAL; code[index++].a = 6;
				// end

				// n * fat(n-1);
				code[index].f = LOD; code[index++].a = 9;
				code[index].f = LOD; code[index++].a = value;
				code[index].f = OPR; code[index++].a = 4;
				code[index].f = STO; code[index++].a = 4;
				code[index].f = OPR; code[index++].a = 0;
				// end

				for(int i = 0 ; i < index; i++){
					code[index].l = 0;
				}

				for(int i = 0 ; i < index; i++){
					code[index].l = 0;
				}
			}
			break;
		case 1: // Fibonacci Recursivo
		{

			// if (n <= 1)
			// 	return n;
			// return fib(n - 1) + fib(n - 2);

			// main(void)
				code[index].f = INT; code[index++].a = 3;   // Allocates memory
				code[index].f = LIT; code[index++].a = 8;  // Starts paramater fib(value)
				code[index].f = STO; code[index++].a = 6;   // Pass as Parameter fib(value)
				code[index].f = CAL; code[index++].a = 6;   // Calls function
				code[index].f = LOD; code[index++].a = 6;   // Result
				code[index].f = OPR; code[index++].a = 0;   // END

			// fib(value:int)
				int value = 3;
				code[index].f = INT; code[index++].a = 5;   // Allocates memory
				// Loads n and 1 for check : n <= 1 ? return 1 else: calculate fib(n-1)
				code[index].f = LOD; code[index++].a = value;
				code[index].f = LIT; code[index++].a = 1;
				code[index].f = OPR; code[index++].a = 10;
				code[index].f = JPC; code[index++].a = 12;
				// end

				// Case n <= 1 return n
				// code[index].f = LOD; code[index++].a = value;
				// code[index].f = STO; code[index++].a = 4;
				code[index].f = OPR; code[index++].a = 0;
				//

				// Case n > 1 , do fib(n-1)
				code[index].f = LOD; code[index++].a = value;
				code[index].f = LIT; code[index++].a = 1;
				code[index].f = OPR; code[index++].a = 3;
				code[index].f = STO; code[index++].a = value + 5;
				code[index].f = CAL; code[index++].a = 6;
				// end

				// fib(n -1) + fib(n - 2);
				code[index].f = LOD; code[index++].a = value + 5; // fib(n - 1)
				code[index].f = LOD; code[index++].a = value + 5 * 2; // fib(n - 2)
				code[index].f = OPR; code[index++].a = 2;
				code[index].f = STO; code[index++].a = value;
				code[index].f = OPR; code[index++].a = 0;
				// end

				for(int i = 0 ; i < index; i++){
					code[index].l = 0;
				}
		}
		break;
		default:
			break;
	}
}

int main(){
  // Main:

	// 0 : Fatorial Recursivo
	// 1 : Fibonacci Recursivo
	question(0);
  // PCode Machine:
  Instruction i;
	puts(" start pl/0 ");
	t = 0;
	b = 1;
	p = 0;
	s[1] = 0;
	s[2] = 0;
	s[3] = 0;
	printf("\n| t |  b |  p |");
	do {
			i =  code[p];
			printf("\n|%2d | %2d | %2d | ", t, b, p);
			p++;
			switch (i.f){
					case LIT:
							t++;
							s[t] = i.a;
							break;
					case OPR:
							switch(i.a){
									case 0:
											t = b - 1;
											p = s[t + 3];
											b = s[t + 2];
											break;
									case 1: // Inversor
											s[t] = -s[t];
											break;
									case 2: // Sum
											t = t - 1;
											s[t] = s[t] + s[t + 1];
											break;
									case 3: // Subtract
											t = t - 1;
											s[t] = s[t] - s[t + 1];
											break;
									case 4: // Multiply
											t = t - 1;
											s[t] = s[t] * s[t + 1];
											break;
									case 5: // Divide
											t = t - 1;
											s[t] = s[t] / s[t + 1];
											break;
									case 6: // Module of 2
											s[t] = (s[t]) % 2;
											break;
									case 7: // Equals
											t = t - 1;
											s[t] = (s[t] == s[t + 1]);
											break;
									case 8: // Difference
											t = t - 1;
											s[t] = (s[t] != s[t + 1]);
											break;
									case 9: // Less than
											t = t - 1;
											s[t] = (s[t] < s[t + 1]);
											break;
									case 10: // Less/equal than
											t = t - 1;
											s[t] = (s[t] <= s[t + 1]);
											break;
									case 11: // Greater than
											t = t - 1;
											s[t] = (s[t] > s[t + 1]);
											break;
									case 12: // Greater/equal than
											t = t - 1;
											s[t] = (s[t] >= s[t + 1]);
											break;
							}
							break;
					case LOD: // Loads a variable of the stack
							t++;
							s[t] = s[base(i.l) + i.a];
							break;
					case STO: // Store a variable into the stack
							// stack[Memory allocation (x) + i.a] = topstack;
							s[base(i.l) + i.a] = s[t];
							t--;
							break;
					case CAL: // Calls a function on the stack
							s[t + 1] = base(i.l);
							s[t + 2] = b;
							s[t + 3] = p;
							b = t + 1;
							p = i.a;
						break;
					case INT: // 'Allocates' a int variable
							t = t + i.a;
							break;
					case JMP: // Jumps to instruction of code i.a
							p = i.a;
							break;
					case JPC: // Jumps to instruction i.a case the top value is 0
							if (s[t] == 0){
									p = i.a;
									t = t - 1;
							}
							break;
					case END: // 'Ends'
							p = 0;
							break;
			}
			for(int j = 1; j <= t; j++){
					printf("[%d]", s[j]);
			}
	} while(p != 0);
  return 0;
}
