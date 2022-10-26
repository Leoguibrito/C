#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "banco.h"


// Faz uma transferência de valor unidades monetárias da conta origem
// para a conta destino. Se a conta origem tiver saldo insuficiente, o
// saldo fica negativo, simbolizando cheque especial.  
// 
// Essa função é "unsafe": ela não aplica nenhum mecanismo de
// sincronização e pode produzir valores errados se utilizada
// concorrentemente com a mesma conta.
//

int origem_ant = 0;
int destino_ant = 0;
sem_t semaforo;

// Definida em helper.c
extern void transferir_unsafe(conta_t *origem, conta_t *destino, double valor);

void transferir(conta_t *origem, conta_t *destino, double valor);

// Thread que faz  transferências aleatórias. Definida em helper.c
void* caixa_func(void *arg);

// Versão thread-safe da função transferir_unsafe.
void transferir(conta_t *origem, conta_t *destino, double valor) {
    if (origem -> id == destino_ant ||  origem_ant == destino -> id || origem -> id == origem_ant) {
        origem_ant = origem -> id;
        destino_ant = destino -> id;

        sem_wait(&semaforo);
        pthread_mutex_lock(&origem->mutex);
        pthread_mutex_lock(&destino->mutex);

        transferir_unsafe(origem, destino, valor);

        pthread_mutex_unlock(&origem->mutex);
        pthread_mutex_unlock(&destino->mutex);
        sem_post(&semaforo);

    } else {
        origem_ant = origem -> id;
        destino_ant = destino -> id;

        pthread_mutex_lock(&origem->mutex);
        pthread_mutex_lock(&destino->mutex);

        transferir_unsafe(origem, destino, valor);

        pthread_mutex_unlock(&origem->mutex);
        pthread_mutex_unlock(&destino->mutex);
    }

    
}


int main(int argc, char* argv[]) {
    // Cria as contas com algum saldo e inicializa o mutex de cada conta
    sem_init(&semaforo,0,1);

    for (int i = 0; i < NUM_CONTAS; i++) {
        contas[i].id = i;
        contas[i].saldo = 1000.00;
        pthread_mutex_init(&contas[i].mutex, NULL);
    }
  
    // Cria as threads
    pthread_t caixas[NUM_CAIXAS];
    for (int i = 0; i < NUM_CAIXAS; i++)
        pthread_create(&caixas[i], NULL, caixa_func, NULL);
  
    // Esperar as threads terminarem
    for (int i = 0; i < NUM_CAIXAS; i++)
        pthread_join(caixas[i], NULL);
  
    // Destroi os mutexes
    for (int i = 0; i < NUM_CONTAS; i++)
        pthread_mutex_destroy(&contas[i].mutex);

    sem_destroy(&semaforo);  
    return 0;
}
