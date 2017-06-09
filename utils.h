#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **split(char *str, int sep, size_t *pInt);

void concat(char s1[], size_t len1, char s2[]);

char* join(char** strv, char sep);

void free_strv(char *strv[]);

//Obtiene n lineas del archivo (para si llegó al eof).
//Si n es NULL lee el archivo completo.
//Devuelve un array (dinamically allocated) terminado en NULL con las lineas (dynamically allocated) en forma de string.
char **obtener_lineas(FILE *file, size_t n, size_t *pInt);
