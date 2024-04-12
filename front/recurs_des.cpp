//#include "front.h"
#include "recurs_des.h"

Node_t *GetG ( char *buffer, struct Tree_t *Tree ) //const  // +
{
    struct Position_t Position = {};
    Position.name_storage = (Name_t *)calloc ( size, sizeof ( Name_t ) ); // change
    for (int i = 0; i < size; ++i )  {
        Position.name_storage[i].type = free_cell;
    }
    Position.data = buffer;
    Position.index = 0;

    Tree->start = GetFunc( &Position );
    Tree->name_storage = Position.name_storage;

    //assert ( Position.data[Position.index] == '\0' );

    return Tree->start;
}

Node_t *Get_Number ( struct Position_t *Position )  // +
{
    int val = 0;
    int prev_index = Position->index;

    while ( isdigit ( Position->data[Position->index] ) ) {
        val = val * 10 + Position->data[Position->index] - '0';
        ++(Position->index);
    }
    //assert ( Position->index > prev_index );

    return Create_Node ( NUM, val, nullptr, nullptr );
}

Node_t *GetId ( struct Position_t *Position )
{
    char arg[20] = "";
    int counter = 0;

    while ( isalpha ( Position->data[Position->index] ) != 0 )
    {
        sprintf (arg + counter, "%c", Position->data[Position->index] );
        (Position->index)++;
        counter++;
    }
    if ( Position->data[Position->index] == '(' ) {
        int func_name_code = Search_Func_Name ( arg, Position->name_storage );     // name
        //printf ("func_code = %d\n", func_name_code);  // debug
        (Position->index)++;
        Node_t* val = GetE ( Position );
        (Position->index)++;

        return Create_Node ( FUNC, func_name_code, nullptr, val );
    }
    Node_t *val3 = Create_Node ( VAR, Search_Var_Name ( arg, Position->name_storage ), nullptr, nullptr );

    return val3;
}

Node_t *GetE ( struct Position_t *Position )  // +
{
    Node_t *val = GetT ( Position );

    while ( Position->data[Position->index] == '+' ||
            Position->data[Position->index] == '-' )
    {
        int element = Position->data[Position->index];
        (Position->index)++;
        Node_t *val_2 = GetT ( Position );

        switch ( element ) {
            case '+' : {
                val = Create_Node( OP, OP_ADD, val, val_2 );
                break;
            }
            case '-' : {
                val = Create_Node( OP, OP_SUB, val, val_2 );
                break;
            }
            default : {
                printf ( "Error" );
                break;
            }
        }
    }
    return val;
}

Node_t *GetT ( struct Position_t *Position )  // +
{
    Node_t *val = GetP ( Position );

    while ( Position->data[Position->index] == '*' ||
            Position->data[Position->index] == '/' ) {
        char element = Position->data[Position->index];
        (Position->index)++;

        Node_t *val_2 = GetP ( Position );

        switch ( element ) {
            case '*' : {
                val = Create_Node( OP, OP_MUL, val, val_2 );
                break;
            }
            case '/' : {
                val = Create_Node( OP, OP_DIV, val, val_2 );
                break;
            }
            default : {
                printf ( "Error" );    // more information
                break;
            }
        }
    }
    return val;
}

Node_t *GetP ( struct Position_t *Position )  // +
{
    if ( Position->data[Position->index] == '(' ) {
        Node_t *val = nullptr;
        (Position->index)++;
        val = GetE ( Position );

        //assert ( Position->data[Position->index] == ')' );
        (Position->index)++;

        return val;
    }
    else if ( isalpha ( Position->data[Position->index] ) ) {

        Node_t *val = GetId ( Position );

        // function Id '(' E ')'

        return val;
    }
    else {

        return Get_Number ( Position );
    }
}

Node_t* GetA ( struct Position_t *Position )  // +- comment
{
    Node_t* val = GetE ( Position );
    // �������� � �� ������ ������� ����� ��������� � ���� ������ � �������� ����������
    if ( Position->data[Position->index] == '=' ||
         Position->data[Position->index] == '>' ||
         Position->data[Position->index] == '<') // �������� ������/������
    {
        char operation = Position->data[Position->index];
        (Position->index)++;
        Node_t* val_2 = GetId ( Position );

        return Create_Node( OP, operation, val, val_2 ); //
    }
    return val;
}

Node_t *GetFunc ( struct Position_t *Position )  // + comment
{
    char arg[20] = {};
    int counter = 0;
    int old_position = Position->index;

    while ( isalpha ( Position->data[Position->index] ) )  {
        sprintf ( arg + counter, "%c", Position->data[Position->index] );
        (Position->index)++;  // up
        counter++;   // counter ++
    }

    if ( Position->data[Position->index] == '(' ) // for functions without an argument
    {
        (Position->index)++;

        if ( Position->data[Position->index] == ')' ) {
            (Position->index)++;
        }
        Node_t* val = GetLim ( Position );    // change name
        (Position->index)++;

        if ( Position->data[Position->index] == '\0' ) { // change

            return Create_Node ( FUNC_HEAD, Search_Func_Name ( arg, Position->name_storage ), val, nullptr ); // add more argument
        }
        else {   // wtf     // if '\n'

            return Create_Node ( FUNC_HEAD, Search_Func_Name ( arg, Position->name_storage ), val, GetFunc ( Position) );  // this
        }
    }
    Position->index = old_position; // wtf

    return GetLim ( Position );
}

