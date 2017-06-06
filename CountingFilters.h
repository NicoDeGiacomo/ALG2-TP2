
#ifndef COUNTINGFILTERS_H
#define COUNTINGFILTERS_H

#include <stdbool.h>
#include <stddef.h>

//Struct:
struct counting_filter;
typedef struct counting_filter counting_filter_t;

//Primitivas:

//Crea un CountingFilet
counting_filter_t* counting_filter_crear(size_t size);

//Borra un CountingFilet
void counting_filter_destruir(counting_filter_t* counting_filter);

//Aumentar filtro -> Agrega el filtro si no existe -> Aumenta en todos los x arrays el valor del filtro en 1
void counting_filter_aumentar(counting_filter_t* counting_filter, const char* key);

//Ver valor del filtro -> Devuelve el valor minimo de los x arrays para el filtro
size_t counting_filter_obtener(counting_filter_t* counting_filter, const char* key);

#endif // COUNTINGFILTERS_H
