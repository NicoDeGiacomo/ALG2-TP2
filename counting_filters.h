
#ifndef COUNTINGFILTERS_H
#define COUNTINGFILTERS_H

#include <stdbool.h>
#include <stddef.h>

//Struct:
struct counting_filter;
typedef struct counting_filter counting_filter_t;

//Primitivas:

//Crea un CountingFilet
counting_filter_t* counting_filter_crear();

//Borra un CountingFilet
void counting_filter_destruir(counting_filter_t* counting_filter);

//Aumentar filtro -> Aumenta en 1 el filtro
void counting_filter_aumentar(counting_filter_t* counting_filter, const char* key);

//Ver valor del filtro -> Devuelve el valor minimo de los x arrays para el filtro
size_t counting_filter_obtener(counting_filter_t* counting_filter, const char* key);

//Aumentar filtros -> Aumenta en 1 todos los filtros
void counting_filter_aumentar_arr(counting_filter_t* counting_filter, const char** key, size_t size);

#endif // COUNTINGFILTERS_H
