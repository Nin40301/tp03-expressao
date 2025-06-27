#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "expressao.h"

typedef struct NoChar {
    char dado;
    struct NoChar* prox;
} NoChar;

typedef struct {
    NoChar* topo;
} PilhaChar;

PilhaChar* criarPilhaChar() {
    PilhaChar* p = (PilhaChar*)malloc(sizeof(PilhaChar));
    if (!p) {
        perror("Erro ao alocar PilhaChar");
        exit(EXIT_FAILURE);
    }
    p->topo = NULL;
    return p;
}

void empilharChar(PilhaChar* p, char c) {
    NoChar* novo = (NoChar*)malloc(sizeof(NoChar));
    if (!novo) {
        perror("Erro ao alocar NoChar");
        exit(EXIT_FAILURE);
    }
    novo->dado = c;
    novo->prox = p->topo;
    p->topo = novo;
}

char desempilharChar(PilhaChar* p) {
    if (p->topo == NULL) {
        return '\0';
    }
    NoChar* temp = p->topo;
    char c = temp->dado;
    p->topo = temp->prox;
    free(temp);
    return c;
}

char topoChar(PilhaChar* p) {
    if (p->topo == NULL) {
        return '\0';
    }
    return p->topo->dado;
}

int estaVaziaChar(PilhaChar* p) {
    return p->topo == NULL;
}

void liberarPilhaChar(PilhaChar* p) {
    while (!estaVaziaChar(p)) {
        desempilharChar(p);
    }
    free(p);
}

typedef struct NoFloat {
    float dado;
    struct NoFloat* prox;
} NoFloat;

typedef struct {
    NoFloat* topo;
} PilhaFloat;

PilhaFloat* criarPilhaFloat() {
    PilhaFloat* p = (PilhaFloat*)malloc(sizeof(PilhaFloat));
    if (!p) {
        perror("Erro ao alocar PilhaFloat");
        exit(EXIT_FAILURE);
    }
    p->topo = NULL;
    return p;
}

void empilharFloat(PilhaFloat* p, float f) {
    NoFloat* novo = (NoFloat*)malloc(sizeof(NoFloat));
    if (!novo) {
        perror("Erro ao alocar NoFloat");
        exit(EXIT_FAILURE);
    }
    novo->dado = f;
    novo->prox = p->topo;
    p->topo = novo;
}

float desempilharFloat(PilhaFloat* p) {
    if (p->topo == NULL) {
        return NAN;
    }
    NoFloat* temp = p->topo;
    float f = temp->dado;
    p->topo = temp->prox;
    free(temp);
    return f;
}

int estaVaziaFloat(PilhaFloat* p) {
    return p->topo == NULL;
}

void liberarPilhaFloat(PilhaFloat* p) {
    while (!estaVaziaFloat(p)) {
        desempilharFloat(p);
    }
    free(p);
}

typedef struct NoString {
    char* dado;
    struct NoString* prox;
} NoString;

typedef struct {
    NoString* topo;
} PilhaString;

PilhaString* criarPilhaString() {
    PilhaString* p = (PilhaString*)malloc(sizeof(PilhaString));
    if (!p) {
        perror("Erro ao alocar PilhaString");
        exit(EXIT_FAILURE);
    }
    p->topo = NULL;
    return p;
}

void empilharString(PilhaString* p, char* s) {
    NoString* novo = (NoString*)malloc(sizeof(NoString));
    if (!novo) {
        perror("Erro ao alocar NoString");
        exit(EXIT_FAILURE);
    }
    novo->dado = strdup(s);
    if (!novo->dado) {
        perror("Erro ao duplicar string para empilhar");
        free(novo);
        exit(EXIT_FAILURE);
    }
    novo->prox = p->topo;
    p->topo = novo;
}

char* desempilharString(PilhaString* p) {
    if (p->topo == NULL) {
        return NULL;
    }
    NoString* temp = p->topo;
    char* s = temp->dado;
    p->topo = temp->prox;
    free(temp);
    return s;
}

