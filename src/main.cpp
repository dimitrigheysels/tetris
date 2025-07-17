
#include <SFML/Graphics.hpp>
#include "ui.h"
#include "game.h"
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <execinfo.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void segfault_sigaction(int signal, siginfo_t *si, void *arg)
{
    printf("Caught segfault at address %p\n", si->si_addr);

    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", signal);
    backtrace_symbols_fd(array, size, STDERR_FILENO);

    std::cin.get();
}

int main()
{
    UI ui;
    Game game;
    sf::Clock clock_;

    struct sigaction sa;

    memset(&sa, 0, sizeof(struct sigaction));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = segfault_sigaction;
    sa.sa_flags = SA_SIGINFO;

    sigaction(SIGSEGV, &sa, NULL);

    // game loop
    while (game.is_running())
    {
        sf::Event event;
        /*while*/ if (ui.poll_event(event))
        {
            game.update(event);
        }
        else if (clock_.getElapsedTime().asSeconds() > 0.5f)
        {
            game.update();
            clock_.restart();
        }

        ui.clear();
        game.render(ui);
        ui.display();
    }

    // cleanup
    ui.close();

    return 0;
}