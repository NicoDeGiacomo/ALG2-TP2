#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEPARADOR ','

char **split(char *str, int sep, size_t *pInt);

void free_strv(char *strv[]);

//Obtiene n lineas del archivo (para si llegó al eof).
//Si n es NULL lee el archivo completo.
//Devuelve un array (dinamically allocated) terminado en NULL con las lineas (dynamically allocated) en forma de string.
char **obtener_lineas(FILE *file, size_t n, size_t *pInt);
