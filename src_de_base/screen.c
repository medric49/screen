#include <stdlib.h>
#include <inttypes.h>
#include <screen.h>


#define PORT_COM 0x3d4
#define PORT_DON 0x3d5

#define TOP_LINE 0
#define START_LINE 1
#define START_COL 0

uint32_t lig0 = 0;
uint32_t col0 = 0;

uint8_t c_text0 = 15;
uint8_t c_fond0 = 0;
uint8_t cli0 = 0;

uint16_t *ptr_mem(uint32_t lig, uint32_t col) {
    return (uint16_t*)(2*(lig*80 + col) +0xB8000);
}

void ecrit_car(uint32_t lig, uint32_t col, char c, uint8_t c_text, uint8_t c_fond, uint8_t cli) {
    uint16_t* addr = ptr_mem(lig, col);
    *addr = c | (cli<<15) | (c_fond<<12) | (c_text<<8) ;
}

void efface_ecran(uint8_t tout) {
    int i;
    int j;

    for(i = (tout==0?START_LINE:TOP_LINE); i< 25; i++) {
        for(j=0; j<80; j++) {
            *ptr_mem(i,j) = 32 | (0<<15) | (0<<12) | (15<<8);
        }
    }
    lig0 = START_LINE;
    col0 = START_COL;
    place_curseur(lig0, col0);
}

void place_curseur(uint32_t lig, uint32_t col) {
    uint16_t pos =(uint16_t)(lig*80+col);
    outb(0x0F, PORT_COM);
    outb((uint8_t)((pos<<8)>>8), PORT_DON);
    outb(0x0E, PORT_COM);
    outb((uint8_t)(pos>>8), PORT_DON);
}

void traite_car(char c) {
    uint8_t code = (uint8_t) c;

    if ( code >= 32 && code <= 126 ) {
        ecrit_car(lig0, col0, c, c_text0, c_fond0, cli0);
        col0 = (col0+1)%80;
        if (col0 == 0){
          lig0 ++;
        }
    } else {
        switch (code) {
            case 8:
              if (col0>0){
                col0 --;
              }
              break;
            case 9:
              col0 = ((col0 / 8)+1)*8;
              if (col0 >79){
                col0 = 79;
              }
              break;
            case 10:
              lig0 ++;
              col0 = 0;
              break;
            case 12:
              efface_ecran(0);
              break;
            case 13:
              col0 = 0;
              break;
        }
    }
    if(lig0 >= 25)
          defilement();
    place_curseur(lig0, col0);
}

void defilement() {
  uint16_t *l0 = ptr_mem(START_LINE,START_COL);
  uint16_t *l1 = ptr_mem(START_LINE+1,START_COL);
  memmove(l0, l1, 2*80*(25 - (START_LINE + 1) ));
  int j;
  for(j=0; j<80; j++) {
    *ptr_mem(24,j) = 32 | (0<<15) | (0<<12) | (15<<8);
  }
  lig0 = 24;
  col0 = START_COL;
  place_curseur(lig0, col0);
}

void console_putbytes(char *chaine, int32_t taille) {
  int i;
  for (i = 0; i<taille; i++) {
    traite_car(chaine[i]);
  }
}


