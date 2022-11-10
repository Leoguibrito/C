#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

/* ---------- Definições Globais. ---------- */
#define TEMPO_BASE 1000000

sem_t semaphore_IC;
sem_t semaphore_CI;

typedef enum { 
	CONTINENTE, 
	ILHA 
} cabeceira_t;

typedef struct {
	int id;
	cabeceira_t cabeceira;
	pthread_t thread;
} veiculo_t;
/* ---------------------------------------- */

/* ---------- Variáveis Globais. ---------- */
char cabeceiras[2][11] = { { "CONTINENTE" }, { "ILHA" } };
int total_veiculos;
int veiculos_turno;

int status_cabeceira;
int contador_carros;
// ToDo: Adicione aque quaisquer outras variávels globais necessárias.
/* ---------------------------------------- */


/* Inicializa a ponte. */
void ponte_inicializar() {
	
	// ToDo: IMPLEMENTAR!
	sem_init(&semaphore_CI, 0, veiculos_turno);		//Sentido Inicial
	sem_init(&semaphore_IC, 0, 0);
	
	status_cabeceira = 0;
	/* Imprime direção inicial da travessia. NÃO REMOVER! */
	printf("\n[PONTE] *** Novo sentido da travessia: CONTINENTE -> ILHA. ***\n\n");
	fflush(stdout);
}

/* Função executada pelo veículo para ENTRAR em uma cabeceira da ponte. */
void ponte_entrar(veiculo_t *v) {
	if (v->cabeceira == 0) {
		sem_wait(&semaphore_CI);
	} else {
		sem_wait(&semaphore_IC);
	}
	
	// ToDo: IMPLEMENTAR!
}

/* Função executada pelo veículo para SAIR de uma cabeceira da ponte. */
void ponte_sair(veiculo_t *v) {
	// ToDo: IMPLEMENTAR!
	/* Você deverá imprimir a nova direção da travessia quando for necessário! */
	contador_carros++;
	if(contador_carros == veiculos_turno){	
		printf("\n[PONTE] *** Novo sentido da travessia: %s -> %s. ***\n\n", cabeceiras[v->cabeceira], cabeceiras[!v->cabeceira]);
		fflush(stdout);

		if(status_cabeceira == 0){
			for(int i = 0; i < contador_carros; i++){
				sem_post(&semaphore_IC);
			}
		} else { 
			for(int i = 0; i < contador_carros; i++){
				sem_post(&semaphore_CI);
			}
		}
		status_cabeceira = !status_cabeceira;
		contador_carros = 0;
	}
}

/* FINALIZA a ponte. */
void ponte_finalizar() {

	// ToDo: IMPLEMENTAR!
	sem_destroy(&semaphore_IC);
	sem_destroy(&semaphore_CI);
	
	/* Imprime fim da execução! */
	printf("[PONTE] FIM!\n\n");
	fflush(stdout);
}

/* Implementa o comportamento de um veículo. */
void * veiculo_executa(void *arg) {
	veiculo_t *v = (veiculo_t *) arg;
	
	printf("[Veiculo %3d] Aguardando para entrar na ponte pelo(a) %s.\n", v->id, cabeceiras[v->cabeceira]);
	fflush(stdout);

	/* Entra na ponte. */
	ponte_entrar(v);
	printf("[Veiculo %3d] ENTROU na ponte pelo(a) %s.\n", v->id, cabeceiras[v->cabeceira]);
	fflush(stdout);

	/* Faz a travessia. */
	usleep(TEMPO_BASE + rand() % 1000);
	
	/* Seta cabeceira oposta para sair. */
	v->cabeceira = !v->cabeceira;

	/* Sai da ponte na cabeceira oposta. */
	printf("[Veiculo %3d] SAIU da ponte pelo(a) %s.\n", v->id, cabeceiras[v->cabeceira]);
	fflush(stdout);
	ponte_sair(v);

	pthread_exit(NULL);
}

/* Função principal: NÃO PODE ALTERAR! */
int main(int argc, char **argv) {
 
	if (argc < 3) {
		printf("Indique a quantidade total de veiculos e o numero de veiculos que podem atravessar a ponte por turno:\n\n %s [QTD_TOTAL_VEICULOS] [VEICULOS_POR_TURNO]\n\n", argv[0]);
		return 1;
	}

	total_veiculos = atoi(argv[1]);
	veiculos_turno = atoi(argv[2]);

	if (total_veiculos % (veiculos_turno * 2)) {
		printf("ERRO: O valor [QTD_TOTAL_VEICULOS] deve ser divisivel por ([VEICULOS_POR_TURNO] * 2)!\n\n %s [QTD_TOTAL_VEICULOS] [VEICULOS_POR_TURNO]\n\n", argv[0]);
		return 1;
	}

	srand(time(NULL));

	/* Aloca os veículos. */
	veiculo_t veiculos[total_veiculos];

	ponte_inicializar(total_veiculos, veiculos_turno);

	/* Cria os veículos. */
	for (int i = 0; i < total_veiculos; i++) {
		/* Define o id do veículo. */
		veiculos[i].id = i;
		
		/* Escolhe aleatoreamente se o veículo entra pela ILHA ou CONTINENTE. */
		veiculos[i].cabeceira = i % 2; 
		
		/* Cria a thread veículo. */
		pthread_create(&veiculos[i].thread, NULL, veiculo_executa, (void *) &veiculos[i]);		
	}

	/* Aguarda o término da execução de todos os veículos antes de finalizar o programa. */
	for (int i = 0; i < total_veiculos; i++) {
		pthread_join(veiculos[i].thread, NULL);
	}

	ponte_finalizar();

	return 0;
}