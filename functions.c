#include "functions.h"

u_int16_t *ptr_mem(u_int32_t lig, u_int32_t col) {
    return (u_int16_t*)(2*(lig*80 + col) +0xB8000);
}

void ecrit_car(u_int32_t lig, u_int32_t col, char c, u_int8_t c_text, u_int8_t c_fond, u_int8_t cli) {
    u_int16_t* addr = ptr_mem(lig, col);
    *addr = c | (cli<<15) | (c_fond<<12) | (c_text<<8) ;
}

void efface_ecran() {
    int i;
    int j;
    for(i; i< 25; i++) {
        for(j; j<80; j++) {
            *ptr_mem(i,j) = 32 | (0<<15) | (0<<12) | (15<<8);
        }
    }
}

void place_curseur(u_int32_t lig, u_int32_t col) {
    outb(PORT_COM, 0x0F);

    outb(PORT_DON, 0x0E);
}