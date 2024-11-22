//#include "front.h"
#include "recurs_des.h"

Node_t *Get_General ( struct Language_t *language )
{
    assert ( language != nullptr );

    struct Position_t position = {};
    position.data = language->d_array.data;
    assert ( position.data != nullptr );
    position.index = 0;
    language->tree.start = Get_Func ( &position, language );

    return language->tree.start;
}

int Get_Binary_Element ( const struct Token_t token, const enum Mode_t mode )
{
    if ( token.type == NODE_TYPE_OP && mode == MODE_ADD_SUB &&
       ( token.cell_code == '+' || token.cell_code == '-' ) ) {

        return token.cell_code;
    }
    else if ( token.type == NODE_TYPE_OP && mode == MODE_MUL_DIV &&
            ( token.cell_code == '*' || token.cell_code == '/' ) ) {

        return token.cell_code;
    }
    else if ( token.type == NODE_TYPE_OP && mode == MODE_MORE_LESS_EQUAL_EQUIVALENCE &&
            ( token.cell_code == '>' || token.cell_code == '<' ||
              token.cell_code == '=' || token.cell_code == '~' ) ) {

        return token.cell_code;
    }

    return 0;
}

Node_t *Get_Expression ( struct Position_t *position )
{
    assert ( position != nullptr );

    Node_t *term_node = Get_Term ( position );

    for ( int element = 0;( element = Get_Binary_Element ( position->data[position->index], MODE_ADD_SUB ) ) != 0; ) {
        (position->index)++;
        Node_t *term_node_right = Get_Term ( position );

        term_node = Create_Node ( NODE_TYPE_OP, element, term_node, term_node_right );
    }

    return term_node;
}

Node_t *Get_Term ( struct Position_t *position )
{
    assert ( position != nullptr );

    Node_t *part_node = Get_Power ( position );

    for ( int element = 0; ( element = Get_Binary_Element ( position->data[position->index], MODE_MUL_DIV ) ) != 0; ) {
        (position->index)++;
        Node_t *part_node_right = Get_Power ( position );

        part_node = Create_Node( NODE_TYPE_OP, element, part_node, part_node_right );
    }

    return part_node;
}

Node_t *Get_Partititon ( struct Position_t *position )
{
    assert ( position != nullptr );

    struct Node_t *new_node = nullptr;

    if ( position->data[position->index].type == NODE_TYPE_OP &&
         position->data[position->index].cell_code == '(' ) {

        (position->index)++;
        new_node =  Get_Comparison ( position );

        assert ( position->data[position->index].cell_code == ')' );

        (position->index)++;

        return new_node;
    }
    else if ( position->data[position->index].type == NODE_TYPE_FUNC ) {   ////// Get_Func
        new_node = Create_Node ( NODE_TYPE_FUNC, position->data[position->index].cell_code, nullptr, nullptr );

        (position->index)++;
        assert ( position->data[position->index].cell_code == '(' );  //
        (position->index)++;

        new_node = Get_Func_Arg ( position, &new_node );

        assert ( position->data[position->index].cell_code == ')' );  //
        (position->index)++;

        //assert ( position->data[position->index].cell_code == ';' );  //
        //(position->index)++;

        return new_node;
    }
    else if ( position->data[position->index].type == NODE_TYPE_INPUT ||
              position->data[position->index].type == NODE_TYPE_OUTPUT ) {
        new_node = Create_Node ( position->data[position->index].type, position->data[position->index].cell_code, nullptr, nullptr );
        assert ( position->data[position->index+1].cell_code == '(' );
        assert ( position->data[position->index+2].cell_code == ')' );
        position->index += 2;
    }
    else {
        new_node = Create_Node ( position->data[position->index].type,
                                 position->data[position->index].cell_code, nullptr, nullptr );
    }
    (position->index)++;

    return new_node;
}

Node_t *Get_Power ( struct Position_t *position )
{
    assert ( position != nullptr );

    Node_t *part_node_1 = Get_Partititon ( position );

    while ( position->data[position->index].type == NODE_TYPE_OP &&
            position->data[position->index].cell_code == '^' ) {
        (position->index)++;
        Node_t *part_node_2 = Get_Partititon ( position );
        part_node_1 = Create_Node( NODE_TYPE_OP, OP_POW, part_node_1, part_node_2 );
    }

    return part_node_1;
}

Node_t *Get_Comparison ( struct Position_t *position )
{
    assert ( position != nullptr );

    Node_t *exp_node = Get_Expression ( position );

    for ( int element = 0; ( element = Get_Binary_Element ( position->data[position->index], MODE_MORE_LESS_EQUAL_EQUIVALENCE ) ) != 0;  ) {
        (position->index)++;
        Node_t *exp_node_right = Get_Expression ( position );

        exp_node = Create_Node ( NODE_TYPE_OP, element, exp_node, exp_node_right );
    }

    return exp_node;
}

Node_t *Get_Statement_List ( struct Position_t *position )  // +
{
    assert ( position != nullptr );

    struct Node_t *new_node = nullptr;

    if ( position->data[position->index].type != NODE_TYPE_IF     &&
         position->data[position->index].type != NODE_TYPE_ELSE   &&
         position->data[position->index].type != NODE_TYPE_WHILE  &&
         position->data[position->index].type != NODE_TYPE_VAR    &&
         position->data[position->index].type != NODE_TYPE_FUNC   &&
         position->data[position->index].type != NODE_TYPE_RETURN &&
       ( position->data[position->index].type != NODE_TYPE_OP || position->data[position->index].cell_code != OP_SEMICLON ) ) {

        return new_node;
    }

    Node_t *state_node = Get_Statement ( position );
    Node_t *else_node = nullptr;
    Node_t *func_node_right = Get_Statement_List ( position );
    new_node = Create_Node ( NODE_TYPE_OP, OP_CONNECT, state_node, func_node_right );

    return new_node;
}

