#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 5  // Tamanho do buffer circular
#define NUM_PRODUCERS 1  // Número de produtores
#define NUM_CONSUMERS 1  // Número de consumidores

// Estrutura do buffer circular
typedef struct {
    int buffer[BUFFER_SIZE];
    int in;     // Índice para inserção
    int out;    // Índice para remoção
} CircularBuffer;

// Variáveis globais
CircularBuffer cb;
HANDLE emptySlots;    // Semáforo para controlar espaços vazios
HANDLE fullSlots;     // Semáforo para controlar espaços ocupados
HANDLE mutex;         // Mutex para exclusão mútua
BOOL running = TRUE;  // Flag para controle de execução

// Inicializa o buffer e mecanismos de sincronização
void initBuffer() {
    cb.in = 0;
    cb.out = 0;
    
    // Cria semáforos e mutex
    emptySlots = CreateSemaphore(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL);
    fullSlots = CreateSemaphore(NULL, 0, BUFFER_SIZE, NULL);
    mutex = CreateMutex(NULL, FALSE, NULL);
}

// Função do Produtor
DWORD WINAPI producer(LPVOID lpParam) {
    int producerId = *((int*)lpParam);
    int item = 1;

    while (running) {
        printf("Produtor %d quer produzir item %d\n", producerId, item);
        
        // Espera por um espaço vazio
        WaitForSingleObject(emptySlots, INFINITE);
        
        // Obtém acesso exclusivo ao buffer
        WaitForSingleObject(mutex, INFINITE);
        
        // Coloca o item no buffer
        cb.buffer[cb.in] = item;
        printf("Produtor %d produziu item %d na posicao %d\n", producerId, item, cb.in);
        cb.in = (cb.in + 1) % BUFFER_SIZE;
        
        // Libera o mutex
        ReleaseMutex(mutex);
        
        // Sinaliza que há um novo item
        ReleaseSemaphore(fullSlots, 1, NULL);
        
        item++;
        Sleep(1000);  // Simula tempo de produção (1 segundo)
    }
    
    free(lpParam);
    return 0;
}

// Função do Consumidor
DWORD WINAPI consumer(LPVOID lpParam) {
    int consumerId = *((int*)lpParam);

    while (running) {
        // Espera por um item disponível
        WaitForSingleObject(fullSlots, INFINITE);
        
        // Obtém acesso exclusivo ao buffer
        WaitForSingleObject(mutex, INFINITE);
        
        // Remove o item do buffer
        int item = cb.buffer[cb.out];
        printf("Consumidor %d consumiu item %d da posicao %d\n", consumerId, item, cb.out);
        cb.out = (cb.out + 1) % BUFFER_SIZE;
        
        // Libera o mutex
        ReleaseMutex(mutex);
        
        // Sinaliza que há um espaço vazio
        ReleaseSemaphore(emptySlots, 1, NULL);
        
        Sleep(2000);  // Simula tempo de consumo (2 segundos)
}
 free(lpParam);
    return 0;
}

int main() {
    HANDLE* producerThreads;
    HANDLE* consumerThreads;
    int* producerIds;
    int* consumerIds;
    int i;

    // Aloca memória para os arrays
    producerThreads = (HANDLE*)malloc(NUM_PRODUCERS * sizeof(HANDLE));
    consumerThreads = (HANDLE*)malloc(NUM_CONSUMERS * sizeof(HANDLE));
    producerIds = (int*)malloc(NUM_PRODUCERS * sizeof(int));
    consumerIds = (int*)malloc(NUM_CONSUMERS * sizeof(int));

    // Inicializa o buffer e mecanismos de sincronização
    initBuffer();

    // Cria threads produtoras
    for (i = 0; i < NUM_PRODUCERS; i++) {
        producerIds[i] = i + 1;
        int* id = (int*)malloc(sizeof(int));
        *id = producerIds[i];
        producerThreads[i] = CreateThread(NULL, 0, producer, id, 0, NULL);
    }

    // Cria threads consumidoras
    for (i = 0; i < NUM_CONSUMERS; i++) {
        consumerIds[i] = i + 1;
        int* id = (int*)malloc(sizeof(int));
        *id = consumerIds[i];
        consumerThreads[i] = CreateThread(NULL, 0, consumer, id, 0, NULL);
    }

    printf("Pressione Enter para encerrar...\n");
    getchar();

    // Sinaliza para as threads pararem
    running = FALSE;

    // Espera todas as threads terminarem
    WaitForMultipleObjects(NUM_PRODUCERS, producerThreads, TRUE, INFINITE);
    WaitForMultipleObjects(NUM_CONSUMERS, consumerThreads, TRUE, INFINITE);

    // Fecha os handles das threads
    for (i = 0; i < NUM_PRODUCERS; i++) {
        CloseHandle(producerThreads[i]);
    }
    for (i = 0; i < NUM_CONSUMERS; i++) {
        CloseHandle(consumerThreads[i]);
    }

    // Fecha handles dos semáforos e mutex
    CloseHandle(emptySlots);
    CloseHandle(fullSlots);
    CloseHandle(mutex);

    // Libera memória alocada
    free(producerThreads);
    free(consumerThreads);
    free(producerIds);
    free(consumerIds);

    return 0;
}