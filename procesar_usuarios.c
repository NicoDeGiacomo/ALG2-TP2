#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CountingFilters.h"

int procesar_usuarios(const char* name){
	
	FILE* tweets = fopen(name, "r");
	if(!tweets) {
		fprintf(stderr, "Unexpected error.\n");
		return 1;
	}

	counting_filter_t* filter = counting_filter_crear();
	if (!filter){
        fclose(tweets);
        fprintf(stderr, "Unexpected error.\n");
		return 1;
	}


	
	size_t esUsuario = 1;
	char charLectura = 0;
	char usuario[140]; //Max en MELI?
	
	while(!feof(tweets)) {
		charLectura = fgetc(tweets);
		
		//TODO: Append char a nombre usuario.
		
		if(charLectura == '\n') {
			//TODO: Reiniciar Usuario.
			esUsuario = 1;
			continue;
		}
		
		if(charLectura == ',') {
			if(!filtro_existe(bloom, usuario)) {
				agregar_filtro(bloom, usuario);
			}
			
			aumentar_filtro(bloom, usuario);
		}
	}
	
}

int main(int argc, char const *argv[]){
    if (argc != 2){
        fprintf(stderr, "Usage: ./procesar_tweets <inputfile>\n");
        return 1;
    }

    return procesar_usuarios(argv[1]);
}
