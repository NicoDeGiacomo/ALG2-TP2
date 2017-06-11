#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char **split(char *str, int sep, size_t *tam) {

    size_t largo = strlen(str);
    char buffer[largo]; //El largo maximo es el numero de caracteres (Ningun separador)

    //Largo: Cantidad de separadores + 1 - Le tengo que sumar otro para el NULL del final
    int cant_sep = 0;
    for (int g = 0; str[g]; g++)
        if(str[g]==sep)
            cant_sep++;
    char** strv = malloc(sizeof(char*) * (cant_sep + 2));
    if (!strv)
        return NULL;

    int count = 0;
    for (int i = 0, j = 0; i <= largo; ++i, ++j) {

        if(str[i] == sep || str[i] == '\0'){
            buffer[j] = '\0';
            strv[count] = malloc(sizeof(char) * (j + 1 )); //Largo del buffer + '\0'
            if(!strv[count]){
                while (count > 0)
                    free(strv[count--]);
                free(strv);
            }
            strcpy(strv[count], buffer);
            count ++;
            j = -1; //++j lo va a poner en cero
            continue;
        }

        buffer[j] = str[i];
    }

    strv[count] = NULL; //Marco el final
    *tam = (size_t) count;
    return strv;
}

void free_strv(char *strv[]){
    int count = 0;
    char* a = strv[count];
    while (a){
        free(a);
        count ++;
        a = strv[count];
    }
    free(strv);
}

//Obtiene n lineas del archivo (para si llegó al eof).
//Si n es NULL lee el archivo completo.
//Devuelve un array (dinamically allocated) terminado en NULL con las lineas (dynamically allocated) en forma de string.
//Cant es la cantidad de lineas leidas
char **obtener_lineas(FILE *file, size_t n, size_t *cant) {

    size_t var_size = 100;
    char** var = malloc(sizeof(char*) * (var_size));

    for (size_t i = 0; (!n)||(i < n); ++i) {

        size_t size = 100;
        char *str = malloc(sizeof(char)*size);
        if(!str)
            return NULL;

        size_t len = 0;
        int ch=fgetc(file);
        if(ch == EOF){
            if (!n)
                n = i;
            break;
        }
        while(ch != '\n'){
            if(len==size){
                str = realloc(str, sizeof(char)*(size+=size));
                if(!str)
                    return NULL;
            }
            str[len]= (char) ch;
            len++;
            ch=fgetc(file);
        }
        str[len]='\0';
        if (i == var_size-1){
            var = realloc(var, sizeof(char*)*(var_size+=var_size));
        }
        var[i] = str;
    }

    var[n] = NULL;
    if(cant)
        *cant = n;

    return var;
}
