//Nombre: Nicolás De Giácomo
//Padron: 99702
//Corrector: Matias C.
#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

struct nodo;
typedef struct nodo nodo_t;

struct lista{
    nodo_t* primero;
    nodo_t* ultimo;
    size_t largo;
};

struct nodo{
    nodo_t* prox;
    void* dato;
};

struct lista_iter{
    nodo_t* actual;
    nodo_t* anterior;
    lista_t* lista;
};

nodo_t* nodo_crear(nodo_t* prox, void* dato){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (!nodo)
        return NULL;
    nodo->dato = dato;
    nodo->prox = prox;
    return nodo;
}

lista_t *lista_crear(void){
    lista_t* lista = malloc(sizeof(lista_t));
    if (!lista)
        return NULL;
    lista->primero = NULL;
    lista->ultimo = NULL;
    lista->largo = 0;
    return lista;
}

bool lista_esta_vacia(const lista_t *lista){
    return !lista->largo;
}

bool lista_insertar_primero(lista_t *lista, void *dato){
    nodo_t* nodo = nodo_crear(lista->primero, dato);
    if(!nodo)
        return false;
    lista->primero = nodo;
    lista->largo++;
    if(lista->largo == 1)
        lista->ultimo = nodo;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
    nodo_t* nodo = nodo_crear(NULL, dato);
    if(!nodo)
        return false;
    lista->largo++;
    if (lista->ultimo)
        lista->ultimo->prox = nodo;
    else
        lista->primero = nodo;
    lista->ultimo = nodo;
    return true;
}

void *lista_borrar_primero(lista_t *lista){
    if (lista_esta_vacia(lista))
        return NULL;
    void* dato = lista->primero->dato;
    nodo_t* prox = lista->primero->prox;
    free(lista->primero);
    lista->primero = prox;
    lista->largo --;
    if (lista_esta_vacia(lista)){
        lista->ultimo = NULL;
    }
    return dato;
}

void *lista_ver_primero(const lista_t *lista){
	if (lista_esta_vacia(lista))
		return NULL;
    return lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t* lista){
	if (lista_esta_vacia(lista))
		return NULL;
    return lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista){
    return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)){
    if (lista_esta_vacia(lista)){
        free(lista);
        return;
    }
    if(destruir_dato)
        destruir_dato(lista_borrar_primero(lista));
    else
        lista_borrar_primero(lista);
    return lista_destruir(lista, destruir_dato);
}

lista_iter_t *lista_iter_crear(lista_t *lista){
    lista_iter_t* iter = malloc(sizeof(lista_iter_t));
    if (!iter)
        return NULL;
    iter->actual = lista->primero;
    iter->anterior = NULL;
    iter->lista = lista;
    return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter){
    if (lista_iter_al_final(iter))
        return false;
    iter->anterior = iter->actual;
    iter->actual = iter->actual->prox;
    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
	if (lista_iter_al_final(iter))
        return NULL;
    return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter){
    return !iter->actual;
}

void lista_iter_destruir(lista_iter_t *iter){
    free(iter);
    return;
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
    nodo_t* nodo = nodo_crear(iter->actual, dato);
    if (!nodo)
        return false;
    if (iter->anterior)
        iter->anterior->prox = nodo;
    else
        iter->lista->primero = nodo;
    iter->actual = nodo;
    iter->lista->largo ++;
    if (!nodo->prox)
        iter->lista->ultimo = nodo;
    return true;
}

void *lista_iter_borrar(lista_iter_t *iter){
    if (lista_esta_vacia(iter->lista) || lista_iter_al_final(iter))
        return NULL;
    void* dato = iter->actual->dato;
    nodo_t* prox = iter->actual->prox;
    free(iter->actual);
    iter->actual = prox;
    if (iter->anterior)
        iter->anterior->prox = iter->actual;
    else
        iter->lista->primero = prox;
    if (!iter->actual)
    	iter->lista->ultimo = iter->anterior;
    iter->lista->largo --;
    return dato;
}

void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra){
    if (lista_esta_vacia(lista))
        return;
    void* dato = lista_borrar_primero(lista);
    if (visitar(dato, extra))
        lista_iterar(lista, visitar, extra);
    lista_insertar_primero(lista,dato);
    return;
}
