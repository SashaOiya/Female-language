#ifndef BACK_END
#define BACK_END

#include "front.h"

Errors_t Back_End ( struct Language_t *language, char *output_file_name );
void Back_End_Body ( struct Language_t *language, const struct Node_t *tree_node );
const char *Get_Op_Asm_Code ( const int value );
void Back_End_Count_Id ( struct Language_t *language, const struct Node_t *tree_node, int *add_id );

#endif //BACK_END
