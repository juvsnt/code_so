/*
=============================================================================
SISTEMA DE BIBLIOTECA DIGITAL - DEMONSTRAÇÃO DE SYSTEM CALLS EM C
=============================================================================

DESCRIÇÃO:
Sistema básico de gerenciamento de biblioteca que demonstra o uso DIRETO
de system calls do Windows para manipulação de arquivos.

FUNCIONALIDADES:
✓ Cadastro de livros no catálogo
✓ Listagem de todos os livros
✓ Busca de livros por título
✓ Controle de empréstimos
✓ Relatório de status da biblioteca

SYSTEM CALLS UTILIZADAS:
- CreateFile: Criação e abertura de arquivos
- WriteFile: Escrita de dados no catálogo
- ReadFile: Leitura do catálogo para consultas
- CloseHandle: Fechamento de recursos
- SetFilePointer: Navegação no arquivo

ARQUIVOS UTILIZADOS:
- biblioteca.txt: Catálogo principal de livros
- emprestimos.txt: Registro de empréstimos
- relatorio.txt: Relatório de status

AUTOR: Juvenal + Claude
DISCIPLINA: Sistemas Operacionais
=============================================================================
*/

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TITULO 100
#define MAX_AUTOR 100
#define MAX_ISBN 20
#define MAX_STATUS 20
#define MAX_LINHA 300
#define MAX_LIVROS 1000

// Estrutura para representar um livro
typedef struct {
    int id;
    char titulo[MAX_TITULO];
    char autor[MAX_AUTOR];
    char isbn[MAX_ISBN];
    char status[MAX_STATUS]; // "Disponivel" ou "Emprestado"
} Livro;

// Variáveis globais para nomes dos arquivos
const char* ARQUIVO_CATALOGO = "biblioteca.txt";
const char* ARQUIVO_EMPRESTIMOS = "emprestimos.txt";
const char* ARQUIVO_RELATORIO = "relatorio.txt";

// Protótipos das funções
void cadastrarLivro();
void listarLivros();
void buscarLivro();
void emprestarLivro();
void gerarRelatorio();
void exibirMenu();
void tratarErro(const char* operacao);
int lerLivrosDoArquivo(Livro livros[], int maxLivros);
void salvarLivrosNoArquivo(Livro livros[], int totalLivros);
void registrarEmprestimo(Livro* livro);

// 1. SYSTEM CALL: CreateFile + WriteFile - Cadastro de livro
void cadastrarLivro() {
    printf("\n=== CADASTRO DE LIVRO ===\n");
    
    Livro novoLivro;
    char linha[MAX_LINHA];
    DWORD bytesEscritos;
    
    // Entrada de dados
    printf("ID do livro: ");
    scanf("%d", &novoLivro.id);
    getchar(); // Limpar buffer
    
    printf("Titulo: ");
    fgets(novoLivro.titulo, MAX_TITULO, stdin);
    novoLivro.titulo[strcspn(novoLivro.titulo, "\n")] = 0; // Remove \n
    
    printf("Autor: ");
    fgets(novoLivro.autor, MAX_AUTOR, stdin);
    novoLivro.autor[strcspn(novoLivro.autor, "\n")] = 0;
    
    printf("ISBN: ");
    fgets(novoLivro.isbn, MAX_ISBN, stdin);
    novoLivro.isbn[strcspn(novoLivro.isbn, "\n")] = 0;
    
    strcpy(novoLivro.status, "Disponivel");
    
    // SYSTEM CALL: CreateFile - Abertura de arquivo para escrita (append)
    HANDLE hArquivo = CreateFile(
        ARQUIVO_CATALOGO,           // Nome do arquivo
        GENERIC_WRITE,              // Acesso de escrita
        FILE_SHARE_READ,            // Compartilhamento de leitura
        NULL,                       // Segurança padrão
        OPEN_ALWAYS,                // Abre existente ou cria novo
        FILE_ATTRIBUTE_NORMAL,      // Atributos normais
        NULL                        // Sem template
    );
    
    if (hArquivo == INVALID_HANDLE_VALUE) {
        tratarErro("criar/abrir arquivo do catalogo");
        return;
    }
    
    // Posicionar no final do arquivo para append
    SetFilePointer(hArquivo, 0, NULL, FILE_END);
    
    // Formatar linha para escrita
    sprintf(linha, "%d|%s|%s|%s|%s\n", 
            novoLivro.id, novoLivro.titulo, novoLivro.autor, 
            novoLivro.isbn, novoLivro.status);
    
    // SYSTEM CALL: WriteFile - Escrita de dados
    BOOL sucesso = WriteFile(
        hArquivo,                   // Handle do arquivo
        linha,                      // Buffer de dados
        strlen(linha),              // Número de bytes
        &bytesEscritos,             // Bytes escritos
        NULL                        // Sem overlap
    );
    
    if (!sucesso) {
        tratarErro("escrever no arquivo");
        CloseHandle(hArquivo);
        return;
    }
    
    // SYSTEM CALL: CloseHandle - Fechamento do arquivo
    CloseHandle(hArquivo);
    
    printf("✓ Livro cadastrado com sucesso! (%lu bytes escritos)\n", bytesEscritos);
}

