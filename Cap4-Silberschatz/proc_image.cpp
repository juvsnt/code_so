#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

// Função que simula processamento pesado em um pedaço do vetor
void processa(std::vector<int>& dados, int inicio, int fim) {
    for (int i = inicio; i < fim; i++) {
        dados[i] = dados[i] * 2; // Exemplo simples: "filtro"
    }
}

int main() {
    const int N = 50'000'000; // 50 milhões de "pixels"
    std::vector<int> dados(N, 1);

    auto inicio = std::chrono::high_resolution_clock::now();

    // --- Modo sequencial (1 thread) ---
    processa(dados, 0, N);

    auto meio = std::chrono::high_resolution_clock::now();

    // --- Modo paralelo (2 threads) ---
    std::thread t1(processa, std::ref(dados), 0, N/2);
    std::thread t2(processa, std::ref(dados), N/2, N);
    t1.join();
    t2.join();

    auto fim = std::chrono::high_resolution_clock::now();

    auto tempoSeq = std::chrono::duration_cast<std::chrono::milliseconds>(meio - inicio).count();
    auto tempoPar = std::chrono::duration_cast<std::chrono::milliseconds>(fim - meio).count();

    std::cout << "Tempo sequencial: " << tempoSeq << " ms\n";
    std::cout << "Tempo paralelo:   " << tempoPar << " ms\n";
}
