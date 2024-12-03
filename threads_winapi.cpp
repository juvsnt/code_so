#include <iostream>
#include <windows.h> // Biblioteca para threads no Windows

// Função que será executada pela thread
DWORD WINAPI tarefa1(LPVOID lpParam) {
    for (int i = 0; i < 5; ++i) {
        std::cout << "Tarefa 1 executando...\n";
        Sleep(500); // Pausa de 500ms
    }
    return 0;
}

// Outra função para ser executada pela thread
DWORD WINAPI tarefa2(LPVOID lpParam) {
    for (int i = 0; i < 5; ++i) {
        std::cout << "Tarefa 2 executando...\n";
        Sleep(500); // Pausa de 500ms
    }
    return 0;
}

int main() {
    // Criando duas threads
    HANDLE thread1 = CreateThread(NULL, 0, tarefa1, NULL, 0, NULL);
    HANDLE thread2 = CreateThread(NULL, 0, tarefa2, NULL, 0, NULL);

    // Aguarda as threads terminarem
    WaitForSingleObject(thread1, INFINITE);
    WaitForSingleObject(thread2, INFINITE);

    // Fecha os handles das threads
    CloseHandle(thread1);
    CloseHandle(thread2);

    std::cout << "Todas as threads terminaram.\n";
    return 0;
}