#ifndef ASM
#define ASM

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comand_code.h"
#include "stack.h"

const int line_size = 3 * sizeof ( elem_t );

//typedef float char_t;

/*#ifdef DEBUG
#define $ fprintf (stderr, "%d\n", __LINE__ );
#else
#define $
#endif*/

struct Line_t {
    char *start  = nullptr;
    elem_t element = 0;   // argument
    int registerr = 0; //?
};

struct Text_t {
    int file_size = 0;
    Line_t * line_array;
    int n_lines = 0;
    int error_indificate = 0;
};


struct Cpu {

    elem_t registers[4];
    elem_t* ram;
};

typedef elem_t reg;
// Registers
struct Register_t {
    struct reg {
       elem_t rx = 0;
    };  //?


    struct reg arr[3] = {};
};

void Assembler ( );
int AsmCompare ( Line_t line_array, Stack_Data_t *Stack, int *pointer, Stack_Data_t *Pointer );
int GetFileSize ( FILE * f );
int AsmDtor ( char *buffer, Line_t *line_array, FILE *comand_f );
int AsmCompile ( Text_t *Text, char *buffer, FILE *code_f );
void GetPointer ( int *labels_array, char *buffer, Text_t *Text, int *ip, char * ref_buffer, Stack_Data_t *Pointer );

#endif // ASM
