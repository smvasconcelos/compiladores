#include <stdio.h>
#include <stdlib.h>

#define CXMAX 200			// {size of code array}
#define STACKSIZE 500 // Size of stack

const int amax = 2047, levmax = 3; //{maximum address} {maximum depth of block nesting}

// Intructions codes
enum fct
{
	LIT,
	OPR,
	LOD,
	STO,
	CAL,
	INT,
	JMP,
	JPC,
	END
};

typedef struct
{
	enum fct f;
	int l;
	int a;
} Instruction;

Instruction code[CXMAX]; // Array of instructions
int s[STACKSIZE];				 // Stack
int p, b, t;						 //  {program-, base-, topstack-registers}

// {find base l levels down}
int base(int l)
{
	int b1;
	b1 = b;
	while (l > 0)
	{
		b1 = s[b1];
		l = l - 1;
	}
	return b1;
}

// Questions
int instructions(int which)
{
	switch (which)
	{
	case 0: // Sums two integers 3 + 3
	{
		code[0].f = INT;
		code[0].l = 0;
		code[0].a = 1;
		code[1].f = LIT;
		code[1].l = 0;
		code[1].a = 3;
		code[2].f = STO;
		code[2].l = 0;
		code[2].a = 0;
		code[3].f = LIT;
		code[3].l = 0;
		code[3].a = 3;
		code[4].f = OPR;
		code[4].l = 0;
		code[4].a = 2;
		code[5].f = END;
		code[5].l = 0;
		code[5].a = 0;
	}
	break;
	case 1: // Sums all numbers between 1 and 10
	{
		code[0].f = INT;
		code[0].l = 0;
		code[0].a = 3; // Allocates memory
		code[1].f = LIT;
		code[1].l = 0;
		code[1].a = 1; // Starts Variable
		code[2].f = LIT;
		code[2].l = 0;
		code[2].a = 2; // Starts Variable
		code[3].f = LIT;
		code[3].l = 0;
		code[3].a = 3; // Starts Variable
		code[4].f = LIT;
		code[4].l = 0;
		code[4].a = 4; // Starts Variable
		code[5].f = LIT;
		code[5].l = 0;
		code[5].a = 5; // Starts Variable
		code[6].f = LIT;
		code[6].l = 0;
		code[6].a = 6; // Starts Variable
		code[7].f = LIT;
		code[7].l = 0;
		code[7].a = 7; // Starts Variable
		code[8].f = LIT;
		code[8].l = 0;
		code[8].a = 8; // Starts Variable
		code[9].f = LIT;
		code[9].l = 0;
		code[9].a = 9; // Starts Variable
		code[10].f = LIT;
		code[10].l = 0;
		code[10].a = 10; // Starts Variable
		code[11].f = OPR;
		code[11].l = 0;
		code[11].a = 2; // Sums Variable
		code[12].f = OPR;
		code[12].l = 0;
		code[12].a = 2; // Sums Variable
		code[13].f = OPR;
		code[13].l = 0;
		code[13].a = 2; // Sums Variable
		code[14].f = OPR;
		code[14].l = 0;
		code[14].a = 2; // Sums Variable
		code[15].f = OPR;
		code[15].l = 0;
		code[15].a = 2; // Sums Variable
		code[16].f = OPR;
		code[16].l = 0;
		code[16].a = 2; // Sums Variable
		code[17].f = OPR;
		code[17].l = 0;
		code[17].a = 2; // Sums Variable
		code[18].f = OPR;
		code[18].l = 0;
		code[18].a = 2; // Sums Variable
		code[19].f = OPR;
		code[19].l = 0;
		code[19].a = 2; // Sums Variable
		code[20].f = END;
		code[20].l = 0;
		code[20].a = 0; // Ends Program
	}
	break;
	case 2: // Sums all numbers between 1 and 100 (loop)
	{
		code[0].f = INT;
		code[0].l = 0;
		code[0].a = 6; // Allocates Variable
		code[1].f = LIT;
		code[1].l = 0;
		code[1].a = 2; // Starts Counter
		code[2].f = STO;
		code[2].l = 0;
		code[2].a = 3; // Saves Counter
		code[3].f = LIT;
		code[3].l = 0;
		code[3].a = 99; // Sets Max
		code[4].f = STO;
		code[4].l = 0;
		code[4].a = 4; // Saves max
		code[5].f = LIT;
		code[5].l = 0;
		code[5].a = 1; // Starts initial sum = 1
		code[6].f = STO;
		code[6].l = 0;
		code[6].a = 5; // Saves initual sum inicial
		code[7].f = LOD;
		code[7].l = 0;
		code[7].a = 3; // Starts loop (load aux)
		code[8].f = LOD;
		code[8].l = 0;
		code[8].a = 5; // Loads Sum
		code[9].f = OPR;
		code[9].l = 0;
		code[9].a = 2; // Sums aux
		code[10].f = STO;
		code[10].l = 0;
		code[10].a = 5; // Saves sum
		code[11].f = LOD;
		code[11].l = 0;
		code[11].a = 3; // Loads aux
		code[12].f = LOD;
		code[12].l = 0;
		code[12].a = 4; // Loads máx
		code[13].f = OPR;
		code[13].l = 0;
		code[13].a = 10; // Less than
		code[14].f = JPC;
		code[14].l = 0;
		code[14].a = 19; // Exit loop
		code[15].f = LOD;
		code[15].l = 0;
		code[15].a = 3; // Load aux
		code[16].f = OPR;
		code[16].l = 0;
		code[16].a = 2; // Aux + 1
		code[17].f = STO;
		code[17].l = 0;
		code[17].a = 3; // Saves aux
		code[18].f = JMP;
		code[18].l = 0;
		code[18].a = 7; // End of loop
		code[19].f = END;
		code[19].l = 0;
		code[19].a = 0; // End of program
	}
	break;
	case 3:
	{
	}
	break;
	case 4:
	{
	}
	break;
	default:
		break;
	}
}

