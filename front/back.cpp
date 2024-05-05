#include "back.h"

Errors_t Back_End ( struct Language_t *language, char *output_file_name )
{
    assert ( language != nullptr );
    language->out_file = fopen ( output_file_name, "w" );
    if ( !language->out_file ) {

        return ERR_FOPEN;
    }

    Back_End_Body ( language, language->tree.start );

    fclose ( language->out_file );

    return NO_ERR;
}
                                                  // const
void Back_End_Body ( struct Language_t *language, struct Node_t *tree_node )
{
    assert ( language != nullptr );

    if ( tree_node == nullptr ) {
$
        return ;
    }

    if ( tree_node->type == NODE_TYPE_IF && language->id % 2 == 0 ) {     // transport
        tree_node->type = NODE_TYPE_WHILE;
    }

    if ( tree_node->type == NODE_TYPE_FUNC && ( tree_node->right != nullptr )  ) {
$       fprintf ( language->out_file, "\n: %d;\t\\\\%s\n", language->name_cell[tree_node->value].name_code, language->name_cell[tree_node->value].data );
        language->tree.current_func = language->name_cell[tree_node->value].name_code;
    }
    else if ( tree_node->type == NODE_TYPE_FUNC && ( tree_node->right == nullptr )  ) {
$       fprintf ( language->out_file, "call %d;\n", language->name_cell[tree_node->value].name_code );
        language->id += 2;

        return ;
    }
    else if ( tree_node->type == NODE_TYPE_VAR ) {
$       fprintf ( language->out_file, "push r%cx;\n", language->name_cell[tree_node->value].name_code + 'a' );
        language->id += 2;
    }
    else if ( tree_node->type == NODE_TYPE_NUM ) {
$       fprintf ( language->out_file, "push %d;\n", tree_node->value );
        language->id += 2;
    }
    else if ( tree_node->type == NODE_TYPE_OP && tree_node->value == OP_POW ) {
        Back_End_Body ( language, tree_node->left  );
        Back_End_Body ( language, tree_node->right );
        language->id += 1;
        // push [1]
        fprintf ( language->out_file, "hlt;\n" );

        return ;
    }
    else if ( tree_node->type == NODE_TYPE_OP && tree_node->value == OP_EQUAL ) {
        Back_End_Body ( language, tree_node->right );
$       fprintf ( language->out_file, "pop r%cx;\n", language->name_cell[tree_node->left->value].name_code + 'a' );
        language->id += 2;

        return ;
    }
    else if ( tree_node->type == NODE_TYPE_OP && ( tree_node->value == OP_EQUIVALENCE ||
                                                   tree_node->value == OP_MORE        ||
                                                   tree_node->value == OP_LESS          ) ) {
        Back_End_Body ( language, tree_node->left  );
        Back_End_Body ( language, tree_node->right );
        language->id += 2;
$       fprintf ( language->out_file, "%s %d;\n", Get_Op_Asm_Code ( tree_node->value ), language->operator_counter );

        return ;
    }
    else if ( tree_node->type == NODE_TYPE_OP && tree_node->value != OP_CONNECT && tree_node->value != OP_SEMICLON ) {
        Back_End_Body ( language, tree_node->left  );
        Back_End_Body ( language, tree_node->right );
        language->id += 1;
$       fprintf ( language->out_file, "%s;\n", Get_Op_Asm_Code ( tree_node->value ) );

        return ;
    }
    else if ( tree_node->type == NODE_TYPE_RETURN ) {
        language->id += 1;
        Back_End_Body ( language, tree_node->left );
        if ( language->tree.current_func == 0 ) {
            fprintf ( language->out_file, "hlt;\n" );
        }
$       else {
            fprintf ( language->out_file, "ret;\n" );
        }

        return ;
    }
    else if ( tree_node->type == NODE_TYPE_IF || tree_node->type == NODE_TYPE_ELSE ) { // else
        Back_End_Body ( language, tree_node->left  );
        fprintf ( language->out_file, "call %d;\n", language->operator_counter + 1 );
        language->id += 2;
        int prev_operatot_counter = language->operator_counter + 1;
        fprintf ( language->out_file, "\n: %d;\n", language->operator_counter );

        language->operator_counter += 2;
        Back_End_Body ( language, tree_node->right );
        fprintf ( language->out_file, "ret;\n\n" ); // +1
        fprintf ( language->out_file, "\n: %d;\t\n", prev_operatot_counter );  ////
        fprintf ( language->out_file, "jmp %d;\n", language->id + 3 ); // +3
        //fprintf ( language->out_file, "ret;\n\n" );     // +4

        language->id += 3;

        return ;
    }
    else if ( tree_node->type == NODE_TYPE_WHILE ) {
        int prev_id = language->id;
        Back_End_Body ( language, tree_node->left );
        fprintf ( language->out_file, "mjm %d;\n", language->operator_counter + 1 );
        language->id += 2;
        int prev_operator_counter = language->operator_counter + 1;
        fprintf ( language->out_file, "\n: %d;\n", language->operator_counter );

        language->operator_counter += 2;
        Back_End_Body ( language, tree_node->right );
        fprintf ( language->out_file, "jmp %d;\n", prev_id );  // +2
        //fprintf ( language->out_file, "ret;\n\n" ); // +3
        fprintf ( language->out_file, "\n: %d;\t\n", prev_operator_counter );  ////
        fprintf ( language->out_file, "jmp %d;\n\n", language->id + 4 ); // +4
        //fprintf ( language->out_file, "ret;\n\n" );     // +5

        language->id += 2;

        return ;
    }

    Back_End_Body ( language, tree_node->left  );
    Back_End_Body ( language, tree_node->right );

}

const char *Get_Op_Asm_Code ( const int value )
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
        case OP_EQUIVALENCE : {

            return "je";
        }
        break;
        case OP_MORE : {

            return "jb";
        }
        break;
        case OP_LESS : {

            return "ja";
        }
        break;
        default : {
            printf ( "ASM CODE ERROR \n" );
        }
        break;
    }

    return "ASM CODE ERROR";
}

