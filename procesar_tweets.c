#include <stdio.h>
#include <stdlib.h>
#include "count_min_sketch.h"
#include "heap.h"
#include "utils.h"
#include "hash.h"


struct filter_result{
    char* key;
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

filter_result_t *filter_result_crear(const char *key, size_t value) {
    filter_result_t* filter_result = malloc(sizeof(filter_result_t));
    filter_result->key = calloc(strlen(key)+1, sizeof(char));
    strcpy(filter_result->key, key);
    filter_result->value = value;
    return filter_result;
}

void filter_result_destruir(filter_result_t* filter_result){
    free((void *) filter_result->key);
    free(filter_result);
}

void imprimir_tweets(const char *tweet) {
    //TODO: No tengo idea como se debería imprimir esto.
    printf("%s\n", tweet);
}

void left_shift(char **arr, size_t* tam) {
    for (size_t k = 0; k <= *tam-1; k++){
        arr[k] = arr[k+1];
    }
    *tam=*tam-1;
    return;
}

bool eliminar_repetidos(char **arr, size_t* tam) {
    hash_t* hash = hash_crear(NULL);

    for (size_t i = 0; i < *tam; ++i) {
        hash_guardar(hash, arr[i], NULL);
    }

    hash_iter_t* iter = hash_iter_crear(hash);
    if (!iter)
        return false;

    size_t cont = 0;
    while (!hash_iter_al_final(iter)){
        arr[cont] = realloc(arr[cont], sizeof(char)*(strlen((char *) hash_iter_ver_actual(iter)) + 1));
        strcpy(arr[cont], (char *) hash_iter_ver_actual(iter));
        cont++;
        hash_iter_avanzar(iter);
    }

    hash_iter_destruir(iter);
    hash_destruir(hash);

    *tam = cont;
    return true;
}

int procesar_tweets(size_t n, size_t k){
    size_t n_lineas = 0;
    char** lineas = obtener_lineas(stdin, n, &n_lineas);

    count_min_sketch_t* filter = count_min_sketch_crear();
    if (!filter){
        fprintf(stderr, "Unexpected error.\n");
        return 1;
    }

    heap_t* heap = heap_crear(filter_result_cmp);
	if(!heap) {
		count_min_sketch_destruir(filter);
		fprintf(stderr, "Unexpected error.\n");
        return 1;
	}

    size_t n_encolados = 0;
    for (size_t j = 0; j < n_lineas; ++j) {
        size_t n_tags = 0;
        char **tags = split(lineas[j], SEPARADOR, &n_tags);

        //Ignoro el usuario
        left_shift(tags, &n_tags);

        count_min_sketch_aumentar_arr(filter, (const char **) tags, n_tags);

        //Elimino los repetidos
        if(!eliminar_repetidos(tags, &n_tags)){
            count_min_sketch_destruir(filter);
            heap_destruir(heap, free);
            free_strv(lineas);
            fprintf(stderr, "Unexpected error.\n");
            return 1;
        }

        for (size_t i = 0; i < n_tags; ++i) {

            if (n_encolados < k){
                filter_result_t *result = filter_result_crear(tags[i], count_min_sketch_obtener(filter, tags[i]));
                //count_min_sketch_reiniciar(filter, tags[i]);
                if (!result) {
                    count_min_sketch_destruir(filter);
                    heap_destruir(heap, free);
                    free_strv(lineas);
                    fprintf(stderr, "Unexpected error.\n");
                    return 1;
                }

                n_encolados++;
                heap_encolar(heap, result);

            }else{
                if (count_min_sketch_obtener(filter, tags[i]) > ((filter_result_t*)heap_ver_max(heap))->value) {
                    free(heap_desencolar(heap));

                    filter_result_t *result = filter_result_crear(tags[i], count_min_sketch_obtener(filter, tags[i]));
                    if (!result) {
                        free_strv(lineas);
                        count_min_sketch_destruir(filter);
                        heap_destruir(heap, free);
                        free_strv(lineas);
                        fprintf(stderr, "Unexpected error.\n");
                        return 1;
                    }

                    heap_encolar(heap, result);
                }
            }
        }

        free_strv(tags);
    }

    while (!heap_esta_vacio(heap)){
        filter_result_t* filter_result = heap_desencolar(heap);
        imprimir_tweets(filter_result->key);
        filter_result_destruir(filter_result);
    }

    free_strv(lineas);
    count_min_sketch_destruir(filter);
    heap_destruir(heap, free);

    return 0;
}

int main(int argc, char const *argv[]){
    freopen("tweets_head.txt","r",stdin);
    if (argc != 3){
        //TODO: Checkear que n y k sean numeros enteros positivos distintos de cero
        fprintf(stderr, "Usage: ./procesar_tweets <n> <k>\n");
        return 1;
    }

    return procesar_tweets((size_t) atoi(argv[1]), (size_t) atoi(argv[2]));
}
