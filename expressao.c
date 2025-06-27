#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "expressao.h"

typedef struct {
    float itens[MAX_EXPRESSAO];
    int topo;
} PilhaNumeros;

typedef struct {
    char itens[MAX_EXPRESSAO][20];
    int topo;
} PilhaOperadores;

static void empilharNumero(PilhaNumeros* p, float valor) {
    p->itens[++p->topo] = valor;
}

static float desempilharNumero(PilhaNumeros* p) {
    if (p->topo < 0) {
        fprintf(stderr, "Erro: pilha de numeros vazia\n");
        exit(EXIT_FAILURE);
    }
    return p->itens[p->topo--];
}

static void empilharOperador(PilhaOperadores* p, const char* operador) {
    strcpy(p->itens[++p->topo], operador);
}

static char* desempilharOperador(PilhaOperadores* p) {
    if (p->topo < 0) {
        fprintf(stderr, "Erro: pilha de operadores vazia\n");
        exit(EXIT_FAILURE);
    }
    return p->itens[p->topo--];
}

static int ehOperador(const char* token) {
    return strcmp(token, "+") == 0 || strcmp(token, "-") == 0 ||
           strcmp(token, "*") == 0 || strcmp(token, "/") == 0 ||
           strcmp(token, "^") == 0 || strcmp(token, "%") == 0;
}

static int ehFuncao(const char* token) {
    return strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 ||
           strcmp(token, "tan") == 0 || strcmp(token, "log") == 0 ||
           strcmp(token, "raiz") == 0;
}

static int precedencia(const char* op) {
    if (ehFuncao(op)) return 5;
    if (strcmp(op, "^") == 0) return 4;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0) return 3;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 2;
    return 0;
}

static int associativoDireita(const char* op) {
    return strcmp(op, "^") == 0 || ehFuncao(op);
}

