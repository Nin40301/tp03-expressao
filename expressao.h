#ifndef EXPRESSAO_H
#define EXPRESSAO_H

typedef struct {
    char posFixa[512]; // Forma pós-fixa (ex: "3 4 + 5 *")
    char infixa[512];  // Forma infixa (ex: "(3 + 4) * 5")
    float valor;       // Valor calculado
} Expressao;

char* getFormaInFixa(char* posFixa);    // Converte pós-fixa → infixa
char* getFormaPosFixa(char* infixa);    // Converte infixa → pós-fixa
float getValorPosFixa(char* posFixa);   // Calcula valor da pós-fixa
float getValorInFixa(char* infixa);     // Calcula valor da infixa

#endif