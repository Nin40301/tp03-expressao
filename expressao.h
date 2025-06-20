#ifndef EXPRESSAO_H
#define EXPRESSAO_H

// Estrutura para armazenar as expressoes
typedef struct {
    char posFixa[512];  // Forma pos-fixa (notacao polonesa reversa)
    char inFixa[512];   // Forma infixa (notacao convencional)
    float Valor;        // Valor numerico calculado
} Expressao;

// Prototipos das funcoes
char *converterParaInFixa(char *Str); 
char *converterParaPosFixa(char *Str);
float calcularExpressaoPosFixa(char *StrPosFixa);
float calcularExpressaoInFixa(char *StrInFixa);

#endif