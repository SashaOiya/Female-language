#ifndef PROCESSOR
#define PROCESSOR

#include <stdio.h>
#include <assert.h>
#include <cmath>
#include <ctype.h>

#include "stack.h"
#include "comand_code.h"
#include "my_program.h" //FIXME really bad naming

enum Arg_Indicator_t {
    ARG_END      = -1,
    ARG_INPUT    = 0,
    ARG_OUTPUT   = 1,
    ARG_INPUT_IN = 2,
    ARG_FUNC     = 3,
    ARG_ERROR    = 4,
    ARG_NO_ERROR = 5
};

struct Register_t {
    struct reg {
        elem_t rx = 0;
    };
    struct reg arr[3] = {};
};

struct Vm_t {
    int file_size = 0;
    elem_t *data   = {};
    Stack_t stack   = {};
    Register_t registers = {};
    Error_t ret_error = NO_ERR;
    elem_t ram[100] = {};
};

enum Register {
    RAX = 1,
    RBX = 2,
    RCX = 3
};

Error_t Processor_Ctor ( struct Vm_t *vm_spu, const char *encode_file );
void    Processor_Dtor ( struct Vm_t *vm_spu );

Error_t Processor ( struct Vm_t *vm_spu );
Arg_Indicator_t Processing ( struct Vm_t *vm_spu, int *ip, Stack_t *Ret_Stack );

#endif // PROCESSOR
