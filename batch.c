/*programa batch simples em linguagem C que executa uma série de operações, 
como criação de arquivos e escrita de dados nesses arquivos.*/

#include <stdio.h>
#include <stdlib.h>

void createFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erro ao criar o arquivo %s\n", filename);
        return;
    }
    fprintf(file, "Conteúdo inicial do arquivo %s\n", filename);
    fclose(file);
    printf("Arquivo %s criado com sucesso.\n", filename);
}

void writeFile(const char *filename, const char *content) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", filename);
        return;
    }
    fprintf(file, "%s\n", content);
    fclose(file);
    printf("Conteudo escrito no arquivo %s\n", filename);
}

void batchProcess() {
    createFile("arquivo1.txt");
    writeFile("arquivo1.txt", "Adicionando mais conteudo ao arquivo 1");
    createFile("arquivo2.txt");
    writeFile("arquivo2.txt", "Conteudo do arquivo 2");
}

int main() {
    batchProcess();
    printf("Processamento em lote concluido.\n");
    return 0;
}
