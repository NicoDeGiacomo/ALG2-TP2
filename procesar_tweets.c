#include <stdio.h>
#include <stdlib.h>
#include "count_min_sketch.h"
#include "heap.h"
#include "utils.h"

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

filter_result_t *filter_result_crear(const char *key, size_t value) {
    filter_result_t* filter_result = malloc(sizeof(filter_result_t));
    filter_result->key = key;
    filter_result->value = value;
    return filter_result;
}

void imprimir_tweets(const char *tweet) {
    //TODO: No tengo idea como se debería imprimir esto.
    printf("%s", tweet);
}

int procesar_tweets(size_t n, size_t k){
    size_t n_lineas = 0;
    char** lineas = obtener_lineas(stdin, n, &n_lineas);

    count_min_sketch_t* filter = counting_filter_crear();
    if (!filter){
        fprintf(stderr, "Unexpected error.\n");
        return 1;
    }
	
    heap_t* heap = heap_crear(filter_result_cmp);
	if(!heap) {
		counting_filter_destruir(filter);
		fprintf(stderr, "Unexpected error.\n");
        return 1;
	}
	
	//TODO: ESTA CONTANDO AL USUARIO COMO OTRO TAG
    //TODO: Sacar hardcodeo de ','
    //TODO: Hay que preguntar/pensar como es que se hace para guardar el historico total + los n actuales (dos filtros distintos ?)


    for (size_t j = 0; j < n_lineas; ++j) {
        size_t n_tags = 0;
        char **tags = split(lineas[j], ',', &n_tags);



        size_t n_encolados = 0;

        counting_filter_aumentar_arr(filter, (const char **) tags, n_tags);

        for (size_t i = 0; i < n_tags; ++i) {

            if (n_encolados < k){
                filter_result_t *result = filter_result_crear(tags[i], counting_filter_obtener(filter, tags[i]));
                if (!result) {
                    counting_filter_destruir(filter);
                    heap_destruir(heap, free);
                    fprintf(stderr, "Unexpected error.\n");
                    return 1;
                }

                n_encolados++;
                heap_encolar(heap, result);

            }else{
                if (filter_result_cmp(tags[i], heap_ver_max(heap)) == -1) {
                    free(heap_desencolar(heap));

                    filter_result_t *result = filter_result_crear(tags[i], counting_filter_obtener(filter, tags[i]));
                    if (!result) {
                        counting_filter_destruir(filter);
                        heap_destruir(heap, free);
                        fprintf(stderr, "Unexpected error.\n");
                        return 1;
                    }

                    heap_encolar(heap, result);
                }
            }
        }

        free_strv(tags);
    }

    free_strv(lineas);
    counting_filter_destruir(filter);
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
