#include <stdlib.h>
#include <cpu.h>
#include <stdio.h>
#include <screen.h>
#include <time.h>
#include <process.h>



void kernel_start(void)
{
    param_horloge();
    masque_IRQ(0, 0);
    init_traitant_IT(32, traitant_IT_32);
    efface_ecran(1);
    printf("Vsion 1.0\nPowered by Medric\n\n");

    init_processus();

    proc();
    // sti();
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }

}

