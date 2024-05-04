#ifndef ASM
#define ASM

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comand_code.h"
#include "stack.h"

const int line_size = 3 * sizeof ( elem_t );
const int distance_command_const    = 1;
const int distance_command_register = 2;
const int distance_command_ram      = 2;

#define const_pointer    assembler->text.data + j + distance_command_const
#define register_pointer assembler->text.data + j + distance_command_register
#define ram_pointer      assembler->text.data + j + distance_command_ram

#define Push_Arg_Val(arg_passed,arg) Stack_Push ( &(assembler->stack),( arg_passed << 5 ) | command_arr[i].code ); \
                                     Stack_Push ( &(assembler->stack),  arg );                                     \
                                     ++(assembler->text.ip);

static const int reg_passed   = 0b001;
static const int const_passed = 0b010;
static const int ram_passed   = 0b100;

struct Line_t {
    char *start  = nullptr;
    elem_t element = 0;   // argument   // element pointer
    int registerr = 0; //?
    int ram = 0;
    int passed_args = 0b000;
};

struct Text_t {
    int file_size = 0;
    Line_t * line_array = {};
    int n_lines = 0;
    char *data = {};
    int ip = 0;
};

struct Asm_t {
    Text_t text = {};
    Stack_t stack = {};
    int labels_array[10]  = {};
};

Error_t Assembler_Ctor ( struct Asm_t *assembler, const char *command_file );
void Assembler_Dtor ( struct Asm_t *assembler );

Error_t Assembler_Compare ( struct Asm_t *assembler, struct Line_t *line_array );
Error_t Assembler_Compile ( struct Asm_t *assembler );

void Split_Data_Into_Lines ( struct Asm_t *assembler );
Error_t Wtite_Code_To_File ( struct Asm_t *assembler, const char *encode_file );

#endif // ASM
