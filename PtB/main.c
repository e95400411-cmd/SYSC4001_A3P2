#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
//shared memory?????????????

#define SEM "/mutex"
sem_t* sem;

int n = 2;
int status;
pid_t* pids = NULL;

void killall() {
    if (pids != NULL) {
        for (int i = 0; i < n; i++) {
            kill(pids[i], SIGTERM);
            waitpid(pids[i], &status, WNOHANG);
        }
    }
    sem_close(sem);
    sem_unlink(SEM);
    printf("process finished\n");
    free(pids);
    exit(0);
}


void sig_catcher(int signum) {
    if (signum == SIGTERM || signum == SIGINT) {
        printf("main terminated\n");
        killall();
    }
}


int main(int argc, char* argv[]) {
    //signal catcher
    signal(SIGTERM, sig_catcher);
    signal(SIGINT, sig_catcher);
    
    //create semaphore
    sem = sem_open(SEM, O_CREAT| O_EXCL, 0666, 1);
    if (sem == SEM_FAILED){
        perror("semaphore failed to create");
        exit(1);
    }
    //sem_close(sem); //sem not needed for this process


    //number of processes
    //int n; 
    if (argv[1] != NULL) {
        n = atoi(argv[1]);
    }
    
    printf("num of TAs = %d\n", n);

    //child processes
    pids = malloc(sizeof(pid_t) * n);
    assert(pids != NULL);

    for (int i = 0; i < n; i++) {
        pids[i] = fork();
        if (pids[i] < 0) {
            perror("fork err");
            exit(1);
        }
        else if (pids[i] == 0) {
            char num[8];
            sprintf(num, "%d", i);
            execl("./ta", num, NULL);
        }
    }
    printf("parent\n");
    
    //int status;
    do {
        pid_t terminated = wait(&status); //if one of the children exits, all terminate
        if (errno == ECHILD) { //if no children remaining, stop waiting
            break;
        }
        else if (status != 0) {
            perror("child process exit failure");
        }
    } while (status != 0); //only terminate other processes if child exited successfully

    killall();
}