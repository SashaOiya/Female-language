#include "back.h"

// call 0;
// hlt;

void Back_End ( struct Language_t *language, const struct Node_t *tree_node )
{
    assert ( language != nullptr );

    if ( tree_node == nullptr ) {

        return ;
    }
    if ( tree_node->type == NODE_TYPE_FUNC && ( tree_node->right != nullptr )  ) {  // push tree_node->right->value
        fprintf ( language->out_file, ": %d;\n", language->name_cell[tree_node->value].name_code );
    }
    else if ( tree_node->type == NODE_TYPE_FUNC && ( tree_node->right == nullptr )  ) {
        fprintf ( language->out_file, "call %d;\n", language->name_cell[tree_node->value].name_code );
    }
    else if ( tree_node->type == NODE_TYPE_OP && tree_node->value == OP_EQUAL ) {
        Back_End ( language, tree_node->right );
        fprintf ( language->out_file, "pop r%dx;\n", language->name_cell[tree_node->left->value].name_code + 'a' );
    }
    else if ( tree_node->type == NODE_TYPE_VAR ) {
        fprintf ( language->out_file, "push r%dx;\n", language->name_cell[tree_node->left->value].name_code + 'a' );
    }
    else if ( tree_node->type == NODE_TYPE_NUM ) {
        fprintf ( language->out_file, "push %d;\n", tree_node->value );
    }
    else if ( tree_node->type == NODE_TYPE_OP ) {
        fprintf ( language->out_file, "%s;\n", Get_Op_Asm_Code ( tree_node->value ) );
    }

    Back_End ( language, tree_node->left  );
    Back_End ( language, tree_node->right );

}

const char *Get_Op_Asm_Code ( int value )
{
    switch ( value ) {
        case OP_ADD : {

            return "add";
        }
        break;
        case OP_SUB : {

            return "sub";
        }
        break;
        case OP_DIV : {

            return "div";
        }
        break;
        case OP_MUL : {

            return "mul";
        }
        break;
        default : {
            printf ( "ASM CODE ERROR \n" );
        }
        break;
    }

    return "ASM CODE ERROR";
}


