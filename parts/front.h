#ifndef FRONT_END
#define FRONT_END

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "../includes/dynamic_array.h"

const int cell_mul_coeff = 2;
const int energency_val  = -1;
const int max_name_size  = 35;

enum Option_t {
    OP_ADD = '+',  // +
    OP_SUB = '-',  // -
    OP_DIV = '/',  // /
    OP_MUL = '*',  // *
    OP_VAR = 'x',  // x
    OP_BRA = '(',  // (
    CL_BRA = ')',  // )
    OP_POW = '^',  // ^ // ny okeu
    OP_SIN = 's',  // sin
    OP_COS = 'c',  // cos
    OP_TG  = 't',  // tan
    OP_CTG = 'g',  // ctg

    OP_EQUAL = '=', // =
    OP_MORE  = '>', // >
    OP_LESS  = '<', // <
    OP_SEMICLON = ';',
    OP_W_BRA = '{',
    CL_W_BRA = '}',
    OP_CONNECT = '`',
    OP_EQUIVALENCE = '~',
    OP_COMMA = ','
};

struct Node_t {
    int value      = {};
    Node_Type_t type;
    Node_t *left     = 0;
    Node_t *right    = 0;
};

struct Tree_t {
    Node_t *start = nullptr;
    int current_func = 0;
    //struct Name_t *name_storage = nullptr;
    //Array
};

struct File_t {
    char *out_buffer = nullptr;
    int file_size = 0;
};

struct Name_Cell_t {
    int type = 0;
    int name_code = 0;
    char *data = nullptr; // char*
    int value = 0;
};

struct Language_t {
    struct Tree_t tree = {};
    struct File_t file = {};
    struct Dynamic_Array_t d_array = {};
    struct Name_Cell_t *name_cell = nullptr;
    int cell_n = 2;
    int operator_counter = 0;
    FILE *out_file = nullptr;  // hyita
    int id = 0;
};

Errors_t Language_Ctor ( struct Language_t *language, const char *input_file_name );
void     Language_Dtor ( struct Language_t *language );

Errors_t File_Reader ( struct File_t *file, const char *input_file_name );
char *   File_Skip_Spaces ( char *data, int file_size );
int      Get_File_Size ( FILE * f );

Errors_t Tree_Graph_Dump ( const struct Language_t *language );
void     Tree_Dump_Body  ( const struct Language_t *language, const struct Node_t *tree, FILE *tree_dump );
void     Tree_Text_Dump  ( const struct Language_t *language, const struct Node_t *tree_node );
const char *Get_Dump_Op_Name ( const struct Language_t *language, const struct Node_t *tree_node );

Node_t *Create_Node ( Node_Type_t option, int value, struct Node_t *left, struct Node_t *right );
Node_t *Copy_Node ( const struct Node_t *tree);
void Node_Free ( struct Node_t **tree );


Errors_t Search_Tokens        ( struct Language_t *language );
int      Search_Free_Cell     ( struct Language_t *language );
int      Search_Func_Var_Name ( struct Language_t *language, char* name, const Node_Type_t type );

#endif  // FRONT_END
