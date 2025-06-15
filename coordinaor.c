#include "coordinator.h"
#include "agent.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

void launch_agents() {
    const char *names[] = {"Gamma", "Beta", "Alpha", "Delta"};
    int pipes[4][2];
    pid_t pids[4];

    for (int i = 0; i < 4; i++) {
        pipe(pipes[i]);
        if ((pids[i] = fork()) == 0) {
            close(pipes[i][1]); // close write end
            start_agent(names[i], pipes[i][0]);
            exit(0);
        } else {
            close(pipes[i][0]); // close read end
        }
    }

    sleep(1);
    for (int i = 0; i < 4; i++) {
        write(pipes[i][1], "Identify!\n", 10);
        sleep(1);
        write(pipes[i][1], "Status?\n", 8);
    }

    sleep(1);
    for (int i = 0; i < 4; i++) {
        kill(pids[i], SIGUSR1);
        sleep(1);
        kill(pids[i], SIGUSR2);
        sleep(1);
        kill(pids[i], SIGTERM);
    }

    for (int i = 0; i < 4; i++) {
        wait(NULL);
    }
}
