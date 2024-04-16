#ifndef DYNAMIC_ARRAY
#define DYNAMIC_ARRAY

//#include "front.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define INFORMATION __PRETTY_FUNCTION__, __FILE__

#ifdef DEBUG
#define $ printf ( "function <%s> line <%d>\n ", __PRETTY_FUNCTION__, __LINE__ );
#else
#define $
#endif

const int d_array_mul_coeff = 2;

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
    OK_OCCURR = 9,
    NO_ERR    = 10,
};

struct Token_t {
    Node_Type_t type;
    int cell_code = 0;
};

struct Dynamic_Array_t {
    Token_t *data = nullptr;
    int capacity   = 0;
    int size       = 1;
    int status     = 0;
};

Errors_t Dymanic_Array_Ctor ( struct Dynamic_Array_t *d_array );
Errors_t Dynamic_Array_Resize ( struct Dynamic_Array_t *d_array );
void Dynamic_Array_Dtor ( struct Dynamic_Array_t *d_array );
void Dynamic_Array_Push ( struct Dynamic_Array_t *d_array, const Token_t value );
void Dynamic_Array_Dump (  struct Dynamic_Array_t *d_array, const char* func_name, const char* file_name );

#endif  // DYNAMIC_ARRAY
