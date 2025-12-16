//Creamos el archivo syscount.c para el usuario

#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"

// Funcion para mostrar el syscount
int
main(int argc, char *argv[])
{
    int n;

    // Si es 1, no pide un parametro y te muestra un resumen de los syscall y sus llamadas
    if(argc == 1){
    for(int i = 0; i < NSYSCALLS; i++){
        int c = getsyscount(i);
        if(c > 0)
            printf(1, "syscall %d: %d\n", i, c);
        }
        exit();
    }
    // Si es 2, te pide el parametro (numero) para mostrarte las llamadas del syscall
    if(argc != 2){
        printf(1, "uso: syscount <num_syscall>\n");
        exit();
    }

    n = atoi(argv[1]);

    printf(1, "syscall %d fue llamada %d veces\n",
        n, getsyscount(n));

    exit();
}
