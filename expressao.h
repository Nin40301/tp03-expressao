#ifndef EXPRESSAO_H
#define EXPRESSAO_H

typedef struct {
    char posFixa[512];  // Expressão na forma pós-fixa (ex: "3 12 4 + *")
    char inFixa[512];   // Expressão na forma infixa (ex: "3 * (12 + 4)")
    float Valor;        // Valor numérico da expressão
} Expressao;

char *getFormaInFixa(char *Str);      // Converte pós-fixa para infixa
char *getFormaPosFixa(char *Str);     // Converte infixa para pós-fixa
float getValorPosFixa(char *Str);     // Avalia expressão pós-fixa
float getValorInFixa(char *Str);      // Avalia expressão infixa (via conversão para pós-fixa)

#endif