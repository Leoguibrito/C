#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

// Lê o conteúdo do arquivo filename e retorna um vetor E o tamanho dele
// Se filename for da forma "gen:%d", gera um vetor aleatório com %d elementos
//
// +-------> retorno da função, ponteiro para vetor malloc()ado e preenchido
// | 
// |         tamanho do vetor (usado <-----+
// |         como 2o retorno)              |
// v                                       v
double* load_vector(const char* filename, int* out_size);


// Avalia o resultado no vetor c. Assume-se que todos os ponteiros (a, b, e c)
// tenham tamanho size.
void avaliar(double* a, double* b, double* c, int size);


typedef struct {
    
    double * c; 
    double * b;
    double * a; 
    int loop_start;
    int loop_end;

}thread_argument;


void* vector_sum (void* arg) {
    thread_argument * vector_info = (thread_argument *) arg;

    int loop_start = vector_info->loop_start;
    int loop_end = vector_info->loop_end;
   
   
    for (int k = loop_start; k < loop_end; k++) {
        //*c = *b + *a;

        vector_info->c[k] = vector_info->b[k] + vector_info->a[k];
    }

    pthread_exit(NULL);

}


int main(int argc, char* argv[]) {
    // Gera um resultado diferente a cada execução do programa
    // Se **para fins de teste** quiser gerar sempre o mesmo valor
    // descomente o srand(0)
    srand(time(NULL)); //valores diferentes
    //srand(0);        //sempre mesmo valor

    //Temos argumentos suficientes?
    if(argc < 4) {
        printf("Uso: %s n_threads a_file b_file\n"
               "    n_threads    número de threads a serem usadas na computação\n"
               "    *_file       caminho de arquivo ou uma expressão com a forma gen:N,\n"
               "                 representando um vetor aleatório de tamanho N\n",
               argv[0]);
        return 1;
    }
  

    //Lê números de arquivos para vetores alocados com malloc
    int a_size = 0, b_size = 0;
    double* a = load_vector(argv[2], &a_size);
    if (!a) {
        //load_vector não conseguiu abrir o arquivo
        printf("Erro ao ler arquivo %s\n", argv[2]);
        return 1;
    }
    double* b = load_vector(argv[3], &b_size);
    if (!b) {
        printf("Erro ao ler arquivo %s\n", argv[3]);
        return 1;
    }
    
    //Garante que entradas são compatíveis
    if (a_size != b_size) {
        printf("Vetores a e b tem tamanhos diferentes! (%d != %d)\n", a_size, b_size);
        return 1;
    }
    //Cria vetor do resultado 
    double* c = malloc(a_size*sizeof(double));

    //Quantas threads?
    int n_threads = atoi(argv[1]);
    if (!n_threads) {
        printf("Número de threads deve ser > 0\n");
        return 1;
    } else if (n_threads > a_size) {
        n_threads = a_size;
    }

    // Calcula com uma thread só. Programador original só deixou a leitura 
    // do argumento e fugiu pro caribe. É essa computação que você precisa 
    // paralelizar
    //for (int i = 0; i < a_size; ++i) 
    //     c[i] = a[i] + b[i];
    
   
    
    int task_size = (int) a_size/n_threads;
    thread_argument vector_inst[n_threads]; //vector instructions

    pthread_t threads[n_threads];


    int start = 0;
    for (int i = 0; i < n_threads; i++) {

        vector_inst[i].loop_start = start;
        vector_inst[i].loop_end = vector_inst[i].loop_start + task_size; //Isso eh feito para evitar erros ja que task_size é um arredondamento para baixo.
        vector_inst[i].a =  a;
        vector_inst[i].b =  b;
        vector_inst[i].c =  c;

        start += task_size;

        if (i == n_threads - 1) {
            vector_inst[i].loop_end = a_size;
        }

        pthread_create(&threads[i], NULL, vector_sum, (void *) &vector_inst[i]);
    }

    for (int i = 0; i < n_threads; ++i)
        pthread_join(threads[i], NULL);




    //    +---------------------------------+
    // ** | IMPORTANTE: avalia o resultado! | **
    //    +---------------------------------+
    avaliar(a, b, c, a_size);


    //Importante: libera memória
    free(a);
    free(b);
    free(c);

    return 0;
    }
