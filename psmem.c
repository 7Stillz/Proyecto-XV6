#include "types.h"
#include "stat.h"
#include "user.h"
#include "uproc.h"

int
main(int argc, char *argv[])
{
  // Definimos el máximo de procesos a leer
  enum { MAX_PROCS = 64 };
  struct uproc table[MAX_PROCS];
  int i;
  
  // Nombres de los estados (Coinciden con proc.h)
  static char *states[] = {
    [0] "UNUSED  ",
    [1] "EMBRYO  ",
    [2] "SLEEPING",
    [3] "RUNNABLE",
    [4] "RUNNING ",
    [5] "ZOMBIE  "
  };

  // Llamamos a nuestra syscall
  // Pasamos el arreglo y el tamaño máximo
  int n = getprocsinfo(table, MAX_PROCS);

  if(n < 0){
    printf(2, "Error al obtener info de procesos\n");
    exit();
  }

  // Imprimimos la cabecera
  printf(1, "PID\tNombre\t\tEstado\t\tMemoria (Bytes)\n");
  printf(1, "---\t------\t\t------\t\t---------------\n");

  // Recorremos los datos recibidos
  for(i = 0; i < n; i++){
    // Filtramos estados invalidos por seguridad
    char *state_name = "UNKNOWN ";
    if(table[i].state >= 0 && table[i].state <= 5)
      state_name = states[table[i].state];

    printf(1, "%d\t%s\t\t%s\t%d\n", 
           table[i].pid, 
           table[i].name, 
           state_name, 
           table[i].sz);
  }

  exit();
}
