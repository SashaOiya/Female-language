#include "assembler.h"
#include "../includes/my_program.h"

//you should do a lot of work to pass this, you just can give up, but i am sure, that's is not your aim

Error_t Assembler_Ctor ( struct Asm_t *assembler, const char *command_file )
{
    assert ( assembler    != nullptr );
    assert ( command_file != nullptr );

    Stack_Ctor ( &(assembler->stack) );

    FILE *command_f = fopen ( command_file, "rb" );
    if ( !command_f ) {
        perror ( "File opening failed" );

        return F_OPEN_ERR;
    }

    assembler->text.file_size = Get_File_Size ( command_f );

    assembler->text.data = ( char *)calloc ( assembler->text.file_size + 1, sizeof ( char ) );
    if ( !assembler->text.data ) {

        return MEMMORY_ERR;
    }

    int ret_code = fread ( assembler->text.data, sizeof ( assembler->text.data[0] ), assembler->text.file_size, command_f );
    if ( ret_code != assembler->text.file_size ) {
        if ( feof ( command_f ) ) {
            printf ( "Error reading %s: unexpected end of file\n", command_file );
        }
        else if ( ferror ( command_f ) ) {
            perror ( "Error reading command_file\n" );
        }
        fclose ( command_f );

        return F_READ_ERR;
    }
    fclose ( command_f );

    Error_t ret_err = Text_Remove_Comments ( &(assembler->text) );
    if ( ret_err != NO_ERR ) {

        return ret_err;
    }

    assembler->text.line_array = ( Line_t *)calloc ( assembler->text.n_lines, sizeof ( Line_t ) );
    if ( !assembler->text.line_array ) {

        return MEMMORY_ERR;
    }

    return NO_ERR;
}

Error_t Assembler_Compile ( struct Asm_t *assembler )
{
    assert ( assembler != nullptr );

$   Split_Data_Into_Lines ( assembler );

$   for ( int i = 0, j = 0; i < assembler->text.n_lines; ++j, ++i ) {
        if ( strcmp ( assembler->text.line_array[i].start, command_arr[COLON].str ) == 0 ) {
            ;
        }
        else {
            Assembler_Compare ( assembler, &(assembler->text.line_array[i]) );
        }
    }
    if ( Stack_Verificator ( &(assembler->stack) ) != STACK_NO_ERR ) {

        return STACK_ERR;
    }
    Stack_Dump ( &(assembler->stack), INFORMATION );

    return NO_ERR;
}

void Split_Data_Into_Lines ( struct Asm_t *assembler )
{
    assert ( assembler != nullptr );

    for ( int i = 0, j = 0; i < assembler->text.n_lines; ++j, ++i ) {
        assembler->text.line_array[i].start = assembler->text.data + j;

        while ( assembler->text.data[j] != '\0' ) {
            if ( assembler->text.data[j] == ' ' ) {
                assembler->text.data[j] = '\0';

$               assembler->text.line_array[i].element = atoi ( const_pointer ); // my_atoi ( a, *b );
                assembler->text.line_array[i].passed_args |= const_passed;

                if ( assembler->text.data[j + 1] == 'r' &&
                     assembler->text.data[j + 3] == 'x' &&
                     assembler->text.data[j + 4] == '\0' ) {
                    assembler->text.line_array[i].registerr = *(register_pointer) - ( 'a' - distance_command_const );
                    assembler->text.line_array[i].passed_args = ( assembler->text.line_array[i].passed_args & 0 ) | reg_passed;
                }
                if ( assembler->text.data[j + 1] == '[' ) {
                    assembler->text.line_array[i].passed_args = ( assembler->text.line_array[i].passed_args & 0 ) | ram_passed;
                    int counter = 1;
                    for ( ; isdigit  ( assembler->text.data[j + 1 + counter] ); ++counter ) {   // isspaces
                        ;
                    }
                    assembler->text.line_array[i].ram = atoi ( ram_pointer );
                    if ( assembler->text.data[j + 1 + counter] != ']' ) {
                        assembler->text.line_array[i].passed_args |= reg_passed;
                        printf ( "RAM REG %d\n", ( assembler->text.data[j + 3 + counter] - ( 'a' - distance_command_const ) ) ); //
                        assembler->text.line_array[i].registerr = assembler->text.data[j + 3 + counter] - ( 'a' - distance_command_const );
                    }
                }
                ++(assembler->text.ip);
            }
            ++j;
        }

$       if ( strcmp ( assembler->text.line_array[i].start, ":" ) == 0 ) {
            --(assembler->text.ip);
$           assembler->labels_array[assembler->text.line_array[i].element] = assembler->text.ip;
        }
        else {
            ++(assembler->text.ip);
        }
    }
}

Error_t Assembler_Compare ( struct Asm_t *assembler, Line_t *line_array )  //name
{
    assert ( assembler  != nullptr );
    assert ( line_array != nullptr );

    const int n_commands = sizeof ( command_arr ) / sizeof ( Command_t );

$   for ( int i = 0; i < n_commands; ++i ) {
$       if ( strcmp ( line_array->start, command_arr[i].str ) == 0 ) {  // separate jmp
            if ( i == JA  || i == JB  || i == JE  || i == MJM ||
                 i == JAE || i == JBE || i == JNE || i == CALL ) {
                Stack_Push ( &(assembler->stack), ( const_passed << 5 ) | command_arr[i].code );
                Stack_Push ( &(assembler->stack), assembler->labels_array[line_array->element] );
                ++(assembler->text.ip);
            }
            else {
                if ( ( line_array->passed_args & ram_passed ) != 0 ) {
                    Push_Arg_Val ( line_array->passed_args, line_array->ram )
                    if ( ( line_array->passed_args & reg_passed ) != 0 ) {
                        Stack_Push ( &(assembler->stack),  line_array->registerr );
                        ++(assembler->text.ip);
                    }
                }
                else if ( ( line_array->passed_args & reg_passed ) != 0 ) {
                    Push_Arg_Val ( line_array->passed_args, line_array->registerr )
                }
                else if ( ( line_array->passed_args & const_passed ) != 0 ) {
                    Stack_Push ( &(assembler->stack), ( const_passed << 5 ) | command_arr[i].code );
                    Stack_Push ( &(assembler->stack),  line_array->element );
                    ++(assembler->text.ip);
                }
                else {
                    Stack_Push ( &(assembler->stack), command_arr[i].code );
                 }
            }
$           break;
        }
    }

$   return NO_ERR;
}

Error_t Wtite_Code_To_File ( struct Asm_t *assembler, const char *encode_file )
{
    assert ( assembler   != nullptr );
    assert ( encode_file != nullptr );

    FILE *code_f = fopen ( encode_file, "wb" );
    if ( !code_f ) {
        perror ( "File opening failed" );

        return F_OPEN_ERR;
    }
    assembler->stack.data[assembler->text.ip] = '\0';

    int written_objects_n = fwrite ( assembler->stack.data, sizeof ( elem_t ), assembler->text.ip, code_f );
    fclose ( code_f );
    if ( written_objects_n < assembler->text.ip ) {
        printf ( "Write error in the file\n" );

        return F_WRITE_ERR;
    }

    return NO_ERR;
}

void Assembler_Dtor ( struct Asm_t *assembler )
{
    assert( assembler != nullptr);

    Stack_Dtor ( &(assembler->stack) );
    //free ( assembler->text.data );
    //free ( assembler->text.line_array );
    printf ( "\nDTOR\n" );

    assembler->text.data       = nullptr;
    assembler->text.line_array = nullptr;
}
