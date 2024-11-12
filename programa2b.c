#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h> // Para usar CreateToolhelp32Snapshot e PROCESSENTRY32

// Função para obter o PID do processo pai
DWORD GetParentProcessID() {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }
    
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    DWORD ppid = 0;
    DWORD pid = GetCurrentProcessId();

    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (pe32.th32ProcessID == pid) {
                ppid = pe32.th32ParentProcessID;
                break;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return ppid;
}

// Função auxiliar para criar um novo processo
void create_process() {
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
        return;
    }

    // Imprime os identificadores do processo filho
    printf("Processo filho criado. PID: %lu, PPID: %lu\n", pi.dwProcessId, GetCurrentProcessId());

    // Fecha os manipuladores do processo e do thread
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    // Cria 3 processos filhos
    for (int i = 0; i < 3; i++) {
        create_process();
    }

    return 0;
}
