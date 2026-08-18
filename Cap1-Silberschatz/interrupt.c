/*
Programa em C (Linux/POSIX) que instala tratadores de sinal para SIGINT
(Ctrl+C) e SIGTSTP (Ctrl+Z) e executa um laço infinito imprimindo uma
mensagem a cada segundo. Demonstra como o sistema operacional desvia o
fluxo de execução para uma rotina de tratamento de interrupção (signal
handler) quando um desses sinais é recebido.
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void trata_interrupcao(int sig) {
    printf("\n>>> Interrupção recebida! Código: %d\n", sig);
}

int main() {
    signal(SIGINT, trata_interrupcao); // Ctrl+C
    signal(SIGTSTP, trata_interrupcao); // Ctrl+Z

    while (1) {
        printf("Executando tarefa principal...\n");
        sleep(1);
    }
    return 0;
}
