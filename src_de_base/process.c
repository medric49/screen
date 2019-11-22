#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <malloc.c.h>
#include <cpu.h>

void init_processus() {
    maxpid = -1;
    startActivable = NULL;
    endActivable = NULL;

    char nom[7] = "idle";
    for(int i = 0; i < NOMBREDEPROCESSUS; i++) {
        if(i != 0)
            sprintf(nom, "proc%d", i);
        cree_processus(proc, nom);
    }
    actifProcess = shift();
    
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
        if(maxpid != 0) {
            process->pile[TAILLEPILE - 1] = (int)code;
            process->registres[1] = (int)(process->pile + TAILLEPILE - 1);
        }
        
        // tableProcessus[maxpid] = process;

        push(process);
        return maxpid;
    }
    return -1;
}

void ordonnance() {
    // Process *tmp = actifProcess;
    push(actifProcess);
    actifProcess = shift();
    
    // tableProcessus[(tmp->pid+1)%NOMBREDEPROCESSUS];
    // tmp->etat = ACTIVABLE;
    // actifProcess->etat = ELU;
    ctx_sw(endActivable->registres, actifProcess->registres);
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
        sti();
        hlt();
        cli();
    }
}

Process *shift() {
    if(startActivable == NULL)
        return NULL;
    Process *tmp = startActivable;
    startActivable = startActivable->suiv;
    tmp->etat = ELU;

    if(startActivable == NULL)
        endActivable = NULL;
    
    return tmp;
}

void push(Process *process) {
    if(endActivable != NULL)
        endActivable->suiv = process;
    process->suiv = NULL;
    endActivable = process;
    endActivable->etat = ACTIVABLE;
    if(startActivable == NULL)
        startActivable = process;
}
