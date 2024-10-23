#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hFile;
    DWORD dwBytesRead = 0;
    char buffer[1024];
    BOOL bSuccess = FALSE;

    hFile = CreateFile("exemplo.txt",          // Nome do arquivo
                       GENERIC_READ,           // Acesso de leitura
                       0,                      // Compartilhamento
                       NULL,                   // Segurança padrão
                       OPEN_EXISTING,          // Abrir apenas se existir
                       FILE_ATTRIBUTE_NORMAL,  // Atributos e flags
                       NULL);                  // Sem modelos

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    bSuccess = ReadFile(hFile, buffer, sizeof(buffer) - 1, &dwBytesRead, NULL);

    if (!bSuccess) {
        printf("Erro ao ler o arquivo\n");
        CloseHandle(hFile);
        return 1;
    }

    buffer[dwBytesRead] = '\0';  // Adiciona um terminador nulo ao final dos dados lidos
    printf("Conteudo lido:\n%s\n", buffer);

    CloseHandle(hFile);
    return 0;
}
