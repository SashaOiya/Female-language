#include "recurs_des.h"
#include "front.h"
#include "back.h"
#include "log.h"
#include "midl.h"

static const int DUMP_COUNTER = 100;

int main ( int argc, char *argv[] )
{
    struct Language_t language = {};
                                               // errors
    Language_Ctor ( &language, ( argc > 0 ) ? argv[1] : nullptr );

    language.tree.start = Get_General ( &language );
    Optimization ( language.tree.start );

    Tree_Text_Dump ( &language, language.tree.start );
    Tree_Graph_Dump ( &language );

    Back_End ( &language, argv[2] );   // hlt
    Language_Dtor ( &language );

    return 0;
}

Errors_t Language_Ctor ( struct Language_t *language, char *input_file_name ) // wtf baaad   + dtor
{
    assert ( language != nullptr );
    assert ( input_file_name != nullptr );
    //assert ( output_file_name != nullptr );

    FILE *input_f = fopen ( input_file_name, "r" );
    if ( !input_f ) {

        return ERR_FOPEN;
    }

    language->name_cell = (Name_Cell_t *)calloc ( language->cell_n * sizeof ( Name_Cell_t ), 1);
    if ( language->name_cell == nullptr ) {

        return ERR_CALLO;
    }

    Errors_t ret_err = File_Reader ( &language->file, input_f );
    /*if ( ret_err != OK_FILE ) {

        return ret_err;
    }*/

    language->file.out_buffer = File_Skip_Spaces ( language->file.out_buffer, language->file.file_size );
    if ( language->file.out_buffer == nullptr ) {

        return ERR_CALLO;
    }
    // dtor all

    Dymanic_Array_Ctor ( &language->d_array );   // dtor
    printf ( "%s\n", language->file.out_buffer );
    Search_Tokens ( language );
    // errors

    free ( language->file.out_buffer );
    fclose ( input_f );

    language->operator_counter = Search_Free_Cell ( language );

    return NO_ERR;
}

Errors_t File_Reader ( struct File_t *file, FILE *input_f )  // +
{
    assert ( file != nullptr );
    assert ( input_f != nullptr );

    file->file_size = Get_File_Size ( input_f );

    file->out_buffer = ( char *)calloc ( file->file_size + 1, sizeof ( char ) );
    if ( !file->out_buffer ) {

        return ERR_CALLO;
    }

    int ret_code = fread ( file->out_buffer, sizeof( file->out_buffer[0] ), file->file_size, input_f );
    if ( ret_code != file->file_size  ) {
        if ( feof ( input_f ) ) {
            printf ( "Error reading front text file: unexpected end of file\n" );
        }
        else if ( ferror ( input_f ) ) {
            perror ( "Error reading test.bin" );
        }
       return ERR_FREAD;
    }

    return OK_FILE;
}

int Get_File_Size ( FILE * f ) // +
{
    int prev = ftell ( f );

    fseek ( f, 0, SEEK_END );
    size_t size_not_blue = ftell ( f );
    fseek ( f, prev, SEEK_SET );

    return size_not_blue;
}

char *File_Skip_Spaces ( char *data, int file_size )   // +
{
    assert ( data != nullptr );
    assert ( file_size != 0  );

    char *buffer = (char *)calloc ( ( file_size + 1 ) * sizeof ( data[0] ), 1 );
    if ( buffer == nullptr ) {

        return buffer;
    }

    int position = 0;

    for ( ; *data != '\0'; ++data ) {
        while ( isspace ( *data ) ) {
            ++data;
        }
        buffer[position++] = *data;
    }
    buffer[position] = '\0';
    free ( data );

    return buffer;
}

