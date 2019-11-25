#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <malloc.c.h>
#include <cpu.h>
#include <time.h>

void init_processus() {
    maxpid = -1;
    startActivable = NULL;
    endActivable = NULL;

    cree_processus(idle, "idle");
    cree_processus(proc1, "proc1");
    cree_processus(proc2, "proc2");
    cree_processus(proc3, "proc3");

    actifProcess = shiftActivable();
    
    // actifProcess->etat = ELU;
}

int cree_processus(void (*code)(void), char *nom) {
    if(maxpid < NOMBREDEPROCESSUS-1) {
        maxpid ++;
        Process *process = malloc(sizeof(Process));
        if (process == NULL)
            return -1;
        process->pid = maxpid;
        sprintf(process->nom,"%s", nom);
        process->etat = ACTIVABLE;
        process->eveil = 0;
        if(maxpid != 0) {
            process->pile[TAILLEPILE - 1] = (int)code;
            process->registres[1] = (int)(process->pile + TAILLEPILE - 1);
        }
        
        // tableProcessus[maxpid] = process;

        pushActivable(process);
        return maxpid;
    }
    return -1;
}

void ordonnance() {
    // Process *tmp = actifProcess;
    
    while(startEndormi != NULL && startEndormi->eveil <= getTime()) {
        Process *p = shiftEndormi();
        pushActivable(p);
    }
    pushActivable(actifProcess);
    actifProcess = shiftActivable();
    
    // tableProcessus[(tmp->pid+1)%NOMBREDEPROCESSUS];
    // tmp->etat = ACTIVABLE;
    // actifProcess->etat = ELU;
    
    if(endActivable != NULL) // S'il y a au moins un processus activable
        ctx_sw(endActivable->registres, actifProcess->registres);
}

char* mon_nom() {
    return actifProcess->nom;
}
int mon_pid() {
    return actifProcess->pid;
}

void idle() {
    for (;;) {
        sti();
        hlt();
        cli();
    }
}

void proc1() {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        dors(1);
    }
}
void proc2() {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        dors(2);
    }
}
void proc3() {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        dors(3);
    }
}

void dors(uint32_t nbr_secs) {
    actifProcess->eveil = nbr_secs+getTime();

    Process *tmp = actifProcess;
    pushEndormi(actifProcess);
    actifProcess = shiftActivable();

    ctx_sw(tmp->registres, actifProcess->registres);
}

Process *shiftActivable() {
    if(startActivable == NULL)
        return NULL;
    Process *tmp = startActivable;
    startActivable = startActivable->suiv;
    tmp->etat = ELU;

    if(startActivable == NULL)
        endActivable = NULL;
    tmp->suiv = NULL;
    return tmp;
}

void pushActivable(Process *process) {

    if(process == NULL)
        return;
    
    if(endActivable != NULL)
        endActivable->suiv = process;
    process->suiv = NULL;
    endActivable = process;
    endActivable->etat = ACTIVABLE;
    if(startActivable == NULL)
        startActivable = process;
}

Process *shiftEndormi() {
    if(startEndormi == NULL) // Si la liste est vide
        return NULL;

    Process *tmp = startEndormi;
    startEndormi = startEndormi->suiv;
    tmp->etat = ACTIVABLE;
    tmp->suiv = NULL;    
    return tmp;
}

void pushEndormi(Process *process) {
    if(process == NULL)
        return;
    
    process->etat = ENDORMI;
    if(startEndormi == NULL) { // Si la liste est vide
        startEndormi = process;
        process->suiv = NULL;
        return;
    }
    // Si la liste n'est pas vide

    Process *tmp2 = NULL;
    Process *tmp = startEndormi;

    while(tmp->suiv != NULL && tmp->eveil <= process->eveil) {
        tmp2 = tmp;
        tmp = tmp->suiv;
    }
        
    if(tmp->eveil <= process->eveil) {
        process->suiv = tmp->suiv;
        tmp->suiv = process;
    } else {
        if(tmp2 == NULL) {
            process->suiv = tmp;
            startEndormi = process;
        }
        else {
            process->suiv = tmp;
            tmp2->suiv = process;    
        }
    }
}
