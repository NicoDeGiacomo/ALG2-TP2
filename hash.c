#include <stdbool.h>				//Nombres: Nicolás De Giácomo - Gastón N. Ponce
#include <stdio.h>					//Padrones: 99702 - 99723
#include <stdlib.h>					//DNIs: 39.490.629 - 40.393.064
#include <string.h>
#include "hash.h"
#include "lista.h"

#define TAMANIO_INICIAL 1009
#define REDIM_MAX 0.7
#define REDIM_MIN 0.25
#define COEF_REDIM 2


/* STRUCTS */

struct hash_iter{
    size_t iterados;
    size_t indice;
    const hash_t* hash;
    lista_iter_t* lista_iter;
};

struct hash{
    size_t cantidad;
    size_t tam;
    lista_t** tabla;
    hash_destruir_dato_t destructor;
};

struct campo{
    char* key;
    void* value;
};
typedef struct campo campo_t;


/* DECLARACION DE LAS FUNCIONES AUXILIARES */

bool redimensionar_hash(hash_t* hash, size_t tam);
lista_t** crear_tabla(size_t tam);
void destruir_tabla(hash_t *hash, bool eliminar_campos);
void destruir_campo(hash_t* hash, campo_t* campo);
size_t hashear(const char* key, size_t length, size_t max_size);
bool recorrer_lista(lista_iter_t* iterLista, const char* key);
lista_iter_t* obtener_aux(const hash_t* hash, const char* clave);






/* PRIMITIVAS DE HASH */

hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
	hash_t* hash = malloc(sizeof(hash_t));
	if(!hash)
        return NULL;

	hash->cantidad = 0;
    hash->tam = TAMANIO_INICIAL;
	hash->destructor = destruir_dato;

    hash->tabla = crear_tabla(TAMANIO_INICIAL);
    if(!hash->tabla){
        free(hash);
        return NULL;
    }
	
    return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {

    if((hash->cantidad/hash->tam) >= REDIM_MAX) {
        if(!redimensionar_hash(hash, hash->tam * COEF_REDIM))
            return false;
    }

    lista_iter_t* iter = obtener_aux(hash, clave);

    if(lista_iter_al_final(iter)){
        campo_t* campo = malloc(sizeof(campo_t));
        if (!campo)
            return false;
        campo->key = malloc(sizeof(char)*strlen(clave)+1);
        if (!campo->key){
            free(campo);
            return false;
        }
        strcpy(campo->key, clave);
        campo->value = dato;

        lista_iter_insertar(iter, campo);

        hash->cantidad ++;
    }else{
        if (hash->destructor)
            hash->destructor(((campo_t*)lista_iter_ver_actual(iter))->value);
        ((campo_t*)lista_iter_ver_actual(iter))->value = dato;
    }

    lista_iter_destruir(iter);
    return true;
}

void *hash_borrar(hash_t *hash, const char *clave) {
    lista_iter_t* iter = obtener_aux(hash, clave);
    if(lista_iter_al_final(iter)){
        lista_iter_destruir(iter);
        return NULL;
    }

    campo_t* campo = lista_iter_ver_actual(iter);
    void* value = ((campo_t* )lista_iter_ver_actual(iter))->value;
    free(campo->key);
    free(campo);
    lista_iter_borrar(iter);
    lista_iter_destruir(iter);

    hash->cantidad--;
    if((hash->cantidad/hash->tam) >= REDIM_MIN) {
        redimensionar_hash(hash,hash->tam / COEF_REDIM);
    }

    return value;
}

void *hash_obtener(const hash_t *hash, const char *clave) {
    lista_iter_t* iter = obtener_aux(hash, clave);
	if(lista_iter_al_final(iter)){
        lista_iter_destruir(iter);
        return NULL;
    }

    void* value = ((campo_t* )lista_iter_ver_actual(iter))->value;
    lista_iter_destruir(iter);
    return value;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
    lista_iter_t* iter = obtener_aux(hash, clave);
    if(lista_iter_al_final(iter)){
        lista_iter_destruir(iter);
        return false;
    }

    lista_iter_destruir(iter);
    return true;
}



size_t hash_cantidad(const hash_t *hash) {
	return hash->cantidad;
}

void hash_destruir(hash_t *hash) {
    destruir_tabla(hash, true);
    free(hash);
}


/* PRIMITIVAS DEL ITERADOR EXTERNO */