Node_t *GetLim ( struct Position_t *Position )   // + comment  // my designation
{
    char arg[20] = {};// name and number
    int counter = 0;
    int old_position = Position->index;

    while ( isalpha ( Position->data[Position->index] ) )  {
        sprintf ( arg + counter, "%c", Position->data[Position->index] );
        (Position->index)++;  // up
        counter++;   // counter ++
    }

    if ( strcmp ( arg, "if" ) == 0 || strcmp ( arg, "while" ) == 0 ) {
        (Position->index) = old_position;
        Node_t* val = GetCondOp ( Position );
        (Position->index)++;

        if ( Position->data[Position->index] != '.' &&  // wtf
             Position->data[Position->index] != '/')   // my designation
        {
            Node_t* val2 = GetLim ( Position );   // name

            return Create_Node ( OP, SLASH, val, val2 );
        }
        return Create_Node ( OP, SLASH, val, nullptr );
    }
    Position->index = old_position;    // why
    Node_t* val = GetA ( Position );
    int prev_index = Position->index;  // prev_index
    (Position->index)++;

    if ( Position->data[Position->index] != '/' &&
         Position->data[Position->index] != '.') { // ��� ����� � �������� => �� ����� ���������� \0

        Node_t* val2 = GetLim ( Position );    // name

        return Create_Node ( OP, LIMIT, val, val2 );
    }
    else if ( Position->data[Position->index] == '/' &&
              Position->data[prev_index]      == '`' ) {

        return Create_Node ( OP, LIMIT, val, nullptr );
    }
    else if ( Position->data[Position->index] == '/' ) {

        return Create_Node ( OP, SLASH, val, nullptr );
    }

    return Create_Node ( OP, LIMIT, val, nullptr );
}

Node_t* GetOp ( struct Position_t *Position )
{
    if ( Position->data[Position->index] == '\\') {
        (Position->index)++;

        return GetLim ( Position );
    }

    return 0;
}

Node_t* GetDifferentCond ( struct Position_t *Position )  // change all
{
    Node_t* val = GetA ( Position );   // name

    if ( Position->data[Position->index] == '&' ) {
        (Position->index) += 2;
        Node_t* val2 = GetDifferentCond ( Position );

        return Create_Node ( OP, AND, val, val2 );
    }

    return val;
}

int Search_Func_Name ( char* name, Name_t *name_cell )   // change all
{
    for (int i = 0; i < size; i++) {
        if ( name_cell[i].type == FUNC ) {
            if (strcmp (name, name_cell[i].value) == 0) {

                return name_cell[i].name_code;
            }
        }
    }
    int cell = Search_Free_Cell ( name_cell );
    name_cell[cell].type = FUNC;
    name_cell[cell].value = strdup ( name );
    name_cell[cell].name_code = cell;

    return cell;
}

int Search_Free_Cell ( Name_t *name_cell ) // change all
{
    for (int i = 0; i < size; ++i) {
        if ( name_cell[i].type == free_cell ) {

            return i;
        }
    }
    //name_cell = (Name*)realloc (name_cell, size * 2); // �������� ����� �������� size
    //return size; // ����� �� ��� ��� ������ ��������, ����� ��������
}

int Search_Var_Name ( char* name, Name_t *name_cell )   // change all
{
    for ( int i = 0; i < size; ++i ) {
        if ( name_cell[i].type == VAR ) {
            if ( strcmp ( name, name_cell[i].value ) == 0 ) {

                return name_cell[i].name_code;
            }
        }
    }
    int cell = Search_Free_Cell ( name_cell );
    name_cell[cell].type = VAR;
    //name_cell[cell].value = ( char *)calloc ( size, sizeof ( char ) );
    name_cell[cell].value = strdup (name);
    name_cell[cell].name_code = cell;

    return cell;
}

Node_t* GetCondOp ( struct Position_t* Position )  //
{
    char arg[20] = "";
    int counter = 0;
    int old_position = Position->index;
    while ( isalpha ( Position->data[Position->index] ) != 0 )
    {
        sprintf (arg + counter, "%c", Position->data[Position->index] );
        (Position->index)++;
        counter++;
    }
    if ( strcmp ( arg, "if" ) == 0 )
    {
        Node_t* val = GetDifferentCond ( Position );
        Node_t* val2 = GetOp ( Position );

        return Create_Node ( KEY_WORD, KEY_W_IF, val, val2);
    }
    else if (strcmp (arg, "while") == 0)
    {
        Node_t* val = GetDifferentCond ( Position );
        Node_t* val2 = GetOp ( Position );

        return Create_Node ( KEY_WORD, KEY_W_WHILE, val, val2);
    }
    return 0;
}

