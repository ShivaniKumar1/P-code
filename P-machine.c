// Assignment: Homework #1 (P-Machine)
// Students: Nicole Freites & Shivani Kumar

#include <stdio.h>
#include <stdlib.h>

#define MAX_DATA_STACK_HEIGHT 1000
#define MAX_CODE_LENGTH 500

typedef struct instruction{
	int op; // opcode
	int l;  // L
	int m;  // M
}instruction;

int stack[MAX_DATA_STACK_HEIGHT]; // globally declaring stack

// This function finds a variable in a different activation record some L levels down
int base(int l, int base) // l stands for L in the instruction format
{
  int b1; // Find base L levels down
  b1 = base;
  while(l > 0)
  {
    b1 = stack[b1 - 1];
    l--;
  }
  return b1;
}

// Function to print the stack.
void printStack(int sp, int ar[])
{
  int activationRecordIndex = 0;
  for(int j = MAX_DATA_STACK_HEIGHT - 1; j >= sp; j--)
  {
    printf("%d ", stack[j]);

    // Prints the line to separate the activation records if the sp stored 
    // in the activation array is the same as the current index.
    if (ar[activationRecordIndex] == j)
    {
      printf("| ");
      activationRecordIndex++;
    }
  }
  printf("\n");
}

int main()
{
  // Initialization of the pointer that will store the instructions from the input file.
  instruction *code = calloc(MAX_CODE_LENGTH, sizeof(instruction));

  // Initial values for PM/0 CPU registers
  int sp = MAX_DATA_STACK_HEIGHT;
  int bp = sp - 1;
  int pc = 0;
  instruction ir;

  // Initialization of array that stores sp at a given operation in order to 
  // separate the activation records during the print function.
  int activationRecord[MAX_CODE_LENGTH];
  int arIndex = 0;

  // Used to keep track of the index of the array of instructions.
  int index = 0;

  printf("Line\tOP\tL\tM\n");

  // Load data while the end of the file is not reached.
  while(!feof(stdin))
  {
    // Reads in the OP, L, and M components and stores it in a pointer.
    scanf("%d %d %d", &code[index].op, &code[index].l, &code[index].m);

    // Based on the OP component, the following will print the operations in order given by the user.
    switch(code[index].op)
    {
      case 1:
        printf(" %d\tlit\t0\t%d\n", index, code[index].m);
        break;

      case 2:
        printf(" %d\topr\t0\t%d\n", index, code[index].m);
        break;

      case 3:
        printf(" %d\tlod\t%d\t%d\n", index, code[index].l, code[index].m);
        break;

      case 4:
        printf(" %d\tsto\t%d\t%d\n", index, code[index].l, code[index].m);
        break;

      case 5:
        printf(" %d\tcal\t%d\t%d\n", index, code[index].l, code[index].m);
        break;

      case 6:
        printf(" %d\tinc\t0\t%d\n", index, code[index].m);
        break;

      case 7:
        printf(" %d\tjmp\t0\t%d\n", index, code[index].m);
        break;

      case 8:
        printf(" %d\tjpc\t0\t%d\n", index, code[index].m);
        break;

      case 9:
        printf(" %d\tsio\t0\t1\n", index);
        break;

      case 10:
        printf(" %d\tsio\t0\t2\n", index);
        break;

      case 11:
        printf(" %d\tsio\t0\t3\n", index);
        break;
    }

    index++;
  }

  // Flag to signal that the program is finished. If set to zero, the program has ended.
  int haltFlag = 1;
  index = 0;

  printf("\n\n\t\tpc\tbp\tsp\tstack\n");
  printf("Initial values\t%d\t%d\t%d\n", pc, bp, sp);

  while(haltFlag == 1)
  {
    // Fetch cycle.
    ir = code[pc];
    pc = pc + 1;
    
    // Execute cycle.
    switch(ir.op)
    {
      // LIT: pushes constant value (literal) M onto the stack
      case 1:
        sp = sp - 1;
        stack[sp] = ir.m;

        printf("%d lit %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
        printStack(sp, activationRecord);
        
        index = pc;
        break;

      // OPR: operation to be performed on the data at the top of stack
      case 2:     
        switch(ir.m)
        {
          // RET: continues execution at previous activation record.
          case 0:
            sp = bp + 1;
            pc = stack[sp - 4];
            bp = stack[sp - 3];

            printf("%d opr %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);

            index = pc;
            break;

          // NEG: the value in current position of stack will turn negative.
          case 1:
            stack[sp] = stack[sp] * -1;

            printf("%d opr %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);

            index = pc;
            break;

          // ADD: sets position on stack to the previous value and next value added together.
          case 2:
            sp = sp + 1;
            stack[sp] = stack[sp] + stack[sp - 1];

            printf("%d opr %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);

            index = pc;
            break;

          // SUB: sets position on stack to the previous value and next value subtracted together.
          case 3:
            sp = sp + 1;
            stack[sp] = stack[sp] - stack[sp - 1];

            printf("%d opr %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);

            index = pc;
            break;

          // MUL: sets position on stack to the previous value and next value multiplied together.
          case 4:
            sp = sp + 1;
            stack[sp] = stack[sp] * stack[sp - 1];

            printf("%d opr %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);

            index = pc;
            break;

          // DIV: sets position on stack to the previous value and next value divided.
          case 5:
            sp = sp + 1;
            stack[sp] = (stack[sp] / stack[sp - 1]);

            printf("%d opr %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);

            index = pc;
            break;

          // ODD: determines if the current value on the stack is even or odd.
          // Stores 1 if true, 0 if false.
          case 6:
            stack[sp] = stack[sp] % 2;

            printf("%d opr %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);

            index = pc;
            break;

          // MOD: stores the remainder of the previous value and next value divided.
          case 7:
            sp = sp + 1;
            stack[sp] = stack[sp] % stack[sp - 1];

            printf("%d opr %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);

            index = pc;
            break;

          // EQL: determines if the previous value and current value on the stack are equal.
          // Stores 1 if true, 0 if false.
          case 8:
            sp = sp + 1;
            stack[sp] = (stack[sp] == stack[sp - 1]);

            printf("%d opr %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);

            index = pc;
            break;

          // NEQ: determines if the previous value and current value on the stack are not equal.
          // Stores 1 if true, 0 if false.
          case 9:
            sp = sp + 1;
            stack[sp] = (stack[sp] != stack[sp - 1]);

            printf("%d opr %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);

            index = pc;
            break;

          // LSS: determines if the current value on the stack is less than the previous value.
          // Stores 1 if true, 0 if false.
          case 10:
            sp = sp + 1;
            stack[sp] = (stack[sp] < stack[sp - 1]);

            printf("%d opr %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);

            index = pc;
            break;

          // LEQ: determines if the current value on the stack is less than or equal to the previous value.
          // Stores 1 if true, 0 if false.
          case 11:
            sp = sp + 1;
            stack[sp] = (stack[sp] <= stack[sp - 1]);

            printf("%d opr %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);

            index = pc;
            break;

          // GTR: determines if the current value on the stack is greater than the previous value.
          // Stores 1 if true, 0 if false.
          case 12:
            sp = sp + 1;
            stack[sp] = (stack[sp] > stack[sp - 1]);

            printf("%d opr %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);

            index = pc;
            break;

          // GEQ: determines if the current value on the stack is greater than or equal to the previous value.
          // Stores 1 if true, 0 if false.
          case 13:
            sp = sp + 1;
            stack[sp] = (stack[sp] >= stack[sp - 1]);

            printf("%d opr %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);

            index = pc;
            break;
        }
        break;

      // LOD: loads value to top of stack from stack location at offset M
      case 3:
        sp = sp - 1;
        stack[sp] = stack[base(code[sp].l, bp) - code[sp].m];

        printf("%d lod %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
        printStack(sp, activationRecord);

        index = pc;
        break;

      // STO: stores value to top of stack from stack location at offset M
      case 4:
        stack[base(ir.l, bp) - ir.m] = stack[sp];
        sp = sp + 1;

        printf("%d sto %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);

        printStack(sp, activationRecord);
        index = pc;
        break;

      // CAL: calls procedure at code index M
      case 5:
        stack[sp - 1] = 0;             // space to return value
        stack[sp - 2] = base(ir.l,bp); // static link
        stack[sp - 3] = bp;            // dynamic link
        stack[sp - 4] = pc;            // return address
        bp = sp - 1;
        pc = ir.m;

        printf("%d cal %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);

        // Stores current sp value onto activation record array.
        activationRecord[arIndex] = sp;
        arIndex = arIndex + 1;
        printStack(sp, activationRecord);

        index = pc;
        break;

      // INC: allocates M locals
      case 6:
        sp = sp - ir.m;

        printf("%d inc %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
        printStack(sp, activationRecord);

        index = pc;
        break;

      // JMP: jumps to instruction M
      case 7:
        pc = ir.m;

        printf("%d jmp %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
        printStack(sp, activationRecord);

        index = pc;
        break;

      // JPC: jump to instruction M if top stack element is 0
      case 8:
        if((stack[sp]) == 0)
        {
          pc = ir.m;
        }

        sp = sp + 1;

        printf("%d jpc %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
        printStack(sp, activationRecord);

        index = pc;
        break;

      // SIO: writes the top stack element to the screen
      case 9:
        printf("%d", stack[sp]);
        sp = sp + 1;

        printf("%d sio %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
        printStack(sp, activationRecord);

        index = pc;
        break;

      // SIO: read in input from the user and stores input on top of the stack
      case 10:
        sp = sp - 1;
        scanf("%d", &stack[sp]);

        printf("%d sio %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
        printStack(sp, activationRecord);

        index = pc;
        break;

      // SIO: end of program
      case 11:
        haltFlag = 0; // set Halt Flag to zero

        printf("%d sio %d %d\t%d\t%d\t%d\t", index, ir.l, ir.m, pc, bp, sp);
        printStack(sp, activationRecord);

        break;
    }
  }
  
  // Frees the code array.
  free(code);
  code = NULL;

  return 0;  
}