hash_iter_t *hash_iter_crear(const hash_t *hash) {
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if(!iter)
        return NULL;

    iter->hash = hash;

    size_t count = 0;
    while (lista_esta_vacia(hash->tabla[count])){
        if(count == hash->tam-1)
            break;
        count++;
    }
    iter->lista_iter = lista_iter_crear(hash->tabla[count]);
    iter->indice = count;
    iter->iterados = 0;
	return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
    if (hash_iter_al_final(iter))
        return false;
    lista_iter_avanzar(iter->lista_iter);
    iter->iterados++;

    while (lista_iter_al_final(iter->lista_iter) || lista_esta_vacia(iter->hash->tabla[iter->indice])) {
        iter->indice++;
        lista_iter_destruir(iter->lista_iter);
        if (iter->indice >= iter->hash->tam)
            return false;
        iter->lista_iter = lista_iter_crear(iter->hash->tabla[iter->indice]);
        if (!iter->lista_iter)
            return false;
    }

    return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
    if (hash_iter_al_final(iter))
        return NULL;

    campo_t* campo = lista_iter_ver_actual(iter->lista_iter);
    if (campo)
        return campo->key;

    return NULL;
}

bool hash_iter_al_final(const hash_iter_t *iter) {
	return iter->hash->cantidad == iter->iterados;
}



void hash_iter_destruir(hash_iter_t* iter) {
    if (!hash_iter_al_final(iter) || iter->hash->cantidad == 0)
        free(iter->lista_iter);
    free(iter);
}

bool redimensionar_hash(hash_t* hash, size_t tam) {

    lista_t** tabla = crear_tabla(tam);
	if(!tabla)
        return false;

    hash_iter_t* iter = hash_iter_crear(hash);
    if(!iter) {
		free(tabla);
		return false;
	}

    while (!hash_iter_al_final(iter)){
        campo_t* campo = lista_iter_ver_actual(iter->lista_iter);
        size_t indice = hashear(campo->key, strlen(campo->key),tam);

        if(!lista_insertar_ultimo(tabla[indice], campo) || (!hash_iter_avanzar(iter) && !hash_iter_al_final(iter))){
            lista_t** old_tabla = hash->tabla;
            hash->tabla = tabla;
            destruir_tabla(hash, false);
            hash->tabla = old_tabla;
            hash_iter_destruir(iter);
            return false;
        }

    }

    hash_iter_destruir(iter);
    destruir_tabla(hash, false);
    hash->tabla = tabla;
    hash->tam = tam;
    return true;
}

/* FUNCIONES AUXILIARES */

//Funcion de Hashing
size_t hashear(const char* key, size_t length, size_t max_size) {
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

void destruir_campo(hash_t* hash, campo_t* campo){
    free(campo->key);
    if (hash->destructor)
        hash->destructor(campo->value);
    free(campo);
}

//Destruye la tabla (y todas las listas dentro de la misma)
void destruir_tabla(hash_t *hash, bool eliminar_campos) {
    for (int i = 0; i < hash->tam; ++i) {
        lista_iter_t* iter = lista_iter_crear(hash->tabla[i]);
        if(!iter)
            continue;
        while (!lista_iter_al_final(iter)){
            campo_t* campo = lista_iter_borrar(iter);
            if (eliminar_campos)
                destruir_campo(hash, campo);
        }
        lista_iter_destruir(iter);
        lista_destruir(hash->tabla[i], NULL);
    }
    free(hash->tabla);
}

//Recorre un iterador de lista hasta encontrar el campo deseado
//Devuelve true si encuentra el campo, false en caso contrario
bool recorrer_lista(lista_iter_t* iterLista, const char* key){
    while(!lista_iter_al_final(iterLista)) {
        campo_t* campo = lista_iter_ver_actual(iterLista);
        if(!strcmp(campo->key, key)){
            return true;
        }
        lista_iter_avanzar(iterLista);
    }
    return false;
}

//Devuelve un lista_iter_t* Apuntando al campo actual,
// El iterador va a estar al final si no e encontró el campo
lista_iter_t* obtener_aux(const hash_t* hash, const char* clave) {
	size_t key = hashear(clave, strlen(clave), hash->tam);
    lista_iter_t* iterLista = lista_iter_crear(hash->tabla[key]);
    if (!iterLista)
        return NULL;

    recorrer_lista(iterLista, clave);

	return iterLista;
}
