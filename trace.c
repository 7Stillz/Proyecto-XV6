#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int estado;

  // Verificamos si el usuario envió argumentos
  if(argc < 2){
    printf(2, "Uso: trace 0|1\n");
    exit();
  }

  // Convertimos el texto "0" o "1" a numero entero (atoi)
  estado = atoi(argv[1]);

  // Llamamos a nuestra nueva syscall
  trace(estado);

  exit();
}
