#include <stdlib.h>
#include <cpu.h>
#include <string.h>

#define PORT_COM 0x3d4
#define PORT_DON 0x3d5

uint32_t lig0 = 0;
uint32_t col0 = 0;

uint8_t c_text0 = 13;
uint8_t c_fond0 = 1;
uint8_t cli0 = 0;



uint16_t *ptr_mem(uint32_t lig, uint32_t col);
void ecrit_car(uint32_t lig, uint32_t col, char c, uint8_t c_text, uint8_t c_fond, uint8_t cli);
void place_curseur(uint32_t lig, uint32_t col);
void traite_car(char c);
void defilement();
void console_putbytes(char *chaine, int32_t taille);


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
    for(i=0; i< 25; i++) {
        for(j=0; j<80; j++) {
            *ptr_mem(i,j) = 32 | (0<<15) | (0<<12) | (15<<8);
        }
    }
    lig0 = 0;
    col0 = 0;
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
              efface_ecran();
              lig0 = 0;
              col0 = 0;
              break;
            case 13:
              col0 = 0;
              break;
        }
    }
    place_curseur(lig0, col0);
}

void defilement() {
  uint16_t *l0 = ptr_mem(0,0);
  uint16_t *l1 = ptr_mem(1,0);
  memmove(l0, l1, 2*70*25);
  int j;
  for(j=0; j<80; j++) {
    *ptr_mem(24,j) = 32 | (0<<15) | (0<<12) | (15<<8);
  }
  lig0 = 24;
  col0 = 0;
  place_curseur(lig0, col0);
}

void console_putbytes(char *chaine, int32_t taille) {
  int i;
  for (i = 0; i<taille; i++) {
    traite_car(chaine[i]);
  }
}