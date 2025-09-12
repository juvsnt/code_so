/*copia o conteúdo de um arquivo de origem para um arquivo de destino, usando a API POSIX. Este programa irá solicitar 
ao usuário os nomes dos arquivos de origem e destino, e incluirá verificação de erros para assegurar que o arquivo de 
origem exista e que os dados sejam copiados corretamente.
*/


#include <stdio.h>
#include <stdlib.h>

void copyFile(const char *sourceFile, const char *destFile) {
    FILE *source, *dest;
    char buffer[BUFSIZ];
    size_t n;

    // Abrir arquivo de origem
    source = fopen(sourceFile, "rb");
    if (source == NULL) {
        perror("Erro ao abrir arquivo de origem");
        exit(EXIT_FAILURE);
    }

    // Abrir arquivo de destino
    dest = fopen(destFile, "wb");
    if (dest == NULL) {
        perror("Erro ao abrir arquivo de destino");
        fclose(source);
        exit(EXIT_FAILURE);
    }

    // Copiar conteúdo
    while ((n = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        if (fwrite(buffer, 1, n, dest) != n) {
            perror("Erro ao escrever no arquivo de destino");
            fclose(source);
            fclose(dest);
            exit(EXIT_FAILURE);
        }
    }

    // Verificar erros na leitura
    if (ferror(source)) {
        perror("Erro ao ler o arquivo de origem");
    }

    printf("Arquivo copiado com sucesso!\n");

    // Fechar arquivos
    fclose(source);
    fclose(dest);
}

int main() {
    char sourceFile[256];
    char destFile[256];

    // Solicitar nomes dos arquivos ao usuário
    printf("Digite o nome do arquivo de origem: ");
    scanf("%255s", sourceFile);
    printf("Digite o nome do arquivo de destino: ");
    scanf("%255s", destFile);

    // Copiar o arquivo
    copyFile(sourceFile, destFile);

    return 0;
}
