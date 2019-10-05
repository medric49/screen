#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <cpu.h>
#include <segment.h>
#include <string.h>
#include <screen.h>

#include <time.h>

size_t t = 0;
uint32_t QUARTZ = 0x1234DD;
uint8_t CLOCKFREQ = 50;


void print_top_right(char *txt, size_t length) {
    size_t i;
    for (i = 0; i<length; i++) {
        ecrit_car(0, 80-length+i, txt[i], 5, 7, 0);
    }
}

void tic_PIT() {
    outb(0x20, 0x20); // Previent le controleur qu'on commence un traitant
    t++;
    char hours[12];
    sprintf(hours, "%ds", (t*50)/1000 );
    print_top_right(hours, strlen(hours));
}

void init_traitant_IT(int8_t num_IT, void(*traitant)(void) ) {
    uint64_t *pos = (uint64_t*) (0x1000 + num_IT*2 ); // Position du traitant d'interruption

    uint32_t addrT = (uint32_t)(*traitant);
    uint16_t secondT = (uint16_t) (addrT>>16);
    uint16_t firstT = (uint16_t)( addrT - ((uint32_t)secondT<<16) );

    uint32_t first = (uint32_t)( firstT | (KERNEL_CS<<16)) ;
    uint32_t second =  (uint32_t)((uint16_t)0x8E00 ) | ( (uint32_t)secondT << 16);
    *pos = ((uint64_t)first<<32)|second;
}

void param_horloge() {
    outb(0x34, 0x43);
    uint32_t tmp1 = QUARTZ / CLOCKFREQ;
    uint8_t tmp2 = (uint8_t)(tmp1 % 256);
    outb(tmp2, 0x40);
    outb((uint8_t)(tmp1-tmp2), 0x40);
}

void masque_IRQ(uint32_t num_IRQ, bool masque) {
    uint8_t masques = inb(0x21); // Recupère les masques d'interruption
    if (masque == 1)
        outb( masques | (1<<num_IRQ) , 0x21);
    else
        outb( masques & (0<<num_IRQ) , 0x21);
}