int estaVaziaString(PilhaString* p) {
    return p->topo == NULL;
}

void liberarPilhaString(PilhaString* p) {
    while (!estaVaziaString(p)) {
        char* temp_str = desempilharString(p);
        if (temp_str) {
            free(temp_str);
        }
    }
    free(p);
}

int precedencia(char op) {
    if (op == '^') return 4;
    if (op == '*' || op == '/' || op == '%') return 3;
    if (op == '+' || op == '-') return 2;
    return 0;
}

int ehOperador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

int ehFuncao(const char* token) {
    return strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 ||
           strcmp(token, "tg") == 0 || strcmp(token, "log") == 0 ||
           strcmp(token, "raiz") == 0;
}

char* getFormaPosFixa(char* infixa) {
    PilhaChar* p_op = criarPilhaChar();
    char* posFixa = (char*)malloc(sizeof(char) * 512);
    if (!posFixa) {
        perror("Erro ao alocar posFixa");
        liberarPilhaChar(p_op);
        return NULL;
    }
    posFixa[0] = '\0';
    int j = 0;

    char temp_token[50];

    for (int i = 0; infixa[i] != '\0'; i++) {
        if (isspace(infixa[i])) continue;

        if (isdigit(infixa[i]) || (infixa[i] == '-' && (i == 0 || infixa[i-1] == '(' || ehOperador(infixa[i-1])) && isdigit(infixa[i+1]))) {
            while (isdigit(infixa[i]) || infixa[i] == '.') {
                posFixa[j++] = infixa[i++];
            }
            posFixa[j++] = ' ';
            i--;
        } else if (isalpha(infixa[i])) {
            int k = 0;
            while (isalpha(infixa[i]) && k < 49) {
                temp_token[k++] = infixa[i++];
            }
            temp_token[k] = '\0';
            i--;

            if (ehFuncao(temp_token)) {
                // Para simplificar, empilhamos um marcador para funções na pilha de operadores
                empilharChar(p_op, 'F');
            } else {
                fprintf(stderr, "Erro: Funcao desconhecida '%s'\n", temp_token);
                free(posFixa);
                liberarPilhaChar(p_op);
                return NULL;
            }
        }
        else if (infixa[i] == '(') {
            empilharChar(p_op, infixa[i]);
        } else if (infixa[i] == ')') {
            while (!estaVaziaChar(p_op) && topoChar(p_op) != '(') {
                posFixa[j++] = desempilharChar(p_op);
                posFixa[j++] = ' ';
            }
            if (!estaVaziaChar(p_op) && topoChar(p_op) == '(') {
                desempilharChar(p_op);
            } else {
                fprintf(stderr, "Erro: Parenteses nao balanceados (falta '(' ).\n");
                free(posFixa);
                liberarPilhaChar(p_op);
                return NULL;
            }
            if (!estaVaziaChar(p_op) && topoChar(p_op) == 'F') {
                posFixa[j++] = desempilharChar(p_op);
                posFixa[j++] = ' ';
            }
        } else if (ehOperador(infixa[i])) {
            while (!estaVaziaChar(p_op) && precedencia(topoChar(p_op)) >= precedencia(infixa[i])) {
                posFixa[j++] = desempilharChar(p_op);
                posFixa[j++] = ' ';
            }
            empilharChar(p_op, infixa[i]);
        } else {
            fprintf(stderr, "Erro: Caractere invalido na expressao infixa: '%c'\n", infixa[i]);
            free(posFixa);
            liberarPilhaChar(p_op);
            return NULL;
        }
    }

    while (!estaVaziaChar(p_op)) {
        char op = desempilharChar(p_op);
        if (op == '(') {
            fprintf(stderr, "Erro: Parenteses nao balanceados (falta ')' ).\n");
            free(posFixa);
            liberarPilhaChar(p_op);
            return NULL;
        }
        posFixa[j++] = op;
        posFixa[j++] = ' ';
    }

    posFixa[j] = '\0';
    liberarPilhaChar(p_op);
    return posFixa;
}

