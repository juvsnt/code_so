// Exemplo didático de programação concorrente em C++.
// O programa cria duas threads, cada uma executando uma função (tarefa1 e tarefa2).
// Enquanto as threads estão em execução, ambas imprimem mensagens intercaladas,
// simulando atividades com pequenos atrasos (sleep_for).
// O main aguarda o término das duas threads (join) antes de finalizar.
// Esse exemplo demonstra como funções podem rodar em paralelo, aproveitando
// recursos de multiprocessamento e ilustrando conceitos básicos de concorrência.


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