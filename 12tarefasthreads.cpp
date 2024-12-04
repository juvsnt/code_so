#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h> // Biblioteca para recursos específicos do Windows

// Função que será executada por cada thread
void tarefa(int nucleo) {
    // Define a afinidade da thread para o núcleo especificado
    HANDLE threadHandle = GetCurrentThread();
    DWORD_PTR mask = 1 << nucleo; // Máscara binária para o núcleo (ex: 1 << 0 = 0001, 1 << 1 = 0010, etc.)
    SetThreadAffinityMask(threadHandle, mask);

    for (int i = 0; i < 5; ++i) {
        std::cout << "[Thread " << nucleo << "] Executando no nucleo " << nucleo << "...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simula trabalho
    }
}

int main() {
#ifdef _WIN32 // Verifica se o código está sendo compilado em um sistema Windows
    std::cout << "Executando em ambiente Windows 11.\n";

    const int numNucleos = 12; // Número de processadores lógicos (ajuste conforme necessário)
    std::thread threads[numNucleos];

    // Criando uma thread para cada núcleo lógico
    for (int i = 0; i < numNucleos; ++i) {
        threads[i] = std::thread(tarefa, i);
    }

    // Aguarda todas as threads terminarem
    for (int i = 0; i < numNucleos; ++i) {
        threads[i].join();
    }

    std::cout << "Todas as threads terminaram.\n";
#else
    std::cerr << "Este programa só pode ser executado em sistemas Windows.\n";
#endif
    return 0;
}