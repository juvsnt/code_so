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
    createFile("batch_file1.txt");
    writeFile("batch_file1.txt", "Adicionando mais conteudo ao batch_file1");
    createFile("batch_file2.txt");
    writeFile("batch_file2.txt", "Conteudo do batch_file2");
}

int main() {
    batchProcess();
    printf("Processamento em lote concluido.\n");
    return 0;
}
