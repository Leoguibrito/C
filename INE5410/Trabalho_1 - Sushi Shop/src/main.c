#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "args.h"
#include "customer.h"
#include "sushi_chef.h"
#include "hostess.h"
#include "globals.h"


int main (int argc, char** argv) {
    /* Read command line options */
    config_t config = parse(argc, argv);

    /* Setup random seed */
    srand(time(0));

    /* Setup store status -> CRIADO */
    globals_set_store_open(1);

    /* Setup simulation virtual clock */
    globals_set_virtual_clock(virtual_clock_init(&config));

    /* Setup sushi conveyor belt */
    globals_set_conveyor_belt(conveyor_belt_init(&config));

    /* Setup food count -> CRIADO */
    int produced_food[5] = {0, 0, 0, 0, 0};
    globals_set_produced_food(produced_food);

    /* Setup consumed count -> CRIADO */
    int consumed_food[5] = {0, 0, 0, 0, 0};
    globals_set_consumed_food(consumed_food);

    /* Setup satisfied count -> CRIADO */
    globals_set_satisfied_customers(0);

    /* Init the Sushi Chef */
    sushi_chef_t* sushi_chef = sushi_chef_init();

    /* Setup customer queue */
    globals_set_queue(queue_init());

    /* Init the Hostess */
    hostess_t* hostess = hostess_init();

    /* Join threads and free used memory */
    hostess_finalize(hostess);
    sushi_chef_finalize(sushi_chef);

    // PRINTA AS 3 VARIAVEIS GLOBAIS REQUISITADAS -- ✅
    int* consumed = globals_get_consumed_food();
    int* produced = globals_get_produced_food();
    int satsfied = globals_get_satisfied_customers();
    for (int i = 0; i < 5; i++) {    
        switch (i)
        {
        case 0:
            printf("\nCONTADOR DE PRATOS -- 🍣 PREPARADOS: %d",produced[i]);
            printf("\nCONTADOR DE PRATOS -- 🍣 CONSUMIDOS: %d",consumed[i]);
            break;
        case 1:
            printf("\nCONTADOR DE PRATOS -- 🍡 PREPARADOS: %d",produced[i]);
            printf("\nCONTADOR DE PRATOS -- 🍡 CONSUMIDOS: %d",consumed[i]);
            break;
        case 2:
            printf("\nCONTADOR DE PRATOS -- 🍜 PREPARADOS: %d",produced[i]);
            printf("\nCONTADOR DE PRATOS -- 🍜 CONSUMIDOS: %d",consumed[i]);
            break;
        case 3:
            printf("\nCONTADOR DE PRATOS -- 🍙 PREPARADOS: %d",produced[i]);
            printf("\nCONTADOR DE PRATOS -- 🍙 CONSUMIDOS: %d",consumed[i]);
            break;
        case 4:
            printf("\nCONTADOR DE PRATOS -- 🧈 PREPARADOS: %d",produced[i]);
            printf("\nCONTADOR DE PRATOS -- 🧈 CONSUMIDOS: %d",consumed[i]);

            break;
        }
    }
    printf("\nCLIENTES SATISFEITOS: %d\n", satsfied);

    globals_finalize();

    return EXIT_SUCCESS;
}
