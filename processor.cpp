#include "processor.h"

Error_t Processor_Ctor ( struct Vm_t *vm_spu, const char *encode_file )
{
    assert ( vm_spu != nullptr );
    assert ( encode_file != nullptr );

    Stack_Ctor ( &(vm_spu->stack) );

    FILE *code_f = fopen ( encode_file, "rb" );
    if ( !code_f ) {
        perror ( "File opening failed" );
        Stack_Dtor ( &(vm_spu->stack) );

        return F_OPEN_ERR;
    }

    vm_spu->file_size = Get_File_Size ( code_f ) / sizeof ( elem_t ) + 1;

    vm_spu->data = (elem_t *)calloc ( vm_spu->file_size, sizeof ( elem_t ) );
    if ( !vm_spu->data ) {

        return MEMMORY_ERR;
    }

    int ret_code = fread ( vm_spu->data, sizeof ( elem_t ), vm_spu->file_size, code_f );
    if ( ret_code != vm_spu->file_size - 1 ) {
        if ( feof ( code_f ) ) {
            printf ( "Error reading %s: unexpected end of file\n", encode_file );
        }
        else if ( ferror ( code_f ) ) {
            perror ( "Error reading command_file\n" );
        }
        fclose ( code_f );

        return F_READ_ERR;
    }
    fclose ( code_f );

    return NO_ERR;
}

Error_t Processor ( struct Vm_t *vm_spu )
{
    assert ( vm_spu != nullptr );

    int max_ip = vm_spu->file_size * sizeof ( elem_t );

    Stack_t ret_stack  = {};
    Stack_Ctor ( &ret_stack );

    for ( int ip = 0; ip < max_ip; ++ip ) {
        if ( Processing ( vm_spu, &ip, &ret_stack ) == ARG_END ) {

            return NO_ERR; // break;
        }
        // else
        Stack_Dump ( &(vm_spu->stack), INFORMATION );
    }
$   Stack_Dtor ( &ret_stack );

    return NO_ERR;
}

