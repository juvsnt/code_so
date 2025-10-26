#include <iostream>
#include <thread>
#include <chrono>

// Função que simula atender uma requisição
void atender(int id) {
    std::cout << "Atendendo cliente " << id << "...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2)); // espera simulada
    std::cout << "Cliente " << id << " atendido!\n";
}

int main() {
    auto inicio = std::chrono::high_resolution_clock::now();

    // --- Modo sequencial ---
    atender(1);
    atender(2);

    auto meio = std::chrono::high_resolution_clock::now();

    // --- Modo paralelo ---
    std::thread t1(atender, 1);
    std::thread t2(atender, 2);
    t1.join();
    t2.join();

    auto fim = std::chrono::high_resolution_clock::now();

    auto tempoSeq = std::chrono::duration_cast<std::chrono::seconds>(meio - inicio).count();
    auto tempoPar = std::chrono::duration_cast<std::chrono::seconds>(fim - meio).count();

    std::cout << "\nTempo sequencial: " << tempoSeq << "s\n";
    std::cout << "Tempo paralelo:   " << tempoPar << "s\n";
}