float getValorPosFixa(char* posFixaOriginal) {
    PilhaFloat* p_val = criarPilhaFloat();
    char* posFixaCopia = strdup(posFixaOriginal);
    if (!posFixaCopia) {
        perror("Erro ao duplicar string posFixa para avaliacao");
        liberarPilhaFloat(p_val);
        return NAN;
    }

    char* token = strtok(posFixaCopia, " ");
    float resultado = NAN;

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1])) || (token[0] == '.' && isdigit(token[1]))) {
            empilharFloat(p_val, atof(token));
        } else if (ehFuncao(token) || (token[0] == 'F' && token[1] == '\0')) {
            float a = desempilharFloat(p_val);
            if (isnan(a)) {
                fprintf(stderr, "Erro: Expressao pos-fixada invalida (operando insuficiente para funcao '%s').\n", token);
                goto error_cleanup;
            }

            if (strcmp(token, "sen") == 0) {
                empilharFloat(p_val, sin(a * M_PI / 180.0));
            } else if (strcmp(token, "cos") == 0) {
                empilharFloat(p_val, cos(a * M_PI / 180.0));
            } else if (strcmp(token, "tg") == 0) {
                empilharFloat(p_val, tan(a * M_PI / 180.0));
            } else if (strcmp(token, "log") == 0) {
                if (a <= 0.0f) {
                    fprintf(stderr, "Erro: Logaritmo de numero nao positivo (%.2f).\n", a);
                    goto error_cleanup;
                }
                empilharFloat(p_val, log10(a));
            } else if (strcmp(token, "raiz") == 0) {
                if (a < 0.0f) {
                    fprintf(stderr, "Erro: Raiz quadrada de numero negativo (%.2f).\n", a);
                    goto error_cleanup;
                }
                empilharFloat(p_val, sqrt(a));
            } else if (strcmp(token, "F") == 0) {
                fprintf(stderr, "Erro: Marcador de funcao generico 'F' encontrado sem nome de funcao explicito.\n");
                goto error_cleanup;
            }
        } else if (ehOperador(token[0]) && token[1] == '\0') {
            float b = desempilharFloat(p_val);
            float a = desempilharFloat(p_val);

            if (isnan(a) || isnan(b)) {
                fprintf(stderr, "Erro: Expressao pos-fixada invalida (operando insuficiente para operador '%c').\n", token[0]);
                goto error_cleanup;
            }

            switch (token[0]) {
                case '+': empilharFloat(p_val, a + b); break;
                case '-': empilharFloat(p_val, a - b); break;
                case '*': empilharFloat(p_val, a * b); break;
                case '/':
                    if (b == 0.0f) {
                        fprintf(stderr, "Erro: Divisao por zero.\n");
                        goto error_cleanup;
                    }
                    empilharFloat(p_val, a / b);
                    break;
                case '%':
                    if (b == 0.0f) {
                        fprintf(stderr, "Erro: Modulo por zero.\n");
                        goto error_cleanup;
                    }
                    empilharFloat(p_val, (float)((int)a % (int)b));
                    break;
                case '^': empilharFloat(p_val, pow(a, b)); break;
            }
        } else {
            fprintf(stderr, "Erro: Token desconhecido na expressao pos-fixada: '%s'.\n", token);
            goto error_cleanup;
        }
        token = strtok(NULL, " ");
    }

    if (estaVaziaFloat(p_val)) {
        fprintf(stderr, "Erro: Expressao pos-fixada vazia ou mal formada (nenhum resultado).\n");
        goto error_cleanup;
    }
    resultado = desempilharFloat(p_val);
    if (!estaVaziaFloat(p_val)) {
        fprintf(stderr, "Erro: Expressao pos-fixada mal formada (operandos extras).\n");
        while(!estaVaziaFloat(p_val)) desempilharFloat(p_val);
        resultado = NAN;
    }

