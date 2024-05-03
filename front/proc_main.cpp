#include "processor.h"

int main ( const int argc, const char* argv[] )
{
    Vm_t vm_spu = {};

    if ( ( vm_spu.ret_error = Processor_Ctor ( &vm_spu, argv[1] ) ) == NO_ERR ) {
$       vm_spu.ret_error = Processor ( &vm_spu );
    }
$
    Stack_Verificator ( &(vm_spu.stack) );
$   Processor_Dtor ( &vm_spu );
$
    return 0;
}