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
    printf("Rango: ");
    scanf("%d %d", &rangoInferior, &rangoSuperior); 
    int rango = rangoSuperior - rangoInferior;
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
    printf("%d primos encontrados entre %d y %d", numPrimos, rangoInferior, rangoSuperior);
    return 0;
}