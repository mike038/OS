/** ============================================================================
 *  Fundamentos de Sistemas Operativos
 *  Practica 3
 *
 *  Equipo 12
 *  Diego Orozco Alvarado
 *  Miguel Alejandro Aguilar Tun
 *  ============================================================================
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <semaphore.h> // Include semaphore library

#define NPROCS 4
#define SERIES_MEMBER_COUNT 200000

double *sums;
double x = 1.0;

int *proc_count;
int *start_all;
double *res;

sem_t start_semaphore; // Semaphore to synchronize start
sem_t proc_semaphore;  // Semaphore to synchronize process count

double get_member(int n, double x) {
    int i;
    double numerator = 1;
    for (i = 0; i < n; i++)
        numerator = numerator * x;
    if (n % 2 == 0)
        return (-numerator / n);
    else
        return numerator / n;
}

void proc(int proc_num) {
    int i;
    sem_wait(&start_semaphore); // Wait for start signal
    sums[proc_num] = 0;
    for (i = proc_num; i < SERIES_MEMBER_COUNT; i += NPROCS)
        sums[proc_num] += get_member(i + 1, x);
    sem_post(&proc_semaphore); // Signal completion
    exit(0);
}

void master_proc() {
    int i;
    sleep(1);
    sem_post(&start_semaphore); // Signal start
    for (i = 0; i < NPROCS; i++)
        sem_wait(&proc_semaphore); // Wait for all processes to finish
    *res = 0;
    for (i = 0; i < NPROCS; i++)
        *res += sums[i];
    exit(0);
}

int main() {
    long long start_ts;
    long long stop_ts;
    long long elapsed_time;
    struct timeval ts;
    int i;
    int p;
    int shmid;
    void *shmstart;

    shmid = shmget(0x1234, NPROCS * sizeof(double) + 2 * sizeof(int), 0666 | IPC_CREAT);
    shmstart = shmat(shmid, NULL, 0);
    sums = shmstart;

    proc_count = shmstart + NPROCS * sizeof(double);
    start_all = shmstart + NPROCS * sizeof(double) + sizeof(int);
    res = shmstart + NPROCS * sizeof(double) + 2 * sizeof(int);

    *proc_count = 0;

    sem_init(&start_semaphore, 1, 0); // Initialize semaphore
    sem_init(&proc_semaphore, 1, 0);  // Initialize semaphore

    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec; // Tiempo inicial

    for (i = 0; i < NPROCS; i++) {
        p = fork();
        if (p == 0)
            proc(i);
    }

    master_proc();

    // Parent process waits for all child processes to finish
    for (int i = 0; i < NPROCS + 1; i++)
        wait(NULL);

    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec; // Tiempo final
    elapsed_time = stop_ts - start_ts;

    printf("El recuento de ln(1 + x) miembros de la serie de Mercator es %d\n", SERIES_MEMBER_COUNT);
    printf("El valor del argumento x es %f\n", (double)x);
    printf("Tiempo = %lld segundos\n", elapsed_time);
    printf("El resultado es %10.8f\n", *res);
    printf("Llamando a la función ln(1 + %f) = %10.8f\n", x, log(1 + x));

    shmdt(shmstart);
    shmctl(shmid, IPC_RMID, NULL);

    sem_destroy(&start_semaphore); // Destroy semaphore
    sem_destroy(&proc_semaphore);  // Destroy semaphore

    return 0;
}
