/*Versão para Linux*/

#include <iostream>
#include <thread> // Biblioteca para threads
#include <chrono> // Para simular atrasos

// Função que será executada por uma thread
void tarefa1() {
    for (int i = 0; i < 5; ++i) {
        std::cout << "Tarefa 1 executando...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simula trabalho
    }
}

// Outra função para ser executada por uma thread
void tarefa2() {
    for (int i = 0; i < 5; ++i) {
        std::cout << "Tarefa 2 executando...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simula trabalho
    }
}

int main() {
    // Criando duas threads
    std::thread thread1(tarefa1);
    std::thread thread2(tarefa2);

    // Aguarda as threads terminarem
    thread1.join();
    thread2.join();

    std::cout << "Todas as threads terminaram.\n";
    return 0;
}