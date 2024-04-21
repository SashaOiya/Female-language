#include "dynamic_array.h"

Errors_t Dymanic_Array_Ctor ( struct Dynamic_Array_t *d_array )
{
    assert ( d_array != nullptr );

    d_array->data = (Token_t *)calloc( d_array->size * sizeof ( Token_t ), 1 );
    if ( !d_array->data ) {

        return ERR_CALLO;
    }
$
    return NO_ERR;
}

Errors_t Dynamic_Array_Resize ( struct Dynamic_Array_t *d_array )
{
    assert ( d_array != nullptr );

    d_array->size *= d_array_mul_coeff;

    Token_t *new_dynamic_array = (Token_t *)calloc ( d_array->size * sizeof ( Token_t ), 1 );
    if ( !new_dynamic_array ) {

        return ERR_CALLO;
    }

    memcpy ( new_dynamic_array, d_array->data, d_array->size * ( sizeof ( Token_t ) / d_array_mul_coeff ) );
    free ( d_array->data );
    d_array->data = new_dynamic_array;

    return NO_ERR;
}

void Dynamic_Array_Dtor ( struct Dynamic_Array_t *d_array )
{
    assert ( d_array != nullptr );
$
    for ( int i = 0; i < d_array->capacity; ++i ) {
        free ( d_array->data[i].data );
    }

    free ( d_array->data );
    d_array->data = nullptr;
}

void Dynamic_Array_Push ( struct Dynamic_Array_t *d_array, const Token_t value )
{
    assert ( d_array != nullptr );

    ++(d_array->capacity);

$   if ( d_array->capacity == d_array->size ) {
$       while ( Dynamic_Array_Resize ( d_array ) != NO_ERR ) {     // error
$           ;
        }
$   }
$   *( d_array->data + d_array->capacity - 1 ) = value;
}

void Dynamic_Array_Dump (  struct Dynamic_Array_t *d_array, const char* func_name, const char* file_name )
{
    assert ( func_name != nullptr );
    assert ( file_name != nullptr );
    assert ( d_array   != nullptr );

$   printf ( "Dynamic Array [%p] ", d_array->data );
    printf ( " called from%s\n "
             "{\n\t%s  \n\t{ \n "
             " \t\tsize = %d \n "
             " \t\tcapacity = %d \n "
             " \t\tdata [%p]:\n", file_name, func_name, d_array->size, d_array->capacity, d_array->data  );
$   for ( int i = 0; i < d_array->size; ++i ) {
$       printf ( "\t\tdata[%d] = ", i );
        printf ( "type %d\t", d_array->data[i].type );
        printf ( "cell_code %d\n", d_array->data[i].cell_code );
    }
$   printf ( "\t}\n}\n");
}
