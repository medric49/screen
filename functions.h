#include <stdlib.h>
#include <cpu.h>

#define PORT_COM 0x3d4
#define PORT_DON 0x3d5

u_int16_t *ptr_mem(u_int32_t lig, u_int32_t col);
void ecrit_car(u_int32_t lig, u_int32_t col, char c, u_int8_t c_text, u_int8_t c_fond, u_int8_t cli);