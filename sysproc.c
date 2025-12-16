#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "uproc.h"
#include "syscall.h"

extern int syscall_count[NSYSCALLS];

extern int trace_active;

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_trace(void)
{
  int n;

  // argint(0, &n) busca el primer argumento (índice 0) y lo guarda en 'n'.
  // Si devuelve < 0, significa que hubo un error al leer.
  if(argint(0, &n) < 0)
    return -1;

  // Asignamos el valor recibido a nuestra variable de control
  trace_active = n;

  return 0; // Retornamos 0 indicando éxito
}

int
sys_getprocs(void)
{
  return get_process_count();
}

int
sys_getprocsinfo(void)
{
  struct uproc *table;
  int max;

  // Leemos los argumentos: (puntero, entero)
  if(argptr(0, (void*)&table, sizeof(*table)) < 0 || argint(1, &max) < 0)
    return -1;

  return get_process_info(table, max);
}

/*
 * Syscall ENTREGABLE 3:
 * devuelve el número de invocaciones de una syscall
 */
int
sys_getsyscount(void)
{
  int num;

  if(argint(0, &num) < 0)
    return -1;

  if(num < 0 || num >= NSYSCALLS)
    return -1;

  return syscall_count[num];
}
