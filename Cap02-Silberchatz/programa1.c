/*
Cria um processo filho (child.exe) via CreateProcess, aguarda seu término com
WaitForSingleObject e, em seguida, altera uma variável global no processo pai.
Demonstra que processo pai e processo filho possuem espaços de memória
independentes (a alteração feita pelo pai não é vista pelo filho, e vice-versa).
*/

#include <windows.h>
#include <stdio.h>

int value = 5;

int main() {
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Caminho para o executável do processo filho
    char commandLine[] = "child.exe";

    // Cria um novo processo
    if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "CreateProcess falhou. Erro: %d\n", GetLastError());
        return 1;
    }

    // Espera pelo processo filho terminar
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Incrementa o valor como se fosse no processo filho
    value += 15;

    // Fecha os manipuladores do processo e do thread
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    // Imprime o valor como no processo pai
    printf("PARENT: value = %d\n", value); // LINHA A

    return 0;
}