Errors_t Search_Tokens ( struct Language_t *language )  // copy_pasta
{
    assert ( language != nullptr );

    for ( int i = 0; i < language->file.file_size; ) {
        char element_name[max_name_size] = {};
        int counter = 0;
        Token_t token = {};

        for ( ; isalpha ( language->file.out_buffer[i] ) ; ++i, ++counter )  {    //
            element_name[counter] = language->file.out_buffer[i];
        }
$       element_name[counter] = '\0';

        if ( counter > 0 ) {   /////// howwwwwwwwwwwwwww :((((
            if ( strcmp ( element_name, "itakenooffence" ) == 0 ) {    // � �� ��������
                token.type = NODE_TYPE_IF;
            }
            else if ( strcmp ( element_name, "iamoffended" ) == 0 ) {     // � ���������
                token.type = NODE_TYPE_ELSE;
            }
            else if ( strcmp ( element_name, "oneminute" ) == 0 ) {    // ��� ������
                token.type = NODE_TYPE_WHILE;
            }
            else if ( strcmp ( element_name, "leavemealone" ) == 0 ) {  // �������
                token.type = NODE_TYPE_RETURN;
            }
            else if ( strcmp ( element_name, "givememoney" ) == 0 ) {  // ��� ��� �����
                token.type = NODE_TYPE_INPUT;
            }
            else if ( strcmp ( element_name, "gototheshop" ) == 0 ) {  // ��� � �������
                token.type = NODE_TYPE_OUTPUT;
            }
            else if ( language->file.out_buffer[i] == '(' ) {
                token.type = NODE_TYPE_FUNC;
                token.cell_code = Search_Func_Var_Name ( language, element_name, token.type );
                if ( token.cell_code == energency_val ) {

                    return ERR_CELLR;
                }
$           }
            else {
$               token.type = NODE_TYPE_VAR;
$               token.cell_code = Search_Func_Var_Name ( language, element_name, token.type );
                if ( token.cell_code == energency_val ) {

                    return ERR_CELLR;
                }
$           }
        }
        else if ( counter == 0 ) {
            int sign = 1;
            if ( language->file.out_buffer[i] == '-' ) {
                if ( isalpha ( language->file.out_buffer[i-1] ) || isdigit ( language->file.out_buffer[i-1] ) ) {
                }
                else {
                    sign = -1;
                    ++i;
                }
            }

            for ( ; isdigit ( language->file.out_buffer[i] ) ; ++i, ++counter )  {    //
                element_name[counter] = language->file.out_buffer[i];
            }
$           element_name[counter] = '\0';

            if ( counter > 0 ) {
                token.type = NODE_TYPE_NUM;
$               token.cell_code = atoi ( element_name ) * sign;
            }
            else {
                element_name[counter++] = language->file.out_buffer[i];
                element_name[counter] = '\0';
                token.type = NODE_TYPE_OP;
$               token.cell_code = language->file.out_buffer[i];
                ++i;
            }
        }
$       token.data = strdup ( element_name );  // option
        Dynamic_Array_Push ( &(language->d_array), token );
    }
    Dynamic_Array_Dump ( &(language->d_array), INFORMATION );

    return NO_ERR;
}

int Search_Func_Var_Name ( struct Language_t *language, char* name, const Node_Type_t requested_type ) // +
{
    assert ( language != nullptr );
    assert ( name != nullptr );

    if ( requested_type != NODE_TYPE_VAR &&
         requested_type != NODE_TYPE_FUNC ) {

        return 0;
    }

    for ( int i = 0; i < language->cell_n; ++i ) {
$       if ( language->name_cell[i].type == requested_type ) {
            if ( strcmp ( name, language->name_cell[i].data ) == 0) {
$
                return language->name_cell[i].name_code;
            }
        }
    }
$
    int cell = Search_Free_Cell ( language );
    if ( cell == energency_val ) {

        return cell;
    }
    language->name_cell[cell].type = requested_type;
    language->name_cell[cell].data = strdup ( name );  // free
    language->name_cell[cell].name_code = cell;

    return cell;
}

int Search_Free_Cell ( struct Language_t *language )  // +
{
    assert ( language != nullptr );

    for ( int i = 0; i < language->cell_n; ++i ) {
        if ( language->name_cell[i].type == FREE_CELL ) {
$
            return i;
        }
    }
    language->cell_n *= cell_mul_coeff;
    const int n_bytes = ( language->cell_n / 2 ) * sizeof ( Name_Cell_t );
    language->name_cell = (Name_Cell_t *)realloc ( language->name_cell, n_bytes * 2 );  // free
    if ( language->name_cell == nullptr ) {

        return energency_val;
    }

    memset (  (Name_Cell_t *)( (char *)language->name_cell + n_bytes ), 0, n_bytes );
$
    return Search_Free_Cell ( language );
}

