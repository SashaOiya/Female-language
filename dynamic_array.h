#ifndef DYNAMIC_ARRAY
#define DYNAMIC_ARRAY

#include "front.h"


#define INFORMATION __PRETTY_FUNCTION__, __FILE__

const int d_array_mul_coeff = 2;

enum Error_t {
    NO_ERR        = 0,
    ENCOD_ERR     = 1,
    F_WRITE_ERR   = 2,
    F_READ_ERR    = 3,
    F_OPEN_ERR    = 4,
    MEMMORY_ERR   = 5,
    INPUT_VAL_ERR = 6,
    STACK_ERR     = 7
};

struct Dynamic_Array_t {
    Token_t *data = nullptr;
    int capacity   = 0;
    int size       = 1;
    int status     = 0;
};

Error_t Dymanic_Array_Ctor ( struct Dynamic_Array_t *d_array );
Error_t Dynamic_Array_Resize ( struct Dynamic_Array_t *d_array );
void Dynamic_Array_Dtor ( struct Dynamic_Array_t *d_array );
void Dynamic_Array_Push ( struct Dynamic_Array_t *d_array, const Token_t value );
void Dynamic_Array_Dump (  struct Dynamic_Array_t *d_array, const char* func_name, const char* file_name );

#endif  // DYNAMIC_ARRAY
