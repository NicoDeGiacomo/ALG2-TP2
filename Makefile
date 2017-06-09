# Flags de gcc. Se usan automáticamente para compilar *.c → *.o
CFLAGS := -g -std=c99 -Wall -Wformat=2
CFLAGS += -Wshadow -Wpointer-arith -Wunreachable-code
CFLAGS += -Wconversion -Wno-sign-conversion -Wbad-function-cast

# Se pueden comentar durante el desarrollo.
CFLAGS += -Werror

# Si se ejecuta `make` a secas, se compilarán procesar_tweets y procesar_usuarios.
#
# También se puede invocar `make procesar_tweets` o `make procesar_usuarios` para
# compilar una parte del TP solamente.

all: procesar_tweets procesar_usuarios hash.o lista.o heap.o counting_filters.o