#ifndef __GLOBALS_H__
#define __GLOBALS_H__


#include "conveyor_belt.h"
#include "virtual_clock.h"
#include "queue.h"

// Variaveis Criadas

/**
 * @brief Inicia um status da loja (de modo global)
 * 
 * @param store_open 
 */
extern void globals_set_store_open(int store_open);

/**
 * @brief Retorna um status da loja (de modo global)
 * 
 * @return int
 */
extern int globals_get_store_open();

/**
 * @brief Inicia um contador de cada comida (de modo global)
 * 
 * @param *produced_count 
 */
extern void globals_set_produced_food(int *produced_count);

/**
 * @brief Retorna um contador de cada comida (de modo global)
 * 
 * @return int* 
 */
extern int* globals_get_produced_food();

/**
 * @brief Inicia um contador de cada comida (de modo global)
 * 
 * @param *consumed_count 
 */
extern void globals_set_consumed_food(int *consumed_count);

/**
 * @brief Retorna um contador de cada comida (de modo global)
 * 
 * @return int* 
 */
extern int* globals_get_consumed_food();

/**
 * @brief Inicia um contador de cada comida (de modo global)
 * 
 * @param satisfied_count 
 */
extern void globals_set_satisfied_customers(int satisfied_count);

/**
 * @brief Retorna um contador de cada comida (de modo global)
 * 
 * @return int 
 */
extern int globals_get_satisfied_customers();


/**
 * @brief Inicia um relógio virtual (de modo global)
 * 
 * @param virtual_clock 
 */
extern void globals_set_virtual_clock(virtual_clock_t *virtual_clock);

/**
 * @brief Retorna um relógio virtual (de modo global)
 * 
 * @return virtual_clock_t* 
 */
extern virtual_clock_t *globals_get_virtual_clock();

/**
 * @brief Inicia uma esteira de sushi (de modo global).
 * 
 * @param conveyor_belt
 */
extern void globals_set_conveyor_belt(conveyor_belt_t *conveyor_belt);

/**
 * @brief Retorna uma esteira de sushi (de modo global)
 * 
 * @return conveyor_belt_t* 
 */
extern conveyor_belt_t *globals_get_conveyor_belt();

/**
 * @brief Inicia uma fila (de modo global)
 * 
 * @param queue 
 */
extern void globals_set_queue(queue_t *queue);

/**
 * @brief Retorna uma fila (de modo global)
 * 
 * @return queue_t* 
 */
extern queue_t *globals_get_queue();


// FIM

/**
 * @brief Finaliza todas as variáveis globais.
 * 
 */
extern void globals_finalize();

#endif  // __GLOBALS_H__
