
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "count_min_sketch.h"

#define PRIME_NUMBER 1009
#define TAM 50777
#define CANT_TABLAS 3


/*ESTRUCTURAS*/

struct count_min_sketch{
    size_t** tabla;
};


/*AUXILIARES*/

size_t jenkins_hash(const char *key, size_t length);
size_t prime_hash(const char *key);
size_t hash_33(const char *key);


/*PRIMITIVAS*/

count_min_sketch_t* count_min_sketch_crear(){
    count_min_sketch_t* count_min_sketch = malloc(sizeof(count_min_sketch_t));
    if(!count_min_sketch)
        return NULL;

    count_min_sketch->tabla = calloc(CANT_TABLAS, sizeof(size_t*));
    if(!count_min_sketch->tabla){
        free(count_min_sketch);
        return NULL;
    }

	for (int i = 0; i < CANT_TABLAS; i++) {
		count_min_sketch->tabla[i] = calloc(TAM, sizeof(size_t));
		if(!count_min_sketch->tabla[i]){
			for (int j = i; j > 0; j--) {
				free(count_min_sketch->tabla[j - 1]);
			}
			free(count_min_sketch->tabla);
			free(count_min_sketch);
			return NULL;
		}
	}

    return count_min_sketch;
}

void count_min_sketch_destruir(count_min_sketch_t* count_min_sketch){
    for (int i = 0; i < CANT_TABLAS; i++) {
		free(count_min_sketch->tabla[i]);
	}
	free(count_min_sketch->tabla);
    free(count_min_sketch);
}

void count_min_sketch_aumentar(count_min_sketch_t* count_min_sketch, const char* key){
    count_min_sketch->tabla[0][jenkins_hash(key, (size_t) strlen(key))] += 1;
    count_min_sketch->tabla[1][prime_hash(key)] += 1;
    count_min_sketch->tabla[2][hash_33(key)] += 1;
    return;
}

void count_min_sketch_aumentar_arr(count_min_sketch_t* count_min_sketch, const char** key, size_t size){
    for (size_t i = 0; i < size; ++i) {
        count_min_sketch_aumentar(count_min_sketch, key[i]);
    }
    return;
}

size_t count_min_sketch_obtener(count_min_sketch_t* count_min_sketch, const char* key) {
    size_t one = count_min_sketch->tabla[0][jenkins_hash(key, strlen(key))];
    size_t two = count_min_sketch->tabla[1][prime_hash(key)];
    size_t three = count_min_sketch->tabla[2][hash_33(key)];
    if(one < two && one < three)
        return one;
    if(two < one && two < three)
        return two;
    return three;
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