// 2. SYSTEM CALL: CreateFile + ReadFile - Listagem de livros
void listarLivros() {
    printf("\n=== CATALOGO DA BIBLIOTECA ===\n");
    
    // SYSTEM CALL: CreateFile - Abertura para leitura
    HANDLE hArquivo = CreateFile(
        ARQUIVO_CATALOGO,           // Nome do arquivo
        GENERIC_READ,               // Acesso de leitura
        FILE_SHARE_READ,            // Compartilhamento
        NULL,                       // Segurança padrão
        OPEN_EXISTING,              // Abre arquivo existente
        FILE_ATTRIBUTE_NORMAL,      // Atributos normais
        NULL                        // Sem template
    );
    
    if (hArquivo == INVALID_HANDLE_VALUE) {
        printf("Arquivo do catalogo nao encontrado ou vazio.\n");
        return;
    }
    
    // Obter tamanho do arquivo
    DWORD tamanhoArquivo = GetFileSize(hArquivo, NULL);
    if (tamanhoArquivo == 0) {
        printf("Catalogo vazio.\n");
        CloseHandle(hArquivo);
        return;
    }
    
    // Alocar buffer para leitura
    char* buffer = (char*)malloc(tamanhoArquivo + 1);
    if (!buffer) {
        printf("Erro de memoria.\n");
        CloseHandle(hArquivo);
        return;
    }
    
    DWORD bytesLidos;
    
    // SYSTEM CALL: ReadFile - Leitura do arquivo completo
    BOOL sucesso = ReadFile(
        hArquivo,                   // Handle do arquivo
        buffer,                     // Buffer de leitura
        tamanhoArquivo,             // Bytes a ler
        &bytesLidos,                // Bytes lidos
        NULL                        // Sem overlap
    );
    
    if (!sucesso) {
        tratarErro("ler arquivo");
        free(buffer);
        CloseHandle(hArquivo);
        return;
    }
    
    buffer[bytesLidos] = '\0'; // Terminar string
    
    // SYSTEM CALL: CloseHandle
    CloseHandle(hArquivo);
    
    // Processar e exibir dados
    printf("ID\t| Titulo\t\t\t| Autor\t\t\t| ISBN\t\t| Status\n");
    printf("--------------------------------------------------------------------------------\n");
    
    char* linha = strtok(buffer, "\n");
    int contador = 0;
    
    while (linha != NULL) {
        if (strlen(linha) > 0) {
            Livro livro;
            sscanf(linha, "%d|%99[^|]|%99[^|]|%19[^|]|%19s", 
                   &livro.id, livro.titulo, livro.autor, livro.isbn, livro.status);
            
            printf("%d\t| %-25s\t| %-20s\t| %-12s\t| %s\n", 
                   livro.id, livro.titulo, livro.autor, livro.isbn, livro.status);
            contador++;
        }
        linha = strtok(NULL, "\n");
    }
    
    printf("\nTotal de livros: %d (Lidos %lu bytes)\n", contador, bytesLidos);
    free(buffer);
}

