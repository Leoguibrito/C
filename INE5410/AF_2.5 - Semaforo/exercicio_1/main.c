#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>

FILE* out;

sem_t semaphore_A;
sem_t semaphore_B;

void *thread_a(void *args) {
    for (int i = 0; i < *(int*)args; ++i) {
	//      +---> arquivo (FILE*) destino
	//      |    +---> string a ser impressa
	//      v    v
        sem_wait(&semaphore_A);
        fprintf(out, "A");
        // Importante para que vocês vejam o progresso do programa
        // mesmo que o programa trave em um sem_wait().
        fflush(stdout);

        
        sem_post(&semaphore_B);
    }
    return NULL;
}

void *thread_b(void *args) {
    for (int i = 0; i < *(int*)args; ++i) {

        sem_wait(&semaphore_B);
        fprintf(out, "B");
        fflush(stdout);

        sem_post(&semaphore_A);
    }
    return NULL;
}

int main(int argc, char** argv) {

    sem_init(&semaphore_A,0,1); 
    sem_init(&semaphore_B,0,1);


    if (argc < 2) {
        printf("Uso: %s [ITERAÇÕES]\n", argv[0]);
        return 1;
    }
    int iters = atoi(argv[1]);
    srand(time(NULL));
    out = fopen("result.txt", "w");

    pthread_t ta, tb;

    // Cria threads
    
    pthread_create(&ta, NULL, thread_a, &iters);
    pthread_create(&tb, NULL, thread_b, &iters);

    // Espera pelas threads
    pthread_join(ta, NULL);
    pthread_join(tb, NULL);

    //Imprime quebra de linha e fecha arquivo
    fprintf(out, "\n");
    fclose(out);
  
    
    sem_destroy(&semaphore_A);
    sem_destroy(&semaphore_B);
    return 0;
}
