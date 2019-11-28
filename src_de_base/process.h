#include <stdlib.h>
#include <stddef.h>
#include <inttypes.h>


#define TAILLEMAXNOM 50
#define TAILLEPILE 512
#define NOMBREDEPROCESSUS 8

enum Etat {ELU, ACTIVABLE, ENDORMI, MOURANT};

typedef struct Process {
    int pid;
    char nom[TAILLEMAXNOM];
    enum Etat etat;
    int registres[5];
    uint32_t pile[TAILLEPILE];
    struct Process* suiv;
    int eveil;
} Process;

Process *tableProcessus[NOMBREDEPROCESSUS];
Process* actifProcess;
int maxpid;

Process* startActivable;
Process* endActivable;

Process* startEndormi;

Process* startMourant;

void ctx_sw(int* old, int* new);
void init_processus();
int cree_processus(void (*code)(void), char *nom);
void idle();
void proc1();
void proc2();
void proc3();
void ordonnance();
char* mon_nom();
int mon_pid();
void dors(uint32_t nbr_secs);
void fin_processus();


void pushActivable(Process* process);
Process* shiftActivable();

void pushEndormi(Process* process);
Process* shiftEndormi();

void pushMourant(Process *process);
void cleanMourant();