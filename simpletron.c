#include <stdio.h>
#include <stdlib.h> // exit()
#include <locale.h> // setlocale()

// Operações de Entrada/Saída
#define READ        10
#define WRITE       11

// Operações de Carregamento/Armazenamento
#define LOAD        20
#define STORE       21

// Operações Aritméticas
#define ADD         30
#define SUBTRACT    31
#define DIVIDE      32
#define MULTIPLY    33

// Operações de Transferência de Controle (resultado no acumulador)
#define BRANCH      40
#define BRANCHNEG   41
#define BRANCHZERO  42
#define HALT        43

// Extras
#define MEM         100
#define SENTINELA   -99999

#define MAX         +9999
#define MIN         -9999

// Variáveis globais
int memoria[MEM] = {0};
int acumulador = 0;

int contadorInstrucao = 0;
int registroInstrucao = 0;

int codigoOperacao = 0;
int operando = 0;

void welcome();
void load();
void execute();
void dump();

int main()
{
    setlocale(LC_ALL, "Portuguese");

    welcome();
    load();
    execute();

    return 0;
}

void load()
{
    int input;

    while (contadorInstrucao < MEM)
    {
        printf("%.2d ? ", contadorInstrucao);
        scanf("%d", &input);

        if (input == SENTINELA)
        {
            contadorInstrucao = 0;
            break;
        }
        else if (input < MIN || input > MAX)
        {
            printf("Número fora do intervalo (%+d, %+d)!\n", MIN, MAX);
            printf("Insira uma operação válida.\n");
        }
        else
        {
            memoria[contadorInstrucao] = input;
            contadorInstrucao++;
        }
    }
    printf("*** Carregamento do programa concluido ***\n\n");
}

void execute()
{
    printf("*** Início da execução do programa ***\n");

    while (contadorInstrucao < MEM)
    {
        registroInstrucao = memoria[contadorInstrucao];

        codigoOperacao = registroInstrucao / 100;
        operando       = registroInstrucao % 100;

        switch (codigoOperacao)
        {
            case READ:      printf("Digite um valor:\n");
                            printf("? ");
                            scanf("%d", &memoria[operando]);

                            // Erro Fatal
                            if (memoria[operando] < MIN || memoria[operando] > MAX)
                            {
                                printf("*** Número fora do intervalo (%+d, %+d)! ***\n", MIN, MAX);
                                printf("*** Interrupção anormal da execução do Simpletron ***\n");
                                exit(1);
                            }
                            break;

            case WRITE:     printf("Número: %+05d\n", memoria[operando]);
                            break;

            case LOAD:      acumulador = memoria[operando];
                            break;

            case STORE:     memoria[operando] = acumulador;
                            break;

            case ADD:       acumulador += memoria[operando];

                            // Erro Fatal
                            if (acumulador < MIN || acumulador > MAX)
                            {
                                printf("*** Soma - Overflow no acumulador ***\n", MIN, MAX);
                                printf("*** Interrupção anormal da execução do Simpletron ***\n");
                                exit(1);
                            }
                            break;

            case SUBTRACT:  acumulador -= memoria[operando];

                            // Erro Fatal
                            if (acumulador < MIN || acumulador > MAX)
                            {
                                printf("*** Subtração - Overflow no acumulador ***\n", MIN, MAX);
                                printf("*** Interrupção anormal da execução do Simpletron ***\n");
                                exit(1);
                            }
                            break;

            case DIVIDE:    if (memoria[operando] != 0)
                            {
                                acumulador /= memoria[operando];
                            }
                            else // Erro Fatal
                            {
                                printf("*** Tentativa de dividir por zero ***\n");
                                printf("*** Interrupção anormal da execução do Simpletron ***\n");
                                exit(1);
                            }
                            break;

            case MULTIPLY:  acumulador *= memoria[operando];

                            // Erro Fatal
                            if (acumulador < MIN || acumulador > MAX)
                            {
                                printf("*** Multiplicação - Overflow no acumulador ***\n", MIN, MAX);
                                printf("*** Interrupção anormal da execução do Simpletron ***\n");
                                exit(1);
                            }
                            break;

            case BRANCH:    contadorInstrucao = operando;
                            continue;

            case BRANCHNEG: if (acumulador < 0)
                            {
                                contadorInstrucao = operando;
                                continue;
                            }
                            break;

            case BRANCHZERO:if (acumulador == 0)
                            {
                                contadorInstrucao = operando;
                                continue;
                            }
                            break;

            case HALT:      printf("*** Execução do Simpletron concluída ***\n\n");
                            dump();
                            return;
        }
        contadorInstrucao++;
    }
}

void dump()
{
    printf("REGISTROS:\n");
    printf("acumulador          %+05d\n", acumulador);
    printf("contadorInstrucao   %.2d\n", contadorInstrucao);
    printf("registroInstrucao   %+05d\n", registroInstrucao);
    printf("codigoOperacao      %.2d\n", codigoOperacao);
    printf("operando            %.2d\n\n", operando);

    printf("MEMÓRIA:\n");

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
            printf(" %+05d", memoria[i+j]);
        }
        printf("\n");
    }
}

void welcome()
{
    printf("*** Bem vindo ao Simpletron! ***\n");
    printf("*** Por favor digite uma instrução (ou palavra de dados) de seu programa por vez. ***\n");
    printf("*** Digitarei o numero da posição e um ponto de interrogação (?). ***\n");
    printf("*** Digite então a palavra para aquela posição. ***\n");
    printf("*** Digite o valor sentinela -99999 para encerrar a digitação de seu programa. ***\n");
}
