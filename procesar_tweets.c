#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void procesar_tweets(int n, int k);

void procesar_tweets(int n, int k){

}

int main(int argc, char const *argv[]){
    if (argc != 3){
        fprintf(stderr, "Please provide 2 arguments\n");
        return 1;
    }

    procesar_tweets(atoi(argv[1]), atoi(argv[2]));
    return 0;
}