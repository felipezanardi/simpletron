#include <stdio.h>
#include <stdlib.h> // exit()

// Input/Output Operations
#define READ        10
#define WRITE       11

// Load/Store Operations
#define LOAD        20
#define STORE       21

// Arithmetic Operations
#define ADD         30
#define SUBTRACT    31
#define DIVIDE      32
#define MULTIPLY    33

// Transfer of Control Operations
#define BRANCH      40
#define BRANCHNEG   41
#define BRANCHZERO  42
#define HALT        43

// Others
#define MEM         100     // memory size
#define SENTINEL   -99999

#define MAX         +9999
#define MIN         -9999

typedef struct
{
    int memory[MEM];
    int accumulator;

    int instructionCounter;
    int instructionRegister;

    int operationCode;
    int operand;
} Simpletron;

void welcome();
void menu(Simpletron *s);
void style(char *text);

void load(Simpletron *s);
void loadFile(FILE *program, Simpletron *s);

void execute(Simpletron *s);
void dump(Simpletron *s);

int main()
{
    Simpletron simpletron = {{0}, 0, 0, 0, 0, 0};

    welcome();
    menu(&simpletron);
    execute(&simpletron);

    return 0;
}

void welcome()
{
    style("Welcome to Simpletron!");
    style("Please enter your program one instruction");
    style("(or data word) at a time. I will type the");
    style("location number and a question mark (?). ");
    style("You then type the word for that location.");
    style("Type the sentinel -99999 to stop entering");
    style("your program.");
}

void menu(Simpletron *s)
{
    int option;

    do
    {
        printf("\n");
        style("MENU");
        printf("1 - Enter program manually\n");
        printf("2 - Load program from a .txt file\n");
        printf("3 - Program loading instructions (important)\n");
        printf("0 - Exit\n");
        printf("Select an option: ");
        scanf("%d", &option);

        if (option == 1)
        {
            load(s);
        }
        else if (option == 2)
        {
            FILE* program = fopen("scripts.txt", "r");
            if (program == NULL)
            {
                style("Error opening file!");
                exit(1);
            }
            loadFile(program, s);

            fclose(program);
        }
        else if (option == 3)
        {
            printf("\n");
            style("Go to the folder where the program's   ");
            style("executable is located (simpletron.exe).");
            style("In that folder, look for the file with ");
            style("the name 'scripts.txt' and insert the  ");
            style("code you want there.");
            style("Save the file and run the .exe again.  ");
        }
        else if (option == 0)
        {
            exit(0);
        }
        else
        {
            style("Invalid option! Try again.");
        }
    } while (option < 0 || option > 2);
}

void style(char *text)
{
    printf("*** %s ***\n", text);
}

void load(Simpletron *s)
{
    int input;

    while (s->instructionCounter < MEM)
    {
        printf("%.2d ? ", s->instructionCounter);
        scanf("%d", &input);

        if (input == SENTINEL)
        {
            break;
        }
        else if (input < MIN || input > MAX)
        {
            style("Instruction out of range (-9999, +9999)");
            style("Enter a valid operation");
        }
        else
        {
            s->memory[s->instructionCounter] = input;
            s->instructionCounter++;
        }
    }
    style("Program loading completed");
    printf("\n");
}

void loadFile(FILE *program, Simpletron *s)
{
    while (fscanf(program, "%d", &s->memory[s->instructionCounter]) == 1 && s->instructionCounter < MEM)
    {
        printf("%.2d ? %d\n", s->instructionCounter, s->memory[s->instructionCounter]);
        if (s->memory[s->instructionCounter] == SENTINEL)
        {
            style("Program loading completed");
            printf("\n");
            return;
        }
        s->instructionCounter++;
    }
}

void execute(Simpletron *s)
{
    style("Program execution begins");

    s->instructionCounter = 0;

    while (s->instructionCounter < MEM)
    {
        s->instructionRegister = s->memory[s->instructionCounter];

        s->operationCode = s->instructionRegister / 100;
        s->operand       = s->instructionRegister % 100;

        switch (s->operationCode)
        {
            case READ:      printf("Enter a value:\n");
                            printf("? ");
                            scanf("%d", &s->memory[s->operand]);

                            // Fatal Error
                            if (s->memory[s->operand] < MIN || s->memory[s->operand] > MAX)
                            {
                                style("Number out of range (-9999, +9999)");
                                style("Simpletron execution abnormally terminated");
                                exit(1);
                            }
                            break;

            case WRITE:     printf("Number: %+05d\n", s->memory[s->operand]);
                            break;

            case LOAD:      s->accumulator = s->memory[s->operand];
                            break;

            case STORE:     s->memory[s->operand] = s->accumulator;
                            break;

            case ADD:       s->accumulator += s->memory[s->operand];

                            // Fatal Error
                            if (s->accumulator < MIN || s->accumulator > MAX)
                            {
                                style("Sum - Accumulator overflow/underflow");
                                style("Simpletron execution abnormally terminated");
                                exit(1);
                            }
                            break;

            case SUBTRACT:  s->accumulator -= s->memory[s->operand];

                            // Fatal Error
                            if (s->accumulator < MIN || s->accumulator > MAX)
                            {
                                style("Subtraction - Accumulator overflow/underflow");
                                style("Simpletron execution abnormally terminated");
                                exit(1);
                            }
                            break;

            case DIVIDE:    if (s->memory[s->operand] != 0)
                            {
                                s->accumulator /= s->memory[s->operand];
                            }
                            else // Fatal Error
                            {
                                style("Attempt to divide by zero");
                                style("Simpletron execution abnormally terminated");
                                exit(1);
                            }
                            break;

            case MULTIPLY:  s->accumulator *= s->memory[s->operand];

                            // Fatal Error
                            if (s->accumulator < MIN || s->accumulator > MAX)
                            {
                                style("Multiplication - Accumulator overflow/underflow");
                                style("Simpletron execution abnormally terminated");
                                exit(1);
                            }
                            break;

            case BRANCH:    s->instructionCounter = s->operand;
                            continue;

            case BRANCHNEG: if (s->accumulator < 0)
                            {
                                s->instructionCounter = s->operand;
                                continue;
                            }
                            break;

            case BRANCHZERO:if (s->accumulator == 0)
                            {
                                s->instructionCounter = s->operand;
                                continue;
                            }
                            break;

            case HALT:      style("Simpletron execution terminated");
                            printf("\n");

                            s->instructionCounter++;
                            dump(s);
                            return;
        }
        s->instructionCounter++;
    }
}

void dump(Simpletron *s)
{
    printf("REGISTERS:\n");
    printf("accumulator             %+05d\n", s->accumulator);
    printf("instructionCounter      %.2d\n", s->instructionCounter);
    printf("instructionRegister     %+05d\n", s->instructionRegister);
    printf("operationCode           %.2d\n", s->operationCode);
    printf("operand                 %.2d\n\n", s->operand);

    printf("MEMORY:\n");

    printf("%2c", ' ');
    for (int i=0; i<10; i++)
    {
        printf("%6d", i);
    }
    printf("\n");

    for (int i = 0; i<MEM; i+=10)
    {
        printf("%.2d", i);

        for (int j = 0; j<10; j++)
        {
            printf(" %+05d", s->memory[i+j]);
        }
        printf("\n");
    }
}