void Node_Free ( struct Node_t **tree_node ) // +
{
    if ( tree_node != nullptr && *tree_node != nullptr ) {
        Node_Free ( &((*tree_node)->left) );
        Node_Free ( &((*tree_node)->right));

        (*tree_node)->left = nullptr;
        (*tree_node)->right = nullptr;

        *tree_node = nullptr;
    }
}

void Language_Dtor ( struct Language_t *language ) // +?
{
    Node_Free ( &(language->tree.start) );

    Dynamic_Array_Dtor ( &(language->d_array) );

    for ( int i = 0; i < language->cell_n; ++i ) {
        free ( language->name_cell[i].data );
    }
    free ( language->name_cell );
}

////////////////////////////////////////////////////////////////

void Tree_Text_Dump ( const struct Language_t *language, const struct Node_t *tree_node ) // +
{
    if ( tree_node == nullptr) {

        return ;
    }
    printf ( " ( " );

    Tree_Text_Dump ( language, tree_node->left  );

    if ( tree_node->type == NODE_TYPE_NUM ) {
        printf ( "%d", tree_node->value );
    }
    else  {
        printf ( "%s", Get_Dump_Op_Name ( language, tree_node ) );
    }

    Tree_Text_Dump ( language, tree_node->right );

    printf ( " ) ");

}

Errors_t Tree_Graph_Dump ( const struct Language_t *language ) // +
{
    FILE *tree_dump = fopen ( "tree.dot", "w" );
    if ( !tree_dump ) {
        perror ( "File opening failed" );

        return ERR_FOPEN;
    }

    fprintf ( tree_dump, "digraph G { \n"
                         "node [shape = record];\n"
                         " \"%p\" ", language->tree.start );

    Tree_Dump_Body ( language, language->tree.start, tree_dump );

    fprintf ( tree_dump, "}\n" );
    fclose ( tree_dump );

    static int file_counter = 0;
    char command_buffer[DUMP_COUNTER] = {};
    fprintf( log(), "<img src=\"tree%d.png\" alt=\"-\" width=\"500\" height=\"600\">\n", file_counter );
    sprintf( command_buffer, "dot -T png tree.dot -o logs/tree%d.png", file_counter++ );
    system ( command_buffer );

    return OK_TREE;
}

void Tree_Dump_Body ( const struct Language_t *language, const struct Node_t *tree, FILE *tree_dump ) // +
{
    if ( tree == nullptr) {

        return ;
    }
    if ( tree->type == NODE_TYPE_NUM ) {
        fprintf ( tree_dump , " \"%p\" [shape = Mrecord, style = filled, fillcolor = lightpink "
                              " label = \"%d \"];\n",tree, tree->value );
    }
    else  {
        fprintf ( tree_dump, " \"%p\" [shape = Mrecord, style = filled, fillcolor = lightpink "
                             " label = \"%s \"];\n", tree, Get_Dump_Op_Name ( language, tree ) );
    }

    if ( tree->left != nullptr ) {
        fprintf ( tree_dump, "\"%p\" -> \"%p\" ", tree, tree->left );
    }

    if ( tree->right != nullptr ) {
        fprintf ( tree_dump, "\n \"%p\" -> \"%p\" \n", tree, tree->right );
    }

    Tree_Dump_Body ( language, tree->left,  tree_dump );
    Tree_Dump_Body ( language, tree->right, tree_dump );
}

