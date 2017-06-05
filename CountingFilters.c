#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

#define TAMANIO_INICIAL 1009
#define REDIM_MAX 0.7
#define REDIM_MIN 0.25
#define COEF_REDIM 2

struct filet {
    size_t cantidad;
    size_t tam;
    lista_t** tabla;
};

filet_t* crear_filete(void) {
	filet_t* filet = malloc(sizeof(filet_t));
	if(!filet)
        return NULL;

	filet->cantidad = 0;
    filet->tam = TAMANIO_INICIAL;

    filet->tabla = crear_tabla(TAMANIO_INICIAL);
    if(!filet->tabla){
        free(filet);
        return NULL;
    }
	
    return filet;
}

void filete_borrar(filet_t* filet) {
    destruir_tabla(filet);
    free(filet);
}

//TODO: Implement me!
bool agregar_filtro(filet_t* filet, const char* clave) {
	
}

//TODO: Implement me!
bool aumentar_filtro(filet_t* filet, const char* clave) {
	
}

//TODO: Implement me!
size_t ver_valor_filtro(filet_t* filet, const char* clave) {
	
}

//TODO: Implement me!
bool aumentar_filtro(filet_t* filet, const char* clave) {
	
}

/* FUNCIONES AUXILIARES */

//Funcion de Hashing Jenkins
//TODO: Robare los Hashings necesarios para salvar esta compañia!
size_t FiletitoALaJenkins(const char* key, size_t length, size_t max_size) {
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

//Crea la Tabla (y todas las listas dentro de la misma)
lista_t** crear_tabla(size_t tam) {
    lista_t** tabla = malloc(sizeof(lista_t*) * tam);
    if(!tabla)
        return NULL;

    for (int i = 0; i < tam; ++i) {
        tabla[i] = lista_crear();
        if(!tabla[i]) {
            for (int j = i-1; j >= 0; --j)
                free(tabla[j]);
            return NULL;
        }
    }
    return tabla;
}

//Destruye la tabla (y todas las listas dentro de la misma)
void destruir_tabla(filet_t* filet) {
    for (int i = 0; i < filet->tam; ++i) {
        lista_destruir(filet->tabla[i], NULL);
    }
    free(filet->tabla);
}
