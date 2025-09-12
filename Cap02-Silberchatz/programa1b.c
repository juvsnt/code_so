#include <windows.h>
#include <stdio.h>

int value = 5;

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "child") == 0) {
        // Este é o processo filho
        value += 15;
        printf("CHILD: value = %d\n", value);
        return 0;
    }

    // Este é o processo pai
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Cria um novo processo que executa o mesmo programa, mas como filho
    char commandLine[] = "child.exe child"; // Substitua "program.exe" pelo nome do seu executável

    if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "CreateProcess falhou. Erro: %d\n", GetLastError());
        return 1;
    }

    // Espera pelo processo filho terminar
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Fecha os manipuladores do processo e do thread
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    // Imprime o valor como no processo pai
    printf("PARENT: value = %d\n", value);

    return 0;
}