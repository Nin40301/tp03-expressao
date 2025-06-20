#ifndef EXPRESSAO_H
#define EXPRESSAO_H

typedef struct {
    char posFixa[512];
    char infixa[512];
    float valor;
} Expressao;

char* getFormaInFixa(char* posFixa);
char* getFormaPosFixa(char* infixa);
float getValorPosFixa(char* posFixa);
float getValorInFixa(char* infixa);

#endif