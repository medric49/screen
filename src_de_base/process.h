#include <stdlib.h>
#include <stddef.h>


#define TAILLEMAXNOM 50
#define TAILLEPILE 512
#define NOMBREDEPROCESSUS 8

enum Etat {ELU, ACTIVABLE};

typedef struct {
    int pid;
    char nom[TAILLEMAXNOM];
    enum Etat etat;
    int registres[5];
    int pile[TAILLEPILE];
} Process;

Process *tableProcessus[NOMBREDEPROCESSUS];
Process* actifProcess;
int maxpid;



void ctx_sw(int* old, int* new);
void init_processus();
int cree_processus(void (*code)(void), char *nom);
void proc();
void ordonnance();
char* mon_nom();
int mon_pid();