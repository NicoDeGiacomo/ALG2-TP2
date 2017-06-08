#include <stdio.h>
#include "utils.c"
#include "hash.h"


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

	
}

int main(int argc, char const *argv[]){
    if (argc != 2){
        fprintf(stderr, "Usage: ./procesar_tweets <inputfile>\n");
        return 1;
    }

    return procesar_usuarios(argv[1]);
}
