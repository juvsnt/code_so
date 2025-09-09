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
