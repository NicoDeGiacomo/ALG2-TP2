#include <stdio.h>
#include <stdlib.h>
#include "CountingFilters.h"

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

    counting_filter_aumentar_arr(filter, (const char **) lineas, n);

    //Falta traer los k mayores -> Hay que buscar clave por clave el valor y usar la tactica del heap magico euclidico
}

int main(int argc, char const *argv[]){
    if (argc != 3){
        //TODO: Checkear que n y k sean numeros enteros positivos distintos de cero
        fprintf(stderr, "Please provide 2 arguments\n");
        return 1;
    }

    return procesar_tweets((size_t) atoi(argv[1]), (size_t) atoi(argv[2]));
}