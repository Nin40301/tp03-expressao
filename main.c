#include <stdio.h>
#include "expressao.h"

int main() {
    // Testes da tabela do enunciado
    Expressao testes[] = {
        {"3 4 + 5 *", "(3 + 4) * 5", 0},
        {"7 2 * 4 +", "7 * 2 + 4", 0},
        {"8 5 2 4 + * +", "8 + (5 * (2 + 4))", 0},
        {"2 3 + log 5 /", "log(2 + 3) / 5", 0},
        {"45 60 + 30 cos *", "(45 + 60) * cos(30)", 0}
    };

    for (int i = 0; i < 5; i++) {
        testes[i].valor = getValorPosFixa(testes[i].posFixa);
        printf("Teste %d:\n", i+1);
        printf("Pós-fixa: %s\n", testes[i].posFixa);
        printf("Infixa: %s\n", getFormaInFixa(testes[i].posFixa));
        printf("Valor: %.2f\n\n", testes[i].valor);
    }

    return 0;
}