#include <stdio.h>
#include <stdlib.h>
#include "counting_filters.h"
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
    size_t cant = 0;
    char** lineas = obtener_lineas(stdin, n, &cant);

    counting_filter_t* filter = counting_filter_crear();
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


    for (size_t j = 0; j < cant; ++j){
        size_t tam = 0;
        char **tweets = split(lineas[j], ',', &tam);


        counting_filter_aumentar_arr(filter, (const char **) tweets, tam);

        for (size_t i = 0; i < tam; ++i) {

            filter_result_t *result = filter_result_crear(tweets[i], counting_filter_obtener(filter, tweets[i]));
            if (!result){
                counting_filter_destruir(filter);
                heap_destruir(heap, free);
                fprintf(stderr, "Unexpected error.\n");
                return 1;
            }

            heap_encolar(heap, result);
        }

        for (size_t i = k; i < n; ++i) {

            if (filter_result_cmp(lineas[i], heap_ver_max(heap)) == -1) {
                free(heap_desencolar(heap));

                filter_result_t *result = filter_result_crear(lineas[i], counting_filter_obtener(filter, lineas[i]));
                if (!result) {
                    counting_filter_destruir(filter);
                    heap_destruir(heap, free);
                    fprintf(stderr, "Unexpected error.\n");
                    return 1;
                }

                heap_encolar(heap, result);
            }
        }

        while (!heap_esta_vacio(heap)) {
            imprimir_tweets(((filter_result_t *) heap_desencolar(heap))->key);
        }

        free_strv(tweets);
    }
    free_strv(lineas);
    counting_filter_destruir(filter);
    heap_destruir(heap, NULL);

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
