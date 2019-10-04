#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <cpu.h>
#include <screen.h>

#include <time.h>

uint64_t t = 0;
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
}

void init_traitant_IT(int32_t num_IT, void(*traitant)(void) ) {
    uint16_t *pos = 0x1000 + 32; // Position du traitant d'interruption


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
    outb(masques | (1<32) , 0x21); // Demasque le masque sur l'interruption 32, celle de l'horloge

}