//                          (principal)
//                               |
//              +----------------+--------------+
//              |                               |
//           filho_1                         filho_2
//              |                               |
//    +---------+-----------+          +--------+--------+
//    |         |           |          |        |        |
// neto_1_1  neto_1_2  neto_1_3     neto_2_1 neto_2_2 neto_2_3

// ~~~ printfs  ~~~
//      principal (ao finalizar): "Processo principal %d finalizado\n"
// filhos e netos (ao finalizar): "Processo %d finalizado\n"
//    filhos e netos (ao inciar): "Processo %d, filho de %d\n"

// Obs:
// - netos devem esperar 5 segundos antes de imprmir a mensagem de finalizado (e terminar)
// - pais devem esperar pelos seu descendentes diretos antes de terminar

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char** argv) {

    for (int i = 0; i < 2; i++) {
        pid_t pid;
        fflush(stdin);
        pid = fork();  // Cria os 2 filhos
        
        if (pid == 0) {
            
            printf("Processo %d, filho de %d\n", getpid(), getppid());
            for (int j = 0; j < 3; j++) {
                fflush(stdin);
                pid_t pid2 = fork();  // Cria os netos
                
                if (pid2 == 0) {
                    printf("Processo %d, filho de %d\n", getpid(), getppid());
                    sleep(5);
                    printf("Processo %d finalizado\n", getpid());
                    exit(0);
                }
            }
            
            while (wait(NULL) >= 0); 
            printf("Processo %d finalizado\n", getpid());
            exit(0); 
        }
    }
    
    while(wait(NULL) >= 0);
    printf("Processo principal %d finalizado\n", getpid());   

    return 0;
}
