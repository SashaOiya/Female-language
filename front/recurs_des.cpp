//#include "front.h"
#include "recurs_des.h"

Node_t *Get_General ( char *buffer, struct Node_t *tree_node )   // not char buffer
{
    struct Position_t position = {};
    position.data = buffer;
    position.index = 0;
    tree_node = Get_Comparison ( &position );

    return tree_node;
}

Node_t *Get_Number ( struct Position_t *position )
{
    int value = 0;
    int prev_index = position->index;

    while ( isdigit ( position->data[position->index] ) ) {
        value = value * 10 + position->data[position->index] - '0';
        ++(position->index);
    }
    assert ( position->index > prev_index );

    return Create_Node ( NODE_TYPE_NUM, value, nullptr, nullptr );
}

Node_t *Get_Index ( struct Position_t *position )
{
    char op_name[20] = {};

    for ( int counter = 0; isalpha ( position->data[position->index] ); (position->index)++, counter++ )  {
        op_name[counter] = position->data[position->index];
    }

    int unary_option = 0;
    // #include with #defines maybe
    /*if ( !strcmp ( op_name, "sin" ) ) {
        unary_option = OP_SIN;
    }
    else if ( !strcmp ( op_name, "cos" ) ) {
        unary_option = OP_COS;
    }
    else if ( !strcmp ( op_name, "tg" ) ) {
        unary_option = OP_TG;
    }
    else if ( !strcmp ( op_name, "ctg" ) ) {
        unary_option = OP_CTG;
    }

    if ( unary_option == OP_SIN || unary_option == OP_COS ||
         unary_option == OP_TG  || unary_option == OP_CTG ) {

        Node_t *exp_node = Get_Expression ( position );
        (position->index)++;

        exp_node = Create_Node( NODE_TYPE_OP, unary_option, nullptr, exp_node );

        return exp_node;
    } */

    return Create_Node ( NODE_TYPE_VAR, OP_VAR, nullptr, nullptr );
}

int Get_Binary_Element ( int sign, enum Mode_t mode )
{
    if ( mode == MODE_ADD_SUB && ( sign == '+' || sign == '-' ) ) {

        return sign;
    }
    else if ( mode == MODE_MUL_DIV && ( sign == '*' || sign == '/' ) ) {

        return sign;
    }
    else if ( mode == MODE_MORE_LESS_EQUAL && ( sign == '>' || sign == '<' || sign == '=' ) ) {

        return sign;
    }

    return 0;
}

Node_t *Get_Expression ( struct Position_t *position )
{
    Node_t *term_node = Get_Term ( position );

    int element = 0;
    while ( ( element = Get_Binary_Element ( position->data[position->index], MODE_ADD_SUB ) ) != 0 ) {
        (position->index)++;
        Node_t *term_node_right = Get_Term ( position );

        term_node = Create_Node ( NODE_TYPE_OP, element, term_node, term_node_right );
    }

    return term_node;
}

Node_t *Get_Term ( struct Position_t *position )
{
    Node_t *part_node = Get_Power ( position );

    int element = 0;  // char
    while ( ( element = Get_Binary_Element ( position->data[position->index], MODE_MUL_DIV ) ) != 0 ) {
        (position->index)++;
        Node_t *part_node_right = Get_Power ( position );

        part_node = Create_Node( NODE_TYPE_OP, element, part_node, part_node_right );
    }

    return part_node;
}

Node_t *Get_Partititon ( struct Position_t *position )
{
    if ( position->data[position->index] == '(' ) {
        (position->index)++;
        Node_t *exp_node = Get_Expression ( position ); //

        //assert ( Position->data[Position->index] == ')' );
        (position->index)++;

        return exp_node;
    }
    else if ( isalpha ( position->data[position->index] ) ) {

        return Get_Index ( position );;
    }
    else {

        return Get_Negative_Num ( position );
    }
}

Node_t *Get_Negative_Num ( struct Position_t *position )
{
    assert ( position != nullptr );

    if ( position->data[position->index] == '-' ) {
        (position->index)++;
        Node_t *num_node = Get_Partititon ( position );  \
        //
        Node_t *neg_node = Create_Node ( NODE_TYPE_NUM, -1, nullptr, nullptr );
        Node_t *value    = Create_Node ( NODE_TYPE_OP, OP_MUL, neg_node, num_node );

        return value;
    }

    return Get_Number ( position );
}

Node_t *Get_Power ( struct Position_t *position )
{
    assert ( position != nullptr );

    Node_t *part_node_1 = Get_Partititon ( position );

    if ( position->data[position->index] == '^' ) {
        (position->index)++;
        Node_t *part_node_2 = Get_Partititon ( position );
        part_node_1 = Create_Node( NODE_TYPE_OP, OP_POW, part_node_1, part_node_2 );
    }

    return part_node_1;
}

Node_t *Get_Comparison ( struct Position_t *position )
{
    Node_t *exp_node = Get_Expression ( position );

    int element = 0;
    while ( ( element = Get_Binary_Element ( position->data[position->index], MODE_MORE_LESS_EQUAL ) ) != 0 ) {
        (position->index)++;
        Node_t *exp_node_right = Get_Expression ( position );

        exp_node = Create_Node ( NODE_TYPE_OP, element, exp_node, exp_node_right );
    }

    return exp_node;
}

