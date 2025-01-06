#ifndef MIDL
#define MIDL

#include <cmath>
#include "front.h"

const int eps = 0.00001;

int Optimization_Const ( struct Node_t *tree );
int Optimization_Option ( struct Node_t **tree );
void Optimization ( struct Node_t *tree );
int Eval_Body ( const struct Node_t *node );
Node_t *difference ( const struct Node_t *tree );

#endif  // MIDL
