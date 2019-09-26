#include "functions.h"

uint16_t *ptr_mem(uint32_t lig, uint32_t col) {
    return (uint16_t*)(2*(lig*80 + col) +0xB8000);
}

void ecrit_car(uint32_t lig, uint32_t col, char c, uint8_t c_text, uint8_t c_fond, uint8_t cli) {
    uint16_t* addr = ptr_mem(lig, col);
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

void place_curseur(uint32_t lig, uint32_t col) {
    uint16_t pos =(uint16_t)(lig*80+col);
    outb(0x0F, PORT_COM);
    outb((uint8_t)((pos<<8)>>8), PORT_DON)
    outb(0x0E, PORT_COM);
    outb((uint8_t)(pos>>8), PORT_DON)
}

void traite_car(char c, uint32_t* lig, uint32_t* col) {
    uint8_t code = (uint8_t) c;

    if ( code >= 32 && code <= 126 ) {
        ecrit_car(lig, col, c, 15, 0, 0);
        *col = (*col+1)%80;
        if (*col == 0){
          *lig ++;
        }
    } else {
        switch (code) {
            case 8:
              if (*col>0){
                *col --;
              }
              break;
            case 9:
              *col = ((*col / 8)+1)*8;
              if (*col >79){
                *col = 79;
              }
              break;
            case 10:
              *lig ++;
              *col = 0;
              break;
            case 12:
              efface_ecran();
              *lig = 0;
              *col = 0;
              break;
            case 13:
              *col = 0;
              break;
        }
    }
    place_curseur(*lig, *col);

}