/*
 =========================================================================
  Demonstração Didática do Mecanismo de Interrupção em C++

  Este programa simula um "instalador" em execução contínua,
  exibindo uma barra de progresso na tela.

  Funcionalidades principais:
  - Fluxo normal: o programa avança a instalação de 0% até 100%.
  - Interrupção via teclado: ao pressionar Ctrl+C, o programa desvia
    o fluxo para uma rotina de tratamento (ISR simulada), que pergunta
    se o usuário deseja Cancelar ou Retomar a instalação.
  - Retomada: caso opte por retomar, o progresso continua do ponto salvo.
  - Cancelamento: caso opte por cancelar, o programa encerra com segurança.
  - (Opcional no Windows) Tecla 'P': pausa e retoma a instalação sem encerrar,
    ilustrando a chegada de eventos assíncronos.

  Conceitos de Sistemas Operacionais demonstrados:
  - Interrupção: desvio automático do fluxo normal por evento externo.
  - ISR (Interrupt Service Routine): rotina chamada para tratar o evento.
  - Salvamento e restauração de contexto: progresso é preservado durante a
    interrupção e retomado após o tratamento.
  - Concorrência: diferentes eventos (Ctrl+C e tecla P) podem alterar o
    comportamento do programa enquanto a execução principal prossegue.

 =========================================================================
*/

#include <csignal>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#ifdef _WIN32
  #include <conio.h>   // _kbhit, _getch
  #define CLEAR "cls"
#else
  #include <unistd.h>  // usleep
  #define CLEAR "clear"
#endif

// ---- Sinais de "hardware" (didático)
std::atomic<bool> g_interrupted{false};   // setado pelo handler de SIGINT
std::atomic<bool> g_paused{false};        // pausa didática (tecla 'p' no Windows)

void sigint_handler(int) {
    // Não faça I/O pesado aqui; apenas sinalize de forma atômica.
    g_interrupted.store(true);
}

// Barra de progresso simples
void print_progress_bar(int percent) {
    const int width = 40;
    int filled = (percent * width) / 100;
    std::cout << "\r[";
    for (int i = 0; i < filled; ++i) std::cout << '=';
    for (int i = filled; i < width; ++i) std::cout << ' ';
    std::cout << "] " << percent << "%  Instalando..." << std::flush;
}

// “ISR didática”: executa quando g_interrupted = true
// Mostra que o fluxo foi desviado, pergunta ao usuário o que fazer e retorna a decisão.
enum class Action { Resume, Cancel };

Action simulated_ISR_prompt() {
    std::cout << "\n\n>>> INTERRUPCAO VIA TECLADO (SIGINT / Ctrl+C) DETECTADA!\n";
    std::cout << "O SO desviou o fluxo para a rotina de servico de interrupcao (ISR).\n";
    std::cout << "Deseja [C]ancelar a instalacao ou [R]etomar? ";
    std::cout << "(digite C ou R e pressione Enter): ";

    while (true) {
        std::string line;
        if (!std::getline(std::cin, line)) {
            // Entrada inesperada: por segurança, cancela
            return Action::Cancel;
        }
        if (!line.empty()) {
            char c = std::toupper(static_cast<unsigned char>(line[0]));
            if (c == 'C') return Action::Cancel;
            if (c == 'R') return Action::Resume;
        }
        std::cout << "Opcao invalida. Digite C (cancelar) ou R (retomar): ";
    }
}

#ifdef _WIN32
// Thread opcional (Windows/MinGW): tecla 'P' pausa/retoma sem bloquear o loop.
// Isso ajuda a mostrar que eventos externos podem alterar o estado do programa.
void key_listener_thread() {
    while (true) {
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 'p' || ch == 'P') {
                bool newState = !g_paused.load();
                g_paused.store(newState);
                std::cout << "\n>>> Estado: " << (newState ? "PAUSADO" : "RETOMADO") << " (tecla P)\n";
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
}
#endif

int main() {
    // Associa Ctrl+C ao "sinal de hardware" (interrupção)
    std::signal(SIGINT, sigint_handler);

#ifdef _WIN32
    // Limpa a tela (puramente cosmético)
    std::system(CLEAR);
    std::cout << "Demonstracao de Interrupcao (C++ / MinGW)\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Use Ctrl+C para simular interrupcao via teclado (SIGINT).\n";
    std::cout << "Opcional: pressione 'P' para Pausar/Retomar (thread de escuta).\n\n";
    // Inicia thread opcional de pausa
    std::thread listener(key_listener_thread);
    listener.detach();
#else
    std::system(CLEAR);
    std::cout << "Demonstração de Interrupcao (C++)\n";
    std::cout << "---------------------------------\n";
    std::cout << "Use Ctrl+C para simular interrupcao via teclado (SIGINT).\n\n";
#endif

    int progress = 0;

    while (progress <= 100) {
        // Verifica “interrupção de hardware”
        if (g_interrupted.load()) {
            // Salvar “contexto” (didático): neste demo basta lembrar o progresso
            int saved_progress = progress;

            // Desvia para ISR (rotina de serviço)
            Action a = simulated_ISR_prompt();

            if (a == Action::Cancel) {
                std::cout << "\nInstalacao cancelada pelo usuario (via interrupcao).\n";
                std::cout << "Estado salvo: progresso em " << saved_progress << "%.\n";
                std::cout << "Encerrando com segurança...\n";
                std::cout << std::flush;
                return 0;
            } else {
                std::cout << "Retomando a instalacao a partir de " << saved_progress << "%...\n";
                std::cout << std::flush;
                // “Reabilita interrupções” e segue
                g_interrupted.store(false);
            }
        }

        // Pausa didática (Windows): mostra ocioso aguardando evento
#ifdef _WIN32
        if (g_paused.load()) {
            std::cout << "\r[PAUSADO] Pressione 'P' para retomar..." << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(120));
            continue;
        }
#endif

        // Trabalho “normal” do programa (fluxo principal)
        print_progress_bar(progress);
        std::this_thread::sleep_for(std::chrono::milliseconds(120)); // simula trabalho/IO
        progress++;

        if (progress == 100) {
            print_progress_bar(progress);
            std::cout << "\nInstalacao concluida com sucesso!\n";
            break;
        }
    }

    return 0;
}
