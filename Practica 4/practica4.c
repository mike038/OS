/** ============================================================================
 *  Fundamentos de Sistemas Operativos
 *  Practica 4
 *
 *  Equipo 6
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
#include <sys/msg.h>
#include <sys/types.h>

#define NPROCS 4
#define SERIES_MEMBER_COUNT 200000
#define MSG_TYPE 1

struct msg_buffer {
    long msg_type;
    double msg_value;
};

double x = 1.0;

double get_member(int n, double x) {
    int i;
    double numerator = 1;
    for(i = 0; i < n; i++) {
        numerator = numerator * x;
    }
    if(n % 2 == 0) {
        return (-numerator / n);
    }
    else {
        return numerator / n;
    }
}

void proc(int proc_num, int msg_id) {
    struct msg_buffer msg;
    int i;
    double sum = 0;

    for(i = proc_num; i < SERIES_MEMBER_COUNT; i += NPROCS) {
        sum += get_member(i + 1, x);
    }

    msg.msg_type = MSG_TYPE;
    msg.msg_value = sum;

    if(msgsnd(msg_id, &msg, sizeof(double), 0) == -1) {
        perror("msgsnd");
        exit(1);
    }

    exit(0);
}

double master_proc(int msg_id) {
    struct msg_buffer msg;
    int i;
    double total_sum = 0;

    for(i = 0; i < NPROCS; i++) {
        if(msgrcv(msg_id, &msg, sizeof(double), MSG_TYPE, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        total_sum += msg.msg_value;
    }

    return total_sum;
}

int main() {
    struct timeval ts;
    long long start_ts, stop_ts, elapsed_time;
    int i, p, msg_id;
    double result;

    msg_id = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
    if(msg_id == -1) {
        perror("msgget");
        exit(1);
    }

    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec; // Tiempo inicial

    for(i = 0; i < NPROCS; i++) {
        p = fork();
        if(p == 0) {
            proc(i, msg_id);
        } 
        else if (p == -1) {
            perror("fork");
            exit(1);
        }
    }

    result = master_proc(msg_id);
    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec; // Tiempo final
    elapsed_time = stop_ts - start_ts;

    printf("El recuento de ln(1 + x) miembros de la serie de Mercator es %d\n", SERIES_MEMBER_COUNT);
    printf("El valor del argumento x es %f\n", (double)x);   
    printf("Tiempo = %lld segundos\n", elapsed_time);
    printf("El resultado es %10.8f\n", result);
    printf("Llamando a la función ln(1 + %f) = %10.8f\n", x, log(1 + x));
    
    msgctl(msg_id, IPC_RMID, NULL);
}