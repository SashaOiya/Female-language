#include "disasm.h"
#include "stack.h"

int main ()
{
    int n_comands = 0;

    Dis_Assembler ( &n_comands );

    return 0;
}

int Dis_Assembler ( int *n_comands )
{
    FILE *comand_f = fopen ( "code.txt", "rb" );
    FILE *code_f   = fopen ( "disasm.txt", "wb" );

    Comand_Code CC = {};
    Text_t Text    = {};

    Text.file_size = GetFileSize ( comand_f ) / sizeof ( char_t );

    char_t *buffer = ( char_t *)calloc ( Text.file_size + 1, sizeof ( char_t ) );
    fread ( buffer, sizeof( buffer[0] ), Text.file_size, comand_f );    //return value

    Split ( &Text, code_f, CC, buffer );

    *n_comands = Text.n_lines;

    //AsmDtor ( buffer, Text.line_array, code_f );

    return Text.file_size;
}

int GetFileSize ( FILE * f )
{
    int prev = ftell ( f );

    fseek ( f, 0, SEEK_END );
    size_t sizet = ftell ( f );
    fseek ( f, prev, SEEK_SET );

    return sizet;
}

int Compare ( FILE *code, Comand_Code cc, char_t start, char_t ptr_elements, char *output_buffer )
{
    //printf ( "%g\n", start );
$   for ( int i = 0; i < cc.n_comands; ++i ) {
$       if ( start == cc.arr[i].code  ) {
            //int size = strlen ( cc.arr[i].str );
            strcpy( output_buffer, cc.arr[i].str );
            printf ( "%c\n", output_buffer[0] );
            //output_buffer[size-1] = ' ';
            //output_buffer[size] = ptr_elements ;
            //output_buffer[size+1] = ((int)ptr_elements >> 8);
            //output_buffer[size+2] = ((int)ptr_elements >> 16);
            //output_buffer[size+3] = ((int)ptr_elements >> 24);
            //output_buffer[size] = ptr_elements;
$
$           break;
        }
    }

$   return 0; // error code
}

int AsmDtor ( char *buffer, Line_t *line_array, FILE *comand_f )
{
    free(buffer);
    free(line_array);

    fclose ( comand_f ); //error
}

int Split ( Text_t *Text, FILE *code_f, Comand_Code CC, char_t *buffer )
{
    int g = 0;
    for ( int i = 0; i <= Text->file_size; ++i ) {
        ++g;
    }
    Text->n_lines = g / 3;
    Text->line_array = ( Line_t *)calloc ( Text->n_lines, sizeof ( Line_t) );
    char *output_buffer = ( char *)calloc ( Text->n_lines * 11 + 1, sizeof ( char ) );

    for ( int i = 0, j = 0; i < Text->n_lines; ++j, ++i ) {

        Compare ( code_f, CC, buffer[i*3], buffer[i*3+2], &output_buffer[11*i] );
    }

    fwrite ( output_buffer, sizeof (char), Text->n_lines * 11, code_f );
}
