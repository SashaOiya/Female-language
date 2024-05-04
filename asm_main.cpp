#include "assembler.h"

int main ( const int argc, const char* argv[] )
{
    struct Asm_t assembler = {};

    Error_t err_code = Assembler_Ctor ( &assembler, argv[1] );

    if ( err_code == NO_ERR ) {
        err_code = Assembler_Compile ( &assembler );

        if ( err_code == NO_ERR ) {
            Wtite_Code_To_File ( &assembler, argv[2] );
        }
    }

    // error handling
    Assembler_Dtor ( &assembler );

    return 0;
}
