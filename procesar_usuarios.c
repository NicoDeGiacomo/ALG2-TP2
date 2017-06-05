#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CountingFilters.h"

void procesar_usuarios(const char* name){
	
	FILE* tweets = fopen(name, "r");
	if(!tweets) {
		printf("No se pudo inicializar el archivo de prueba.\n");
		return;
	}
	
	filet_t* bloom = crear_filete();
	if(!bloom) {
		printf("Error de proceso de filet mignon.\n");
		fclose(tweets);
		return;
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
        fprintf(stderr, "Please provide 1 arguments\n");
        return 1;
    }

    procesar_usuarios(argv[1]);
    return 0;
}
