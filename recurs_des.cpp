//#include "front.h"
#include "recurs_des.h"

Node_t *Get_General ( struct Language_t *language )
{
    assert ( language != nullptr );

    struct Position_t position = {};
    position.data = language->d_array.data;
    assert ( position.data != nullptr );
    position.index = 0;
    language->tree.start = Get_Statement_List ( &position );

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
    else if ( token.type == NODE_TYPE_OP && mode == MODE_MORE_LESS_EQUAL &&
            ( token.cell_code == '>' || token.cell_code == '<' || token.cell_code == '=' ) ) {

        return token.cell_code;
    }

    return 0;
}

Node_t *Get_Expression ( struct Position_t *position )
{
    assert ( position != nullptr );

    Node_t *term_node = Get_Term ( position );

    for ( int element = 0;( element = Get_Binary_Element ( position->data[position->index], MODE_ADD_SUB ) ) != 0; ) {
        int prev_cell = position->index;
        (position->index)++;
        Node_t *term_node_right = Get_Term ( position );

        term_node = Create_Node ( NODE_TYPE_OP, element, term_node, term_node_right );
        term_node->token_n = prev_cell;
    }

    return term_node;
}

Node_t *Get_Term ( struct Position_t *position )
{
    assert ( position != nullptr );

    Node_t *part_node = Get_Power ( position );

    for ( int element = 0; ( element = Get_Binary_Element ( position->data[position->index], MODE_MUL_DIV ) ) != 0; ) {
        int prev_cell = position->index;
        (position->index)++;
        Node_t *part_node_right = Get_Power ( position );

        part_node = Create_Node( NODE_TYPE_OP, element, part_node, part_node_right );
        part_node->token_n = prev_cell;
    }

    return part_node;
}

Node_t *Get_Partititon ( struct Position_t *position )
{
    assert ( position != nullptr );

    struct Node_t *new_node = nullptr;
    int prev_cell = 0;

    if ( position->data[position->index].type == NODE_TYPE_OP &&
         position->data[position->index].cell_code == '(' ) {

        (position->index)++;
        prev_cell = position->index;
        new_node =  Get_Comparison ( position );
        (position->index)++;

        return new_node;
    }
    else {
        prev_cell = position->index;
        new_node = Create_Node ( position->data[position->index].type,
                                 position->data[position->index].cell_code, nullptr, nullptr );
    }
    (position->index)++;
    new_node->token_n = prev_cell;

    return new_node;
}

Node_t *Get_Power ( struct Position_t *position )
{
    assert ( position != nullptr );

    Node_t *part_node_1 = Get_Partititon ( position );

    while ( position->data[position->index].type == NODE_TYPE_OP &&
            position->data[position->index].cell_code == '^' ) { // while or if
        int prev_cell = position->index;
        (position->index)++;
        Node_t *part_node_2 = Get_Partititon ( position );
        part_node_1 = Create_Node( NODE_TYPE_OP, OP_POW, part_node_1, part_node_2 );
        part_node_1->token_n = prev_cell;
    }

    return part_node_1;
}

Node_t *Get_Comparison ( struct Position_t *position )
{
    assert ( position != nullptr );

    Node_t *exp_node = Get_Expression ( position );

    for ( int element = 0; ( element = Get_Binary_Element ( position->data[position->index], MODE_MORE_LESS_EQUAL ) ) != 0;  ) {
        int prev_cell = position->index;
        (position->index)++;
        Node_t *exp_node_right = Get_Expression ( position );

        exp_node = Create_Node ( NODE_TYPE_OP, element, exp_node, exp_node_right );
        exp_node->token_n = prev_cell;
    }

    return exp_node;
}

Node_t *Get_Statement_List ( struct Position_t *position )
{
    assert ( position != nullptr );

    struct Node_t *new_node = nullptr;
    int prev_cell = 0;

    if ( position->data[position->index].type == NODE_TYPE_OP &&
         position->data[position->index].cell_code == '{' ) {  // assert  ( == '{' );

        (position->index)++;
        //prev_cell = position->index;
        while ( position->data[position->index].type == NODE_TYPE_IF ||
                position->data[position->index].type == NODE_TYPE_WHILE ||
                position->data[position->index].type == NODE_TYPE_VAR ||
                position->data[position->index].type == NODE_TYPE_FUNC ||
                ( position->data[position->index].type == NODE_TYPE_OP && position->data[position->index].cell_code == OP_SEMICLON ) ) {

            if ( position->data[position->index].type == NODE_TYPE_IF ||
                 position->data[position->index].type == NODE_TYPE_WHILE ) {
                Node_Type_t prev_type = position->data[position->index].type;
                int prev_cell = position->index;  //
                (position->index)++;

                assert ( position->data[position->index].cell_code == '(' );

                Node_t *comp_node_left = Get_Comparison ( position );   // >   <

                assert  ( position->data[position->index].cell_code == '{' );

                new_node = Get_Statement_List ( position );

                new_node = Create_Node ( prev_type, 0, comp_node_left, new_node );     // 0
                new_node->token_n = prev_cell;

                //assert ( position->data[position->index].cell_code == '}' );
            }
            else if ( position->data[position->index].type == NODE_TYPE_OP &&
                      position->data[position->index].cell_code == OP_SEMICLON ) {
                prev_cell = position->index;
                (position->index)++;
                new_node = Create_Node ( NODE_TYPE_OP, OP_SEMICLON, nullptr, nullptr );
                new_node->token_n = prev_cell;

                //assert ( position->data[position->index].cell_code == '}' );
            }
            else if ( position->data[position->index].type == NODE_TYPE_VAR ) {
                printf ( "LOX\n" );
                new_node = Get_Comparison ( position );
                assert ( position->data[position->index].cell_code == ';' );
                //(position->index)++;
            }
            //assert ( position->data[position->index].cell_code == '}' );
            (position->index)++;     // maybe not

            //new_node = Create_Node ( NODE_TYPE_OP, OP_CONNECT, new_node, nullptr ); //not nullptr
        }
        return new_node;
        //assert ( position->data[position->index].cell_code == '}' );
    }

    return new_node;
}

