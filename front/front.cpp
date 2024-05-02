#include "recurs_des.h"
#include "front.h"
//#include "back.h"
// Dtor[[[[[[[[[[[[[[[[
static const int DUMP_COUNTER = 100;

int main ( int argc, char *argv[] )
{
    struct Language_t language = {};

    Language_Ctor ( &language, ( argc > 0 ) ? argv[1] : nullptr, argv[2] );

    language.if_while_code_counter = Search_Free_Cell ( &language );

    language.tree.start = Get_General ( &language );

    Tree_Text_Dump ( &language, language.tree.start );
    Tree_Graph_Dump ( &language );

    Back_End ( &language, language.tree.start );   // hlt

    Dynamic_Array_Dtor ( &(language.d_array) );

    return 0;
}

Errors_t Language_Ctor ( struct Language_t *language, char *input_file_name, char *output_file_name )
{
    assert ( language != nullptr );
    assert ( input_file_name != nullptr );
    assert ( output_file_name != nullptr );

    FILE *input_f = fopen ( input_file_name, "r" );
    if ( !input_f ) {

        return ERR_FOPEN;
    }

    language->name_cell = (Name_Cell_t *)calloc ( language->cell_n * sizeof ( Name_Cell_t ), 1);
    //if ( File_Reader ( &language->file, input_f ) != OK_FILE ) {

    //    return ERR_FREAD;
    //
    File_Reader ( &language->file, input_f );

    language->file.out_buffer = File_Skip_Spaces ( language->file.out_buffer, language->file.file_size );
    printf ( "%s\n", language->file.out_buffer ); // debug

    Dymanic_Array_Ctor ( &language->d_array );   // dtor
    Search_Tokens ( language );
    // errors

    fclose ( input_f );

    language->out_file = fopen ( output_file_name, "w" );
    if ( !language->out_file ) {

        return ERR_FOPEN;
    }

    return NO_ERR;
}