Arg_Indicator_t Processing ( struct Vm_t *vm_spu, int *ip, Stack_t *ret_stack )
{
$   assert ( vm_spu    != nullptr );
    assert ( ip        != nullptr );
    assert ( ret_stack != nullptr );
$
    int command       = vm_spu->data[*ip] & 0b11111;
    int register_flag = vm_spu->data[*ip] & 0b100000;
    int const_flag    = vm_spu->data[*ip] & 0b1000000;
    int ram_flag      = vm_spu->data[*ip] & 0b10000000;
    int value = 0;

    if ( const_flag || ( register_flag && !ram_flag ) ) {
        ++(*ip);
        value = vm_spu->data[*ip];
    }
    else if ( ram_flag ) {
        ++(*ip);
        value = vm_spu->data[*ip];
        if ( register_flag ) {
            ++(*ip);
            value +=vm_spu->registers.arr[vm_spu->data[*ip]].rx;
        }
    }

    switch ( command ) {
        case POP  : {
            if ( ( register_flag && !ram_flag ) != 0 ) {
                vm_spu->registers.arr[value].rx = Stack_Pop( &(vm_spu->stack) );
            }
            else if ( ram_flag != 0 ) {
                Stack_Push( &(vm_spu->stack), vm_spu->ram[value] );
            }
            }
            break;
        case PUSH : {
            if ( ( register_flag && !ram_flag ) != 0 ) { //
                Stack_Push( &(vm_spu->stack), vm_spu->registers.arr[value].rx );
            }
            else if ( const_flag != 0 ){
                Stack_Push( &(vm_spu->stack), value );
            }
            else if ( ram_flag != 0 ) {
                vm_spu->ram[value] = Stack_Pop( &(vm_spu->stack) );
            }
            }
            break;
        case HLT  : {

            return  ARG_END;
            }
            break;
        case ADD  : {
                elem_t temp = Stack_Pop( &(vm_spu->stack) ) + Stack_Pop( &(vm_spu->stack) );
                Stack_Push( &(vm_spu->stack), temp );
            }
            break;
        case MUL : {
                elem_t temp = Stack_Pop( &(vm_spu->stack) ) * Stack_Pop( &(vm_spu->stack) );
                Stack_Push( &(vm_spu->stack), temp );
            }
            break;
        case DIV  : {
                elem_t temp_1 = Stack_Pop( &(vm_spu->stack) );
                elem_t temp_2 = Stack_Pop( &(vm_spu->stack) );
                if ( temp_2 == 0 ) {
                    printf ( "Div error\n" );

                    return ARG_END;
                }
                else {
                    elem_t temp = temp_1 / temp_2;
                    Stack_Push( &(vm_spu->stack), temp );
                }
            }
            break;
        case SUB  : {
                elem_t a = Stack_Pop( &(vm_spu->stack) );
                elem_t b = Stack_Pop( &(vm_spu->stack) );
                elem_t temp = a - b;
                Stack_Push( &(vm_spu->stack), temp );
            }
            break;
       case IN   : {
                elem_t temp = 0;
                printf ( "input your value : " );
                scanf ( SPECIFIER, &temp );
                Stack_Push( &(vm_spu->stack), temp );
            }
            break;
       case OUT  : {
                Stack_Pop( &(vm_spu->stack) );
            }
            break;
       case JMP : {
                //Stack_Push ( ret_stack, *ip );
                printf ( "\nregisters %d\n", value );
                printf ( "\n code %d\n", vm_spu->data[value] );  //
                *ip = value - 1;
            }
            break;
        case JA : {
                Stack_Push ( ret_stack, *ip );  //
                elem_t temp_1 = Stack_Pop ( &(vm_spu->stack) );
                elem_t temp_2 = Stack_Pop ( &(vm_spu->stack) );
                if ( temp_1 > temp_2 ) {
                    *ip = value - 1;
                }
            }
            break;
        case JAE : {
                Stack_Push ( ret_stack, *ip );
                elem_t temp_1 = Stack_Pop ( &(vm_spu->stack) );
                elem_t temp_2 = Stack_Pop ( &(vm_spu->stack) );
                if ( temp_1 >= temp_2 ) {
                    *ip = value - 1;
                }
            }
            break;
        case JNE : {
                Stack_Push ( ret_stack, *ip );
                elem_t temp_1 = Stack_Pop ( &(vm_spu->stack) );
                elem_t temp_2 = Stack_Pop ( &(vm_spu->stack) );
                if ( temp_1 != temp_2 ) {
                    *ip = value - 1;
                }
            }
            break;
        case JE : {
                Stack_Push ( ret_stack, *ip );
                elem_t temp_1 = Stack_Pop ( &(vm_spu->stack) );
                elem_t temp_2 = Stack_Pop ( &(vm_spu->stack) );
                if ( temp_1 == temp_2 ) {
                    *ip = value - 1;
                }
            }
            break;
        case JB : {
                Stack_Push ( ret_stack, *ip );   //
                elem_t temp_1 = Stack_Pop ( &(vm_spu->stack) );
                elem_t temp_2 = Stack_Pop ( &(vm_spu->stack) );
                if ( temp_1 < temp_2 ) {
                    *ip = value - 1;
                }
            }
            break;
        case JBE : {
                Stack_Push ( ret_stack, *ip );
                elem_t temp_1 = Stack_Pop ( &(vm_spu->stack) );
                elem_t temp_2 = Stack_Pop ( &(vm_spu->stack) );
                if ( temp_1 <= temp_2 ) {
                    *ip = value - 1;
                }
            }
            break;
        case RET : {
                *ip = Stack_Pop ( ret_stack );
                printf ( "%d\n", *ip);
            }
            break;
        case CALL : {
$               Stack_Push ( ret_stack, *ip );
$               *ip = value - 1;
            }
            break;
       default : {

            return ARG_ERROR;
            }
            break;
    }
    printf ( "\n code %d\n", vm_spu->data[*ip] );

    return ARG_NO_ERROR;
}

void Processor_Dtor ( struct Vm_t *vm_spu )
{
    assert ( vm_spu != nullptr );
$
    Stack_Dtor ( &(vm_spu->stack) );
$   free ( vm_spu->data );
$   vm_spu->data = nullptr;
$
}