// 3. SYSTEM CALL: CreateFile + ReadFile - Busca de livros
void buscarLivro() {
    printf("\n=== BUSCAR LIVRO ===\n");
    printf("Digite o titulo (ou parte dele): ");
    
    char termoBusca[MAX_TITULO];
    fgets(termoBusca, MAX_TITULO, stdin);
    termoBusca[strcspn(termoBusca, "\n")] = 0;
    
    // SYSTEM CALL: CreateFile para leitura
    HANDLE hArquivo = CreateFile(
        ARQUIVO_CATALOGO,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    
    if (hArquivo == INVALID_HANDLE_VALUE) {
        printf("Arquivo do catalogo não encontrado.\n");
        return;
    }
    
    DWORD tamanhoArquivo = GetFileSize(hArquivo, NULL);
    char* buffer = (char*)malloc(tamanhoArquivo + 1);
    DWORD bytesLidos;
    
    // SYSTEM CALL: ReadFile
    ReadFile(hArquivo, buffer, tamanhoArquivo, &bytesLidos, NULL);
    buffer[bytesLidos] = '\0';
    
    // SYSTEM CALL: CloseHandle
    CloseHandle(hArquivo);
    
    printf("\nResultados da busca:\n");
    printf("----------------------------------------------------------------\n");
    
    char* linha = strtok(buffer, "\n");
    int encontrados = 0;
    
    while (linha != NULL) {
        if (strlen(linha) > 0) {
            Livro livro;
            sscanf(linha, "%d|%99[^|]|%99[^|]|%19[^|]|%19s", 
                   &livro.id, livro.titulo, livro.autor, livro.isbn, livro.status);
            
            // Busca case-insensitive simples
            char tituloLower[MAX_TITULO], buscaLower[MAX_TITULO];
            strcpy(tituloLower, livro.titulo);
            strcpy(buscaLower, termoBusca);
            
            // Converter para minúsculas
            for(int i = 0; tituloLower[i]; i++) {
                tituloLower[i] = tolower(tituloLower[i]);
            }
            for(int i = 0; buscaLower[i]; i++) {
                buscaLower[i] = tolower(buscaLower[i]);
            }
            
            if (strstr(tituloLower, buscaLower) != NULL) {
                printf("ID: %d | %s | %s | %s\n", 
                       livro.id, livro.titulo, livro.autor, livro.status);
                encontrados++;
            }
        }
        linha = strtok(NULL, "\n");
    }
    
    if (encontrados == 0) {
        printf("Nenhum livro encontrado com esse titulo.\n");
    } else {
        printf("\n%d livro(s) encontrado(s).\n", encontrados);
    }
    
    free(buffer);
}

// 4. SYSTEM CALL: Leitura, modificação e escrita - Empréstimo
void emprestarLivro() {
    printf("\n=== EMPRESTIMO DE LIVRO ===\n");
    printf("Digite o ID do livro: ");
    
    int idLivro;
    scanf("%d", &idLivro);
    
    Livro livros[MAX_LIVROS];
    int totalLivros = lerLivrosDoArquivo(livros, MAX_LIVROS);
    
    if (totalLivros == -1) {
        printf("Erro ao ler arquivo do catalogo.\n");
        return;
    }
    
    int encontrou = 0;
    for (int i = 0; i < totalLivros; i++) {
        if (livros[i].id == idLivro) {
            if (strcmp(livros[i].status, "Disponivel") == 0) {
                strcpy(livros[i].status, "Emprestado");
                encontrou = 1;
                
                // Registrar empréstimo
                registrarEmprestimo(&livros[i]);
                
                printf("✓ Livro '%s' emprestado com sucesso!\n", livros[i].titulo);
            } else {
                printf("✗ Livro ja esta emprestado!\n");
                return;
            }
            break;
        }
    }
    
    if (!encontrou) {
        printf("✗ Livro nao encontrado!\n");
        return;
    }
    
    // Salvar mudanças no arquivo
    salvarLivrosNoArquivo(livros, totalLivros);
}

// 5. SYSTEM CALL: CreateFile + WriteFile - Geração de relatório
void gerarRelatorio() {
    printf("\n=== GERANDO RELATORIO ===\n");
    
    Livro livros[MAX_LIVROS];
    int totalLivros = lerLivrosDoArquivo(livros, MAX_LIVROS);
    
    if (totalLivros == -1) {
        printf("Erro ao ler catalogo.\n");
        return;
    }
    
    int disponiveis = 0, emprestados = 0;
    for (int i = 0; i < totalLivros; i++) {
        if (strcmp(livros[i].status, "Disponivel") == 0) {
            disponiveis++;
        } else {
            emprestados++;
        }
    }
    
    // SYSTEM CALL: CreateFile para relatório
    HANDLE hRelatorio = CreateFile(
        ARQUIVO_RELATORIO,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,              // Sempre criar novo
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    
    if (hRelatorio == INVALID_HANDLE_VALUE) {
        tratarErro("criar arquivo de relatorio");
        return;
    }
    
    char relatorio[2000];
    DWORD bytesEscritos;
    
    // Montar relatório
    sprintf(relatorio, 
        "=== RELATORIO DA BIBLIOTECA DIGITAL ===\n\n"
        "Total de livros: %d\n"
        "Livros disponiveis: %d\n"
        "Livros emprestados: %d\n"
        "Taxa de ocupacao: %.1f%%\n\n"
        "=== DETALHES DOS LIVROS ===\n",
        totalLivros, disponiveis, emprestados,
        totalLivros > 0 ? (emprestados * 100.0 / totalLivros) : 0.0
    );
    
    // SYSTEM CALL: WriteFile - Escrever cabeçalho
    WriteFile(hRelatorio, relatorio, strlen(relatorio), &bytesEscritos, NULL);
    
    // Escrever detalhes dos livros
    for (int i = 0; i < totalLivros; i++) {
        char linha[MAX_LINHA];
        sprintf(linha, "ID: %d | %s | %s | %s\n", 
                livros[i].id, livros[i].titulo, livros[i].autor, livros[i].status);
        
        // SYSTEM CALL: WriteFile para cada livro
        WriteFile(hRelatorio, linha, strlen(linha), &bytesEscritos, NULL);
    }
    
    // SYSTEM CALL: CloseHandle
    CloseHandle(hRelatorio);
    
    printf("✓ Relatorio gerado em '%s'\n", ARQUIVO_RELATORIO);
    printf("\n=== RESUMO DO RELATORIO ===\n");
    printf("Total de livros: %d\n", totalLivros);
    printf("Disponiveis: %d | Emprestados: %d\n", disponiveis, emprestados);
}

// Função auxiliar para ler livros do arquivo
int lerLivrosDoArquivo(Livro livros[], int maxLivros) {
    HANDLE hArquivo = CreateFile(
        ARQUIVO_CATALOGO,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    
    if (hArquivo == INVALID_HANDLE_VALUE) {
        return -1;
    }
    
    DWORD tamanhoArquivo = GetFileSize(hArquivo, NULL);
    char* buffer = (char*)malloc(tamanhoArquivo + 1);
    DWORD bytesLidos;
    
    ReadFile(hArquivo, buffer, tamanhoArquivo, &bytesLidos, NULL);
    buffer[bytesLidos] = '\0';
    CloseHandle(hArquivo);
    
    int contador = 0;
    char* linha = strtok(buffer, "\n");
    
    while (linha != NULL && contador < maxLivros) {
        if (strlen(linha) > 0) {
            sscanf(linha, "%d|%99[^|]|%99[^|]|%19[^|]|%19s", 
                   &livros[contador].id, livros[contador].titulo, 
                   livros[contador].autor, livros[contador].isbn, 
                   livros[contador].status);
            contador++;
        }
        linha = strtok(NULL, "\n");
    }
    
    free(buffer);
    return contador;
}

// Função auxiliar para salvar livros no arquivo
void salvarLivrosNoArquivo(Livro livros[], int totalLivros) {
    HANDLE hArquivo = CreateFile(
        ARQUIVO_CATALOGO,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    
    if (hArquivo == INVALID_HANDLE_VALUE) {
        tratarErro("salvar dados");
        return;
    }
    
    for (int i = 0; i < totalLivros; i++) {
        char linha[MAX_LINHA];
        sprintf(linha, "%d|%s|%s|%s|%s\n", 
                livros[i].id, livros[i].titulo, livros[i].autor, 
                livros[i].isbn, livros[i].status);
        
        DWORD bytesEscritos;
        WriteFile(hArquivo, linha, strlen(linha), &bytesEscritos, NULL);
    }
    
    CloseHandle(hArquivo);
}

// Função auxiliar para registrar empréstimo
void registrarEmprestimo(Livro* livro) {
    HANDLE hArquivo = CreateFile(
        ARQUIVO_EMPRESTIMOS,
        GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    
    if (hArquivo != INVALID_HANDLE_VALUE) {
        SetFilePointer(hArquivo, 0, NULL, FILE_END);
        
        char linha[MAX_LINHA];
        sprintf(linha, "Emprestimo - ID: %d | Titulo: %s | Data: [atual]\n", 
                livro->id, livro->titulo);
        
        DWORD bytesEscritos;
        WriteFile(hArquivo, linha, strlen(linha), &bytesEscritos, NULL);
        CloseHandle(hArquivo);
    }
}

// Função para tratar erros
void tratarErro(const char* operacao) {
    DWORD erro = GetLastError();
    printf("ERRO ao %s. Codigo: %lu\n", operacao, erro);
}

// Função para exibir menu
void exibirMenu() {
    printf("\n=== MENU PRINCIPAL ===\n");
    printf("1. Cadastrar Livro\n");
    printf("2. Listar Todos os Livros\n");
    printf("3. Buscar Livro\n");
    printf("4. Emprestar Livro\n");
    printf("5. Gerar Relatorio\n");
    printf("0. Sair\n");
    printf("Escolha uma opcao: ");
}

// Função principal
int main() {
    int opcao;
    
    printf("🏛️  SISTEMA DE BIBLIOTECA DIGITAL 📚\n");
    printf("=====================================\n");
    printf("Demonstracao de System Calls em C\n");
    
    do {
        exibirMenu();
        scanf("%d", &opcao);
        getchar(); // Limpar buffer
        
        switch (opcao) {
            case 1:
                cadastrarLivro();
                break;
            case 2:
                listarLivros();
                break;
            case 3:
                buscarLivro();
                break;
            case 4:
                emprestarLivro();
                break;
            case 5:
                gerarRelatorio();
                break;
            case 0:
                printf("👋 Encerrando sistema... Ate logo!\n");
                break;
            default:
                printf("✗ Opcao invalida! Tente novamente.\n");
        }
        
    } while (opcao != 0);
    
    return 0;
}