Node_t *Get_Statement ( struct Position_t *position )
{
    assert ( position != nullptr );

    Node_t *new_node = nullptr;

    if ( position->data[position->index].type == NODE_TYPE_IF ||
         position->data[position->index].type == NODE_TYPE_WHILE ||
         position->data[position->index].type == NODE_TYPE_ELSE ) {   // remove else
        //
        Node_Type_t prev_type = position->data[position->index].type;
        (position->index)++;

        assert ( position->data[position->index].cell_code == '(' );    //

        Node_t *comp_node_left = Get_Comparison ( position );   // >   <

        assert ( position->data[position->index].cell_code == '{' );    //
        (position->index)++;

        new_node = Get_Statement_List ( position );
        new_node = Create_Node ( prev_type, 0, comp_node_left, new_node );     // 0

        assert ( position->data[position->index].cell_code == '}' );   //
        (position->index)++;

        //Node_t *else_node = Get_Else_Node ( position );
        //new_node->right = else_node;

        return new_node;
    }
    else if ( position->data[position->index].type == NODE_TYPE_OP &&
              position->data[position->index].cell_code == OP_SEMICLON ) {

        assert ( position->data[position->index - 1].cell_code == '{' );  //

        (position->index)++;
        new_node = Create_Node ( NODE_TYPE_OP, OP_SEMICLON, nullptr, nullptr );

        assert ( position->data[position->index].cell_code == '}' );      //

        return new_node;
    }
    else if ( position->data[position->index].type == NODE_TYPE_VAR ) {
        new_node = Get_Comparison ( position );

        assert ( position->data[position->index].cell_code == ';' );
    }
    else if ( position->data[position->index].type == NODE_TYPE_FUNC ) {
        new_node = Create_Node ( NODE_TYPE_FUNC, position->data[position->index].cell_code, nullptr, nullptr );

        (position->index)++;
        assert ( position->data[position->index].cell_code == '(' );  //
        (position->index)++;

        new_node = Get_Func_Arg ( position, &new_node );

        assert ( position->data[position->index].cell_code == ')' );  //
        (position->index)++;

        assert ( position->data[position->index].cell_code == ';' );  //
        (position->index)++;

        return new_node;
    }
    else if ( position->data[position->index].type == NODE_TYPE_RETURN ) {
        Node_Type_t prev_type = position->data[position->index].type;
        (position->index)++;

        assert ( position->data[position->index].cell_code == '(' );    //

        Node_t *exp_node_left = nullptr ;

        if ( position->data[position->index+1].cell_code != ')' ) {
            (position->index)++;
            assert ( position->data[position->index].type == NODE_TYPE_NUM ||
                     position->data[position->index].type == NODE_TYPE_VAR );
            exp_node_left = Create_Node ( position->data[position->index].type,
                                          position->data[position->index].cell_code, nullptr, nullptr );
        }
        (position->index) += 2;

        new_node = Create_Node ( prev_type, position->data[position->index].cell_code, exp_node_left, nullptr );

        assert ( position->data[position->index].cell_code == ';' );
    }
    (position->index)++;

    return new_node;
}

Node_t *Get_Func ( struct Position_t *position, struct Language_t *language )
{
    assert ( position != nullptr );

    Node_t *func_node = nullptr;

    if ( position->data[position->index].type != NODE_TYPE_FUNC ) {

        return func_node;
    }

    func_node = Create_Node ( NODE_TYPE_FUNC, position->data[position->index].cell_code, nullptr, nullptr );
    (position->index)++;

    assert ( position->data[position->index].cell_code == '(' );   //
    (position->index)++;                             //////////////////////////////////////////////////
                                                                           /////
    func_node = Get_Func_Arg ( position, &func_node );

    assert ( position->data[position->index].cell_code == ')' );  //
    (position->index)++;

    assert ( position->data[position->index].cell_code == '{' );  //
    (position->index)++;

    Node_t *state_list_node = Get_Statement_List ( position );

    assert ( position->data[position->index].cell_code == '}' );  //
    (position->index)++;

    func_node->right = state_list_node;

    Node_t *func_node_right = Get_Func ( position, language );

    func_node = Create_Node ( NODE_TYPE_OP, OP_CONNECT, func_node, func_node_right );

    return func_node;
}

Node_t *Get_Func_Arg ( struct Position_t *position, Node_t **func_node )
{
    assert ( position  != nullptr );
    assert ( func_node != nullptr && *func_node != nullptr );

    Node_t *start_node = *func_node;
                                                 // NODE_TYPE_NUM
    for ( ; position->data[position->index].type == NODE_TYPE_VAR ||
            position->data[position->index].type == NODE_TYPE_NUM ; ++(position->index) ) {
            (*func_node)->left = Create_Node ( position->data[position->index].type, position->data[position->index].cell_code, nullptr, nullptr );
            *func_node = (*func_node)->left;
            if ( position->data[position->index+1].type == NODE_TYPE_OP &&
                 position->data[position->index+1].cell_code == OP_COMMA ) {
                ++(position->index);
            }
        }

    *func_node = start_node;

    return *func_node;
}


