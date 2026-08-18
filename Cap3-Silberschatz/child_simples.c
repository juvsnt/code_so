/*
Processo filho minimalista: apenas imprime uma mensagem indicando que está
em execução. Usado como executável auxiliar (child.exe) pelos programas de
criação de processos deste capítulo.
*/

// child.c
#include <stdio.h>

int main() {
    printf("Processo filho executando corretamente.\n");
    return 0;
}
