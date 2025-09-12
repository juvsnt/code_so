/*
 =========================================================================
  Demonstracao Didatica de Hierarquia de Memoria em C++

  Objetivo:
  - Comparar tempos de acesso a diferentes niveis da hierarquia:
    * Registradores (variavel simples em loop)
    * Cache (vetor pequeno, acessos repetidos)
    * RAM (vetor grande, acessos dispersos)
    * Disco (leitura/escrita em arquivo)

  Conceitos de Sistemas Operacionais e Arquitetura:
  - Acesso mais rapido: registradores < cache < RAM < disco
  - Principio da localidade (espacial e temporal)
  - Impacto direto no desempenho de programas
 =========================================================================
*/

#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <random>

using clock_type = std::chrono::high_resolution_clock;

int main() {
    std::cout << "=== DEMO HIERARQUIA DE MEMORIA ===\n";

    // ---------------- REGISTRADORES ----------------
    {
        auto start = clock_type::now();
        volatile long long soma = 0; // volatile para evitar otimizacao
        for (long long i = 0; i < 100000000; i++) {
            soma += i;
        }
        auto end = clock_type::now();
        double tempo = std::chrono::duration<double>(end - start).count();
        std::cout << "Tempo (registradores): " << tempo << " segundos\n";
    }

    // ---------------- CACHE (vetor pequeno) ----------------
    {
        const int N = 10000; // ~40 KB (cabe em cache L1)
        std::vector<int> v(N, 1);
        long long soma = 0;
        auto start = clock_type::now();
        for (int rep = 0; rep < 10000; rep++) {
            for (int i = 0; i < N; i++) soma += v[i];
        }
        auto end = clock_type::now();
        double tempo = std::chrono::duration<double>(end - start).count();
        std::cout << "Tempo (cache L1/L2 - vetor pequeno): " << tempo << " segundos\n";
    }

    // ---------------- RAM (vetor grande) ----------------
    {
        const int N = 50000000; // ~200 MB (nao cabe em cache)
        std::vector<int> v(N, 1);
        long long soma = 0;

        // acesso com "saltos" para evitar cache-friendly
        auto start = clock_type::now();
        for (int i = 0; i < N; i += 1024) soma += v[i];
        auto end = clock_type::now();

        double tempo = std::chrono::duration<double>(end - start).count();
        std::cout << "Tempo (RAM - vetor grande): " << tempo << " segundos\n";
    }

    // ---------------- DISCO (arquivo) ----------------
    {
        const int N = 10000000; // ~10 MB
        std::vector<char> buffer(N, 'A');

        auto start = clock_type::now();

        // escrita
        {
            std::ofstream f("teste_disco.bin", std::ios::binary);
            f.write(buffer.data(), buffer.size());
        }

        // leitura
        {
            std::ifstream f("teste_disco.bin", std::ios::binary);
            std::vector<char> buffer2(N);
            f.read(buffer2.data(), buffer2.size());
        }

        auto end = clock_type::now();
        double tempo = std::chrono::duration<double>(end - start).count();
        std::cout << "Tempo (disco rigido/SSD - arquivo 10MB): " << tempo << " segundos\n";
    }

    std::cout << "\nFim da demonstracao.\n";
    return 0;
}

/*
 ========================================================================
  Fim do programa:
  - Cada nivel da hierarquia de memoria apresenta um tempo de acesso
    crescente: registradores < cache < RAM < disco.
  - Mostra como o desempenho de programas depende da localidade e do
    gerenciamento eficiente da memoria pelo sistema operacional.
 ========================================================================
*/
