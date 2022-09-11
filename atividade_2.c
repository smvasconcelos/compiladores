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
		case 0: // Fatorial
			{
			// main(void)
				code[index].f = INT; code[index++].a = 5;   // Allocates memory
				code[index].f = LIT; code[index++].a = 6;   // Starts paramater fat(value)
				code[index].f = STO; code[index++].a = 9;   // Pass as Parameter fat(value)
				code[index].f = CAL; code[index++].a = 6;   // Calls function
				code[index].f = LOD; code[index++].a = 10;  // Result
				code[index].f = OPR; code[index++].a = 0;   // END

			// fat(value:int)
				// Starts variables
				code[index].f = INT; code[index++].a = 6;   // Allocates memory
				// I - Counter [3]
				code[index].f = LIT; code[index++].a = 1;   // Starts Counter
				code[index].f = STO; code[index++].a = 3;   // Saves Counter
				// I - Counter [3]
				code[index].f = LIT; code[index++].a = 1;   // Starts Result as 1
				code[index].f = STO; code[index++].a = 5;   // Saves Result in 5
				// Starts variables

				// Multply values
				code[index].f = LOD; code[index++].a = 3;		// Loads Counter I
				code[index].f = LOD; code[index++].a = 5;   // Load Result
				code[index].f = OPR; code[index++].a = 4;   // Result * Counter
				code[index].f = STO; code[index++].a = 5;   // Saves Result
				// Multply values

				// Check if its equal to Value
				code[index].f = LOD; code[index++].a = 3;  	// Load Counter
				code[index].f = LOD; code[index++].a = 4;  	// Load Value
				code[index].f = OPR; code[index++].a = 10; 	// Less Than
				code[index].f = JPC; code[index++].a = 23; 	// Leaves loop
				// End of check

				// I++
				code[index].f = LOD; code[index++].a = 3;  	// Loads counter
				code[index].f = OPR; code[index++].a = 2;  	// Counter ++;
				code[index].f = STO; code[index++].a = 3; 	// Saves counter
				// I++

				code[index].f = JMP; code[index++].a = 11; 	// Jumps to start of loop
				code[index].f = OPR; code[index++].a = 0; // End of loop

				for(int i = 0 ; i < index; i++){
					code[index].l = 0;
				}
			}
			break;
		case 1: // Fibibonacci
		{
			// main(void)
				code[index].f = INT; code[index++].a = 5;   // Allocates memory
				code[index].f = LIT; code[index++].a = 10;  // Starts paramater fat(value)
				code[index].f = STO; code[index++].a = 9;   // Pass as Parameter fat(value)
				code[index].f = CAL; code[index++].a = 6;   // Calls function
				code[index].f = LOD; code[index++].a = 11;  // Result
				code[index].f = OPR; code[index++].a = 0;   // END

			// Fat(value:int)
				int first = 5, second = 6, result = 7, counter = 8, value = 4;
				code[index].f = INT; code[index++].a = 10;   // Allocates memory
				// Starts variables
					code[index].f = LIT; code[index++].a = 0;
					code[index].f = STO; code[index++].a = first;
					code[index].f = LIT; code[index++].a = 1;
					code[index].f = STO; code[index++].a = second;
					code[index].f = LIT; code[index++].a = 0;
					code[index].f = STO; code[index++].a = result;
					code[index].f = LIT; code[index++].a = 0;
					code[index].f = STO; code[index++].a = counter;
				// Starts variables

				// Check i > 1
					code[index].f = LOD; code[index++].a = counter; // Line 15
					code[index].f = LIT; code[index++].a = 1;
					code[index].f = OPR; code[index++].a = 11;
					code[index].f = JPC; code[index++].a = 36;
				// Else

				// FIRST + SECOND
						code[index].f = LOD; code[index++].a = first;
						code[index].f = LOD; code[index++].a = second;
						code[index].f = OPR; code[index++].a = 2;
				// FIRST + SECOND

					// RESULT = FIRST + SECOND
						code[index].f = STO; code[index++].a = result;
					// RESULT = FIRST + SECOND

					// FIRST = SECOND;
					// SECOND = RESULT
						code[index].f = LOD; code[index++].a = second;
						code[index].f = STO; code[index++].a = first;
						code[index].f = LOD; code[index++].a = result;
						code[index].f = STO; code[index++].a = second;
					// FIRST = SECOND;
					// SECOND = RESULT

					// COUNTER < VALUE
						code[index].f = LOD; code[index++].a = counter;
						code[index].f = LOD; code[index++].a = value;
						code[index].f = OPR; code[index++].a = 9;
						code[index].f = JPC; code[index++].a = 39; // Line 30
					// COUNTER < VALUE

					// COUNTER ++
						code[index].f = LIT; code[index++].a = 1;
						code[index].f = LOD; code[index++].a = counter;
						code[index].f = OPR; code[index++].a = 2;
						code[index].f = STO; code[index++].a = counter;
						code[index].f = JMP; code[index++].a = 15; // Inicio do loop
					// COUNTER ++

					// Case Counter < Value
						code[index].f = LOD; code[index++].a = counter; // Result = counter (Line 36)
						code[index].f = STO; code[index++].a = result;  // Result = counter
						code[index].f = JMP; code[index++].a = 27;  // Result = counter
					// Case Counter < Value

					code[index].f = OPR; code[index++].a = 0; // END

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

	// 0 : Fatorial
	// 1 : Fibonacci
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
