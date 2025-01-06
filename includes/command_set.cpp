define_cmd(hlt, 0, {
    arg_indicator = ARG_END;
})

define_cmd ( pop, 0, {
    if ( ( register_flag && !ram_flag ) != 0 ) {
        vm_spu->registers.arr[value].rx = Stack_Pop( &(vm_spu->stack) );
    }
    else if ( ram_flag != 0 ) {
        Stack_Push( &(vm_spu->stack), vm_spu->ram[value] );
    }
    break;
})

define_cmd ( push, 0, {
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
    break; )

define_cmd ( add, 0, {
        elem_t temp = Stack_Pop( &(vm_spu->stack) ) + Stack_Pop( &(vm_spu->stack) );
        Stack_Push( &(vm_spu->stack), temp );
    }
    break; )
