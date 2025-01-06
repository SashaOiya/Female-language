#ifndef BACK_END
#define BACK_END

#include "front.h"

Errors_t Back_End ( struct Language_t *language, char *output_file_name );
void Back_End_Body ( struct Language_t *language, struct Node_t *tree_node );
const char *Get_Op_Asm_Code ( const int value );

#endif //BACK_END
