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
#include <semaphore.h>

#define NPROCS 4
#define SERIES_MEMBER_COUNT 200000

double *sums;
double x = 1.0;

sem_t start_sem;
sem_t done_sem;
int *proc_count;
double *res;

double get_member(int n, double x)
{
  int i;
  double numerator = 1;
  for (i = 0; i < n; i++)
    numerator = numerator * x;
  if (n % 2 == 0)
    return (-numerator / n);
  else
    return numerator / n;
}

void proc(int proc_num)
{
  sums[proc_num] = 0;
  for (int i = proc_num; i < SERIES_MEMBER_COUNT; i += NPROCS)
    sums[proc_num] += get_member(i + 1, x);
  sem_post(&done_sem);
  exit(0);
}

void master_proc()
{
  sleep(1);
  for (int i = 0; i < NPROCS; i++)
    sem_post(&start_sem);

  for (int i = 0; i < NPROCS; i++)
    sem_wait(&done_sem);

  *res = 0;
  for (int i = 0; i < NPROCS; i++)
    *res += sums[i];
  exit(0);
}

int main()
{
  long long start_ts;
  long long stop_ts;
  long long elapsed_time;
  long lElapsedTime;
  struct timeval ts;
  int i;
  int p;
  int shmid;
  void *shmstart;

  // Initialize semaphores
  sem_init(&start_sem, 0, 0); // Initially zero, allowing no processes to proceed
  sem_init(&done_sem, 0, 0);  // Initially zero, indicating no processes are done

  shmid = shmget(0x1234, NPROCS * sizeof(double) + 2 * sizeof(int), 0666 | IPC_CREAT);
  shmstart = shmat(shmid, NULL, 0);
  sums = shmstart;

  proc_count = shmstart + NPROCS * sizeof(double);
  res = shmstart + NPROCS * sizeof(double) + 2 * sizeof(int);

  *proc_count = 0;

  gettimeofday(&ts, NULL);
  start_ts = ts.tv_sec; // Tiempo inicial
  for (i = 0; i < NPROCS; i++)
  {
    p = fork();
    if (p == 0)
      proc(i);
  }
  p = fork();
  if (p == 0)
    master_proc();

  printf("El recuento de ln(1 + x) miembros de la serie de Mercator es %d\n", SERIES_MEMBER_COUNT);
  printf("El valor del argumento x es %f\n", (double)x);

  for (int i = 0; i < NPROCS + 1; i++)
    wait(NULL);

  gettimeofday(&ts, NULL);
  stop_ts = ts.tv_sec; // Tiempo final
  elapsed_time = stop_ts - start_ts;

  printf("Tiempo = %lld segundos\n", elapsed_time);
  printf("El resultado es %10.8f\n", *res);
  printf("Llamando a la función ln(1 + %f) = %10.8f\n", x, log(1 + x));

  shmdt(shmstart);
  shmctl(shmid, IPC_RMID, NULL);

  // Destroy semaphores
  sem_destroy(&start_sem);
  sem_destroy(&done_sem);
  return 0;
}
