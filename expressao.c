#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "expressao.h"

#define MAX_TAM 512

// ----------------------------
// Implementacao da Pilha (TAD)
// ----------------------------

// Estrutura da pilha para numeros
typedef struct {
    float itens[MAX_TAM];
    int topo;
} PilhaNumeros;

// Estrutura da pilha para operadores
typedef struct {
    char itens[MAX_TAM];
    int topo;
} PilhaOperadores;

// Inicializa pilha de numeros
void inicializarPilhaNum(PilhaNumeros *p) {
    p->topo = -1;
}

// Verifica se pilha de numeros esta vazia
int pilhaNumVazia(PilhaNumeros *p) {
    return p->topo == -1;
}

// Empilha numero
void empilharNum(PilhaNumeros *p, float valor) {
    if (p->topo < MAX_TAM-1) {
        p->itens[++p->topo] = valor;
    }
}

// Desempilha numero
float desempilharNum(PilhaNumeros *p) {
    if (!pilhaNumVazia(p)) {
        return p->itens[p->topo--];
    }
    return 0.0f;
}

// Inicializa pilha de operadores
void inicializarPilhaOp(PilhaOperadores *p) {
    p->topo = -1;
}

// Verifica se pilha de operadores esta vazia
int pilhaOpVazia(PilhaOperadores *p) {
    return p->topo == -1;
}

// Empilha operador
void empilharOp(PilhaOperadores *p, char op) {
    if (p->topo < MAX_TAM-1) {
        p->itens[++p->topo] = op;
    }
}

// Desempilha operador
char desempilharOp(PilhaOperadores *p) {
    if (!pilhaOpVazia(p)) {
        return p->itens[p->topo--];
    }
    return '\0';
}

// Retorna o operador no topo
char topoPilhaOp(PilhaOperadores *p) {
    if (!pilhaOpVazia(p)) {
        return p->itens[p->topo];
    }
    return '\0';
}

// ----------------------------
// Funcoes Auxiliares
// ----------------------------

// Retorna a prioridade do operador
int prioridadeOperador(char op) {
    switch(op) {
        case '^': return 4;
        case '*':
        case '/':
        case '%': return 3;
        case '+':
        case '-': return 2;
        default: return 0;
    }
}

// Aplica operacao entre dois numeros
float aplicarOperacao(float a, float b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return powf(a, b);
        case '%': return fmodf(a, b);
        default: return 0.0f;
    }
}

// Aplica funcao matematica
float aplicarFuncaoMat(char *funcao, float valor) {
    if (strcmp(funcao, "log") == 0) return log10f(valor);
    if (strcmp(funcao, "raiz") == 0) return sqrtf(valor);
    if (strcmp(funcao, "sen") == 0) return sinf(valor * M_PI / 180);
    if (strcmp(funcao, "cos") == 0) return cosf(valor * M_PI / 180);
    if (strcmp(funcao, "tg") == 0) return tanf(valor * M_PI / 180);
    return 0.0f;
}

// ----------------------------
// Funcoes Principais
// ----------------------------