const char *Get_Dump_Op_Name ( const struct Language_t *language, const struct Node_t *tree_node )
{
    assert ( language  != nullptr );
    assert ( tree_node != nullptr );

    switch ( tree_node->type ) {
        case NODE_TYPE_OP : {
            switch ( tree_node->value ) {
                case OP_ADD : {

                    return "+";
                }
                break;
                case OP_SUB : {

                    return "-";
                }
                break;
                case OP_DIV : {

                    return "/";
                }
                break;
                case OP_MUL : {

                    return "*";
                }
                break;
                case OP_VAR : {

                    return "x";
                }
                break;
                case OP_POW : {

                    return "^";
                }
                break;
                case OP_MORE : {

                    return "more";
                }
                break;
                case OP_LESS : {

                    return "less";
                }
                break;
                case OP_EQUAL : {

                    return "=";
                }
                break;
                case OP_SEMICLON : {

                    return ";";
                }
                break;
                case OP_W_BRA : {

                    return "{";
                }
                break;
                case CL_W_BRA : {

                    return "}";
                }
                break;
                case OP_CONNECT : {

                    return "`";
                }
                break;
                case OP_EQUIVALENCE : {

                    return "~";
                }
                break;
                default : {
                    printf ( "Error Get_Op_Name with option %c\n", tree_node->value );
                }
            }
        }
        break;
        case NODE_TYPE_IF : {

            return "if";
        }
        break;
        case NODE_TYPE_WHILE : {

            return "while";
        }
        break;
        case NODE_TYPE_ELSE : {

            return "else";
        }
        break;
        case NODE_TYPE_RETURN : {

            return "return";
        }
        break;
        case NODE_TYPE_INPUT : {

            return "input";
        }
        break;
        case NODE_TYPE_OUTPUT : {

            return "output";
        }
        break;
        case NODE_TYPE_VAR : {
            return language->name_cell[tree_node->value].data;
        }
        break;
        case NODE_TYPE_FUNC : {
$           return language->name_cell[tree_node->value].data;
        }
        break;
        default : {
            printf ( "Error in func \"sercg dump op name\" with type [%d]\n", tree_node->type );
        }
        break;
    }

    return "error";
}

/*Node_t *d ( const struct Node_t *tree )  // +
{
    if ( tree == nullptr ) {

        return nullptr;
    }
    if ( tree->type == NODE_TYPE_NUM ) {

        return Create_Node ( NODE_TYPE_NUM, 0, nullptr, nullptr );
    }
    else if ( tree->type == NODE_TYPE_VAR ) {

        return Create_Node ( NODE_TYPE_NUM, 1, nullptr, nullptr );
    }
    else if ( tree->type == NODE_TYPE_OP ){
        switch ( tree->value ) {
            case OP_ADD : {
                Node_t *tree_c = Create_Node ( NODE_TYPE_OP, OP_ADD, d ( tree->left ), d ( tree->right ) );
                Optimization ( tree_c );

                return tree_c;
                break;
            }
            case OP_SUB : {
                Node_t *tree_c = Create_Node ( NODE_TYPE_OP, OP_SUB, d ( tree->left ), d ( tree->right ) );
                Optimization ( tree_c );

                return tree_c;
                break;
            }
            case OP_MUL : {
                Node_t *tree_c = Create_Node ( NODE_TYPE_OP, OP_ADD, Create_Node ( NODE_TYPE_OP, OP_MUL, d ( tree->left ), c ( tree->right ) ),
                                                                     Create_Node ( NODE_TYPE_OP, OP_MUL, c ( tree->left ), d ( tree->right ) ) );
                Optimization ( tree_c );

                return tree_c;
                break;
            }
            case OP_DIV : {
                Node_t *tree_c = Create_Node ( NODE_TYPE_OP, OP_DIV, Create_Node ( NODE_TYPE_OP, OP_SUB, Create_Node ( NODE_TYPE_OP, OP_MUL, d ( tree->left ), c ( tree->right ) ),
                                                                                                         Create_Node ( NODE_TYPE_OP, OP_MUL, c ( tree->left ), d ( tree->right ) ) ),
                                                                     Create_Node ( NODE_TYPE_OP, OP_MUL, c ( tree->right ), c ( tree->right ) ) );
                Optimization ( tree_c );

                return tree_c;
                break;
            }
            default    : {
                printf ( "Option error\n" );
                break;
            }
        }
    }
    return nullptr;
}   */

Node_t *Copy_Node ( const struct Node_t *tree)
{
    if (tree == nullptr) return nullptr;
    Node_t* new_tree = (Node_t*)calloc ( 1, sizeof( Node_t ) );
    new_tree->type  = tree->type;
    new_tree->value = tree->value;
    new_tree->left  = Copy_Node(tree->left);
    new_tree->right = Copy_Node(tree->right);

    return new_tree;
}

Node_t *Create_Node ( Node_Type_t option, int value, struct Node_t *left, struct Node_t *right ) // +
{
    struct Node_t *tree_c = ( Node_t *)calloc ( 1, sizeof ( Node_t ) );
    if ( !tree_c ) {
        free ( tree_c );
    }
    tree_c->type = option;
    tree_c->value = value;
    tree_c->left = left;
    tree_c->right = right;

    return tree_c;
}

