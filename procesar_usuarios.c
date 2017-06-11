#include <stdio.h>
#include "utils.h"
#include "lista.h"
#include "hash.h"

char* parsear_usuario(char* linea, char sep) {
	char* buffer = malloc(strlen(linea));
	if(!buffer)
		return NULL;

	for (size_t i = 0;; i++) {
        if(linea[i] == sep){
            buffer[i] = '\0';
            break;
        }
        buffer[i] = linea[i];
    }

    return buffer;
}

int imprimir_usuarios(hash_t *hash, size_t max) {
    for (size_t i = 0; i <= max; ++i) {
        char str[256] = "";
        snprintf(str, sizeof(str), "%zu", i);

        if (!hash_pertenece(hash, str))
            continue;

        lista_t* lista = hash_obtener(hash, str);

        lista_iter_t* lista_iter = lista_iter_crear(lista);
        if(!lista_iter){
            return 1;
        }

        while (!lista_iter_al_final(lista_iter)){
            printf("%s ", (char *) lista_iter_ver_actual(lista_iter));
            lista_iter_avanzar(lista_iter);
            if (!lista_iter_al_final(lista_iter))
                printf(",");
        }
        printf("\n");
        lista_iter_destruir(lista_iter);
        lista_destruir(lista, NULL);
    }
	return 0;
}

size_t contar_tags(char* str, char sep) {
    size_t cant_sep = 0;
    for (size_t i = 0; str[i]; i++)
        if(str[i] == sep)
            cant_sep++;
    return cant_sep;
}

hash_t *obtener_hash(char **lineas, size_t tam, size_t *max) {
    size_t max_n_tags = 0;

    hash_t* hash = hash_crear(NULL);
    if (!hash)
        return NULL;

    for (size_t i = 0; i < tam; ++i) {
        char* user = parsear_usuario(lineas[i], SEPARADOR);
        size_t n_tags = contar_tags(lineas[i], SEPARADOR);

        if(hash_pertenece(hash, user)) {
            size_t tmp = *((size_t *)hash_borrar(hash, user));
            n_tags += tmp;
        }
        if (n_tags > max_n_tags)
            max_n_tags = n_tags;

        char str[256] = "";
        snprintf(str, sizeof(str), "%zu", n_tags);
        hash_guardar(hash, user, &str);
    }

    *max = max_n_tags;
    return hash;
}

hash_t* invertir_hash(hash_t *hash) {
    //TODO: aca va NULL para destruir ?!
    hash_t* new_hash = hash_crear(NULL);
    if(!hash)
        return NULL;

    hash_iter_t* iter = hash_iter_crear(hash);
    if(!iter)
        return NULL;

    while (!hash_iter_al_final(iter)){
        char* user = (char *) hash_iter_ver_actual(iter);
        size_t cant = *((size_t*)hash_obtener(hash, user));

        char str[256] = "";
        snprintf(str, sizeof(str), "%zu", cant);

        lista_t* lista;
        if(hash_pertenece(new_hash, str)){
            lista = hash_obtener(new_hash, str);
        }else{
            lista = lista_crear();
            if(!lista){
                free(new_hash);
                free(iter);
                return NULL;
            }
        }
        lista_insertar_ultimo(lista, user);
    }

    hash_iter_destruir(iter);
    hash_destruir(hash);
    return new_hash;
}

int procesar_usuarios(const char* name){
	
	FILE* file = fopen(name, "r");
	if(!file) {
		fprintf(stderr, "Unexpected error.\n");
		return 1;
	}

    size_t n_lineas = 0;
    char** lineas = obtener_lineas(file, 0, &n_lineas);
	if(!lineas) {
		fclose(file);
        fprintf(stderr, "Unexpected error.\n");
		return 1;
	}

    //TODO: CASO DE ERROR
    size_t max;
    hash_t* hash = obtener_hash(lineas, n_lineas, &max);

    invertir_hash(hash);

	int exit = imprimir_usuarios(hash, max);
    if (exit)
        fprintf(stderr, "Unexpected error.\n");

    free_strv(lineas);
    //TODO destruir hash
    fclose(file);
	
    return exit;

}

int main(int argc, char const *argv[]){
    if (argc != 2){
        fprintf(stderr, "Usage: ./procesar_usuarios <inputfile>\n");
        return 1;
    }

    return procesar_usuarios(argv[1]);
}
