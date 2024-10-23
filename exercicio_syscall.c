/*
Programa em C que realiza as seguintes operações utilizando chamadas de sistema:
-Cria um arquivo.
-Escreve uma mensagem nesse arquivo.
-Lê o conteúdo do arquivo e o exibe na tela.
-Fecha o arquivo.
*/

#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hFile;
    DWORD dwBytesWritten = 0;
    DWORD dwBytesRead = 0;
    char buffer[100];
    BOOL bSuccess = FALSE;

    // 1. Criação do Arquivo
    hFile = CreateFile("exemplo.txt",          // Nome do arquivo
                       GENERIC_WRITE,          // Acesso de escrita
                       0,                      // Compartilhamento
                       NULL,                   // Segurança padrão
                       CREATE_ALWAYS,          // Cria novo arquivo sempre
                       FILE_ATTRIBUTE_NORMAL,  // Atributos e flags
                       NULL);                  // Sem modelos

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Erro ao criar o arquivo\n");
        return 1;
    }

    // 2. Escrita no Arquivo
    bSuccess = WriteFile(hFile, "Ola, mundo!\n", 13, &dwBytesWritten, NULL);
    if (!bSuccess) {
        printf("Erro ao escrever no arquivo\n");
        CloseHandle(hFile);
        return 1;
    }

    // Fechar o arquivo após a escrita
    CloseHandle(hFile);

    // 3. Leitura do Arquivo
    hFile = CreateFile("exemplo.txt",          // Nome do arquivo
                       GENERIC_READ,           // Acesso de leitura
                       0,                      // Compartilhamento
                       NULL,                   // Segurança padrão
                       OPEN_EXISTING,          // Abre o arquivo existente
                       FILE_ATTRIBUTE_NORMAL,  // Atributos e flags
                       NULL);                  // Sem modelos

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    bSuccess = ReadFile(hFile, buffer, sizeof(buffer) - 1, &dwBytesRead, NULL);
    if (!bSuccess) {
        printf("Erro ao ler do arquivo\n");
        CloseHandle(hFile);
        return 1;
    }

    // Adicionar o terminador nulo para exibir o texto
    buffer[dwBytesRead] = '\0';

    // Exibir o conteúdo lido
    printf("Conteudo do arquivo:\n%s", buffer);

    // 4. Fechar o Arquivo
    CloseHandle(hFile);

    return 0;
}