error_cleanup:
    if (posFixaCopia) free(posFixaCopia);
    liberarPilhaFloat(p_val);
    return resultado;
}

char* getFormaInFixa(char* posFixaOriginal) {
    PilhaString* p_str = criarPilhaString();
    char* posFixaCopia = strdup(posFixaOriginal);
    if (!posFixaCopia) {
        perror("Erro ao duplicar string posFixa para conversao infixa");
        liberarPilhaString(p_str);
        return NULL;
    }

    char* token = strtok(posFixaCopia, " ");
    char* resultado = NULL;

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1])) || (token[0] == '.' && isdigit(token[1]))) {
            empilharString(p_str, token);
        } else if (ehOperador(token[0]) && token[1] == '\0') {
            char* b = desempilharString(p_str);
            char* a = desempilharString(p_str);

            if (a == NULL || b == NULL) {
                fprintf(stderr, "Erro: Expressao pos-fixada invalida (operandos insuficientes para operador '%c').\n", token[0]);
                if (a) free(a);
                if (b) free(b);
                goto error_cleanup;
            }

            char* temp_infixa = (char*)malloc(strlen(a) + strlen(b) + 5 + 1);
            if (!temp_infixa) {
                perror("Erro ao alocar temp_infixa");
                free(a); free(b);
                goto error_cleanup;
            }
            sprintf(temp_infixa, "(%s %c %s)", a, token[0], b);
            empilharString(p_str, temp_infixa);
            free(temp_infixa);
            free(a);
            free(b);

        } else if (ehFuncao(token) || (token[0] == 'F' && token[1] == '\0')) {
            char* a = desempilharString(p_str);
            if (a == NULL) {
                fprintf(stderr, "Erro: Expressao pos-fixada invalida (operando insuficiente para funcao '%s').\n", token);
                goto error_cleanup;
            }

            char* temp_infixa;
            if (strcmp(token, "F") == 0) {
                temp_infixa = (char*)malloc(strlen(a) + 10 + 1);
                if (!temp_infixa) { free(a); goto error_cleanup; }
                sprintf(temp_infixa, "func(%s)", a);
            } else {
                temp_infixa = (char*)malloc(strlen(token) + strlen(a) + 3 + 1);
                if (!temp_infixa) { free(a); goto error_cleanup; }
                sprintf(temp_infixa, "%s(%s)", token, a);
            }
            empilharString(p_str, temp_infixa);
            free(temp_infixa);
            free(a);
        } else {
            fprintf(stderr, "Erro: Token desconhecido na expressao pos-fixada para conversao infixa: '%s'.\n", token);
            goto error_cleanup;
        }
        token = strtok(NULL, " ");
    }

    if (estaVaziaString(p_str)) {
        fprintf(stderr, "Erro: Expressao pos-fixada vazia ou mal formada (nenhum resultado para infixa).\n");
        goto error_cleanup;
    }
    resultado = desempilharString(p_str);
    if (!estaVaziaString(p_str)) {
        fprintf(stderr, "Erro: Expressao pos-fixada mal formada (strings extras na pilha).\n");
        if (resultado) free(resultado);
        resultado = NULL;
        while(!estaVaziaString(p_str)) {
            char* extra_str = desempilharString(p_str);
            if (extra_str) free(extra_str);
        }
    }

error_cleanup:
    if (posFixaCopia) free(posFixaCopia);
    liberarPilhaString(p_str);
    return resultado;
}

float getValorInFixa(char* infixa) {
    char* posFixa = getFormaPosFixa(infixa);
    if (posFixa == NULL) {
        return NAN;
    }

    float valor = getValorPosFixa(posFixa);

    free(posFixa);

    return valor;
}