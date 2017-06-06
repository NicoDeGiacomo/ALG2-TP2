#include <stdbool.h>  /* bool */
#include <stddef.h>	  /* size_t */
#include <stdlib.h>
#include "heap.h"

#define TAMANIO_INICIAL 1000
#define PADRE(i) (i-1)/2
#define HIJO_IZQ(i) (2*i)+1
#define HIJO_DER(i) (2*i)+2
#define REDIM_MAX 0.7
#define REDIM_MIN 0.25
#define COEF_REDIM 2

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

 struct heap{
    void** tabla;
    size_t cantidad;
	size_t tamanio;
    cmp_func_t comparador;
};


/* ******************************************************************
 *                			FUNCIONES APARTE
 * *****************************************************************/
void up_heap(void** tabla, cmp_func_t cmp, size_t actual);
void down_heap(void** tabla, cmp_func_t cmp, size_t cantidad, size_t actual);
void swap(void** pointer1, void** pointer2);

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {

    for (size_t i = cant; i > 0; --i) {
        down_heap(elementos, cmp, cant, i-1);
    }

    for (size_t i = cant; i > 0; --i) {
        swap(&elementos[0], &elementos[i-1]);
        down_heap(elementos, cmp, i-1, 0);
    }
}

bool heap_redimensionar(heap_t* heap, size_t tam_nuevo) {
    void** tabla = realloc(heap->tabla, tam_nuevo * sizeof(void*));
    if (!tabla)
        return false;

    heap->tabla = tabla;
    heap->tamanio = tam_nuevo;
    return true;
}


/* ******************************************************************
 *                    	PRIMITIVAS DEL HEAP
 * *****************************************************************/

heap_t *heap_crear(cmp_func_t cmp) {
	heap_t* heap = malloc(sizeof(heap_t));
    if (!heap)
        return NULL;

    heap->comparador = cmp;
    heap->tabla = malloc(sizeof(void*) * TAMANIO_INICIAL);
    heap->cantidad = 0;
	heap->tamanio = TAMANIO_INICIAL;
    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
	heap_t* heap = heap_crear(cmp);
	if(!heap)
		return NULL;

	for(int i = 0; i < n; i++) {
		heap_encolar(heap, arreglo[i]);
	}

	return heap;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)) {

	while(!heap_esta_vacio(heap)) {
        void* buffer = heap_desencolar(heap);
		if(destruir_elemento)
			destruir_elemento(buffer);
	}

    free(heap->tabla);
	free(heap);
}

size_t heap_cantidad(const heap_t *heap) {
	return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap) {
	return !heap_cantidad(heap);
}

bool heap_encolar(heap_t *heap, void *elem) {

	if((double) heap->cantidad / (double) heap->tamanio >= REDIM_MAX) {
		if(!heap_redimensionar(heap, heap->tamanio * COEF_REDIM))
			return false;
	}

    heap->tabla[heap->cantidad] = elem;
    heap->cantidad++;
    up_heap(heap->tabla, heap->comparador, heap->cantidad-1);
	return true;
}

void *heap_ver_max(const heap_t *heap) {
    if (heap_esta_vacio(heap))
        return NULL;

	return heap->tabla[0];
}

void *heap_desencolar(heap_t *heap) {
    if(heap_esta_vacio(heap))
        return NULL;

    void* buffer = heap->tabla[0];
    heap->tabla[0] = heap->tabla[heap->cantidad-1];
    heap->tabla[heap->cantidad-1] = NULL;
    heap->cantidad--;
    down_heap(heap->tabla, heap->comparador, heap->cantidad, 0);


	if( (heap->cantidad/heap->tamanio) >= REDIM_MIN)
		heap_redimensionar(heap, heap->tamanio / COEF_REDIM);

    return buffer;
}

void up_heap(void** tabla, cmp_func_t cmp, size_t actual){
    if (actual == 0)
        return;
    size_t padre = PADRE(actual);
    if(cmp(tabla[actual], tabla[padre]) > 0){
        swap(&tabla[padre], &tabla[actual]);
        up_heap(tabla, cmp, padre);
    }
}

void down_heap(void** tabla, cmp_func_t cmp, size_t cantidad, size_t actual){
    if(actual >= cantidad)
        return;
    size_t max = actual;
    size_t hijo_izq = HIJO_IZQ(actual);
    size_t hijo_der = HIJO_DER(actual);
    if(hijo_izq < cantidad && cmp(tabla[hijo_izq], tabla[max]) > 0)
        max = hijo_izq;
    if(hijo_der < cantidad && cmp(tabla[hijo_der], tabla[max]) > 0)
        max = hijo_der;
    if(max != actual){
        swap(&tabla[max], &tabla[actual]);
        down_heap(tabla, cmp, cantidad, max);
    }
}

void swap(void** pointer1, void** pointer2){
    void* buffer = *pointer1;
    *pointer1 = *pointer2;
    *pointer2 = buffer;
    return;
}
