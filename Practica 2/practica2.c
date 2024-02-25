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
#include <math.h>
#include <sys/time.h>

int es_primo(int n){
    for (int i = 3; i < n; i+=2) {
        if(n % i == 0){
            return 0;
        }
    }
    return 1;
}

int main(){
    int rangoSuperior, rangoInferior, currentNumber, numPrimos = 0;
    struct timeval ts;
    long long start_ts, stops_ts;
    
    printf("Rango: ");
    scanf("%d %d", &rangoInferior, &rangoSuperior); 
    int rango = rangoSuperior - rangoInferior;

    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec;

    for(int i = 0; i <= rango; i++) {
        currentNumber = rangoInferior + i;

        if(currentNumber == 2){
            numPrimos++;
            continue;
        }
        
        if(currentNumber <= 1 || currentNumber % 2 == 0){
            continue;
        }

        if(es_primo(currentNumber)){
            numPrimos++;
        }
    }

    gettimeofday(&ts, NULL);
    stops_ts = ts.tv_sec;

    printf("%d primos encontrados entre %d y %d\n", numPrimos, rangoInferior, rangoSuperior);
    printf("%lld segundos\n", (stops_ts - start_ts));
    return 0;
}