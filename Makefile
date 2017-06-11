# Flags de gcc. Se usan automáticamente para compilar *.c → *.o
CFLAGS := -g -std=c99 -Wall -Wformat=2
CFLAGS += -Wshadow -Wpointer-arith -Wunreachable-code
CFLAGS += -Wconversion -Wno-sign-conversion -Wbad-function-cast

# Se pueden comentar durante el desarrollo.
CFLAGS += -Werror

# Si se ejecuta `make` a secas, se compilará 'all'.
#
# También se puede invocar `make procesar_tweets` o `make procesar_usuarios` para
# compilar una parte del TP solamente.

all: procesar_tweets procesar_usuarios
procesar_tweets: heap.o count_min_sketch.o utils.o hash.o lista.o
procesar_usuarios: abb.o lista.o utils.o pila.o