void Search_Tokens ( struct Language_t *language )
{
    assert ( language != nullptr );

    for ( int i = 0; i < language->file.file_size; ) {
        char element_name[20] = {};
        int counter = 0;
        Token_t token = {};

        for ( ; isalpha ( language->file.out_buffer[i] ); ++i, ++counter )  {    //
            element_name[counter] = language->file.out_buffer[i];
        }
$       element_name[counter] = '\0';

        if ( counter > 0 ) {
            if ( strcmp ( element_name, "if" ) == 0 ) {
                token.type = NODE_TYPE_IF;
            }
            else if ( strcmp ( element_name, "else" ) == 0 ) {
                token.type = NODE_TYPE_ELSE;
            }
            else if ( strcmp ( element_name, "while" ) == 0 ) {
                token.type = NODE_TYPE_WHILE;
            }
            else if ( strcmp ( element_name, "return" ) == 0 ) {
                token.type = NODE_TYPE_RETURN;
            }
            else if ( language->file.out_buffer[i] == '(' ) {
                            //language->file.out_buffer[i+1] == ')' ) {
            token.type = NODE_TYPE_FUNC;
            token.cell_code = Search_Func_Name ( language, element_name );
$           }
            else {
$               token.type = NODE_TYPE_VAR;
$               token.cell_code = Search_Var_Name ( language, element_name );
$           }
        }
        else if ( counter == 0 ) {
            int sign = 1;
            if ( language->file.out_buffer[i] == '-' ) {
                sign = -1;
                ++i;
            }
            for ( ; isdigit ( language->file.out_buffer[i] ); ++i, ++counter )  {    //
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
}

int Search_Func_Name ( struct Language_t *language, char* name ) // +
{
    assert ( language != nullptr );
    assert ( name != nullptr );

    for ( int i = 0; i < language->cell_n; ++i ) {
$       if ( language->name_cell[i].type == NODE_TYPE_FUNC ) {
            if ( strcmp ( name, language->name_cell[i].data ) == 0) {
$
                return language->name_cell[i].name_code;
            }
        }
    }
$
    int cell = Search_Free_Cell ( language );
    language->name_cell[cell].type = NODE_TYPE_FUNC;
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
    assert ( language->name_cell != nullptr );  // assert
    memset (  (Name_Cell_t *)( (char *)language->name_cell + n_bytes ), 0, n_bytes );
$
    return Search_Free_Cell ( language );
}

int Search_Var_Name ( struct Language_t *language, char* name )  // +
{
    assert ( language != nullptr );
    assert ( name     != nullptr );

    for ( int i = 0; i < language->cell_n; ++i ) {
$       if ( language->name_cell[i].type == NODE_TYPE_VAR ) {
            if ( strcmp ( name, language->name_cell[i].data ) == 0 ) {
$
                return language->name_cell[i].name_code;
            }
        }
    }
    int cell = Search_Free_Cell ( language );
    language->name_cell[cell].type = NODE_TYPE_VAR;
    language->name_cell[cell].data = strdup (name); // free
    language->name_cell[cell].name_code = cell;

    return cell;
}

Errors_t File_Reader ( struct File_t *file, FILE *input_f )
{
    assert ( file != nullptr );
    assert ( input_f != nullptr );

    file->file_size = Get_File_Size ( input_f );

    file->out_buffer = ( char *)calloc ( file->file_size + 1, sizeof ( char ) );
    if ( !file->out_buffer ) {

        return ERR_CALLO;
    }

    int ret_code = fread ( file->out_buffer, sizeof( file->out_buffer[0] ), file->file_size, input_f );
    printf ( "buff_size %d\tret_code %d\n", file->file_size, ret_code );  //aaaaaaaaa
    if ( ret_code != file->file_size  ) {
        if ( feof ( input_f ) ) {
            printf ( "Error reading test.bin: unexpected end of file\n" );
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

char *File_Skip_Spaces ( char *data, int file_size )
{
    char *buffer = (char *)calloc ( file_size + 1, sizeof ( char ) );
    int position = 0;

    for ( ; *data != '\0'; ++data ) {
        while ( *data == ' '  ||
                *data == '\t' ||
                *data == '\n' ) {

            ++data;
        }
        buffer[position++] = *data;
    }
    buffer[position] = '\0';

    return buffer;
}

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
        printf ( "%s", Get_Op_Name ( language, tree_node ) );
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
                             " label = \"%s \"];\n", tree, Get_Op_Name ( language, tree ) );
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

const char *Get_Op_Name ( const struct Language_t *language, const struct Node_t *tree_node )
{
    if ( tree_node->type == NODE_TYPE_OP ) {
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
            printf ( "Error text dump %c\n", tree_node->value );
        }
    }
    }
    else if ( tree_node->type == NODE_TYPE_IF ) {

        return "if";
    }
    else if ( tree_node->type == NODE_TYPE_WHILE ) {

        return "while";
    }
    else if ( tree_node->type == NODE_TYPE_ELSE ) {

        return "else";
    }
    else if ( tree_node->type == NODE_TYPE_RETURN ) {

        return "return";
    }
    else if ( tree_node->type == NODE_TYPE_VAR ||
              tree_node->type == NODE_TYPE_FUNC ) {
        return language->name_cell[tree_node->value].data;
    }
$
    return "error";
}

/*double Eval ( const struct Node_t *node ) // +
{
    if ( node == nullptr ) {

        return 0;
    }
    if ( node->type == NODE_TYPE_NUM ) {

        return node->value;
    }
    else if ( node->type == NODE_TYPE_VAR ){
        printf ( "Error, because i dont't know this value\n" );
    }

    double left  = Eval ( node->left );
    double right = Eval ( node->right );

    if ( node->type == NODE_TYPE_OP ) {
        switch ( (Option_t) node->value ) {
            case OP_ADD : {

                return left + right;
                break;
            }
            case OP_SUB : {

                return left - right;
                break;
            }
            case OP_DIV : {

                return left / right;
                break;
            }
            case OP_MUL : {

                return left * right;
                break;
            }
            default :{
                printf ( "ERROR\n" );

                break;
            }
        }
    }

    return ERR_CTYPE;
}  */

Errors_t FromType_ToOption ( struct Node_t *tree_node ) // ----
{
    if ( !tree_node || tree_node->type != NODE_TYPE_OP ) {

        return OK_TREE;
    }
    /*if ( tree_node->type == OP ) {
        tree_node->value = (Node_Type_t)tree_node->value;
        // error
    } */
    FromType_ToOption ( tree_node->left  );
    FromType_ToOption ( tree_node->right );

    return OK_TREE;
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

int Optimization_Const ( struct Node_t *tree ) // -
{
$   if ( tree == nullptr || tree->right == nullptr || tree->left == nullptr ) {

        return 0;
    }
$   if ( tree->left->type  == NODE_TYPE_NUM &&
         tree->right->type == NODE_TYPE_NUM  ) {

$       //tree->value = (int)Eval ( tree );
$       tree->type = NODE_TYPE_NUM;
$       Node_Free ( &(tree->left  ) );
$       Node_Free ( &(tree->right ) );

        return OK_OCCURR;
    }
    else {
$       return ( Optimization_Const ( tree->left ) ||
                 Optimization_Const ( tree->right ) );
    }
}

int Optimization_Option ( struct Node_t **tree )  // -
{
$   if ( (*tree) == nullptr  ) {

        return 0;
    }
$   if ( (*tree)->type == NODE_TYPE_OP && (*tree)->value == OP_MUL ) {
        if ( (*tree)->left->value  == 0 || (*tree)->right->value == 0 ) {
$           (*tree)->value = 0;
$           (*tree)->type  = NODE_TYPE_NUM;
$           Node_Free ( &((*tree)->left ) );
$           Node_Free ( &((*tree)->right ) );

            return OK_OCCURR;
        }
        if ( (*tree)->left->value  == 1 ) {  // func
            Node_Free ( &((*tree)->left ) );
            (*tree) = (*tree)->right;

            return OK_OCCURR;
        }
        else if ( (*tree)->right->value == 1 ) {
            Node_Free ( &((*tree)->right ) );
            *tree = (*tree)->left; // for this moment

            return OK_OCCURR;
        }

        return 0;
    }
    else {
$       return ( Optimization_Option ( &(*tree)->left ) ||
                 Optimization_Option ( &(*tree)->right )  );
    }
}

void Optimization ( struct Node_t *tree )  // -
{
    int occurrences_n = 0;
    int prev_occuttences_n = 0;

    do {
        prev_occuttences_n = occurrences_n;
        if ( Optimization_Const  ( tree) ||
             Optimization_Option ( &tree ) ) {
            ++occurrences_n;
            //Tree_Text_Dump ( tree );
        }
    } while ( prev_occuttences_n != occurrences_n );
}

void Node_Free ( struct Node_t **tree ) // +
{
    if ( *tree != nullptr ) {
        Node_Free ( &((*tree)->left) );
        Node_Free ( &((*tree)->right));

        (*tree)->left = nullptr;
        (*tree)->right = nullptr;

        (*tree) = nullptr;
    }
}

void Back_End ( struct Language_t *language, const struct Node_t *tree_node )
{
    assert ( language != nullptr );

    if ( tree_node == nullptr ) {
$
        return ;
    }

    if ( tree_node->type == NODE_TYPE_FUNC && ( tree_node->right != nullptr )  ) {
$       fprintf ( language->out_file, "\n: %d;\t\\\\%s\n", language->name_cell[tree_node->value].name_code, language->name_cell[tree_node->value].data );
    }
    else if ( tree_node->type == NODE_TYPE_FUNC && ( tree_node->right == nullptr )  ) {
        Back_End ( language, tree_node->left  );
$       fprintf ( language->out_file, "call %d;\n", language->name_cell[tree_node->value].name_code );
        language->id += 2;

        return ;
    }
    else if ( tree_node->type == NODE_TYPE_OP && tree_node->value == OP_EQUAL ) {
        Back_End ( language, tree_node->right );
$       fprintf ( language->out_file, "pop r%cx;\n", language->name_cell[tree_node->left->value].name_code + 'a' );
        language->id += 2;

        return ;
    }
    else if ( tree_node->type == NODE_TYPE_VAR ) {
        //Back_End ( language, tree_node->left  );
$       fprintf ( language->out_file, "push r%cx;\n", language->name_cell[tree_node->value].name_code + 'a' );
        language->id += 2;
        //return;
    }
    else if ( tree_node->type == NODE_TYPE_NUM ) {
$       fprintf ( language->out_file, "push %d;\n", tree_node->value );
        language->id += 2;
    }
    else if ( tree_node->type == NODE_TYPE_OP && ( tree_node->value == OP_EQUIVALENCE ) ) {   // > < >= <=
        Back_End ( language, tree_node->left  );
        Back_End ( language, tree_node->right );
        language->id += 2;
$       fprintf ( language->out_file, "%s %d;\n", Get_Op_Asm_Code ( tree_node->value ), language->if_while_code_counter );

        return ;
    }
    else if ( tree_node->type == NODE_TYPE_OP && tree_node->value != OP_CONNECT && tree_node->value != OP_SEMICLON ) {   // ;
        Back_End ( language, tree_node->left  );
        Back_End ( language, tree_node->right );
        language->id += 1;
$       fprintf ( language->out_file, "%s;\n", Get_Op_Asm_Code ( tree_node->value ) );

        return ;
    }
    else if ( tree_node->type == NODE_TYPE_RETURN ) {
        language->id += 1;
$       fprintf ( language->out_file, "ret;\n" );
    }
    else if ( tree_node->type == NODE_TYPE_IF ) { // else
        Back_End ( language, tree_node->left  );
        fprintf ( language->out_file, "call %d;\n", language->if_while_code_counter + 1 );
        language->id += 2;
        fprintf ( language->out_file, "\n: %d;\n", language->if_while_code_counter );

        Back_End ( language, tree_node->right );
        fprintf ( language->out_file, "ret;\n\n" ); // +1
        fprintf ( language->out_file, "\n: %d;\t\n", language->if_while_code_counter + 1 );  ////
        fprintf ( language->out_file, "jmp %d;\n", language->id + 4 ); // +3
        fprintf ( language->out_file, "ret;\n\n" );     // +4

        language->id += 4;
        language->if_while_code_counter += 2;

        return ;
    }
    else if ( tree_node->type == NODE_TYPE_WHILE ) { // else
        int prev_id = language->id;
        Back_End ( language, tree_node->left );
        fprintf ( language->out_file, "call %d;\n", language->if_while_code_counter + 1 );
        language->id += 2;
        fprintf ( language->out_file, "\n: %d;\n", language->if_while_code_counter );

        Back_End ( language, tree_node->right );
        fprintf ( language->out_file, "jmp %d;\n", prev_id );  // +2
        fprintf ( language->out_file, "ret;\n\n" ); // +3
        fprintf ( language->out_file, "\n: %d;\t\n", language->if_while_code_counter + 1 );  ////
        fprintf ( language->out_file, "jmp %d;\n", language->id + 6 ); // +4
        fprintf ( language->out_file, "ret;\n\n" );     // +5

        language->id += 4;
        language->if_while_code_counter += 2;

        return ;
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
        case OP_EQUIVALENCE : {

            return "je"; // %d", language->if_while_code_counter; //
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


