#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "syscall.h"

int trace_active = 0;

/*
 * ================================
 * CONTADOR DE SYSCALLS (ENTREGABLE 3)
 * ================================
 */
int syscall_count[NSYSCALLS];

/*
 * ================================
 * NOMBRES DE SYSCALLS (ENTREGABLE 1)
 * ================================
 */
static char *syscall_names[NSYSCALLS] = {
  [SYS_fork]    "fork",
  [SYS_exit]    "exit",
  [SYS_wait]    "wait",
  [SYS_pipe]    "pipe",
  [SYS_read]    "read",
  [SYS_kill]    "kill",
  [SYS_exec]    "exec",
  [SYS_fstat]   "fstat",
  [SYS_chdir]   "chdir",
  [SYS_dup]     "dup",
  [SYS_getpid]  "getpid",
  [SYS_sbrk]    "sbrk",
  [SYS_sleep]   "sleep",
  [SYS_uptime]  "uptime",
  [SYS_open]    "open",
  [SYS_write]   "write",
  [SYS_mknod]   "mknod",
  [SYS_unlink]  "unlink",
  [SYS_link]    "link",
  [SYS_mkdir]   "mkdir",
  [SYS_close]   "close",
  [SYS_trace]   "trace",
  [SYS_getprocs] "getprocs",
  [SYS_getprocsinfo] "getprocsinfo",
  [SYS_getsyscount] "getsyscount",
};

/*
 * Fetch the int at addr from the current process.
 */
int
fetchint(uint addr, int *ip)
{
  struct proc *curproc = myproc();

  if(addr >= curproc->sz || addr+4 > curproc->sz)
    return -1;
  *ip = *(int*)(addr);
  return 0;
}

/*
 * Fetch the nul-terminated string at addr from the current process.
 */
int
fetchstr(uint addr, char **pp)
{
  char *s, *ep;
  struct proc *curproc = myproc();

  if(addr >= curproc->sz)
    return -1;
  *pp = (char*)addr;
  ep = (char*)curproc->sz;
  for(s = *pp; s < ep; s++){
    if(*s == 0)
      return s - *pp;
  }
  return -1;
}

int
argint(int n, int *ip)
{
  return fetchint((myproc()->tf->esp) + 4 + 4*n, ip);
}

int
argptr(int n, char **pp, int size)
{
  int i;

  if(argint(n, &i) < 0)
    return -1;
  if(size < 0 || (uint)i >= myproc()->sz || (uint)i+size > myproc()->sz)
    return -1;
  *pp = (char*)i;
  return 0;
}

int
argstr(int n, char **pp)
{
  int addr;

  if(argint(n, &addr) < 0)
    return -1;
  return fetchstr(addr, pp);
}

/*
 * Declaración de syscalls
 */
extern int sys_fork(void);
extern int sys_exit(void);
extern int sys_wait(void);
extern int sys_pipe(void);
extern int sys_read(void);
extern int sys_kill(void);
extern int sys_exec(void);
extern int sys_fstat(void);
extern int sys_chdir(void);
extern int sys_dup(void);
extern int sys_getpid(void);
extern int sys_sbrk(void);
extern int sys_sleep(void);
extern int sys_uptime(void);
extern int sys_open(void);
extern int sys_write(void);
extern int sys_mknod(void);
extern int sys_unlink(void);
extern int sys_link(void);
extern int sys_mkdir(void);
extern int sys_close(void);
extern int sys_trace(void);
extern int sys_getprocs(void);
extern int sys_getprocsinfo(void);
extern int sys_getsyscount(void);

/*
 * Tabla de syscalls
 */
static int (*syscalls[])(void) = {
  [SYS_fork]    sys_fork,
  [SYS_exit]    sys_exit,
  [SYS_wait]    sys_wait,
  [SYS_pipe]    sys_pipe,
  [SYS_read]    sys_read,
  [SYS_kill]    sys_kill,
  [SYS_exec]    sys_exec,
  [SYS_fstat]   sys_fstat,
  [SYS_chdir]   sys_chdir,
  [SYS_dup]     sys_dup,
  [SYS_getpid]  sys_getpid,
  [SYS_sbrk]    sys_sbrk,
  [SYS_sleep]   sys_sleep,
  [SYS_uptime]  sys_uptime,
  [SYS_open]    sys_open,
  [SYS_write]   sys_write,
  [SYS_mknod]   sys_mknod,
  [SYS_unlink]  sys_unlink,
  [SYS_link]    sys_link,
  [SYS_mkdir]   sys_mkdir,
  [SYS_close]   sys_close,
  [SYS_trace]   sys_trace,
  [SYS_getprocs] sys_getprocs,
  [SYS_getprocsinfo] sys_getprocsinfo,
  [SYS_getsyscount] sys_getsyscount,
};

/*
 * Manejador principal de syscalls
 */
void
syscall(void)
{
  int num;
  struct proc *p = myproc();

  num = p->tf->eax;
  if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {

    /* Contador de syscalls */
    syscall_count[num]++;

    /* ================================
     * TRACE (ENTREGABLE 1)
     * ================================ */
    if(trace_active){
      cprintf("syscall %s(", syscall_names[num]);

      if(num == SYS_read || num == SYS_write){
        int fd, n;
        argint(0, &fd);
        argint(2, &n);
        cprintf("fd=%d, n=%d", fd, n);
      }
      else if(num == SYS_open){
        char *path;
        int mode;
        argstr(0, &path);
        argint(1, &mode);
        cprintf("path=%p, mode=%d", path, mode);
      }
      else if(num == SYS_exec){
        char *path;
        argstr(0, &path);
        cprintf("path=%p", path);
      }

      cprintf(")\n");
    }

    p->tf->eax = syscalls[num]();
  } else {
    cprintf("%d %s: unknown sys call %d\n",
            p->pid, p->name, num);
    p->tf->eax = -1;
  }
}
