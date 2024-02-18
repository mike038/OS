#include <stdio.h>
#include <math.h>
#include <sys/time.h>

/**
 * Funcion para calcular el area bajo la curva f(x) = sqrt(1-y^2)
 * usando la aproximacion de la regla de trapecio
 * @param n -> numero de intervalos
 * @returns aproximacion del area
 * 
*/
double regla_trapecio(int n){
    double h = 1.0 / n;
    double sum = 0.5; // valor inicial ya con los valores de los limites
    double y;

    for (int i = 1; i < n; i++){
        y = i * h;
        sum += sqrt(1.0 - y * y);
    }
    return sum * h;
}

int main(){
    int n;
    double pi;
    struct timeval ts;
    long long start_ts, stop_ts;

    printf("Intervalos(n): ");
    scanf("%d", &n);

    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec;

    pi = regla_trapecio(n) * 4;

    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec;
    
    printf("Pi: %.10lf\n", pi);
    printf("Tiempo: %lld segundos\n", (stop_ts - start_ts));
    return 0;
}