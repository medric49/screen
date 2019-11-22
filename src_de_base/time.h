#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>


void print_top_right(char *txt, size_t length);
void tic_PIT();
void init_traitant_IT(int8_t num_IT, void(*traitant)(void));
void param_horloge();
void masque_IRQ(uint32_t num_IRQ, bool masque);
void traitant_IT_32();
size_t getTime();