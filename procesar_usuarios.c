#include <stdio.h>
#include "utils.c"
#include "hash.h"

#define SEPARADOR ','

char* parsear_usuario(char* linea) {
	char* buffer = malloc(strlen(linea));
	if(!buffer)
		return NULL;
	int pos = 0;
	for (size_t i = 0; str[i]; i++) {
        if(str[i] != sep) {
			buffer[pos] = str[i];
			pos++;
		}
    }
	buffer[pos] = '\0';
    return buffer;
}

bool imprimir_usuarios(lista_t** tabla) {
	for(int i = 0; tabla[i]; i++) {
		lista_iter_t* iter = lista_iter_crear(tabla[i]);
		if(!iter)
			return false;
		
		while(!lista_iter_al_final(iter)) {
			char* usuario = (char*) lista_iter_ver_actual(iter);
			printf("%zu: %s", i, usuario);
			lista_iter_avanzar(iter);
		}
	}
	
	return true;
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
	if(!lineas) {
		hash_destruir(hash);
		fclose(file);
        fprintf(stderr, "Unexpected error.\n");
		return 1;
	}
	
	size_t tamanio = 0;
	while (lineas[i]) {
		tamanio++;
	}

	lista_t** tabla = crear_tabla(tamanio);	
    for (int i = 0; lineas[i] != NULL; ++i) {
        size_t tags = contar_tags(lineas[i], SEPARADOR);
		
		if(!char* usuario = parsear_usuario(lineas[i])) {
			//TODO: Destruir tabla.
			free(lineas);
			hash_destruir(hash);
			fclose(file);
			fprintf(stderr, "Unexpected error.\n");
			return 1;
		}
	    
		free(usuario);
		
        lista_insertar_ultimo(tabla[tags], usuario);
    }

	if(!imprimir_usuarios(tabla)) {
		free(lineas);
		//TODO: Destruir tabla.
		hash_destruir(hash);
		fclose(file);
		fprintf(stderr, "Unexpected error.\n");
		return 1;
	}
	
	free(lineas);
	//TODO: Destruir tabla.
	hash_destruir(hash);
	fclose(file);
	
	//TODO: Codigo repetido?
	
    return 0;

}

int main(int argc, char const *argv[]){
    if (argc != 2){
        fprintf(stderr, "Usage: ./procesar_tweets <inputfile>\n");
        return 1;
    }

    return procesar_usuarios(argv[1]);
}