// Converte expressao infixa para pos-fixa
char *converterParaPosFixa(char *exprInFixa) {
    static char saida[MAX_TAM];
    PilhaOperadores pilhaOp;
    inicializarPilhaOp(&pilhaOp);
    int i = 0, j = 0;
    char token[20];
    int posToken = 0;

    saida[0] = '\0';

    while (exprInFixa[i] != '\0') {
        // Ignora espacos em branco
        if (isspace(exprInFixa[i])) {
            i++;
            continue;
        }

        // Trata numeros (inteiros ou decimais)
        if (isdigit(exprInFixa[i]) || exprInFixa[i] == '.') {
            while (isdigit(exprInFixa[i]) || exprInFixa[i] == '.') {
                saida[j++] = exprInFixa[i++];
            }
            saida[j++] = ' ';
        }
        // Trata funcoes matematicas (log, sen, cos, etc.)
        else if (isalpha(exprInFixa[i])) {
            posToken = 0;
            while (isalpha(exprInFixa[i])) {
                token[posToken++] = exprInFixa[i++];
            }
            token[posToken] = '\0';
            strcat(saida, token);
            strcat(saida, " ");
        }
        // Trata abertura de parenteses
        else if (exprInFixa[i] == '(') {
            empilharOp(&pilhaOp, exprInFixa[i]);
            i++;
        }
        // Trata fechamento de parenteses
        else if (exprInFixa[i] == ')') {
            while (!pilhaOpVazia(&pilhaOp) && topoPilhaOp(&pilhaOp) != '(') {
                saida[j++] = desempilharOp(&pilhaOp);
                saida[j++] = ' ';
            }
            desempilharOp(&pilhaOp); // Remove o '('
            i++;
        }
        // Trata operadores
        else {
            while (!pilhaOpVazia(&pilhaOp) && 
                   prioridadeOperador(topoPilhaOp(&pilhaOp)) >= prioridadeOperador(exprInFixa[i])) {
                saida[j++] = desempilharOp(&pilhaOp);
                saida[j++] = ' ';
            }
            empilharOp(&pilhaOp, exprInFixa[i]);
            i++;
        }
    }

    // Desempilha operadores restantes
    while (!pilhaOpVazia(&pilhaOp)) {
        saida[j++] = desempilharOp(&pilhaOp);
        saida[j++] = ' ';
    }

    saida[j] = '\0';
    return saida;
}

// Converte expressao pos-fixa para infixa
char *converterParaInFixa(char *exprPosFixa) {
    static char saida[MAX_TAM];
    PilhaOperadores pilha;
    inicializarPilhaOp(&pilha);
    char *token = strtok(exprPosFixa, " ");
    char temp[MAX_TAM];
    char operando1[MAX_TAM], operando2[MAX_TAM];

    saida[0] = '\0';

    while (token != NULL) {
        // Se for numero
        if (isdigit(token[0])) {
            strcpy(temp, token);
            strcat(temp, " ");
            empilharOp(&pilha, temp);
        }
        // Se for operador binario
        else if (strchr("+-*/%^", token[0]) && strlen(token) == 1) {
            strcpy(operando2, desempilharOp(&pilha));
            strcpy(operando1, desempilharOp(&pilha));
            
            sprintf(temp, "(%s %c %s)", operando1, token[0], operando2);
            empilharOp(&pilha, temp);
        }
        // Se for funcao matematica
        else {
            strcpy(operando1, desempilharOp(&pilha));
            sprintf(temp, "%s(%s)", token, operando1);
            empilharOp(&pilha, temp);
        }
        token = strtok(NULL, " ");
    }

    strcpy(saida, desempilharOp(&pilha));
    return saida;
}

// Calcula valor da expressao pos-fixa
float calcularExpressaoPosFixa(char *exprPosFixa) {
    PilhaNumeros pilha;
    inicializarPilhaNum(&pilha);
    char *token = strtok(exprPosFixa, " ");

    while (token != NULL) {
        // Se for numero
        if (isdigit(token[0])) {
            empilharNum(&pilha, atof(token));
        }
        // Se for operador binario
        else if (strchr("+-*/%^", token[0]) && strlen(token) == 1) {
            float num2 = desempilharNum(&pilha);
            float num1 = desempilharNum(&pilha);
            empilharNum(&pilha, aplicarOperacao(num1, num2, token[0]));
        }
        // Se for funcao matematica
        else {
            float num = desempilharNum(&pilha);
            empilharNum(&pilha, aplicarFuncaoMat(token, num));
        }
        token = strtok(NULL, " ");
    }

    return desempilharNum(&pilha);
}

// Calcula valor da expressao infixa
float calcularExpressaoInFixa(char *exprInFixa) {
    char *posFixa = converterParaPosFixa(exprInFixa);
    return calcularExpressaoPosFixa(posFixa);
}