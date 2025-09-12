/*Neste código de exemplo (slide 21), O processo pai apenas cria o filho, espera ele terminar e então exibe a mensagem. 
O processo filho executa o comando "dir" e termina. Assim, tudo o que acontece dentro do comando "dir" é feito pelo processo filho, 
e o resto é pelo pai
O comando CreateProcess cria um novo processo. O processo filho executa o comando cmd.exe /c dir 
(ou seja, ele lista os arquivos do diretório atual). O processo pai usa WaitForSingleObject para esperar o processo filho terminar.
Depois que o filho termina, o pai imprime "Filho Completou Execução".
*/
#include <windows.h>
#include <stdio.h>

int main() {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // Inicializa estruturas de STARTUPINFO e PROCESS_INFORMATION
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Criação do Processo
    if (!CreateProcess(NULL,           // Nome do módulo (use NULL para o módulo atual)
                       "C:\\Windows\\System32\\cmd.exe /c dir", // Linha de comando (use o caminho completo do executável)
                       NULL,           // Atributos de segurança do processo
                       NULL,           // Atributos de segurança do thread
                       FALSE,          // Herda os handles do processo pai?
                       0,              // Flags de criação
                       NULL,           // Ambiente do novo processo
                       NULL,           // Diretório atual do novo processo
                       &si,            // Informações da startup
                       &pi)            // Informações sobre o processo criado
        ) {
        fprintf(stderr, "Criação do processo falhou (%d).\n", GetLastError());
        return -1;
    }

    // Aguarda até o processo filho concluir
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Exibe mensagem após a conclusão do processo filho
    printf("Filho Completou Execucao\n");

    // Fecha os handles do processo e thread
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}

