#include "agent.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

static char agent_name[32];

void handle_signal(int sig) {
    if (sig == SIGUSR1) {
        printf("%s: Czekam na rozkazy\n", agent_name);
    } else if (sig == SIGUSR2) {
        printf("%s: Rozpoczynam misję\n", agent_name);
    } else if (sig == SIGTERM) {
        printf("%s: Misja zakończona\n", agent_name);
        exit(0);
    }
}

void start_agent(const char *name, int pipe_read) {
    strncpy(agent_name, name, sizeof(agent_name));
    printf("Imię agenta: Gotowy do misji\n");

    signal(SIGUSR1, handle_signal);
    signal(SIGUSR2, handle_signal);
    signal(SIGTERM, handle_signal);

    char buffer[128];
    while (read(pipe_read, buffer, sizeof(buffer)) > 0) {
        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmp(buffer, "Identify!") == 0) {
            printf("My codename is %s\n", agent_name);
        } else if (strcmp(buffer, "Status?") == 0) {
            printf("%s: My PID is: %d\n", agent_name, getpid());
        }
    }
}
