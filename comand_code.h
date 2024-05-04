#ifndef COMAND_CODE
#define COMAND_CODE

enum Code {
    PUSH  =  0,
    HLT   =  1,
    ADD   =  2,
    MUL   =  3,
    SQRT  =  4,
    SIN   =  5,
    COS   =  6,
    DIV   =  7,
    SUB   =  8,
    IN    =  9,
    OUT   = 10,
    POP   = 11,
    COLON = 12,
    JMP   = 13,
    JA    = 14,
    JAE   = 15,
    JB    = 16,
    JBE   = 17,
    JE    = 18,
    JNE   = 19,
    CALL  = 20,
    RET   = 21
};

struct Command_t {
    const char *str;
    int code;
};

const struct Command_t command_arr[22] = {
                                          { "push", PUSH  }, { "hlt" , HLT  }, { "add" , ADD },
                                          { "mul" , MUL   }, { "sqrt", SQRT }, { "sin" , SIN },
                                          { "cos" , COS   }, { "div" , DIV  }, { "sub" , SUB },
                                          { "in"  , IN    }, { "out" , OUT  }, { "pop" , POP },
                                          { ":"   , COLON }, { "jmp" , JMP  }, { "ja"  , JA  },
                                          { "jae" , JAE   }, { "jb"  , JB   }, { "jbe" , JBE },
                                          { "je"  , JE    }, { "jne" , JNE  }, { "call", CALL },
                                          { "ret" , RET   }
                                         };

#endif  // COMAND_CODE
