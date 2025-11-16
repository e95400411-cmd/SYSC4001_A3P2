#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>



int main(int argc, char* argv[]) {
    //number of processes
    int n; 
    
    if (argv[1] != NULL) {
        n = atoi(argv[1]);
    }
    else {
        //n = rand() % 10;
        n = 2;
    }
    
    printf("num of TAs = %d\n", n);

    //child processes
    pid_t* pids = malloc(sizeof(pid_t) * n);
    assert(pids != NULL);

    for (int i = 0; i < n; i++) {
        pids[i] = fork();
        if (pids[i] < 0) {
            perror("fork err");
            return 1;
        }
        else if (pids[i] == 0) { //child (process 2)
            char num[8];
            sprintf(num, "%d", i);
            execl("./ta", num, NULL);
            //printf("child %d\n", i);
        }
    }
    printf("parent\n");
    
    int status;
    do {
        pid_t terminated = wait(&status); //if one of the children exits, all terminate
    } while (status != 0); //only terminate other processes if child exited successfully

    for (int i = 0; i < n; i++) {
        kill(pids[i], SIGKILL);
        waitpid(pids[i], &status, WNOHANG);
    }
    printf("process finished\n");
    return 0;
}