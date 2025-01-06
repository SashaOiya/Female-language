#include "midl.h"

int Optimization_Const  ( struct Node_t **tree_node ) // +
{
    if ( *tree_node == nullptr || (*tree_node)->right == nullptr || (*tree_node)->left == nullptr ) {
$
        return 0;
    }
$   if ( (*tree_node)->left->type  == NODE_TYPE_NUM &&
         (*tree_node)->right->type == NODE_TYPE_NUM  ) {

$       (*tree_node)->value = Eval_Body ( *tree_node );
$       (*tree_node)->type = NODE_TYPE_NUM;
$       Node_Free ( &((*tree_node)->left)  );
$       Node_Free ( &((*tree_node)->right) );

        return OK_OCCURR;
    }
    else {
$
        return ( Optimization_Const ( &((*tree_node)->left ) ) ||
                 Optimization_Const ( &((*tree_node)->right) ) );
    }
}

int Optimization_Option ( struct Node_t **tree_node )  // +-
{
$   if ( *tree_node == nullptr || (*tree_node)->right == nullptr || (*tree_node)->left == nullptr ) {
$
        return 0;
    }
$   if ( (*tree_node)->type == NODE_TYPE_OP && (*tree_node)->value == OP_MUL ) {
        if ( (*tree_node)->left->value  == 0 || (*tree_node)->right->value == 0 ) {
$           (*tree_node)->value = 0;
$           (*tree_node)->type  = NODE_TYPE_NUM;
$           Node_Free ( &((*tree_node)->left)  );
$           Node_Free ( &((*tree_node)->right) );

            return OK_OCCURR;
        }
        if ( (*tree_node)->left->value  == 1 ) {  // func
            Node_Free ( &((*tree_node)->left) );
            (*tree_node) = (*tree_node)->right;

            return OK_OCCURR;
        }
        else if ( (*tree_node)->right->value == 1 ) {
            Node_Free ( &((*tree_node)->right) );
            *tree_node = (*tree_node)->left; // for this moment

            return OK_OCCURR;
        }
$
        return 0;
    }
    /* else if ( (*tree_node)->value == OP_ADD || (*tree_node)->value == OP_SUB ) {
        if ( (*tree_node)->right->type  == NODE_TYPE_NUM &&
             (*tree_node)->right->value == 0 ) {
            Node_Free ( &((*tree_node)->right) );
            *tree_node = (*tree_node)->left;
$
            return OK_OCCURR;
        }
        else if ( (*tree_node)->left->type  == NODE_TYPE_NUM &&
                  (*tree_node)->left->value == 0 ) {
            Node_Free ( &((*tree_node)->left) );
            *tree_node = (*tree_node)->right;
$           //

            return OK_OCCURR;
        }
$
        return ( Optimization_Option ( &(*tree_node)->left ) ||
                 Optimization_Option ( &(*tree_node)->right )  );
    }  */
    else {
$
       return ( Optimization_Option ( &(*tree_node)->left ) ||
                Optimization_Option ( &(*tree_node)->right )  );
    }
}

void Optimization ( struct Node_t *tree_node )
{
    while ( Optimization_Const  ( &tree_node ) || Optimization_Option ( &tree_node ) );
}

int Eval_Body ( const struct Node_t *node ) // - double or int
{
    if ( node == nullptr ) {

        return 0;
    }
    if ( node->type == NODE_TYPE_NUM ) {

        return node->value;
    }

    double left  = Eval_Body ( node->left );
    double right = Eval_Body ( node->right );

    if ( node->type == NODE_TYPE_OP ) {
        switch ( node->value ) {
            case OP_ADD : {

                return left + right;
                break;
            }
            case OP_SUB : {

                return left - right;
                break;
            }
            case OP_DIV : {
                if ( fabs ( right ) < eps ) {
                    printf ( "Error : div by 0\n");

                    return 0;
                }
                return left / right;
                break;
            }
            case OP_MUL : {

                return left * right;
                break;
            }
            case OP_POW : {
                return pow ( left, right );
                break;
            }
            default : {
                printf ( "ERROR\n" );

                break;
            }
        }
    }

    return ERR_CTYPE;
}

Node_t *difference ( const struct Node_t *tree ) 
{
    if ( tree == nullptr ) {

        return nullptr;
    }
    if ( tree->type == NODE_TYPE_NUM ) {

        return Create_Node ( NODE_TYPE_NUM, 0, nullptr, nullptr );
    }
    else if ( tree->type == NODE_TYPE_VAR ) {

        return Create_Node ( NODE_TYPE_NUM, 1, nullptr, nullptr );
    }
    else if ( tree->type == NODE_TYPE_OP ){
        switch ( tree->value ) {
            case OP_ADD : {
                Node_t *tree_c = Create_Node ( NODE_TYPE_OP, OP_ADD, difference ( tree->left ), difference ( tree->right ) );
                Optimization ( tree_c );

                return tree_c;
                break;
            }
            case OP_SUB : {
                Node_t *tree_c = Create_Node ( NODE_TYPE_OP, OP_SUB, difference ( tree->left ), difference ( tree->right ) );
                Optimization ( tree_c );

                return tree_c;
                break;
            }
            case OP_MUL : {
                Node_t *tree_c = Create_Node ( NODE_TYPE_OP, OP_ADD, Create_Node ( NODE_TYPE_OP, OP_MUL, difference ( tree->left ), Copy_Node ( tree->right ) ),
                                                                     Create_Node ( NODE_TYPE_OP, OP_MUL, Copy_Node ( tree->left ), difference ( tree->right ) ) );
                Optimization ( tree_c );

                return tree_c;
                break;
            }
            case OP_DIV : {
                Node_t *tree_c = Create_Node ( NODE_TYPE_OP, OP_DIV, Create_Node ( NODE_TYPE_OP, OP_SUB, Create_Node ( NODE_TYPE_OP, OP_MUL, difference ( tree->left ), Copy_Node ( tree->right ) ),
                                                                                                         Create_Node ( NODE_TYPE_OP, OP_MUL, Copy_Node ( tree->left ), difference ( tree->right ) ) ),
                                                                     Create_Node ( NODE_TYPE_OP, OP_MUL, Copy_Node ( tree->right ), Copy_Node ( tree->right ) ) );
                Optimization ( tree_c );

                return tree_c;
                break;
            }
            default : {
                printf ( "Option error\n" );
                break;
            }
        }
    }
    return nullptr;
}   

