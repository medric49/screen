#include <cpu.h>
#include <inttypes.h>
#include <stdio.h>
#include <screen.h>
#include <time.h>

// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display

// une fonction bien connue
/*
uint32_t fact(uint32_t n)
{
    uint32_t res;
    if (n <= 1) {
        res = 1;
    } else {
        res = fact(n - 1) * n;
    }
    return res;
}
 */


void kernel_start(void)
{
    param_horloge();
    masque_IRQ(0, 0);

    efface_ecran();
    printf("Vsion 1.0\nPowered by Medric\n\n\n");
    print_top_right("12:30", 5);

    /*
    uint32_t x = fact(5);
    // quand on saura gerer l'ecran, on pourra afficher x
    (void)x;
    // on ne doit jamais sortir de kernel_start
    */

    sti();
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }

}

