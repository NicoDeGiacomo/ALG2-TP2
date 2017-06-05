
#ifndef COUNTINGFILTERS_H
#define COUNTINGFILTERS_H

#include <stdbool.h>
#include <stddef.h>

//Struct:
struct filet;
typedef struct filet filet_t;

//Primitivas:

//Crea un CountingFilet
filet_t* crear_filete(void);

//Borra un CountingFilet
void filete_borrar(filet_t* filet);

//Agregar filtro -> Hashea el filtro (string) x veces (x es la cantidad de funciones hash distintas)
// y setea el valor de un array (un array para cada x) en cero.

bool agregar_filtro(filet_t* filet, const char* clave);

//Aumentar filtro -> Agrega el filtro si no existe -> Aumenta en todos los x arrays el valor del filtro en 1

bool aumentar_filtro(filet_t* filet, const char* clave);

//Ver valor del filtro -> Devuelve el valor minimo de los x arrays para el filtro

size_t ver_valor_filtro(filet_t* filet, const char* clave);

//Filtro existe -> Devuelve si existe el filtro

bool aumentar_filtro(filet_t* filet, const char* clave);

#endif // COUNTINGFILTERS_H
