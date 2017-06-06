#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CountingFilters.h"

#define PRIME_NUMBER 1009

//TODO: TESTs al tda bloom filters?
//NOTA: Para redimensionar habria que re-hashear (no se puede) -- Concusion: No se redimensiona
//Le vamos a pasar 'n' al crearlo y listo


/*ESTRUCTURAS*/

struct counting_filter{
    size_t* tabla1;
    size_t* tabla2;
    size_t tam;
};


/*AUXILIARES*/

size_t jenkins_hash(const char *key, size_t length, size_t max_size);
size_t prime_hash(const char *key, size_t max_size);


/*PRIMITIVAS*/

counting_filter_t* counting_filter_crear(size_t size){
    //TODO: ENCONTRAR EL PRIMO INMEDIATO SIGUIENTE Y CREAR CON ESE TAMAÑO PARA EVITAR COLISIONES
    counting_filter_t* counting_filter = malloc(sizeof(counting_filter_t));
    if(!counting_filter)
        return NULL;

    counting_filter->tabla1 = calloc(size, sizeof(size_t));
    if(!counting_filter->tabla1){
        free(counting_filter);
        return NULL;
    }

    counting_filter->tabla2 = calloc(size, sizeof(size_t));
    if(!counting_filter->tabla2){
        free(counting_filter->tabla1);
        free(counting_filter);
        return NULL;
    }

    counting_filter->tam = size;

    return counting_filter;
}

void counting_filter_destruir(counting_filter_t* counting_filter){
    free(counting_filter->tabla1);
    free(counting_filter->tabla2);
    free(counting_filter);
}

void counting_filter_aumentar(counting_filter_t* counting_filter, const char* key){
    counting_filter->tabla1[jenkins_hash(key, strlen(key), counting_filter->tam)] += 1;
    counting_filter->tabla2[prime_hash(key, counting_filter->tam)] += 1;
    return;
}

void counting_filter_aumentar_arr(counting_filter_t* counting_filter, const char** key, size_t size){
    for (size_t i = 0; i < size; ++i) {
        counting_filter_aumentar(counting_filter, key[i]);
    }
}

size_t counting_filter_obtener(counting_filter_t* counting_filter, const char* key) {
    size_t one = counting_filter->tabla1[jenkins_hash(key, strlen(key), counting_filter->tam)];
    size_t two = counting_filter->tabla2[prime_hash(key, counting_filter->tam)];
    return (one > two)? two : one;
}


/* FUNCIONES AUXILIARES */

size_t jenkins_hash(const char *key, size_t length, size_t max_size) {
    size_t i = 0;
    size_t hash = 0;
    while (i != length) {
        hash += key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash % max_size;
}

size_t prime_hash(const char *key, size_t max_size) {
    size_t hash = 0;

	for(hash = 0; *key != '\0'; key++)
		hash = *key + PRIME_NUMBER * hash;

    return hash % max_size;
}

//TODO: AGREGAR OTRA FUNCION DE HASH! HASH DEL NUMERO 33 ?