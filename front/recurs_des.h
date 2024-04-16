#ifndef RECURS_DESCENT
#define RECURS_DESCENT

#include "front.h"

const int size = 50; // wtf
const int free_cell = -1;

struct Position_t {
    char *data = nullptr;
    int index = 0;
    struct Name_t *name_storage = nullptr;
};

enum Mode_t {    // name
    MODE_ADD_SUB = 1,
    MODE_MUL_DIV = 2,
    MODE_MORE_LESS_EQUAL = 3
};

int Search_Func_Name ( char* name, Name_t *name_cell);
int Search_Free_Cell ( Name_t *name_cell);
int Search_Var_Name ( char* name, Name_t *name_cell );
Node_t* GetCondOp ( struct Position_t* Position );

Node_t *Get_Number ( struct Position_t *Position );
Node_t *Get_General ( char *buffer, struct Node_t *tree_node );
Node_t *Get_Index ( struct Position_t *Position );
Node_t *Get_Expression ( struct Position_t *position );
Node_t *Get_Term ( struct Position_t *position );
Node_t *Get_Partititon ( struct Position_t *position );
Node_t *Get_Negative_Num ( struct Position_t *position );
Node_t *Get_Number ( struct Position_t *Position );
Node_t *Get_Power ( struct Position_t *position );
int Get_Binary_Element ( int sign, enum Mode_t mode );
Node_t *Get_Comparison ( struct Position_t *position );

#endif      // RECURS_DESCENT
