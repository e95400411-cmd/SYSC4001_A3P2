#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>

#define NUM_STUDENTS 20
#define SEM "/mutex"

sem_t* sem;

void sig_catcher(int signum) {
    if (signum == SIGTERM || signum == SIGINT) {
        printf("TA terminated\n");
        sem_close(sem);
        exit(0);
    }
    
}

int main(int argc, char* argv[]) {
    //signal catcher
    signal(SIGTERM, sig_catcher);
    signal(SIGINT, sig_catcher);

    //set random seed
    srand(clock());

    //TA number
    int num = atoi(argv[0]);
    printf("TA: %d, created\n", num);

    //open semaphore
    sem = sem_open(SEM, O_CREAT);
    if (sem == SEM_FAILED){
        perror("semaphore failed to open");
        exit(1);
    }

    while(1) {
        //open student for read
        int sfilenum = rand() % 20;
        char sfile[NUM_STUDENTS];
        sprintf(sfile, "../Students/s%d.txt", sfilenum);
        FILE* studentptr = fopen(sfile, "r");
        if (studentptr == NULL) {
            perror("Student open error");
            exit(1);
        }

        char student[5];
        fgets(student, sizeof(student), studentptr);
        fclose(studentptr);
        
        //terminate immediately if student number = 9999
        if (atoi(student) == 9999) {
            printf("Student 9999 detected: terminating\n");
            sig_catcher(SIGTERM);
        }

        //wait before checking student num?????
        printf("TA: %d, marking exam of student %s\n", num, student);

        int question = rand() % 5 + 1;

        //wait until rubric is available
        sem_wait(sem);
        printf("TA: %d, accessing rubric\n", num);
        FILE* rubricptr = fopen("./rubric.txt", "r+");
        if (rubricptr == NULL) {
            perror("Rubric open error");
            exit(1);
        }

        char rubric[100];
        rubric[0] = '\0';    

        char line[10];
        while (fgets(line, sizeof(line), rubricptr)) {
            strcat(rubric, line);
        }
        rewind(rubricptr);

        //printf("%s\n", rubric);

        //wait 0.75 to 1.25s
        usleep(750000 + rand() % 11 * 50000);


        //edit corresponding rubric question?

        //if rubric should be edited, increment char
        if (rand() % 2) {
            char questionstr[3];
            sprintf(questionstr, "%d,", question);
            char * qtoedit = strstr(rubric, questionstr);
            qtoedit += 2;
            printf("TA: %d, changing rubric question %d from %c to %c\n", num, question, *qtoedit, (*qtoedit - 64) % 26 + 65);
            
            *qtoedit = (*qtoedit - 64) % 26 + 65; //if prev was Z, edit is A

            //write back to rubric
            fputs(rubric, rubricptr);
            
        }
        fclose(rubricptr);
        //allow other processes to access rubric
        printf("TA: %d, releasing rubric\n", num);
        sem_post(sem);

        //wait 2.5s to 3.5s
        usleep(2500000 + rand() % 11 * 100000);
        printf("TA: %d, finished marking student %s, question %d\n", num, student, question);
    }

    //sleep(1);
}