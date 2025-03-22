#include <pthread.h>
#include <bits/pthreadtypes.h>
#include <stdio.h>

static void make_water(void);

// x hydrogen threads, y oxygen threads
// when make_water returns
// return: the amount (n) of n (2x & 1y)
//
// cond_wait, holds mutex, and releases it on condition variable
// wait, releases mutex, waits for condvar to be signaled, then reacquires mutex
// signal, signals a single waiting thread
// broadcast, signals all waiting threads

struct reaction // 
{
    // TODO: Add fields to struct reaction.

    pthread_mutex_t mtx;
    pthread_cond_t H_reached;
    pthread_cond_t O_released;
    //pthread_cond_t H2;
    //pthread_cond_t O;

    int H_amount;
    int total;

};

static void reaction_init(struct reaction *rxn)
{
    // TODO: Initialize fields in *rxn.
    pthread_mutex_init(&rxn->mtx, NULL);
    pthread_cond_init(&rxn->H_reached, NULL);
    pthread_cond_init(&rxn->O_released, NULL);

    rxn->H_amount = 0;
    rxn->total = 0;

}

static void hydrogen(struct reaction *rxn)
{
    // TODO: Implement the behavior of hydrogen atoms.

    // lock H
    // H_amount ++ 
    // if H_amount == 2
    //  signal
    //  while H_amount != 0
    //      wait
    // unlock H

    pthread_mutex_lock(&rxn->mtx);
    rxn->H_amount++;
    printf("H amount: %d\n", rxn->H_amount);
    if (rxn->H_amount >= 2) {
        pthread_cond_signal(&rxn->H_reached);
        while (rxn->H_amount >= 2) {
            pthread_cond_wait(&rxn->O_released, &rxn->mtx);
        }
    }
    pthread_mutex_unlock(&rxn->mtx);

}

static void oxygen(struct reaction *rxn)

{

    // TODO: Implement the behavior of oxygen atoms.

    // lock O
    // while H_amount < 2
    //  wait for signal
    // add to total
    // H_amount = 0
    // signal H
    // unlock

    pthread_mutex_lock(&rxn->mtx);
    
    while (rxn->H_amount < 2) {
        //printf("Waiting, H = %d\n", rxn->H_amount);
        pthread_cond_wait(&rxn->H_reached, &rxn->mtx);
    }

    rxn->total++;
    rxn->H_amount = 0;
    pthread_cond_signal(&rxn->O_released);
    pthread_cond_signal(&rxn->O_released);
    make_water();
    printf("H2O released, total = %d\n", rxn->total);

    pthread_mutex_unlock(&rxn->mtx);

}



//////////////// MAIN

#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

static sem_t h_bonded;
static sem_t o_bonded;
static sem_t w_made;

static void make_water(void)
{
    sem_post(&w_made);
}

static void *hydrogen_thread(void *arg)
{
    hydrogen(arg);
    sem_post(&h_bonded);
    return NULL;
}

static void *oxygen_thread(void *arg)
{
    oxygen(arg);
    sem_post(&o_bonded);
    return NULL;
}

static void alarm_handler(int foo)
{
    (void)foo;
    static const char msg[] = "Failure.\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    exit(1);
}

static void check(sem_t *sem, const char *s, int expected)
{
    printf("Checking that %d %s...\n", expected, s);

    for (int i = 0; i < expected; ++i)
    {
        sem_wait(sem);
    }

    int extra = 0;

    while (sem_trywait(sem) == 0)
    {
        ++extra;
    }

    if (extra != 0)
    {
        printf("Too many %s. Expected %d, got %d, %d extra.\n", s, expected, expected + extra, extra);
        exit(1);
    }
}

int main(void)
{

    srand(getpid() ^ time(NULL));

    signal(SIGALRM, alarm_handler);
    alarm(1);

    static const int total_atoms = 200;
    int hydrogen_atoms = 0;
    int oxygen_atoms = 0;
    int hydrogen_pct = (int)(round(100 * (double)rand() / RAND_MAX));

    struct reaction rxn;
    reaction_init(&rxn);

    sem_init(&h_bonded, 0, 0);
    sem_init(&o_bonded, 0, 0);
    sem_init(&w_made, 0, 0);

    for (int i = 0; i < total_atoms; i++)
    {
        pthread_t tid;

        int ret;

        if ((rand() % 100) < hydrogen_pct)
        {
            ++hydrogen_atoms;
            ret = pthread_create(&tid, NULL, hydrogen_thread, &rxn);
        }
        else
        {
            ++oxygen_atoms;
            ret = pthread_create(&tid, NULL, oxygen_thread, &rxn);
        }

        if (ret != 0)
        {
            perror("pthread_create");
            exit(1);
        }

        if (pthread_detach(tid) != 0)
        {
            perror("pthread_detach");
            exit(1);
        }
    }

    int expected_molecules = hydrogen_atoms / 2;

    if (expected_molecules > oxygen_atoms)
    {
        expected_molecules = oxygen_atoms;
    }

    printf("A reaction with %d hydrogen and %d oxygen atoms should produce %d "
           "water molecules.\n",
           hydrogen_atoms, oxygen_atoms, expected_molecules);

    check(&w_made, "water molecules were produced", expected_molecules);
    check(&h_bonded, "hydrogen atoms were bonded", expected_molecules * 2);
    check(&o_bonded, "oxygen atoms were bonded", expected_molecules);

    puts("Success!\n");
}

// Your previous Plain Text content is preserved below:

// This is just a simple shared plaintext pad, with no execution capabilities.

// When you know what language you'd like to use for your interview,
// simply choose it from the dots menu on the tab, or add a new language
// tab using the Languages button on the left.
// You can also change the default language your pads are created with
// in your account settings: https://app.coderpad.io/settings
// Enjoy your interview!
