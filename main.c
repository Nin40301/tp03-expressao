#include <stdio.h>
#include <string.h>
#include "expressao.h"

// Funcao para testar casos do enunciado
void testarCaso(char *posFixa, char *infixaEsperada, float valorEsperado) {
    printf("\n=== Testando: %s ===\n", posFixa);
    
    // Teste conversao pos-fixa -> infixa
    char temp[512];
    strcpy(temp, posFixa);
    printf("Pos-fixa: %s\n", posFixa);
    printf("Infixa esperada: %s\n", infixaEsperada);
    printf("Infixa obtida: %s\n", converterParaInFixa(temp));
    
    // Teste calculo pos-fixa
    strcpy(temp, posFixa);
    float valor = calcularExpressaoPosFixa(temp);
    printf("Valor esperado: %.2f\n", valorEsperado);
    printf("Valor obtido: %.2f\n", valor);
    
    // Teste conversao infixa -> pos-fixa e calculo
    strcpy(temp, infixaEsperada);
    printf("\nVerificando conversao infixa->posfixa:\n");
    printf("Pos-fixa obtida: %s\n", converterParaPosFixa(temp));
    printf("Valor da infixa: %.2f\n", calcularExpressaoInFixa(temp));
}

int main() {
    printf("===== TESTES AUTOMATICOS =====\n");
    
    // Testes conforme a tabela do enunciado
    testarCaso("3 4 + 5 *", "(3 + 4) * 5", 35.0f);
    testarCaso("7 2 * 4 +", "7 * 2 + 4", 18.0f);
    testarCaso("8 5 2 4 + * +", "8 + (5 * (2 + 4))", 38.0f);
    testarCaso("6 2 / 3 + 4 *", "(6 / 2 + 3) * 4", 24.0f);
    testarCaso("9 5 2 8 * 4 + * +", "9 + (5 * (2 + 8 * 4))", 109.0f);
    testarCaso("2 3 + log 5 /", "log(2 + 3) / 5", 0.14f);
    testarCaso("10 log 3 ^ 2 +", "(log10)^3 + 2", 3.0f);
    testarCaso("45 60 + 30 cos *", "(45 + 60) * cos(30)", 90.93f);
    testarCaso("0.5 45 sen 2 ^ +", "sen(45)^2 + 0.5", 1.0f);

    // Modo interativo
    printf("\n===== MODO INTERATIVO =====\n");
    printf("Digite expressao infixa (ex: (3+4)*5) ou 'sair' para terminar\n");
    
    char entrada[512];
    while (1) {
        printf("\n> ");
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = 0;
        
        if (strcmp(entrada, "sair") == 0) break;
        
        printf("\nExpressao infixa: %s\n", entrada);
        
        char *posFixa = converterParaPosFixa(entrada);
        printf("Forma pos-fixa: %s\n", posFixa);
        
        printf("Forma infixa reconstruida: %s\n", converterParaInFixa(posFixa));
        printf("Valor calculado: %.2f\n", calcularExpressaoInFixa(entrada));
    }

    return 0;
}