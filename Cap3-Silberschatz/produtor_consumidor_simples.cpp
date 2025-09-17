#include <iostream>             // Para std::cout
#include <thread>               // Para std::thread, std::this_thread::sleep_for
#include <mutex>                // Para std::mutex, std::unique_lock
#include <condition_variable>   // Para std::condition_variable
#include <chrono>               // Para std::chrono::milliseconds

// --- Variáveis Globais Compartilhadas ---

// O buffer compartilhado. Para simplificar, um único inteiro.
int buffer; 
int count = 0; // Quantidade de itens no buffer (0 = vazio, 1 = cheio)

// Mutex para proteger o acesso às variáveis compartilhadas (buffer e count)
std::mutex mtx;

// Variável de condição para o produtor esperar (quando o buffer está cheio)
std::condition_variable cv_empty;

// Variável de condição para o consumidor esperar (quando o buffer está vazio)
std::condition_variable cv_fill;

// Número de itens que o produtor irá produzir e o consumidor irá consumir
const int loops = 5; 

// Tamanho máximo do buffer (1 slot para este exemplo pedagógico)
const int MAX_BUFFER_SIZE = 1; 

// --- Funções Auxiliares para o Buffer ---

// Função para 'colocar' um item no buffer
void put(int value) {
    buffer = value;
    count = 1; // Marca o buffer como cheio
    std::cout << "  [Produtor] Produziu: " << value << ". Buffer agora tem " << count << " item(s).\n";
}

// Função para 'pegar' um item do buffer
int get() {
    int tmp = buffer;
    count = 0; // Marca o buffer como vazio
    std::cout << "  [Consumidor] Consumiu: " << tmp << ". Buffer agora tem " << count << " item(s).\n";
    return tmp;
}

// --- Função da Thread Produtor ---
void producer_thread() {
    for (int i = 0; i < loops; ++i) {
        // 1. Cria um unique_lock que adquire o mutex ao ser construído
        std::unique_lock<std::mutex> lock(mtx);
        std::cout << "[Produtor] Tentando produzir item " << i << "...\n";

        // 2. Espera se o buffer estiver cheio (count == MAX_BUFFER_SIZE)
        // O `wait` com predicado (lambda) verifica a condição.
        // Se `count < MAX_BUFFER_SIZE` for falso, libera o lock e dorme.
        // Ao acordar, readquire o lock e reavalia o predicado.
        cv_empty.wait(lock, [&]{ 
            if (count == MAX_BUFFER_SIZE) {
                std::cout << "  [Produtor] Buffer cheio (" << count << "/" << MAX_BUFFER_SIZE << "). Aguardando 'cv_empty'...\n";
                return false; // Condição não atendida, dormir
            }
            std::cout << "  [Produtor] Acordou e re-verificou o buffer.\n";
            return true; // Condição atendida, continuar
        });

        // 3. Coloca o item no buffer
        put(i);
        
        // 4. Sinaliza que o buffer não está mais vazio (pode haver item para consumir)
        cv_fill.notify_one();
        std::cout << "[Produtor] Sinalizou 'cv_fill'. Liberando mutex.\n";

        // 5. O lock é liberado automaticamente quando `lock` sai de escopo
        // (ou explicitamente com lock.unlock() se necessário antes do fim da função).
        // Para este exemplo, deixaremos ele sair de escopo.
        
        // Pequena pausa para simular trabalho
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "[Produtor] Terminou de produzir todos os " << loops << " itens.\n";
}

// --- Função da Thread Consumidor ---
void consumer_thread() {
    for (int i = 0; i < loops; ++i) {
        // 1. Cria um unique_lock que adquire o mutex ao ser construído
        std::unique_lock<std::mutex> lock(mtx);
        std::cout << "    [Consumidor] Tentando consumir item...\n";

        // 2. Espera se o buffer estiver vazio (count == 0)
        cv_fill.wait(lock, [&]{ 
            if (count == 0) {
                std::cout << "      [Consumidor] Buffer vazio (" << count << "/" << MAX_BUFFER_SIZE << "). Aguardando 'cv_fill'...\n";
                return false; // Condição não atendida, dormir
            }
            std::cout << "      [Consumidor] Acordou e re-verificou o buffer.\n";
            return true; // Condição atendida, continuar
        });

        // 3. Pega o item do buffer
        int consumed_item = get();
        
        // 4. Sinaliza que o buffer não está mais cheio (pode haver espaço para produzir)
        cv_empty.notify_one();
        std::cout << "    [Consumidor] Sinalizou 'cv_empty'. Liberando mutex.\n";

        // 5. O lock é liberado automaticamente quando `lock` sai de escopo.
        
        // Pequena pausa para simular trabalho
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
    std::cout << "    [Consumidor] Terminou de consumir todos os " << loops << " itens.\n";
}

// --- Função Principal ---
int main() {
    std::cout << "--- Início do Problema Produtor-Consumidor (1 Produtor, 1 Consumidor) ---\n";
    std::cout << "Tamanho do Buffer: " << MAX_BUFFER_SIZE << " slot(s)\n";
    std::cout << "Itens a processar: " << loops << "\n\n";

    // As variáveis mtx, cv_empty e cv_fill são globais e já foram inicializadas pelos seus construtores padrão.
    // Não há necessidade de funções init/destroy explícitas como em pthreads.

    // Cria as threads produtor e consumidor
    std::thread producer_thr(producer_thread);
    std::thread consumer_thr(consumer_thread);

    // Espera as threads terminarem
    producer_thr.join();
    consumer_thr.join();

    // As variáveis mtx, cv_empty e cv_fill são globais e serão destruídas automaticamente.

    std::cout << "\n--- Fim do Problema Produtor-Consumidor ---\n";

    return 0;
}