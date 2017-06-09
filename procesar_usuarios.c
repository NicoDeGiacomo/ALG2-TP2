#include <stdio.h>
#include "utils.c"
#include "hash.h"
#include "lista.h"

#define SEPARADOR ','

char* parsear_usuario(char* linea, char sep) {
	char* buffer = malloc(strlen(linea));
	if(!buffer)
		return NULL;

	for (size_t i = 0;; i++) {
        if(linea[i] != sep){
            buffer[i] = '\0';
            break;
        }
        buffer[i] = linea[i];
    }

    return buffer;
}

bool imprimir_usuarios(hash_t *hash) {
    hash_iter_t* hash_iter = hash_iter_crear(hash);
    if(!hash_iter)
        return false;

    while (!hash_iter_al_final(hash_iter)){
        char* key = (char *) hash_iter_ver_actual(hash_iter);
        printf("%s: ", key);

        lista_t* lista = hash_obtener(hash, key);
        lista_iter_t* lista_iter = lista_iter_crear(lista);
        if(!lista_iter){
            hash_iter_destruir(hash_iter);
            return false;
        }

        while (!lista_iter_al_final(lista_iter)){
            printf("%s", (char *) lista_iter_ver_actual(lista_iter));
        }
        lista_iter_destruir(lista_iter);
    }
	hash_iter_destruir(hash_iter);

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

    char** lineas = obtener_lineas(file, 0);
	if(!lineas) {
		hash_destruir(hash);
		fclose(file);
        fprintf(stderr, "Unexpected error.\n");
		return 1;
	}

    for (int i = 0; lineas[i] != NULL ; ++i) {
        char* user = parsear_usuario(lineas[i], SEPARADOR);
        size_t n_tags = contar_tags(lineas[i], SEPARADOR);
        char str[256] = "";
        snprintf(str, sizeof(str), "%zu", n_tags);

        lista_t* lista = hash_obtener(hash, user);
        if(!lista)
            lista = lista_crear();
        lista_insertar_ultimo(lista, user);
        hash_guardar(hash, str, lista);
    }

	if(!imprimir_usuarios(hash)) {
        free_strv(lineas);
		hash_destruir(hash);
		fclose(file);
		fprintf(stderr, "Unexpected error.\n");
		return 1;
	}

    free_strv(lineas);
	hash_destruir(hash);
	fclose(file);
	
    return 0;

}

int main(int argc, char const *argv[]){
    if (argc != 2){
        fprintf(stderr, "Usage: ./procesar_usuarios <inputfile>\n");
        return 1;
    }

    return procesar_usuarios(argv[1]);
}
