#include <stdio.h>       //For c lang
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>     //For shared memory
#include <errno.h>
#include <semaphore.h>   //For sem_wait and sem_post
#include <fcntl.h>

sem_t* potentialCPatients_sem;
sem_t* coronaPatient;
sem_t* fluPatient;
int* potentialCPatients;
//int* coronaPatient;
//int* fluPatient;

void* tests_on_patients (void* param){ 
    // need to increment the shared variable potentialCPatients
    // int sem_wait(sem_t *sem);
    sem_wait(potentialCPatients_sem);

    //Using the shared resource

    printf("Value of *potentialCPatients before inc : %d\n", *potentialCPatients);
    *potentialCPatients = (*potentialCPatients) + 1;
    printf("Value of *potentialCPatients after inc : %d\n", *potentialCPatients);

    sem_post(potentialCPatients_sem);

    // With the help of rand() a number in range can be generated as num = (rand() % (upper–lower+1)) + lower

    int test_result = (rand() % 2);
    printf("The random number was : %d\n", test_result);
    // test_result = 0 --> Negative for Corona Virus --> FluPatient
    // test_result = 1 --> Positive for Corona Virus --> CoronaPatient

    if (test_result == 0){

        //Signal semaphore fluPatient
        // int sem_post(sem_t *sem);
        sem_post(fluPatient);

        //Decrement the shared variable --> potentialCPatients

        // int sem_wait(sem_t *sem);
        sem_wait(potentialCPatients_sem);

        //Using the shared resource

        printf("Value of *potentialCPatients before dec : %d\n", *potentialCPatients);
        *potentialCPatients = (*potentialCPatients) - 1;
        printf("Value of *potentialCPatients after dec : %d\n", *potentialCPatients);

        sem_post(potentialCPatients_sem);

    }
    else if (test_result == 1){
        //Signal semaphore coronaPatient
        // int sem_post(sem_t *sem);
        sem_post(coronaPatient);

        //Decrement the shared variable --> potentialCPatients

        // int sem_wait(sem_t *sem);
        sem_wait(potentialCPatients_sem);

        //Using the shared resource

        printf("Value of *potentialCPatients before dec : %d\n", *potentialCPatients);
        *potentialCPatients = (*potentialCPatients) - 1;
        printf("Value of *potentialCPatients after dec : %d\n", *potentialCPatients);

        sem_post(potentialCPatients_sem);

    }

    pthread_exit(0);
}

int main(){ 

    //Generate key --> parameter1 = file that is valid and available
    key_t shmkey = ftok("s.c",6); //Directory and number, file to key, it can be the current file also
    printf("shmkey : %d\n",shmkey); 

    //Create Shared Memory for integer potentialCPatients (That is why size is of an int)
    int shmid = shmget(shmkey, sizeof(int), 0644|IPC_CREAT); 
    //0644 = Permission, size is of an integer value so only one value is stored in shared mem (potentialCPatients)

    if (shmid < 0){
        perror("Shared memory not created successfully\n");
        exit(1);
    }

    //Attaching shared memory segment (identified by shmid) to the address space of the calling process 
    potentialCPatients = (int*) shmat(shmid, NULL, 0);
    //2nd parameter is NULL - the system chooses a suitable unused address at which to attach the shared mem segment

    *potentialCPatients = 0; //Initializing the value in shared memory
    printf("potentialCPatients : %d\n", *potentialCPatients);

    int initial_value = 1;// So that at least one of the function can use it

    // sem_init() initializes the unnamed semaphore while sem_open() creates a new POSIX semaphore or 
    // opens an existing semaphore 
    potentialCPatients_sem = sem_open ("potentialCPatients_sem", O_CREAT|O_EXCL, 0644, initial_value); 

    // name of the semaphore = potentialCPatients_sem(1st parameter)
    // If O_CREAT is specified in (2nd paramter),then the semaphore is created if it does not already exist
    // If both O_CREAT and O_EXCL are specified in (2nd parameter),then an error is returned if a semaphore 
    // with the given name already exists
    // 0644 (3rd parameter) is the permissions to be placed on the new semaphore
    // (4th parameter) is the initial value for the new semaphore

    // If O_CREAT is specified,and a semaphore with the given name already exists,then 3rd and 4th parameters are ignored.
    // On success,sem_open() returns the address of the new semaphore; this address is used when calling other 
    // semaphore-related functions.
  
    sem_unlink("potentialCPatients_sem");
    //unlink prevents the semaphore existing forever - Once the code/execution is done, the semaphore should not exist

    printf("potentialCPatients_sem INTIALIZATION completed\n");

    //Creating semaphores coronaPatient and fluPatient

    initial_value = 0;

    coronaPatient = sem_open ("coronaPatient_sem", O_CREAT|O_EXCL, 0645, initial_value);
    sem_unlink("coronaPatient");
    printf("coronaPatient_sem INTIALIZATION completed\n");

    fluPatient = sem_open ("fluPatient_sem", O_CREAT|O_EXCL, 0646, initial_value);
    sem_unlink("fluPatient_sem");
    printf("fluPatient_sem INTIALIZATION completed\n"); 
 
    //Taking patients - input from user
    unsigned int total_pp = 0;
    printf("Total Potential Coronavirus Patients initial value: %u\n", total_pp);

    printf("Enter Potential Coronavirus Patient entered the hospital \n");
    scanf("%u", &total_pp); // with scanf always & used with the variable
    printf("Patients : %d\n", total_pp);

    //Creating N threads 
    pthread_t tids[total_pp]; // Array of thread ids so that every thread has it's unique id

    int i ;
    for (i = 0; i < total_pp; i++) {         
        pthread_create(&tids[i], NULL, tests_on_patients, NULL);
        // int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);
        // If attr(2nd parameter) is NULL, then the thread is created with default attributes.
        // The new thread starts execution by invoking 3rd parameter 
        // 4th parameter (arg) is passed as the sole argument of 3rd parameter function (start_routine())
        // 4th parameter = NULL coz no arguments to be passed
    } 

    // Wait until all threads have done their work
    for (i = 0; i < total_pp; i++) {
        pthread_join(tids[i], NULL);
        
        printf("Value of *potentialCPatients : %d\n", *potentialCPatients); 

        int *fluPatient;
        sem_getvalue(fluPatient, fluPatient);
        printf("Value of *fluPatient : %d\n", *fluPatient);

        int *coronaPatient;
        sem_getvalue(fluPatient, coronaPatient);
        printf("Value of *coronaPatient : %d\n", *coronaPatient);       
    }   

    // All threads done executing 
    printf("Done all the work\n");
    // Shared memory has to be detached for potentialCPatients variable = good practice

        shmdt(potentialCPatients);
        shmctl(shmid, IPC_RMID, 0);

        //delete semaphores
        sem_destroy(potentialCPatients_sem);
        sem_destroy(coronaPatient);
        sem_destroy(fluPatient);

    printf("Cleared all semaphore stuff\n");

    return 0;
}