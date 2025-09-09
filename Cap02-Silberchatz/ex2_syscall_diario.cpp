/*
Este programa demonstra operações básicas de manipulação de arquivos usando
streams da biblioteca padrão C++. Simula a criação e leitura de um diário
digital através de operações de I/O em arquivo. O objetivo é demonstrar como 
streams C++ abstraem chamadas de sistema complexas,fornecendo uma interface 
simples e segura para operações de I/O.
*/

#include <iostream>
#include <fstream>
#include <string>

int main() {
    // 1. Criar um "canal" para escrever no diário
    std::ofstream meuDiario("diario.txt");
    
    // 2. Escrever no diário
    meuDiario << "Hoje foi um bom dia!";
    meuDiario << "\nPratiquei sobre SysCalls e streams em C++";
    
    // 3. Fechar o diário
    meuDiario.close();
    
    // 4. Abrir o diário para ler
    std::ifstream lerDiario("diario.txt");
    std::string linha;
    
    // 5. Ler e mostrar na tela
    std::cout << "Meu diario diz:\n";
    while (std::getline(lerDiario, linha)) {
        std::cout << linha << std::endl;
    }
    
    return 0;
}