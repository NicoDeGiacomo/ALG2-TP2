#include <stdio.h>
#include "utils.c"
#include "hash.h"

#define SEPARADOR ','

void imprimir_usuarios(char* usuario, size_t tags) {
    printf("%zu: %s", tags, usuario);
}

size_t contar_tags(char* str, char sep) {
    size_t cant_sep = 0;
    for (size_t i = 0; str[i]; i++)
        if(str[i] == sep)
            cant_sep++;
    return cant_sep;
}

int procesar_usuarios(const char* name){
	
	FILE* file = fopen(name, "r");
	if(!file) {
		fprintf(stderr, "Unexpected error.\n");
		return 1;
	}

    hash_t* hash = hash_crear(NULL);
	if (!hash){
        fclose(file);
        fprintf(stderr, "Unexpected error.\n");
		return 1;
	}

    char** lineas = obtener_lineas(file, NULL);


    //Contar la cantidad de comas por usuario

    for (int i = 0; lineas[i] != NULL; ++i) {

        size_t tags = contar_tags(lineas[i], SEPARADOR);
        imprimir_usuarios(lineas[i], tags);
    }

    return 0;

}

int main(int argc, char const *argv[]){
    if (argc != 2){
        fprintf(stderr, "Usage: ./procesar_tweets <inputfile>\n");
        return 1;
    }

    return procesar_usuarios(argv[1]);
}
