#include <stdio.h>

#define MAXNAME 20

typedef struct Variables
{
    char type[7];           // Type of the variable, either scalar
                            // or matrix. Vector is countes as matrix
    char name[MAXNAME + 1]; // Name of the variable
    int fir_dim;            // First dimension
    int sec_dim;            // Second dimension
} Variable;

typedef struct VariableNodes
{
    Variable *var;  // Variable of this node
    VarNode *left;  // VarNode on the left
    VarNode *right; // VarNode on the right. Variables on
                    // the left has names that come earlier
                    // on the dictionary
} VarNode;