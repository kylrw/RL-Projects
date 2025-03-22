#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// condition variables have 3 operations
// wait: unlocks mutex -> waits for a signal -> locks mutex
// broadcast:
// signal: sends signal to condition variable, to indicate that condition may have changed

int fuel = 0;
pthread_mutex_t mtxFuel;
pthread_cond_t condFuel;

void* fuel_filling(void* arg){
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mtxFuel);
        fuel += 60;
        printf("Filling fuel. Remaining: %d\n", fuel);
        pthread_mutex_unlock(&mtxFuel);
        pthread_cond_broadcast(&condFuel);
        sleep(1);
    }
}

void* car(void* arg) {
        pthread_mutex_lock(&mtxFuel);\
        while (fuel < 40) {
            printf("Waiting for fuel to fill.\n");
            pthread_cond_wait(&condFuel, &mtxFuel);
        }
        fuel -= 40;
        printf("Used fuel. Remaining: %d\n", fuel);
        pthread_mutex_unlock(&mtxFuel);
}

int main(int argc, char* argv[]){
    pthread_t th[7];
    pthread_mutex_init(&mtxFuel, NULL);
    pthread_cond_init(&condFuel, NULL);

    for (int i = 0; i < 7; i++){
        if (i == 5) {
            if (pthread_create(&th[i], NULL, &fuel_filling, NULL) != 0) perror("Failed to create");
        } else {
            if (pthread_create(&th[i], NULL, &car, NULL) != 0) perror("Failed to create");

        }
    }

    for (int i = 0; i < 7; i++){
        if (pthread_join(th[i], NULL) != 0) perror("Failed to join");
    }

    pthread_mutex_destroy(&mtxFuel);
    pthread_cond_destroy(&condFuel);
    return 0;
}