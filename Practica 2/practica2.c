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

/**
 * Funcion para saber si un numero es primo.
 * El numero ingresado se divide entre 3 hasta n -1
 * @param n -> numero entero a checar si es primo o no
 * @returns 0 -> cuando el numero no es primo
 *          1 -> cuando el numero es primo
*/


int es_primo(int n){
    for (int i = 3; i < n; i+=2) {
        if(n % i == 0){
            return 0;
        }
    }
    return 1;
}

int main(){
    int rangoSuperior, rangoInferior, numeroActual, numPrimos = 0;
    struct timeval ts;
    long long start_ts, stops_ts;
    
    printf("Rango: ");
    scanf("%d %d", &rangoInferior, &rangoSuperior); 
    int rango = rangoSuperior - rangoInferior;

    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec;

    for(int i = 0; i <= rango; i++) {
        numeroActual = rangoInferior + i;

        if(numeroActual == 2){
            numPrimos++;
            continue;
        }
        
        if(numeroActual <= 1 || numeroActual % 2 == 0){
            continue;
        }

        if(es_primo(numeroActual)){
            numPrimos++;
        }
    }

    gettimeofday(&ts, NULL);
    stops_ts = ts.tv_sec;

    printf("%d primos encontrados entre %d y %d\n", numPrimos, rangoInferior, rangoSuperior);
    printf("%lld segundos\n", (stops_ts - start_ts));
    return 0;
}