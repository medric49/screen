#include <stdlib.h>
#include <stddef.h>

#define TAILLEMAXNOM 50
#define TAILLEPILE 512
#define NOMBREDEPROCESSUS 2

enum Etat {ELU, ACTIVABLE};

typedef struct {
    int pid;
    char nom[TAILLEMAXNOM];
    enum Etat etat;
    int registres[5];
    int pile[TAILLEPILE];
} Process;

Process tableProcessus[NOMBREDEPROCESSUS];
Process* actifProcess;



void ctx_sw(int* old, int* new);
void init_processus();
void idle();
void proc1();
void ordonnance();
char* mon_nom();
int mon_pid();