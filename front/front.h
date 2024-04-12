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
//#include "recurs_des.h"

#ifdef DEBUGG
#define $ printf ( "function <%s> line <%d>\n ", __PRETTY_FUNCTION__, __LINE__ );
#else
#define $
#endif

enum Node_Type_t {
    NUM = 1,
    OP  = 2,
    VAR = 3,
    KEY_WORD = 4,
    FUNC = 5,
    FUNC_HEAD = 6  // change
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

    EQUAL =  '=',    // CHANGE !!
    LIMIT = '`',
    SLASH = '\\',
    AND = '&',
    MORE = '>',
    LESS = '<',
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
    struct Name_t *name_storage = nullptr;
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
};

Errors_t File_Reader ( struct File_t *File );
int GetFileSize ( FILE * f );

//void Analitic ( char *buffer, struct Node_t *tree );
Errors_t Tree_Graph_Dump ( const struct Node_t *tree, const Name_t *name_storage );
void Tree_Dump_Body ( const struct Node_t *tree_node, FILE *tree_dump, const Name_t *name_storage );
void Tree_Text_Dump ( const struct Node_t *tree_node, const Name_t *name_storage );

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

#endif  // FRONT_END
