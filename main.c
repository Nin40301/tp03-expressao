#include <stdio.h>
#include "expressao.h"

int main() {
    Expressao testes[] = {
        {"3 4 + 5 *", "(3 + 4) * 5", 0},
        {"7 2 * 4 +", "7 * 2 + 4", 0},
        {"8 5 2 4 + * +", "8 + (5 * (2 + 4))", 0},
        {"2 3 + log 5 /", "log(2 + 3) / 5", 0},
        {"45 60 + 30 cos *", "(45 + 60) * cos(30)", 0},
        {"0.5 45 sen 2 ^ +", "sen(45)^2 + 0.5", 0},
        {"10 log 3 ^ 2 +", "(log10)^3 + 2", 0}
    };

    for (int i = 0; i < 7; i++) {
        testes[i].valor = getValorPosFixa(testes[i].posFixa);
        printf("Teste %d:\n", i+1);
        printf("Pos-fixa: %s\n", testes[i].posFixa);
        printf("Infixa: %s\n", getFormaInFixa(testes[i].posFixa));
        printf("Valor: %.2f\n\n", testes[i].valor);
    }

    return 0;
}