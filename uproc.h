#ifndef _UPROC_H_   
#define _UPROC_H_

struct uproc {
  int pid;
  int state;
  uint sz;       // Tamaño en memoria
  char name[16]; // Nombre del proceso
};

#endif
