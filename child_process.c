/*Programa que cria 1 processo pai e 5 processos filhos*/

#include <windows.h>
#include <stdio.h>

#define MAX_CHILDREN 5

void createChildProcess(int count) {
    if (count >= MAX_CHILDREN) {
        return; // Parar após criar 5 processos filhos
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Comando para criar um processo filho que executa o próprio programa novamente
    char command[256];
    snprintf(command, sizeof(command), "child_process.exe %d", count + 1);

    // Criação do processo filho
    if (!CreateProcess(NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "Falha na criacao do processo filho (%d).\n", GetLastError());
        return;
    }

    printf("Processo filho criado com PID: %d, Contador: %d\n", pi.dwProcessId, count + 1);

    // Fechar os handles do processo e thread
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main(int argc, char *argv[]) {
    int count = 0;

    if (argc > 1) {
        count = atoi(argv[1]);
    }

    if (count >= MAX_CHILDREN) {
        return 0; // Evitar que filhos criem novos processos além do limite
    }

    if (count == 0) {
        printf("Processo pai, criando filhos...\n");
    }

    // Criação dos processos filhos
    createChildProcess(count);

    printf("Execucao completa. Total de processos criados: %d\n", count + 1);

    return 0;
}

