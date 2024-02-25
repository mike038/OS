/** ============================================================================
 *  Fundamentos de Sistemas Operativos
 *  Practica 2
 * 
 *  Equipo 12
 *  Diego Orozco Alvarado
 *  Miguel Alejandro Aguilar Tun
 *  ============================================================================
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>

int *g;

int es_primo(int n){
    for (int i = 3; i < n; i+=2) {
        if(n % i == 0){
            return 0;
        }
    }
    return 1;
}

void proceso_hijo(long intervaloInicio, long intervaloFinal){
    int currentNumber;
    for(int i = 0; i < intervaloFinal - intervaloInicio; i++) {
        currentNumber = intervaloInicio + i;

        if(currentNumber == 2){
            (*g)++;
            continue;
        }
        
        if(currentNumber <= 1 || currentNumber % 2 == 0){
            continue;
        }

        if(es_primo(currentNumber)){
            (*g)++;
        }
    }
    exit(0);
}

int main(){
    int numHilos;
    long rangoSuperior, rangoInferior, currentNumber, numPrimos = 0;
    struct timeval ts;
    long long start_ts, stops_ts;
    
    printf("Rango: ");
    scanf("%ld %ld", &rangoInferior, &rangoSuperior); 
    int rango = rangoSuperior - rangoInferior;

    printf("Numero de hijos: ");
    scanf("%d", &numHilos);

    int shmid = shmget(0x1234, sizeof(int), 0666 | IPC_CREAT);
    if(shmid == -1){
        printf("Error creando shmget\n");
        return 1;
    }

    g = shmat(shmid, NULL, 0);
    *g  = 0;

    long intervalo = (rangoSuperior - rangoInferior  + numHilos) / numHilos;

    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec;

    for(int i = 0; i < numHilos; i++) {
        long inicioIntervalo = rangoInferior + i * intervalo;
        long finIntervalo = i == numHilos - 1 ? rangoSuperior : inicioIntervalo + intervalo - 1;
        int pid = fork();

        if(pid < 0){
            printf("Error creando fork\n");
            return 1;
        } else if(pid == 0){
            proceso_hijo(inicioIntervalo, finIntervalo);
        }
    }

    for(int i = 0; i < numHilos; i++) {
        wait(NULL);
    }
    
    gettimeofday(&ts, NULL);
    stops_ts = ts.tv_sec;

    printf("%d primos encontrados entre %ld y %ld\n", *g, rangoInferior, rangoSuperior);
    printf("%lld segundos\n", (stops_ts - start_ts));
    return 0;
}