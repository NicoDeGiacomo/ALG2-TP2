//Nombre: Nicolás De Giácomo
//Padron: 99702
//Corrector: Matias C.
#ifndef LISTA_H
#define LISTA_H
#include <stdbool.h>
#include <stdlib.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct lista;
typedef struct lista lista_t;

struct lista_iter;
typedef struct lista_iter lista_iter_t;


/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t *lista_crear(void);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

// Devuelve verdadero o falso, según si la lista tiene o no elementos.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento al principio de la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al principio
// de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento al final de la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del ultimo elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del ultimo, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el ultimo elemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t* lista);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el largo de la lista. Si la lista tiene
// elementos, se devuelve la cantidad, si está vacía devuelve cero.
// Pre: la lista fue creada.
// Post: se devolvió el largo de la lista.
size_t lista_largo(const lista_t *lista);


/* ******************************************************************
 *                    PRIMITIVAS DE LA ITERACION
 * *****************************************************************/

// Crea un iterador.
// Pre: la lista fue creada.
// Post: devuelve un iterador apuntando al primer lugar.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Destruye el iterador.
// Pre: El iterador fue creado.
// Post: se elimino el iterador.
void lista_iter_destruir(lista_iter_t *iter);

// Avanza el iterador y devulve true. Devuelve false si no se puede avanzar
// Pre: El iterador fue creado.
// Post: El iterador apunta al siguiente elemento si es posible.
bool lista_iter_avanzar(lista_iter_t *iter);

// Obtiene el valor del actual elemento de la lista. Devuelve NULL si no es posible
// Pre: El iterador fue creado.
// Post: se devolvió el actual elemento de la lista.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve true si el iterador llego al final. False en caso contrario
// Pre: El iterador fue creado.
// Post: se devolvio el estado del iterador.
bool lista_iter_al_final(const lista_iter_t *iter);

// Inserta un elemento delante del elemento actual. 
// Devuelve true si es posible. False en caso contrario.
// Pre: El iterador fue creado.
// Post: se insertó un elemento delante del elemento actual.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Elimina el actual elemento de la lista y devuelve el valor. 
// Devuelve NULL si no es posible.
// Pre: El iterador fue creado.
// Post: se devolvió el elemento de la lista que fue eliminado.
void *lista_iter_borrar(lista_iter_t *iter);


/* ******************************************************************
 *                    PRIMITIVAS DE ITERADOR INTERNO
 * *****************************************************************/

// Itera sobre la lista y aplica la funcion visitar a cada uno de los datos. 
// Pasandole a la misma el parametro extra (para uso del usuario).
// Si la funcion visitar devuelve false se deja de iterar.
// Pre: la lista fue creada.
// Post: Se aplicó la funcion visitar a todos los datos.
void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra);


/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación de la lista..
void pruebas_lista_alumno(void);

#endif // LISTA_H
