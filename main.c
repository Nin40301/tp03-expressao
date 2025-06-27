#include <stdio.h>
#include <string.h>
#include "expressao.h"

int main() {
    ExpressaoMatematica expr;
    char entrada[MAX_EXPRESSAO];
    char tipo[10];

    printf("Informe o tipo de expressao (infixa/posfixa): ");
    fgets(tipo, sizeof(tipo), stdin);
    tipo[strcspn(tipo, "\n")] = '\0';

    if (strcmp(tipo, "infixa") == 0) {
        printf("Digite a expressao infixa: ");
        fgets(entrada, MAX_EXPRESSAO, stdin);
        entrada[strcspn(entrada, "\n")] = '\0';

        strcpy(expr.infixa, entrada);
        converterParaPosfixa(expr.infixa, expr.posfixa);
        expr.valor = avaliarInfixa(expr.infixa);

        printf("\nForma infixa: %s\n", expr.infixa);
        printf("Forma posfixa: %s\n", expr.posfixa);
        printf("Valor calculado: %.2f\n", expr.valor);
    } else if (strcmp(tipo, "posfixa") == 0) {
        printf("Digite a expressao posfixa: ");
        fgets(entrada, MAX_EXPRESSAO, stdin);
        entrada[strcspn(entrada, "\n")] = '\0';

        strcpy(expr.posfixa, entrada);
        converterParaInfixa(expr.posfixa, expr.infixa);
        expr.valor = avaliarPosfixa(expr.posfixa);

        printf("\nForma posfixa: %s\n", expr.posfixa);
        printf("Forma infixa: %s\n", expr.infixa);
        printf("Valor calculado: %.2f\n", expr.valor);
    } else {
        printf("Tipo de expressao invalido.\n");
        return 1;
    }

    return 0;
}