#ifndef RECURS_DESCENT
#define RECURS_DESCENT

#include "front.h"

const int size = 50; // wtf
const int free_cell = -1;

struct Name_t {
    int type = 0; //wtf wtf wtf wtf wtf wtf
    int name_code = 0;
    char* value = nullptr;
};

struct Position_t {
    char *data = nullptr;
    int index = 0;
    struct Name_t *name_storage = nullptr;
};

int Search_Func_Name ( char* name, Name_t *name_cell);
int Search_Free_Cell ( Name_t *name_cell);
int Search_Var_Name ( char* name, Name_t *name_cell );

Node_t *Get_Number ( struct Position_t *Position );
Node_t *GetG ( char *buffer, struct Node_t *Tree );
Node_t *GetE ( struct Position_t *Position );
Node_t *GetT  ( struct Position_t *Position );
Node_t *GetP  ( struct Position_t *Position );
Node_t *GetId ( struct Position_t *Position );
Node_t *GetId ( struct Position_t *Position );
Node_t *GetA ( struct Position_t *Position );
Node_t *GetFunc ( struct Position_t *Position );
Node_t *GetLim ( struct Position_t *Position );
Node_t* GetOp ( struct Position_t *Position );
Node_t* GetDifferentCond ( struct Position_t *Position );

#endif      // RECURS_DESCENT
