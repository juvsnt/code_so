/* Neste código de exemplo, estamos simulando uma padaria, onde o padeiro (produtor) faz pães
continuamente, coloca pães na prateleira e espera quando a prateleira está cheia.
Já o cliente (consumidor) compra pães da prateleira e espera quando não há pães disponíveis.
A prateleira tem capacidade máxima de 5 pães e é compartilhada entre padeiro e cliente.
A sincronização usa apenas um mutex para controle de acesso e evita que padeiro e cliente
acessem a prateleira ao mesmo tempo*/

#include <windows.h>
#include <stdio.h>

#define MAX_PAES 5    // Tamanho máximo da prateleira

// Variáveis globais
int prateleira[MAX_PAES];  // Buffer compartilhado (prateleira de pães)
int quantidadePaes = 0;    // Quantidade atual de pães na prateleira
HANDLE mutex;              // Controla acesso à prateleira
BOOL continuar = TRUE;     // Controla execução do programa

// Padeiro (Produtor)
DWORD WINAPI padeiro(LPVOID lpParam) {
    int paoFeito = 1;

    while (continuar) {
        WaitForSingleObject(mutex, INFINITE);  // Obtém acesso à prateleira

        if (quantidadePaes < MAX_PAES) {
            prateleira[quantidadePaes] = paoFeito;
            printf("Padeiro: Fiz o pao %d\n", paoFeito);
            quantidadePaes++;
            paoFeito++;
        } else {
            printf("Padeiro: Prateleira cheia! Aguardando...\n");
        }

        ReleaseMutex(mutex);  // Libera acesso à prateleira
        Sleep(1000);          // Espera 1 segundo para fazer outro pão
    }
    return 0;
}

// Cliente (Consumidor)
DWORD WINAPI cliente(LPVOID lpParam) {
    while (continuar) {
        WaitForSingleObject(mutex, INFINITE);  // Obtém acesso à prateleira

        if (quantidadePaes > 0) {
            quantidadePaes--;
            printf("Cliente: Comprei o pao %d\n", prateleira[quantidadePaes]);
        } else {
            printf("Cliente: Prateleira vazia! Aguardando...\n");
        }

        ReleaseMutex(mutex);  // Libera acesso à prateleira
        Sleep(2000);          // Espera 2 segundos para comprar outro pão
    }
    return 0;
}

int main() {
    // Cria mutex para controle de acesso
    mutex = CreateMutex(NULL, FALSE, NULL);

    // Cria threads do padeiro e cliente
    HANDLE threadPadeiro = CreateThread(NULL, 0, padeiro, NULL, 0, NULL);
    HANDLE threadCliente = CreateThread(NULL, 0, cliente, NULL, 0, NULL);

    printf("Padaria aberta! Pressione Enter para fechar...\n");
    getchar();

    // Encerra o programa
    continuar = FALSE;
    Sleep(1000);

    // Fecha handles
    CloseHandle(threadPadeiro);
    CloseHandle(threadCliente);
    CloseHandle(mutex);

    return 0;
}