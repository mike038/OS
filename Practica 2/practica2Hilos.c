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

/**
 * Funcion que calcula si un numero es primo a traves
 * de dividir el numero entre 3 hasta n - 1
 * @param n -> numero entero
 * @return 0 -> si el ingresado no es primo
 *         1 -> si el numero ingresado es primo
*/

int es_primo(int n){
    for (int i = 3; i < n; i+=2) {
        if(n % i == 0){
            return 0;
        }
    }
    return 1;
}

/**
 * Proceso que obtiene la cantidad de numeros primos en un intervalo
 * y al final se le suma a la variable compartida
 * @param intervaloInicio -> inicio del intervalo a checar
 * @param intervaloFinal -> fin del intervalo a checar, entra dentro del rango
*/

void proceso_hijo(long intervaloInicio, long intervaloFinal){
    int currentNumber;
    int numPrimosLocal = 0;
    for(int i = 0; i <= intervaloFinal - intervaloInicio; i++) {
        currentNumber = intervaloInicio + i;

        if(currentNumber == 2){
            numPrimosLocal++;
            // printf("%d\n", currentNumber);
            continue;
        }
        
        if(currentNumber <= 1 || currentNumber % 2 == 0){
            continue;
        }

        if(es_primo(currentNumber)){
            numPrimosLocal++;
            // printf("%d\n", currentNumber);
        }
    }

    *g += numPrimosLocal;
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
    long intervalo = (rango  + numHilos - 1) / numHilos;

    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec;

    for(int i = 0; i < numHilos; i++) {
        long inicioIntervalo = rangoInferior + i * intervalo;
        long finIntervalo = i == numHilos - 1 ? rangoSuperior : inicioIntervalo + intervalo - 1;
        // printf("inicio: %ld, fin: %ld\n", inicioIntervalo, finIntervalo);
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