static void processarOperador(PilhaOperadores* pilhaOp, const char* op, char* saida) {
    while (pilhaOp->topo >= 0 && 
           (precedencia(pilhaOp->itens[pilhaOp->topo]) > precedencia(op) ||
           (precedencia(pilhaOp->itens[pilhaOp->topo]) == precedencia(op) && 
            !associativoDireita(op))) {
        if (strcmp(pilhaOp->itens[pilhaOp->topo], "(") == 0) break;
        strcat(saida, desempilharOperador(pilhaOp));
        strcat(saida, " ");
    }
    empilharOperador(pilhaOp, op);
}

void converterParaPosfixa(const char* infixa, char* posfixa) {
    PilhaOperadores pilhaOp = { .topo = -1 };
    char token[20];
    int pos = 0;
    posfixa[0] = '\0';

    while (infixa[pos] != '\0') {
        while (infixa[pos] == ' ') pos++;

        if (infixa[pos] == '\0') break;

        int i = 0;
        if (isalpha(infixa[pos])) {
            while (isalpha(infixa[pos]) && i < 19) {
                token[i++] = infixa[pos++];
            }
            token[i] = '\0';
            empilharOperador(&pilhaOp, token);
        } else if (isdigit(infixa[pos]) || infixa[pos] == '.' || 
                  (infixa[pos] == '-' && (pos == 0 || infixa[pos-1] == '('))) {
            if (infixa[pos] == '-') token[i++] = infixa[pos++];
            while ((isdigit(infixa[pos]) || infixa[pos] == '.') && i < 19) {
                token[i++] = infixa[pos++];
            }
            token[i] = '\0';
            strcat(posfixa, token);
            strcat(posfixa, " ");
        } else if (infixa[pos] == '(') {
            empilharOperador(&pilhaOp, "(");
            pos++;
        } else if (infixa[pos] == ')') {
            while (pilhaOp.topo >= 0 && strcmp(pilhaOp.itens[pilhaOp.topo], "(") != 0) {
                strcat(posfixa, desempilharOperador(&pilhaOp));
                strcat(posfixa, " ");
            }
            if (pilhaOp.topo < 0) {
                fprintf(stderr, "Erro: parenteses desbalanceados\n");
                exit(EXIT_FAILURE);
            }
            desempilharOperador(&pilhaOp);
            
            if (pilhaOp.topo >= 0 && ehFuncao(pilhaOp.itens[pilhaOp.topo])) {
                strcat(posfixa, desempilharOperador(&pilhaOp));
                strcat(posfixa, " ");
            }
            pos++;
        } else {
            token[0] = infixa[pos++];
            token[1] = '\0';
            processarOperador(&pilhaOp, token, posfixa);
        }
    }

    while (pilhaOp.topo >= 0) {
        if (strcmp(pilhaOp.itens[pilhaOp.topo], "(") == 0) {
            fprintf(stderr, "Erro: parenteses desbalanceados\n");
            exit(EXIT_FAILURE);
        }
        strcat(posfixa, desempilharOperador(&pilhaOp));
        strcat(posfixa, " ");
    }
}

float avaliarPosfixa(const char* posfixa) {
    PilhaNumeros pilha = { .topo = -1 };
    char copia[MAX_EXPRESSAO];
    strcpy(copia, posfixa);
    char* token = strtok(copia, " ");

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            empilharNumero(&pilha, atof(token));
        } else if (ehOperador(token)) {
            if (pilha.topo < 1) {
                fprintf(stderr, "Erro: operandos insuficientes para %s\n", token);
                exit(EXIT_FAILURE);
            }
            float b = desempilharNumero(&pilha);
            float a = desempilharNumero(&pilha);
            
            switch (token[0]) {
                case '+': empilharNumero(&pilha, a + b); break;
                case '-': empilharNumero(&pilha, a - b); break;
                case '*': empilharNumero(&pilha, a * b); break;
                case '/': 
                    if (b == 0) {
                        fprintf(stderr, "Erro: divisao por zero\n");
                        exit(EXIT_FAILURE);
                    }
                    empilharNumero(&pilha, a / b); 
                    break;
                case '^': empilharNumero(&pilha, powf(a, b)); break;
                case '%':
                    if ((int)a != a || (int)b != b) {
                        fprintf(stderr, "Erro: operador %% requer inteiros\n");
                        exit(EXIT_FAILURE);
                    }
                    if (b == 0) {
                        fprintf(stderr, "Erro: modulo por zero\n");
                        exit(EXIT_FAILURE);
                    }
                    empilharNumero(&pilha, (float)((int)a % (int)b));
                    break;
            }
        } else if (ehFuncao(token)) {
            if (pilha.topo < 0) {
                fprintf(stderr, "Erro: operando insuficiente para %s\n", token);
                exit(EXIT_FAILURE);
            }
            float x = desempilharNumero(&pilha);
            
            if (strcmp(token, "sen") == 0) {
                empilharNumero(&pilha, sinf(x * M_PI / 180.0f));
            } else if (strcmp(token, "cos") == 0) {
                empilharNumero(&pilha, cosf(x * M_PI / 180.0f));
            } else if (strcmp(token, "tan") == 0) {
                empilharNumero(&pilha, tanf(x * M_PI / 180.0f));
            } else if (strcmp(token, "log") == 0) {
                if (x <= 0) {
                    fprintf(stderr, "Erro: log de valor nao positivo\n");
                    exit(EXIT_FAILURE);
                }
                empilharNumero(&pilha, log10f(x));
            } else if (strcmp(token, "raiz") == 0) {
                if (x < 0) {
                    fprintf(stderr, "Erro: raiz de valor negativo\n");
                    exit(EXIT_FAILURE);
                }
                empilharNumero(&pilha, sqrtf(x));
            }
        } else {
            fprintf(stderr, "Erro: token invalido '%s'\n", token);
            exit(EXIT_FAILURE);
        }
        token = strtok(NULL, " ");
    }

    if (pilha.topo != 0) {
        fprintf(stderr, "Erro: expressao mal formada\n");
        exit(EXIT_FAILURE);
    }

    return desempilharNumero(&pilha);
}

float avaliarInfixa(const char* infixa) {
    char posfixa[MAX_EXPRESSAO];
    converterParaPosfixa(infixa, posfixa);
    return avaliarPosfixa(posfixa);
}

void converterParaInfixa(const char* posfixa, char* infixa) {
    char pilha[MAX_EXPRESSAO][MAX_EXPRESSAO];
    int topo = -1;
    char copia[MAX_EXPRESSAO];
    strcpy(copia, posfixa);
    char* token = strtok(copia, " ");

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            strcpy(pilha[++topo], token);
        } else if (ehOperador(token)) {
            if (topo < 1) {
                fprintf(stderr, "Erro: operandos insuficientes para %s\n", token);
                exit(EXIT_FAILURE);
            }
            char b[MAX_EXPRESSAO];
            char a[MAX_EXPRESSAO];
            strcpy(b, pilha[topo--]);
            strcpy(a, pilha[topo--]);
            
            sprintf(pilha[++topo], "(%s %s %s)", a, token, b);
        } else if (ehFuncao(token)) {
            if (topo < 0) {
                fprintf(stderr, "Erro: operando insuficiente para %s\n", token);
                exit(EXIT_FAILURE);
            }
            char a[MAX_EXPRESSAO];
            strcpy(a, pilha[topo--]);
            
            sprintf(pilha[++topo], "%s(%s)", token, a);
        } else {
            fprintf(stderr, "Erro: token invalido '%s'\n", token);
            exit(EXIT_FAILURE);
        }
        token = strtok(NULL, " ");
    }

    if (topo != 0) {
        fprintf(stderr, "Erro: expressao mal formada\n");
        exit(EXIT_FAILURE);
    }

    strcpy(infixa, pilha[topo]);
}