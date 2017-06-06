#include <stdio.h>
#include <stdlib.h>
#include "CountingFilters.h"
#include "heap.h"

struct filter_result{
    const char* key;
    size_t value;
};
typedef struct filter_result filter_result_t;


int filter_result_cmp(const void *a, const void *b) {

    size_t n1 = ((filter_result_t*) a)->value;
    size_t n2 = ((filter_result_t*) b)->value;

    //Reverse int compare -> Para que el heap sea de minimos
    if(n1 > n2)
        return -1;
    else if(n1 < n2)
        return 1;
    return 0;
}


//Obtiene n lineas del archivo
char** obtener_lineas(FILE* file, size_t n) {
    char** var = malloc(sizeof(char*) * n);
    for (size_t i = 0; i < n; ++i) {
        fscanf(file, var[i]);
    }
    return var;
}


int procesar_tweets(size_t n, size_t k){
    char** lineas = obtener_lineas(stdin, n);

    counting_filter_t* filter = counting_filter_crear(n);
    if (!filter){
        fprintf(stderr, "Unexpected error.\n");
        return 1;
    }
    //TODO: caso de error
    heap_t* heap = heap_crear(filter_result_cmp);

    counting_filter_aumentar_arr(filter, (const char **) lineas, n);

    for (size_t i = 0; i < k; ++i){
        //TODO: caso de error
        filter_result_t* result = filter_result_crear(lineas[i], counting_filter_obtener(filter, lineas[i]));
        heap_encolar(heap, result);
    }

    for (size_t i = k; i < n; ++i){
        //Agregar al heap si es mayor que el tope
    }
    //En el heap quedan los k mayores.
}

int main(int argc, char const *argv[]){
    if (argc != 3){
        //TODO: Checkear que n y k sean numeros enteros positivos distintos de cero
        fprintf(stderr, "Please provide 2 arguments\n");
        return 1;
    }

    return procesar_tweets((size_t) atoi(argv[1]), (size_t) atoi(argv[2]));
}