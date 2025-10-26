#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

using namespace std;

// Variáveis globais para a solução de Peterson
atomic<bool> flag[2]; // Flags para indicar intenção de entrar na região crítica
atomic<int> turn;     // Variável para indicar a vez de quem pode entrar na região crítica

// Função que simula a região crítica e a seção restante
void peterson(int id) {
    int other = 1 - id; // O outro processo

    for (int i = 0; i < 5; ++i) { // Executa 5 vezes para demonstração
        // Protocolo de entrada
        flag[id].store(true); // Define a flag do processo atual como true
        turn.store(other);    // Dá a vez ao outro processo
        while (flag[other].load() && turn.load() == other) {
            // Espera ocupada
        }

        // Região crítica
        cout << "Thread " << id << " esta na regiao critica." << endl;
        this_thread::sleep_for(chrono::milliseconds(500)); // Simula trabalho na região crítica

        // Protocolo de saída
        flag[id].store(false); // Libera a região crítica

        // Seção restante
        cout << "Thread " << id << " esta na secao restante." << endl;
        this_thread::sleep_for(chrono::milliseconds(500)); // Simula trabalho na seção restante
    }
}

int main() {
    // Inicializa as variáveis globais
    flag[0] = false;
    flag[1] = false;
    turn = 0;

    // Criação de dois threads
    thread t1(peterson, 0);
    thread t2(peterson, 1);

    // Aguarda os threads terminarem
    t1.join();
    t2.join();

    return 0;
}