int main()
{

	// 0: Sums two numbers
	// 1: Sums all numbers between 0 and 10
	// 2: Sums all numbers between 0 and 100 (iterative)
	instructions(0);
	// PCode Machine:
	Instruction i;
	{
		printf("\tstart pl/0 ");
		t = 0;
		b = 1;
		p = 0;
		s[1] = 0;
		s[2] = 0;
		s[3] = 0;
		printf("\n\t| t |  b |  p |\t");
		do
		{
			i = code[p];
			printf("\n\t|%2d | %2d | %2d | ", t, b, p);
			p++;
			switch (i.f)
			{
			case LIT: // Pushes value to stack
				t++;
				s[t] = i.a;
				break;
			case OPR: // Operations switch
				switch (i.a)
				{
				case 0: // Return
					t = b - 1;
					p = s[t + 3];
					b = s[t + 2];
					break;
				case 1: // Inversor
					s[t] = -s[t];
					break;
				case 2: // Sum
					t--;
					s[t] = s[t] + s[t + 1];
					break;
				case 3: // Subtraction
					t--;
					s[t] = s[t] - s[t + 1];
					break;
				case 4: // Multiply
					t--;
					s[t] = s[t] * s[t + 1];
					break;
				case 5: // Divider
					t--;
					s[t] = s[t] / s[t + 1];
					break;
				case 6: // Module
					s[t] = (s[t]) % 2;
					break;
				case 7: // Equals
					t--;
					s[t] = (s[t] == s[t + 1]);
					break;
				case 8: // Difference
					t--;
					s[t] = (s[t] != s[t + 1]);
					break;
				case 9: // Less than
					t--;
					s[t] = (s[t] < s[t + 1]);
					break;
				case 10: // Less than equal
					t--;
					s[t] = (s[t] <= s[t + 1]);
					break;
				case 11: // Greater than
					t--;
					s[t] = (s[t] > s[t + 1]);
					break;
				case 12: // Greater than equal
					t--;
					s[t] = (s[t] >= s[t + 1]);
					break;
				}
				break;
			case LOD: // Loads value to the top of the stack with address i.a
				t++;
				s[t] = s[base(i.l) + i.a];
				break;
			case STO: // Stores to the address i.a
				s[base(i.l) + i.a] = s[t];
				t--;
				break;
			case CAL: // Calls the function with config
			{
				s[t + 1] = base(i.l);
				s[t + 2] = b;
				s[t + 3] = p;
				b = t + 1;
				p = i.a;
			}
			break;
			case INT: // Allocates the momory for a int variable
				t = t + i.a;
				break;
			case JMP: // Jumps to the instruction 'i.a'
				p = i.a;
				break;
			case JPC: // Jumps to 'i.a' case 0
				if (s[t] == 0)
				{
					p = i.a;
					t--;
				}
				break;
			case END:
				p = 0;
				break;
			}
			for (int j = 1; j <= t; j++)
			{
				printf("[%d]", s[j]);
			}
		} while (p != 0);
	}
	return 0;
}
