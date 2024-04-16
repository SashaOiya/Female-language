#ifndef FRONT_END
#define FRONT_END

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "log.h"

#ifdef DEBUG
#define $ printf ( "function <%s> line <%d>\n ", __PRETTY_FUNCTION__, __LINE__ );
#else
#define $
#endif

const int CELL_SIZE = 10;

enum Node_Type_t {
    FREE_CELL        = 0,
    NODE_TYPE_NUM    = 1,
    NODE_TYPE_OP     = 2,
    NODE_TYPE_VAR    = 3,
    NODE_TYPE_FUNC   = 4,
    NODE_TYPE_IF     = 5,
    NODE_TYPE_WHILE  = 6,
    NODE_TYPE_ELSE   = 7,
    NODE_TYPE_RETURN = 8
};

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
    CL_W_BRA = '}'
};

enum Key_Word {
    KEY_W_IF = 1,
    KEY_W_WHILE = 2,
    KEY_W_D     = 3,
    KEY_W_DONT  = 4
}; // more words

struct Node_t {
    int value      = {};
    Node_Type_t type;
    Node_t *left     = 0;
    Node_t *right    = 0;
};

struct Tree_t {
    Node_t *start = nullptr;
    //struct Name_t *name_storage = nullptr;
    //Array
};

enum Errors_t {
    OK_TREE   = 0,
    ERR_FREAD = 1,
    ERR_INPUT = 2,
    ERR_FOPEN = 3,
    ERR_RLINE = 4,
    ERR_CALLO = 5,
    ERR_CYCLE = 6,
    ERR_CTYPE = 7,
    OK_FILE   = 8,
    OK_OCCURR = 9
};

struct File_t {
    char *file_name = nullptr;
    FILE *front_f = nullptr;
    char *out_buffer = nullptr;
    int file_size = 0;
    struct Name_t *name_storage = nullptr;
};

struct Token_t {
    Node_Type_t type;
    int cell_code = 0;
};

struct Name_Cell_t {
    int type = 0;
    int name_code = 0;
    char *data = nullptr; // char*
};

Errors_t File_Reader ( struct File_t *File );
int GetFileSize ( FILE * f );

//void Analitic ( char *buffer, struct Node_t *tree );
void Tree_Dump_Body ( const struct Node_t *tree, FILE *tree_dump );// name_storage
Errors_t Tree_Graph_Dump ( const struct Node_t *tree );
void Tree_Text_Dump ( const struct Node_t *tree_node );

double Eval ( const struct Node_t *node );
Node_t *Create_Node ( Node_Type_t option, int value, struct Node_t *left, struct Node_t *right );
Errors_t FromType_ToOption ( struct Node_t *tree_node );
char *File_Skip_Spaces ( char *data, int file_size );
void Node_Free ( struct Node_t **tree );

Node_t *d ( const struct Node_t *tree );
Node_t *c ( const struct Node_t *tree );

int Optimization_Const ( struct Node_t *tree );
int Optimization_Option ( struct Node_t **tree );
void Optimization ( struct Node_t *tree );

void File_Write_Front ( const struct Node_t *tree );
void File_Write_Asm_Text ( const struct Node_t *tree, FILE *start_f );

const char *Get_Op_Name ( int op_type );

void Search_Tokens ( const struct File_t *file, Name_Cell_t *name_cell, struct Dynamic_Array_t *d_array );
int Search_Var_Name ( char* name, Name_Cell_t *name_cell );
int Search_Free_Cell ( Name_Cell_t *name_cell );
int Search_Func_Name ( char* name, Name_Cell_t *name_cell );

#endif  // FRONT_END
