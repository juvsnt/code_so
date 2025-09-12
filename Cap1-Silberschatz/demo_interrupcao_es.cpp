/*
 =========================================================================
  Demonstracao Didatica de Entrada/Saida Sincrona e Assincrona em C++

  - E/S Sincrona: CPU fica bloqueada aguardando o termino da operacao.
  - E/S Assincrona: a operacao ocorre em paralelo, permitindo que a CPU
    continue executando outras instrucoes.

  Conceitos de Sistemas Operacionais:
  - Bloqueio vs. nao-bloqueio
  - Melhor aproveitamento da CPU em operacoes de I/O
  - Comparacao de tempo entre as duas abordagens
 =========================================================================
*/

#include <iostream>
#include <thread>
#include <chrono>
#include <string>

// Simulacao de uma operacao de E/S "pesada"
void operacao_ES(const std::string& nome, int segundos) {
    std::cout << "[E/S] Iniciando operacao em " << nome << "...\n";
    std::this_thread::sleep_for(std::chrono::seconds(segundos));
    std::cout << "[E/S] Operacao em " << nome << " concluida!\n";
}

int main() {
    using clock = std::chrono::high_resolution_clock;

    // ---------------- E/S SINCRONA ----------------
    std::cout << "\n=== DEMO DE E/S SINCRONA ===\n";
    auto start_sync = clock::now();

    std::cout << "CPU solicita leitura de arquivo...\n";
    operacao_ES("ArquivoGrande.txt", 5); // CPU bloqueada
    std::cout << "CPU retoma apenas apos o termino da E/S.\n";

    auto end_sync = clock::now();
    auto dur_sync = std::chrono::duration<double>(end_sync - start_sync).count();
    std::cout << "Tempo total (sincrono): " << dur_sync << " segundos.\n";

    // ---------------- E/S ASSINCRONA ----------------
    std::cout << "\n=== DEMO DE E/S ASSINCRONA ===\n";
    auto start_async = clock::now();

    std::cout << "CPU solicita leitura de arquivo...\n";
    std::thread t(operacao_ES, "ArquivoGrande.txt", 5);

    for (int i = 1; i <= 5; i++) {
        std::cout << "[CPU] Executando outra tarefa (" << i << ")\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    t.join();
    auto end_async = clock::now();
    auto dur_async = std::chrono::duration<double>(end_async - start_async).count();
    std::cout << "Tempo total (assincrono): " << dur_async << " segundos.\n";

    std::cout << "\nFim da demonstracao.\n";
    return 0;
}

/*
 ========================================================================
  Fim do programa:
  - Na E/S sincrona, a CPU fica bloqueada durante toda a operacao.
  - Na E/S assincrona, CPU e E/S ocorrem em paralelo, resultando em menor
    tempo de espera total e melhor aproveitamento do sistema.
 ========================================================================
*/
