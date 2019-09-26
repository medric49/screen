#include <stdlib.h>
#include <cpu.h>

#define PORT_COM 0x3d4
#define PORT_DON 0x3d5

uint16_t *ptr_mem(uint32_t lig, uint32_t col);
void ecrit_car(uint32_t lig, uint32_t col, char c, uint8_t c_text, uint8_t c_fond, uint8_t cli);
void place_curseur(uint32_t lig, uint32_t col);
void traite_car(char c, uint32_t* lig, uint32_t* col);