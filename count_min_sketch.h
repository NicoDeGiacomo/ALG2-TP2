
#ifndef COUNTINGFILTERS_H
#define COUNTINGFILTERS_H

#include <stdbool.h>
#include <stddef.h>

//Struct:
struct count_min_sketch;
typedef struct count_min_sketch count_min_sketch_t;

//Primitivas:

//Crea un CountingFilet
count_min_sketch_t* count_min_sketch_crear();

//Borra un CountingFilet
void count_min_sketch_destruir(count_min_sketch_t* count_min_sketch);

//Aumentar filtro -> Aumenta en 1 el filtro
void count_min_sketch_aumentar(count_min_sketch_t* count_min_sketch, const char* key);

//Ver valor del filtro -> Devuelve el valor minimo de los x arrays para el filtro
size_t count_min_sketch_obtener(count_min_sketch_t* count_min_sketch, const char* key);

//Pone el filtro en cero
void count_min_sketch_reiniciar(count_min_sketch_t* count_min_sketch, const char* key);

//Aumentar filtros -> Aumenta en 1 todos los filtros
void count_min_sketch_aumentar_arr(count_min_sketch_t* count_min_sketch, const char** key, size_t size);

#endif // COUNTINGFILTERS_H
