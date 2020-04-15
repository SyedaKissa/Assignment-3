#include <stdio.h>          // printf(), scanf()               
#include <stdlib.h>         // exit(), malloc(), free(), rand()
#include <sys/types.h>      // key_t, sem_t, pid_t      
#include <sys/shm.h>        // shmat(), IPC_RMID in shmctl()      
#include <errno.h>          // errno, ECHILD            
#include <semaphore.h>      // sem_open(), sem_destroy(), sem_wait().. 
#include <fcntl.h>          // O_CREAT, O_EXEC 

char* Buffer1;
sem_t* Buffer1_sem;

char* Buffer2;
sem_t* Buffer2_sem;


int main(){

    //Creating Shared Memory for buffer1 
    //0644 = Permission, size is of an integer value so only one value is stored in shared mem (potentialCPatients)
    int shmid_buffer1 = shmget(101010, 1024, 0644|IPC_CREAT); 
    printf("Shmid_buffer1 : %d\n", shmid_buffer1);

    if (shmid_buffer1 < 0){
        perror("Shared memory for buffer1 not created successfully\n");
        exit(0);
    }

    //Attaching shared memory segment (identified by shmid_buffer1) to the address space of the calling process
    //2nd parameter is NULL so the system chooses a suitable unused address at which to attach the shared mem segment 
    Buffer1 = (char*) shmat(shmid_buffer1, NULL, 0);

    if (Buffer1 < 0){
        perror("Shared memory for Buffer 1 not attached successfully\n");
        exit(1);
    } 
    printf("Shared mem work for Buffer 1 done\n");





    //Creating Shared Memory for buffer2
    //0644 = Permission, size is of an integer value so only one value is stored in shared mem (potentialCPatients)
    int shmid_buffer2 = shmget(121212, 1024, 0644|IPC_CREAT); 
    printf("Shmid_buffer2 : %d\n", shmid_buffer2);

    if (shmid_buffer2 < 0){
        perror("Shared memory for buffer2 not created successfully\n");
        exit(0);
    }

    //Attaching shared memory segment (identified by shmid_buffer1) to the address space of the calling process
    //2nd parameter is NULL so the system chooses a suitable unused address at which to attach the shared mem segment 
    Buffer2 = (char*) shmat(shmid_buffer2, NULL, 0);

    if (Buffer2 < 0){
        perror("Shared memory for Buffer 2 not attached successfully\n");
        exit(1);
    }
    printf("Shared mem work for Buffer 2 done\n");





    //Creating Shared Memory for buffer1 semaphore
    //0644 = Permission, size is of an integer value so only one value is stored in shared mem (potentialCPatients)
    int shmid_buffer1_sem = shmget(131313, 1024, 0644|IPC_CREAT); 
    printf("Shmid_buffer1_sem : %d\n", shmid_buffer1_sem);

    if (shmid_buffer1_sem < 0){
        perror("Shared memory for buffer1 semaphore not created successfully\n");
        exit(0);
    }

    //Attaching shared memory segment (identified by shmid_buffer1) to the address space of the calling process
    //2nd parameter is NULL so the system chooses a suitable unused address at which to attach the shared mem segment 
    Buffer1_sem = (sem_t*) shmat(shmid_buffer1_sem, NULL, 0);

    if (Buffer1_sem < 0){
        perror("Shared memory for Buffer 1 semaphore not attached successfully\n");
        exit(1);
    }
    printf("Shared mem work for Buffer 1 semaphore done\n"); 




    //Creating Shared Memory for buffer2 semaphore
    //0644 = Permission, size is of an integer value so only one value is stored in shared mem (potentialCPatients)
    int shmid_buffer2_sem = shmget(141414, 1024, 0644|IPC_CREAT); 
    printf("Shmid_buffer2_sem : %d\n", shmid_buffer2_sem);

    if (shmid_buffer2_sem < 0){
        perror("Shared memory for buffer2 semaphore not created successfully\n");
        exit(0);
    }

    //Attaching shared memory segment (identified by shmid_buffer1) to the address space of the calling process
    //2nd parameter is NULL so the system chooses a suitable unused address at which to attach the shared mem segment 
    Buffer2_sem = (sem_t*) shmat(shmid_buffer2_sem, NULL, 0);

    if (Buffer2_sem < 0){
        perror("Shared memory for Buffer 2 semaphore not attached successfully\n");
        exit(1);
    }
    printf("Shared mem work for Buffer 2 semaphore done\n"); 




	return 0;
}