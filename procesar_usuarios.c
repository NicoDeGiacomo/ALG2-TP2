#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void procesar_usuarios(const char* name);

void procesar_usuarios(const char* name){

}

int main(int argc, char const *argv[]){
    if (argc != 2){
        fprintf(stderr, "Please provide 1 arguments\n");
        return 1;
    }

    procesar_usuarios(argv[1]);
    return 0;
}