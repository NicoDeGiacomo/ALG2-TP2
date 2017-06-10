
#ifndef COUNTINGFILTERS_H
#define COUNTINGFILTERS_H

#include <stdbool.h>
#include <stddef.h>

//Struct:
struct count_min_sketch;
typedef struct count_min_sketch count_min_sketch_t;

//Primitivas:
//TODO: MODIFICAR TODOS LOS NOMBRES A count_min_sketch FORMAT

//Crea un CountingFilet
count_min_sketch_t* counting_filter_crear();

//Borra un CountingFilet
void counting_filter_destruir(count_min_sketch_t* counting_filter);

//Aumentar filtro -> Aumenta en 1 el filtro
void counting_filter_aumentar(count_min_sketch_t* counting_filter, const char* key);

//Ver valor del filtro -> Devuelve el valor minimo de los x arrays para el filtro
size_t counting_filter_obtener(count_min_sketch_t* counting_filter, const char* key);

//Pone el filtro en cero
void counting_filter_reiniciar(count_min_sketch_t* counting_filter, const char* key);

//Aumentar filtros -> Aumenta en 1 todos los filtros
void counting_filter_aumentar_arr(count_min_sketch_t* counting_filter, const char** key, size_t size);

#endif // COUNTINGFILTERS_H
