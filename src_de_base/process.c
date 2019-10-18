#include <stdlib.h>
#include <stdio.h>
#include <process.h>

void init_processus() {
    tableProcessus[0].pid = 0;
    sprintf(tableProcessus[0].nom, "idle");
    tableProcessus[0].etat = ELU;
    
    tableProcessus[1].pid = 1;
    sprintf(tableProcessus[1].nom, "proc1");
    tableProcessus[1].etat = ACTIVABLE;
    tableProcessus[1].pile[TAILLEPILE - 1] =  (int)proc1;
    tableProcessus[1].registres[1] = (int)(tableProcessus[1].pile  + TAILLEPILE - 1);
    actifProcess = tableProcessus;
}


void idle()
{
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        ordonnance();
    }
}

void proc1() {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        ordonnance();
    }
}

void ordonnance() {
    actifProcess->etat = ACTIVABLE;
    if(actifProcess->pid == 0) {
        ctx_sw(actifProcess->registres, tableProcessus[1].registres);
        actifProcess = tableProcessus+1;
    } else {
        ctx_sw(actifProcess->registres, tableProcessus[0].registres);
        actifProcess = tableProcessus;
    }
    actifProcess->etat = ELU;
}

char* mon_nom() {
    return actifProcess->nom;
}
int mon_pid() {
    return actifProcess->pid;
}