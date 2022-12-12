#include <sys/types.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

key_t semkey;
int semid;
struct sembuf sbuf;

void sigHandler(int s){
    semctl(semid, 0, IPC_RMID, 4);
    exit(0);
}

int main () {
    semkey = ftok("/bin", 'b');
    if ((semid = semget(semkey, 0, 0)) == -1) {
        semid = semget(semkey, 4, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    }
    signal(SIGINT, sigHandler);
    while (1) {
        sleep(3);
        printf("B is done\n");

        sbuf.sem_num = 1;
        sbuf.sem_op = 1;
        sbuf.sem_flg = 0;
        semop (semid, &sbuf, 1);
    }
}