// Exemplo de criação e controle de threads usando a API do Windows (WinAPI).
// O programa define duas funções (tarefa1 e tarefa2), cada uma executada em uma thread.
// As threads são criadas com CreateThread e executam em paralelo,
// exibindo mensagens e simulando pausas com Sleep.
// O main utiliza WaitForSingleObject para aguardar o término de cada thread
// e depois fecha os handles com CloseHandle.
// Esse exemplo ilustra conceitos de:
//   - Programação concorrente no Windows
//   - Criação e gerenciamento de threads via WinAPI
//   - Diferença entre uso de threads nativas do sistema e std::thread do C++.


#include <iostream>
#include <thread>
#include <windows.h> // Biblioteca para threads no Windows

// Função que será executada pela thread
DWORD WINAPI tarefa1(LPVOID lpParam) {
    //int i = *(int*)lpParam; // Convertendo o parâmetro para int
    for (int i = 0; i < 5; ++i) {
        std::cout << "Tarefa 1 executando...\n";
        Sleep(500); // Pausa de 500ms
    }
    return 0;
}

// Outra função para ser executada pela thread
DWORD WINAPI tarefa2(LPVOID lpParam) {
    //int i = *(int*)lpParam; // Convertendo o parâmetro para int
    for (int i = 0; i < 5; ++i) {
        std::cout << "Tarefa 2 executando...\n";
        Sleep(500); // Pausa de 500ms
    }
    return 0;
}

int main() {
    // Criando duas threads
    HANDLE thread1 = CreateThread(
        NULL,        // Atributos de segurança padrão
        0,           // Tamanho da pilha padrão
        tarefa1,     // Função a ser executada pela thread
        NULL,        // Parâmetro para a função (nenhum neste caso)
        0,           // Flags de criação (0 = inicia imediatamente) 
        NULL         // ID da thread (não usado aqui)
    );

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