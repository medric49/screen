#include <stdlib.h>
#include <cpu.h>
#include <string.h>
#include <inttypes.h>

uint16_t *ptr_mem(uint32_t lig, uint32_t col);
void ecrit_car(uint32_t lig, uint32_t col, char c, uint8_t c_text, uint8_t c_fond, uint8_t cli);
void place_curseur(uint32_t lig, uint32_t col);
void traite_car(char c);
void defilement();
void console_putbytes(char *chaine, int32_t taille);
void efface_ecran();


