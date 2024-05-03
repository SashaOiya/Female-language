#ifndef BACK_END
#define BACK_END

#include "front.h"

void Back_End ( struct Language_t *language );
void Back_End_Body ( struct Language_t *language, const struct Node_t *tree_node );
const char *Get_Op_Asm_Code ( const int value );

#endif //BACK_END
