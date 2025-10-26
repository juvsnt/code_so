// Exemplo de criação e controle de threads usando a API do Linux (pthread).
// O programa define duas funções (tarefa1 e tarefa2), cada uma executada em uma thread.
// As threads são criadas com pthread_create e executam em paralelo,   

#include <iostream>
#include <pthread.h>
#include <unistd.h> // para usleep

void* tarefa1(void* arg) {
    for (int i = 0; i < 5; ++i) {
        std::cout << "Tarefa 1 executando...\n";
        usleep(500000); // 500ms
    }
    return nullptr;
}

void* tarefa2(void* arg) {
    for (int i = 0; i < 5; ++i) {
        std::cout << "Tarefa 2 executando...\n";
        usleep(500000); // 500ms
    }
    return nullptr;
}

int main() {
    pthread_t thread1, thread2;

    // Criando as threads
    pthread_create(&thread1, nullptr, tarefa1, nullptr);
    pthread_create(&thread2, nullptr, tarefa2, nullptr);

    // Aguarda as threads terminarem
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);

    std::cout << "Todas as threads terminaram.\n";
    return 0;
}
 