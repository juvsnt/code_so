/*
Programa em C++ que realiza as seguintes operações:
-  Atividade nome: at01_exercicio_syscall.pdf
- Cria um arquivo.
- Escreve uma mensagem nesse arquivo.
- Lê o conteúdo do arquivo e o exibe na tela.
- Fecha o arquivo.
*/

#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::string filename = "exemplo2.txt";
    std::string mensagem = "Ola, System Calls! Exemplo 2\n";

    // 1. Criação e escrita no arquivo
    std::ofstream outfile(filename);
    if (!outfile) {
        std::cerr << "Erro ao criar o arquivo\n";
        return 1;
    }
    outfile << mensagem;
    outfile.close();

    // 2. Leitura do arquivo
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Erro ao abrir o arquivo\n";
        return 1;
    }
    std::string conteudo((std::istreambuf_iterator<char>(infile)),
                          std::istreambuf_iterator<char>());
    infile.close();

    // 3. Exibir o conteúdo lido
    std::cout << "Conteudo do arquivo:\n" << conteudo;
}