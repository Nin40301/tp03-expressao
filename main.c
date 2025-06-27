#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "expressao.h"

int main() {
    Expressao testes[] = {
        {"3 4 + 5 *", "(3 + 4) * 5", 0},
        {"7 2 * 4 +", "7 * 2 + 4", 0},
        {"8 5 2 4 + * +", "8 + (5 * (2 + 4))", 0},
        {"2 3 + log 5 /", "log(2 + 3) / 5", 0},
        {"45 60 + 30 cos *", "(45 + 60) * cos(30)", 0},
        {"0.5 45 sen 2 ^ +", "sen(45)^2 + 0.5", 0},
        {"10 log 3 ^ 2 +", "(log10)^3 + 2", 0},
        {"5 0 /", "", 0},
        {"-5 log", "", 0},
        {"5 -2 raiz", "", 0},
        {"(1 + 2", "", 0},
        {"3 + + 4", "", 0},
        {"5 2 + 3 * 4 / 6 +", "((5 + 2) * 3) / 4 + 6", 0},
        {"2 3 ^", "2 ^ 3", 0},
        {"10 2 %", "10 % 2", 0}
    };
    int num_testes = sizeof(testes) / sizeof(Expressao);

    printf("--- Avaliador de Expressoes ---\n\n");

    for (int i = 0; i < num_testes; i++) {
        printf("--- Teste %d ---\n", i + 1);

        char temp_posFixa_valor[512];
        char temp_posFixa_infixa[512];
        char temp_infixa_original[512];

        strcpy(temp_posFixa_valor, testes[i].posFixa);
        testes[i].valor = getValorPosFixa(temp_posFixa_valor);
        printf("Pos-fixa (Original): %s\n", testes[i].posFixa);
        if (isnan(testes[i].valor)) {
            printf("Valor (Pos-fixa): ERRO NO CALCULO\n");
        } else {
            printf("Valor (Pos-fixa): %.2f\n", testes[i].valor);
        }

        strcpy(temp_posFixa_infixa, testes[i].posFixa);
        char* infixa_convertida = getFormaInFixa(temp_posFixa_infixa);
        if (infixa_convertida == NULL) {
            printf("Infixa (Convertida): ERRO NA CONVERSAO\n");
        } else {
            printf("Infixa (Convertida): %s\n", infixa_convertida);
            free(infixa_convertida);
        }

        strcpy(temp_infixa_original, testes[i].infixa);
        float valor_infixa = getValorInFixa(temp_infixa_original);
        printf("Infixa (Original): %s\n", testes[i].infixa);
        if (isnan(valor_infixa)) {
            printf("Valor (Infixa): ERRO NO CALCULO\n");
        } else {
            printf("Valor (Infixa): %.2f\n", valor_infixa);
        }

        printf("\n");
    }

    return 0;
}