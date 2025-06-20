#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "expressao.h"

// Definição da pilha para operadores (usada na conversão infixa -> pós-fixa)
#define MAX_PILHA 100

typedef struct {
    char itens[MAX_PILHA];
    int topo;
} PilhaChar;

void inicializaPilhaChar(PilhaChar *p) {
    p->topo = -1;
}

int pilhaVazia(PilhaChar *p) {
    return p->topo == -1;
}

void empilhaChar(PilhaChar *p, char c) {
    if (p->topo < MAX_PILHA - 1) {
        p->itens[++p->topo] = c;
    }
}

char desempilhaChar(PilhaChar *p) {
    if (!pilhaVazia(p)) {
        return p->itens[p->topo--];
    }
    return '\0';
}

char topoPilhaChar(PilhaChar *p) {
    if (!pilhaVazia(p)) {
        return p->itens[p->topo];
    }
    return '\0';
}

// Definição da pilha para números (usada na avaliação pós-fixa)
typedef struct {
    float itens[MAX_PILHA];
    int topo;
} PilhaFloat;

void inicializaPilhaFloat(PilhaFloat *p) {
    p->topo = -1;
}

void empilhaFloat(PilhaFloat *p, float num) {
    if (p->topo < MAX_PILHA - 1) {
        p->itens[++p->topo] = num;
    }
}

float desempilhaFloat(PilhaFloat *p) {
    if (p->topo >= 0) {
        return p->itens[p->topo--];
    }
    return 0.0;
}

// Funções auxiliares
int prioridade(char op) {
    switch (op) {
        case '^': return 4;
        case '*': case '/': case '%': return 3;
        case '+': case '-': return 2;
        default: return 0;
    }
}

int ehOperador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

int ehFuncao(const char *str, int i) {
    const char *funcoes[] = {"raiz", "sen", "cos", "tg", "log"};
    for (int j = 0; j < 5; j++) {
        int len = strlen(funcoes[j]);
        if (strncmp(&str[i], funcoes[j], len) == 0) {
            return len;
        }
    }
    return 0;
}

// Converte infixa para pós-fixa (Shunting-yard algorithm)
char *getFormaPosFixa(char *inFixa) {
    static char posFixa[512];
    PilhaChar pilha;
    inicializaPilhaChar(&pilha);
    int j = 0;

    for (int i = 0; inFixa[i] != '\0'; i++) {
        if (isspace(inFixa[i])) continue;

        if (isdigit(inFixa[i]) || inFixa[i] == '.') {
            while (isdigit(inFixa[i]) || inFixa[i] == '.') {
                posFixa[j++] = inFixa[i++];
            }
            posFixa[j++] = ' ';
            i--;
        } else if (inFixa[i] == '(') {
            empilhaChar(&pilha, inFixa[i]);
        } else if (inFixa[i] == ')') {
            while (topoPilhaChar(&pilha) != '(') {
                posFixa[j++] = desempilhaChar(&pilha);
                posFixa[j++] = ' ';
            }
            desempilhaChar(&pilha); // Remove '('
        } else if (ehOperador(inFixa[i])) {
            while (!pilhaVazia(&pilha) && prioridade(topoPilhaChar(&pilha)) >= prioridade(inFixa[i])) {
                posFixa[j++] = desempilhaChar(&pilha);
                posFixa[j++] = ' ';
            }
            empilhaChar(&pilha, inFixa[i]);
        } else {
            int len = ehFuncao(inFixa, i);
            if (len > 0) {
                empilhaChar(&pilha, inFixa[i + len - 1]); // Usa último caractere como identificador
                i += len - 1;
            }
        }
    }

    while (!pilhaVazia(&pilha)) {
        posFixa[j++] = desempilhaChar(&pilha);
        posFixa[j++] = ' ';
    }

    posFixa[j] = '\0';
    return posFixa;
}

// Avalia expressão pós-fixa
float getValorPosFixa(char *posFixa) {
    PilhaFloat pilha;
    inicializaPilhaFloat(&pilha);
    char *token = strtok(posFixa, " ");

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            empilhaFloat(&pilha, atof(token));
        } else if (ehOperador(token[0])) {
            float b = desempilhaFloat(&pilha);
            float a = desempilhaFloat(&pilha);
            switch (token[0]) {
                case '+': empilhaFloat(&pilha, a + b); break;
                case '-': empilhaFloat(&pilha, a - b); break;
                case '*': empilhaFloat(&pilha, a * b); break;
                case '/': empilhaFloat(&pilha, a / b); break;
                case '%': empilhaFloat(&pilha, fmod(a, b)); break;
                case '^': empilhaFloat(&pilha, pow(a, b)); break;
            }
        } else {
            float a = desempilhaFloat(&pilha);
            switch (token[0]) {
                case 'r': empilhaFloat(&pilha, sqrt(a)); break;      // raiz
                case 's': empilhaFloat(&pilha, sin(a * M_PI / 180)); break; // sen (graus para radianos)
                case 'c': empilhaFloat(&pilha, cos(a * M_PI / 180)); break; // cos
                case 't': empilhaFloat(&pilha, tan(a * M_PI / 180)); break; // tg
                case 'l': empilhaFloat(&pilha, log10(a)); break;      // log
            }
        }
        token = strtok(NULL, " ");
    }

    return desempilhaFloat(&pilha);
}

// Funções restantes (simples wrappers)
char *getFormaInFixa(char *posFixa) {
    // Implementação simplificada (não requerida no documento)
    static char inFixa[512];
    strcpy(inFixa, "Conversão não implementada");
    return inFixa;
}

float getValorInFixa(char *inFixa) {
    char *posFixa = getFormaPosFixa(inFixa);
    return getValorPosFixa(posFixa);
}