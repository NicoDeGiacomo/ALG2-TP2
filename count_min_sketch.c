#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "count_min_sketch.h"

#define PRIME_NUMBER 1009
#define TAM 50777

//TODO: TESTs al tda bloom filters?
//NOTA: Para redimensionar habria que re-hashear (no se puede) -- Concusion: No se redimensiona
//Le vamos a pasar 'n' al crearlo y listo


/*ESTRUCTURAS*/

struct count_min_sketch{
    size_t* tabla1;
    size_t* tabla2;
    size_t* tabla3;
};


/*AUXILIARES*/

size_t jenkins_hash(const char *key, size_t length);
size_t prime_hash(const char *key);
size_t hash_33(const char *key);


/*PRIMITIVAS*/

count_min_sketch_t* counting_filter_crear(){
    count_min_sketch_t* counting_filter = malloc(sizeof(count_min_sketch_t));
    if(!counting_filter)
        return NULL;

    counting_filter->tabla1 = calloc(TAM, sizeof(size_t));
    if(!counting_filter->tabla1){
        free(counting_filter);
        return NULL;
    }

    counting_filter->tabla2 = calloc(TAM, sizeof(size_t));
    if(!counting_filter->tabla2){
        free(counting_filter->tabla1);
        free(counting_filter);
        return NULL;
    }

    counting_filter->tabla3 = calloc(TAM, sizeof(size_t));
    if(!counting_filter->tabla2){
        free(counting_filter->tabla1);
        free(counting_filter->tabla2);
        free(counting_filter);
        return NULL;
    }

    return counting_filter;
}

void counting_filter_destruir(count_min_sketch_t* counting_filter){
    free(counting_filter->tabla1);
    free(counting_filter->tabla2);
    free(counting_filter->tabla3);
    free(counting_filter);
}

void counting_filter_aumentar(count_min_sketch_t* counting_filter, const char* key){
    counting_filter->tabla1[jenkins_hash(key, (size_t) strlen(key))] += 1;
    counting_filter->tabla2[prime_hash(key)] += 1;
    counting_filter->tabla3[hash_33(key)] += 1;
    return;
}

void counting_filter_aumentar_arr(count_min_sketch_t* counting_filter, const char** key, size_t size){
    for (size_t i = 0; i < size; ++i) {
        counting_filter_aumentar(counting_filter, key[i]);
    }
    return;
}

size_t counting_filter_obtener(count_min_sketch_t* counting_filter, const char* key) {
    size_t one = counting_filter->tabla1[jenkins_hash(key, strlen(key))];
    size_t two = counting_filter->tabla2[prime_hash(key)];
    size_t three = counting_filter->tabla3[hash_33(key)];
    if(one < two && one < three)
        return one;
    if(two < one && two < three)
        return two;
    return three;
}

void counting_filter_reiniciar(count_min_sketch_t* counting_filter, const char* key){
    counting_filter->tabla1[jenkins_hash(key, (size_t) strlen(key))] = 0;
    counting_filter->tabla2[prime_hash(key)] = 0;
    counting_filter->tabla3[hash_33(key)] = 0;
    return;
}


/* FUNCIONES AUXILIARES */

size_t jenkins_hash(const char *key, size_t length){
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
    return hash % TAM;
}

size_t prime_hash(const char *key){
    size_t hash = 0;

	for(hash = 0; *key != '\0'; key++)
		hash = *key + PRIME_NUMBER * hash;

    return hash % TAM;
}

size_t hash_33(const char *key){
    unsigned long hash = 5381;
    int c;
    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;
    return hash % TAM;
}
