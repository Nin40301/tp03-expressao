#include <stdio.h>
#include "expressao.h"

int main() {
    // Testes da tabela fornecida
    const char *testes[][3] = {
        {"3 4 + 5 *", "(3 + 4) * 5", "35"},
        {"7 2 * 4 +", "7 * 2 + 4", "18"},
        {"8 5 2 4 + * +", "8 + (5 * (2 + 4))", "38"},
        {"6 2 / 3 + 4 *", "(6 / 2 + 3) * 4", "24"},
        {"9 5 2 8 * 4 + * +", "9 + (5 * (2 + 8 * 4))", "109"},
        {"2 3 + log 5 /", "log(2 + 3) / 5", "0.14"},
        {"10 log 3 ^ 2 +", "(log10)^3 + 2", "3"},
        {"45 60 + 30 cos *", "(45 + 60) * cos(30)", "90.93"},
        {"0.5 45 sen 2 ^ +", "sen(45)^2 + 0.5", "1"}
    };

    for (int i = 0; i < 9; i++) {
        Expressao exp;
        strcpy(exp.posFixa, testes[i][0]);
        strcpy(exp.inFixa, testes[i][1]);
        exp.Valor = getValorPosFixa(exp.posFixa);

        printf("Teste %d:\n", i + 1);
        printf("Pós-fixa: %s\n", exp.posFixa);
        printf("Infixa: %s\n", exp.inFixa);
        printf("Valor esperado: %s | Valor calculado: %.2f\n\n", testes[i][2], exp.Valor);
    }

    return 0;
}