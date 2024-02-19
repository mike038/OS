/** ============================================================================
 *  Fundamentos de Sistemas Operativos
 *  Practica 1
 * 
 *  Equipo 12
 *  Diego Orozco Alvarado
 *  Miguel Alejandro Aguilar Tun
 *  ============================================================================
*/ 
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>

double resultado = 0.5; // valor inicial ya con los valores de los limites
pthread_mutex_t mutex;

/**
 * Funcion para calcular el area bajo la curva f(x) = sqrt(1-y^2)
 * usando la aproximacion de la regla de trapecio.
 * En este caso se hace uso de mutex y esta implementado para el uso de hilos
 * @param n -> numero de intervalos
 * @returns aproximacion del area
 * 
*/
void *regla_trapecio(void *args){
    int n = *((int *) args);
    double h = 1.0 / n;
    double y;
    double resultado_local = 0;

    for (int i = 1; i < n; i++){
        y = i * h;
        resultado_local += sqrt(1.0 - y * y);
    }

    pthread_mutex_lock(&mutex);
    resultado += resultado_local;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(){
    int i, n, numHilos;
    double pi;
    struct timeval ts;
    long long start_ts, stop_ts;

    printf("Hilo(s): ");
    scanf("%d", &numHilos);

    printf("Intervalos(n): ");
    scanf("%d", &n);

    int intervalo = n / numHilos;
    pthread_t tid[numHilos];
    pthread_mutex_init(&mutex, NULL);
    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec;
    
    for (i = 0; i < numHilos; i++){
        pthread_create(&tid[i], 0, regla_trapecio, &intervalo); // a cada hilo le pasamos un intervalo de la curva
    }
    
    for (i = 0; i < numHilos; i++){
        pthread_join(tid[i], NULL);
    }

    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec;

    printf("Pi: %.10lf\n", resultado * 4 / n);
    printf("Tiempo: %lld segundos\n", (stop_ts - start_ts));
    return 0;
}