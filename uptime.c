#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int ticks = uptime();     // Syscall existente en XV6
  int procs = getprocs();   // La nueva syscall
  
  // En XV6, 100 ticks son aprox 1 segundo.
  int seconds = ticks / 100;
  int decimal = ticks % 100;

  printf(1, "Sistema operativo XV6\n");
  printf(1, "---------------------\n");
  printf(1, "Tiempo activo : %d.%d segundos (%d ticks)\n", seconds, decimal, ticks);
  printf(1, "Procesos activos: %d\n", procs);
  
  exit();
}
