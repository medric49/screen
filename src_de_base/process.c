#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <malloc.c.h>

void init_processus() {
    maxpid = -1;
    char nom[7] = "idle";
    for(int i = 0; i < TAILLEMAXNOM; i++) {
        if(i != 0)
            sprintf(nom, "proc%d", i);
        cree_processus(proc, nom);
    }
    actifProcess = tableProcessus[0];
    actifProcess->etat = ELU;
}

int cree_processus(void (*code)(void), char *nom) {
    if(maxpid < NOMBREDEPROCESSUS-1) {
        maxpid ++;
        Process *process = malloc(sizeof(Process));
        process->pid = maxpid;
        sprintf(process->nom, nom);
        process->etat = ACTIVABLE;
        if(maxpid != 0) {
            process->pile[TAILLEPILE - 1] = (int)code;
            process->registres[1] = (int)(process->pile + TAILLEPILE - 1);
        }
        tableProcessus[maxpid] = process;
        return maxpid;
    }
    return -1;
}

void ordonnance() {
    Process *tmp = actifProcess;
    actifProcess = tableProcessus[(tmp->pid+1)%NOMBREDEPROCESSUS];
    tmp->etat = ACTIVABLE;
    actifProcess->etat = ELU;
    ctx_sw(tmp->registres, actifProcess->registres);
}

char* mon_nom() {
    return actifProcess->nom;
}
int mon_pid() {
    return actifProcess->pid;
}


void proc() {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        ordonnance();
    }
}

