# Avaliador de Expressões Numéricas

## 1. Introdução

Este projeto consiste no desenvolvimento de um programa em linguagem C para avaliar expressões matemáticas. O objetivo principal é permitir a conversão de expressões entre as notações infixa e pós-fixada (polonesa inversa), bem como calcular o resultado numérico dessas expressões. A estrutura de dados fundamental utilizada para a manipulação das operações matemáticas, especialmente no que tange à ordem de precedência dos operadores, é a Pilha[cite: 3].

O sistema suporta as seguintes operações aritméticas básicas:
- Adição (`+`)
- Subtração (`-`)
- Multiplicação (`*`)
- Divisão (`/`)
- Módulo (`%`)
- Potenciação (`^`)

Além disso, contempla funções matemáticas como:
- Seno (`sen`)
- Cosseno (`cos`)
- Tangente (`tg`)
- Logaritmo na base 10 (`log`)
- Raiz Quadrada (`raiz`)

**Observação**: As funções trigonométricas utilizam graus para os cálculos[cite: 4].

A modularidade foi uma prioridade no desenvolvimento, com as funcionalidades distribuídas entre arquivos de cabeçalho e implementação, conforme a estrutura de dados e as regras de projeto[cite: 5].

## 2. Estrutura do Projeto

O projeto é dividido nos seguintes arquivos:

-   `expressao.h`: Define o Tipo Abstrato de Dados (TAD) `Expressao`, que contém campos para as formas infixa e pós-fixada da expressão e seu valor calculado[cite: 7]. Também declara os protótipos das funções responsáveis pelas conversões e avaliações:
    * `getFormaInFixa`: Converte uma expressão pós-fixada para infixa[cite: 8].
    * `getFormaPosFixa`: Converte uma expressão infixa para pós-fixada[cite: 8].
    * `getValorPosFixa`: Calcula o valor numérico de uma expressão pós-fixada[cite: 8].
    * `getValorInFixa`: Calcula o valor numérico de uma expressão infixa[cite: 8].

-   `expressao.c`: Contém a implementação de todas as funcionalidades do sistema. Isso inclui a criação e manipulação das pilhas (Pilha de Caracteres, Pilha de Floats, Pilha de Strings), lógica de precedência de operadores, tokenização de expressões, e chamadas às funções matemáticas da biblioteca `math.h`[cite: 8].
    * A avaliação de expressões pós-fixadas utiliza uma pilha de números (floats)[cite: 9].
    * A reconstrução da forma infixa a partir da pós-fixada é feita com uma pilha de strings[cite: 9].

-   `main.c`: Contém a função principal que demonstra o uso da biblioteca `expressao.h`, realizando testes com um conjunto predefinido de expressões e exibindo os resultados.

## 3. Como Compilar e Executar

Para compilar o projeto, navegue até o diretório raiz do projeto no terminal e execute o seguinte comando:

```bash
gcc expressao.c main.c -o avaliador -lm