#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "expressao.h"

// TAD Pilha (para conversão e cálculo)
typedef struct No {
    char dado;
    struct No* prox;
} No;

typedef struct {
    No* topo;
} Pilha;

Pilha* criarPilha() {
    Pilha* p = (Pilha*)malloc(sizeof(Pilha));
    p->topo = NULL;
    return p;
}

void empilhar(Pilha* p, char c) {
    No* novo = (No*)malloc(sizeof(No));
    novo->dado = c;
    novo->prox = p->topo;
    p->topo = novo;
}

char desempilhar(Pilha* p) {
    if (p->topo == NULL) return '\0';
    No* temp = p->topo;
    char c = temp->dado;
    p->topo = temp->prox;
    free(temp);
    return c;
}

// Funções Auxiliares
int precedencia(char op) {
    if (op == '^') return 4;
    if (op == '*' || op == '/' || op == '%') return 3;
    if (op == '+' || op == '-') return 2;
    return 0;
}

int ehOperador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

// Conversão Infixa → Pós-fixa (Algoritmo Shunting-yard)
char* getFormaPosFixa(char* infixa) {
    Pilha* p = criarPilha();
    char posFixa[512] = "";
    int j = 0;

    for (int i = 0; infixa[i] != '\0'; i++) {
        if (isspace(infixa[i])) continue;

        if (isdigit(infixa[i]) || infixa[i] == '.') {
            while (isdigit(infixa[i]) || infixa[i] == '.') {
                posFixa[j++] = infixa[i++];
            }
            posFixa[j++] = ' ';
            i--;
        } else if (infixa[i] == '(') {
            empilhar(p, infixa[i]);
        } else if (infixa[i] == ')') {
            while (p->topo != NULL && p->topo->dado != '(') {
                posFixa[j++] = desempilhar(p);
                posFixa[j++] = ' ';
            }
            desempilhar(p); // Remove '('
        } else if (ehOperador(infixa[i])) {
            while (p->topo != NULL && precedencia(p->topo->dado) >= precedencia(infixa[i])) {
                posFixa[j++] = desempilhar(p);
                posFixa[j++] = ' ';
            }
            empilhar(p, infixa[i]);
        }
    }

    while (p->topo != NULL) {
        posFixa[j++] = desempilhar(p);
        posFixa[j++] = ' ';
    }

    posFixa[j] = '\0';
    return strdup(posFixa);
}

// Cálculo de Expressão Pós-fixa (com funções especiais)
float getValorPosFixa(char* posFixa) {
    Pilha* p = criarPilha();
    char* token = strtok(posFixa, " ");

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            empilhar(p, atof(token));
        } else if (strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 || strcmp(token, "tg") == 0 || strcmp(token, "log") == 0 || strcmp(token, "raiz") == 0) {
            float a = desempilhar(p);
            if (strcmp(token, "sen") == 0) empilhar(p, sin(a * M_PI / 180.0));
            else if (strcmp(token, "cos") == 0) empilhar(p, cos(a * M_PI / 180.0));
            else if (strcmp(token, "tg") == 0) empilhar(p, tan(a * M_PI / 180.0));
            else if (strcmp(token, "log") == 0) empilhar(p, log10(a));
            else if (strcmp(token, "raiz") == 0) empilhar(p, sqrt(a));
        } else if (ehOperador(token[0])) {
            float b = desempilhar(p);
            float a = desempilhar(p);
            switch (token[0]) {
                case '+': empilhar(p, a + b); break;
                case '-': empilhar(p, a - b); break;
                case '*': empilhar(p, a * b); break;
                case '/': empilhar(p, a / b); break;
                case '%': empilhar(p, (int)a % (int)b); break;
                case '^': empilhar(p, pow(a, b)); break;
            }
        }
        token = strtok(NULL, " ");
    }

    return desempilhar(p);
}

// Outras funções (simplificadas)
char* getFormaInFixa(char* posFixa) {
    // Implementação similar à conversão reversa (usando pilha)
    return strdup("Implementação similar à pós-fixa");
}

float getValorInFixa(char* infixa) {
    char* posFixa = getFormaPosFixa(infixa);
    return getValorPosFixa(posFixa);
}