#include <stdio.h>
#include <stdlib.h>

#define MAXNAME 20
#define MAXLINE 100

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

VarNode root; // Root of the variable tree

void readLines(char fileName[], char progName[])
{
    FILE *fp;

    if ((fopen(fileName, "r")) == NULL)
    {
        fprintf(stderr, "%s: cannot open %s", progName, fileName);
        exit(2);
    }

    char line[MAXLINE];

    // Get each line one by one
    while (fgets(line, MAXLINE, fp) != NULL)
    {
        processLine(line);
    }
}

void processLine(char line[])
{
    // Find the first '#' if any, and cut off this comment
    for (int i = 0; line[i] != '\0'; i++)
    {
        if (line[i] == '#')
        {
            line[i] = '\0';
            return;
        }
    }

    
}