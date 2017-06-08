//Nombre: Nicolás De Giácomo
//Padron: 99702
//Corrector: Matias Cano
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//TODO: HACER EL .H DE ESTE ARCHIVO
char** split(const char* str, char sep){

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
    return strv;
}

void concat(char s1[], size_t len1, char s2[]) {

    if (!s1 || !s2)
        return;

    for (int j = 0; s2[j] != '\0'; len1++, j++) {
        s1[len1] = s2[j];
    }

    s1[len1] = '\0';
}

char* join(char** strv, char sep){
    if (!strv)
        return "";

    size_t len = 0;
    int count = 0;
    //Tamaño output: suma de todos los largos de las cadenas en strv + cantidad de separadores requeridos
    while (strv[count])
        len += strlen(strv[count++]) + 1;
    if (!len){
        char* output = malloc(sizeof(char) * 1);
        strcpy(output, "");
        return output;
    }

    char* output = malloc(sizeof(char) * len );
    strcpy(output, "");

    size_t out_len = 0;
    char separator[2] = {sep, '\0'};

    count = 0;
    while (strv[count]){
        concat(output, out_len, strv[count]);
        out_len += strlen(strv[count]);
        count ++;
        if(strv[count]){
            concat(output, out_len, separator);
            out_len += strlen(separator);
        }
    }
    return output;
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

//Obtiene n lineas del archivo, y para si llegó al eof -> Las n lineas terminan en null
//TODO: SI N ES NULL LEE TODO EL ARCHIVO
char** obtener_lineas(FILE* file, size_t n){

    char** var = malloc(sizeof(char*) * (n + 1));

    for (size_t i = 0; i < n; ++i) {

        size_t size = 100;
        char *str = realloc(NULL, sizeof(char)*size);//size is start size
        if(!str)
            return NULL;

        size_t len = 0;
        int ch=fgetc(file);
        while(ch != '\n'){
            str[len]= (char) ch;
            len++;
            if(len==size){
                str = realloc(str, sizeof(char)*(size+=size));
                if(!str)
                    return NULL;
            }
            ch=fgetc(file);
        }
        str[len]='\0';
        var[i] = str;
    }

    return var;
}
