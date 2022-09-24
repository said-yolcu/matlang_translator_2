
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "definitions.h"

LineBlock *blockList; // Linked list to store the lineBlocks of the program.
                      // This is not null, but is an empty block
LineBlock *lastBlock; // The place to add blocks to the list. It is null
VarNode *theRoot;     // Root of the variable tree
int lineNo = 0;       // Current number of line that is being read

void raiseError(int code, ...); // RaiseError() is used in a lot of functions
                                // so we should declare it here rather than in
                                // each function

int main(int argc, char const *argv[])
{
    void readLines(const char *, const char *);
    void printTree(VarNode *);

    blockList = (LineBlock *)malloc(sizeof(LineBlock));
    lastBlock = NULL;
    blockList->next = lastBlock;

    theRoot = NULL;

    char fileName[] = "/home/said/school/2022_spring_semester/cmpe_230/homeworks/hw_1/ex1.txt";

    readLines(fileName, argv[0]);
    printTree(theRoot);
    // printf("%s\n", theRoot->var->name);
    return 0;
}

void readLines(const char fileName[], const char progName[])
{
    // void processLine(const FILE *, char *);
    char *getWord(const FILE *, char *, int);
    void scalDef(const FILE *);
    void vectDef(const FILE *);
    void getAsn(const FILE *, char *);
    void matrDef(const FILE *);

    FILE *fp;

    // DO NOT Allocate storage to root
    // root = (VarNode *)malloc(sizeof(VarNode));

    if ((fp = fopen(fileName, "r")) == NULL)
    {
        fprintf(stderr, "%s: cannot open %s", progName, fileName);
        exit(2);
    }
    // printf("line 135\n");
    char word[WORDLEN];

    int prevLine = 0;

    // Get each line one by one
    while (getWord(fp, word, WORDLEN) != NULL)
    {
        if (strcmp(word, "scalar") == 0)
        {
            // Scalar definition
            // printf("A scalar\n");
            scalDef(fp);
        }
        else if (strcmp(word, "vector") == 0)
        {
            // Vector definition
            // printf("A vector\n");
            vectDef(fp);
        }
        else if (strcmp(word, "matrix") == 0)
        {
            // Matrix definition
            matrDef(fp);
        }
        else if (strcmp(word, "print") == 0)
        {
            // Print statement
        }
        else if (strcmp(word, "printsep") == 0)
        {
            // Printsep statement
        }
        else if (strcmp(word, "for") == 0)
        {
            // Forl statement
        }
        else if (strcmp(word, "choose") == 0)
        {
            // Choose statement
        }
        else if (strcmp(word, "sqrt") == 0)
        {
            // Sqrt statement
        }
        else if (strcmp(word, "tr") == 0)
        {
            // Tr statement
        }
        else if (strcmp(word, "\n") == 0) // New line is a distinct word
        {
            continue;
        }
        else
        {
            // Possible assignment statement, must check for '='
            getAsn(fp, word);
        }

        // printf("%s\t", word);
    }
    fclose(fp);
}
/*
void processLine(const FILE *fp, char line[])
{
    char *getWord(char *line, char *, int);

    // Find the first '#' if any, and cut off this comment
    for (int i = 0; line[i] != NULLCHAR; i++)
    {
        if (line[i] == '#')
        {
            line[i] = NULLCHAR;
            return;
        }
    }

    char word[WORDLEN];

    while (getWord(line, word, WORDLEN) != NULL)
    {
        printf("%s\n", word);
    }
}
*/
/*
Checks whether the current line is a scalar definition.
Creates a variable accordingly, if so.
*/
void scalDef(const FILE *fp)
{
    char *getWord(const FILE *fp, char *word, int lim);
    VarNode *addTree(VarNode *, Variable *);

    char name[MAXNAME + 1];
    char word[MAXEXPR + 1];

    // printf("line 224\n");

    if (getWord(fp, word, MAXNAME) == NULL || strcmp(name, "\n") == 0)
    {
        raiseError(1); // Error: no variable name specified
    }
    strcpy(name, word);

    // printf("line 232\n");

    if (getWord(fp, word, MAXEXPR) != NULL && strcmp(word, "\n") != 0)
    {
        raiseError(2); // Error: too many arguments for variable declaration
    }

    // printf("line 239\n");

    // Allocate storage for the variable
    Variable *varPtr = (Variable *)malloc(sizeof(Variable));

    // printf("line 244\n");

    // Put the type and name
    varPtr->type = VARSCA;
    // printf("line 248\n");
    strcpy(varPtr->name, name);

    // printf("line 251\n");

    // Add the node to the tree
    theRoot = addTree(theRoot, varPtr);
    /*
    // Create the lineBlock
    LineBlock *blockPtr = (LineBlock *)malloc(sizeof(LineBlock));
    blockPtr->type = SCA;
    blockPtr->statement.var = varPtr;

    // Add it to the blockList
    lastBlock = addBlock(lastBlock, blockPtr);
    // Point to the next position
    lastBlock = lastBlock->next;
    */
}
/*
Get a vector definition. Create an according variable in the tree
*/
void vectDef(const FILE *fp)
{

    char *getWord(const FILE *fp, char *word, int lim);
    VarNode *addTree(VarNode *, Variable *);
    int getInt(char *, int, int);

    char name[MAXNAME + 1];
    char word[MAXEXPR + 1];
    char dimStr[MAXEXPR + 1];
    int dim;

    if (getWord(fp, word, MAXNAME) == NULL || strcmp(name, "\n") == 0)
    {
        raiseError(1); // Error: no variable name specified
    }
    strcpy(name, word);

    if (getWord(fp, word, MAXEXPR) == NULL || strcmp(word, "\n") == 0)
    {
        printf("line 286\n");
        raiseError(4); // Error: no dimension specified, too few arguments
    }
    // printf("line 288\n");

    // Get the dimension
    dim = getInt(word, 1, strlen(word) - 2);
    // printf("line 292\n");

    if (getWord(fp, word, MAXEXPR) != NULL && strcmp(word, "\n") != 0)
    {
        raiseError(2); // Error: too many arguments for variable declaration
    }

    // Allocate storage for the variable
    Variable *varPtr = (Variable *)malloc(sizeof(Variable));

    // Put the type and name
    varPtr->type = VARVEC;
    strcpy(varPtr->name, name);
    varPtr->fir_dim = dim;
    // Allocate memory for the array
    varPtr->arr = (int *)malloc(dim * sizeof(int));

    // Add the node to the tree
    theRoot = addTree(theRoot, varPtr);
    /*
    // Create the lineBlock
    LineBlock *blockPtr = (LineBlock *)malloc(sizeof(LineBlock));
    blockPtr->type = VEC;
    blockPtr->statement.var = varPtr;

    // Add it to the blockList
    lastBlock = addBlock(lastBlock, blockPtr);
    // Point to the next position
    lastBlock = lastBlock->next;
    */
}

/*
Get a matrix definition. Create an according variable in the tree
*/
void matrDef(const FILE *fp)
{

    char *getWord(const FILE *fp, char *word, int lim);
    VarNode *addTree(VarNode *, Variable *);
    int getInt(char *, int, int);

    char name[MAXNAME + 1];
    char word[MAXEXPR + 1];
    char dimStr[MAXEXPR + 1];
    int fir_dim;
    int sec_dim;

    if (getWord(fp, word, MAXNAME) == NULL || strcmp(name, "\n") == 0)
    {
        raiseError(1); // Error: no variable name specified
    }
    strcpy(name, word);

    if (getWord(fp, word, MAXEXPR) == NULL || strcmp(word, "\n") == 0)
    {
        printf("line 333\n");
        raiseError(4); // Error: no dimension specified, too few arguments
    }

    // Index of the first comma
    int comma = (int)(strchr(word, ',') - word);
    // printf("line 338\n");

    // Get the  first dimension
    fir_dim = getInt(word, 1, comma - 1);
    // printf("line 342\n");

    // Get the  second dimension
    sec_dim = getInt(word, comma + 1, strlen(word) - 2);
    // printf("line 346\n");

    if (getWord(fp, word, MAXEXPR) != NULL && strcmp(word, "\n") != 0)
    {
        raiseError(2); // Error: too many arguments for variable declaration
    }

    // Allocate storage for the variable
    Variable *varPtr = (Variable *)malloc(sizeof(Variable));

    // Put the type and name
    varPtr->type = VARMAT;
    strcpy(varPtr->name, name);
    varPtr->fir_dim = fir_dim;
    varPtr->sec_dim = sec_dim;
    // Allocate memory for the array
    varPtr->arr = (int *)malloc(fir_dim * sec_dim * sizeof(int));
    varPtr->arr_step = sec_dim;

    // Add the node to the tree
    theRoot = addTree(theRoot, varPtr);
    /*
    // Create the lineBlock
    LineBlock *blockPtr = (LineBlock *)malloc(sizeof(LineBlock));
    blockPtr->type = MAT;
    blockPtr->statement.var = varPtr;

    // Add it to the blockList
    lastBlock = addBlock(lastBlock, blockPtr);
    // Point to the next position
    lastBlock = lastBlock->next;
    */
}

void getAsn(const FILE *fp, char *word)
{
    // printf("line 331\n");
    char *getWord(const FILE *fp, char *word, int lim);
    VarNode *addTree(VarNode *, Variable *);
    Variable *getVar(VarNode *, char *);
    int getInt(char *, int, int);
    Expression *expressify(char *, int, int);
    void printExpr(Expression *);

    char str[MAXNAME];
    strcpy(str, word);
    // 1
    if (strcmp(str, "=") == 0)
    {
        // Raise error: too early an assignment operator or empty line
        printf("line 345\n");
        exit(1);
    }
    // This is the name of the assignment-destination variable
    char name[MAXNAME];
    strcpy(name, str);
    // 2
    //  Get the var from the variable tree
    Variable *varPtr = getVar(theRoot, name);

    if (varPtr == NULL)
    {
        // Raise error: No such variable exists
        printf("line 356\n");
        printf("%s\n", word);
        exit(1);
    }
    // 3
    //  Create an assign struct
    Assign *assignment = (Assign *)malloc(sizeof(Assign));

    assignment->destVar = varPtr;

    if (getWord(fp, str, MAXEXPR) == NULL || strcmp(str, "\n") == 0)
    {
        // Raise error: meaningless line
        printf("line 370\n");
        exit(1);
    }
    // 4
    int type;    // Type of the variable
    int fir_pos; // First index position in array
    int sec_pos; // Second index position in array

    if (strcmp(str, "=") != 0)
    {
        // printf("line 374\n");
        //  Then it is a [] block
        if ((type = varPtr->type) == VARVEC)
        {
            // printf("line 377\n");
            fir_pos = getInt(str, 1, strlen(str) - 2);
            if (fir_pos > varPtr->fir_dim || fir_pos <= 0)
            {
                // Raise error: index is out of bounds
            }
            assignment->fir_pos = fir_pos;
        }
        else if (type == VARMAT)
        {
            // printf("line 388\n");
            int comma = (int)(strchr(str, ',') - str);
            fir_pos = getInt(str, 1, comma - 1);
            sec_pos = getInt(str, comma + 1, strlen(str) - 2);
            if (fir_pos > varPtr->fir_dim || fir_pos <= 0)
            {
                // Raise error: first index is out of bounds
            }
            if (sec_pos > varPtr->sec_dim || sec_pos <= 0)
            {
                // Raise error: second index is out of bounds
            }
            assignment->fir_pos = fir_pos;
            assignment->sec_pos = sec_pos;
        }
        else
        {
            printf("line 405\n");
            // Raise error: scalar cannot take brackets
            exit(1);
        }

        // Get the next word after the brackets
        if (getWord(fp, str, MAXEXPR) == NULL)
        {
            // Raise error: no word after the brackets
            exit(1);
        }
    }

    // This word should be an assignment operator
    if (/*strcmp(str, "\n") == 0 ||*/ strcmp(str, "=") != 0)
    {
        // Raise error: we are waiting an assignment operator here
        printf("line 433\n");
        exit(1);
    }

    char expr[MAXEXPR];
    strcpy(expr, "");

    if (getWord(fp, str, MAXEXPR) != NULL && strcmp(str, "\n") != 0)
    {
        if (strcmp(str, "{") == 0)
        {
            // This is a curly bracket assignment
            strcat(expr, str);

            while (getWord(fp, str, MAXEXPR) != NULL && strcmp(str, "\n") != 0)
            {
                strcat(expr, str);
            }

            // Process the curly bracket inputs here

            return;
        }
        else
        {
            strcat(expr, str);
        }
    }
    else
    {
        return;
    }

    while (getWord(fp, str, MAXEXPR) != NULL && strcmp(str, "\n") != 0)
    {
        strcat(expr, str);
    }

    assignment->rightSide = expressify(expr, 0, strlen(expr) - 1);
    printExpr(assignment->rightSide);
    //
}

/*
Get the int value in the string betweem the beg and end indices.
Discard preceding and succeeding white spaces.
*/
int getInt(char *str, int beg, int end)
{

    while (isspace(str[beg++]))
        ;

    --beg;
    // printf("line 369\n");
    if (beg > end)
    {
        // printf("line 372\n");
        //  Raise error: No integer
        raiseError(4);
    }
    // printf("line 376\n");

    int theInt = 0;
    int c = str[beg++]; // Initialize it, otherwise it will be space

    if (c < '1' || c > '9')
    {
        raiseError(6); // Expected an integer, this is not an integer
    }
    theInt = 10 * theInt + c - '0';

    c = str[beg++];

    while (beg <= end && !isspace(c))
    {
        if (c < '0' || c > '9')
        {
            raiseError(6); // Expected an integer, this is not an integer
        }
        theInt = 10 * theInt + c - '0';
        c = str[beg++];
    }

    while (beg <= end)
    {
        if (!isspace(c))
        {
            // Raise error: too many dimensions specified
            raiseError(2);
        }
        c = str[beg++];
    }

    return theInt;
}

VarNode *addTree(VarNode *root, Variable *var)
{

    // printf("line 259\n");
    int cond;

    if (root == NULL)
    {
        // printf("line 263\n");

        root = (VarNode *)malloc(sizeof(VarNode));
        root->var = var;
        // printf("%s\n", root->var->name);
        root->left = root->right = NULL;
        // printf("%s\n", theRoot->var->name);
        //  return 0; // Successfully added
    }
    else if ((cond = strcmp(var->name, root->var->name)) > 0) // Go to right child
    {
        // printf("line 272\n");

        root->right = addTree(root->right, var);
    }
    else if (cond < 0) // Go to left
    {
        // printf("line 278\n");

        root->left = addTree(root->left, var);
    }
    else // If they are equal
    {
        // printf("line 284\n");

        raiseError(3, var->name); // Error: a variable with same id is already declared
    }
    return root;
}

LineBlock *addBlock(LineBlock *list, LineBlock *newBlock)
{
    if (list == NULL)
    {
        // The according storage is already created. Do not create it again
        // list = (LineBlock *)malloc(sizeof(LineBlock));
        list = newBlock;
        list->next = NULL;
        return list;
    }

    fprintf(stderr, "Error in addBlock function\n");
    exit(1);
}

/*
Print the variable tree in ascending name order
*/
void printTree(VarNode *root)
{
    // printf("line 544\n");
    if (!root)
    {
        // printf("line 295\n");
        return;
    }
    // printf("line 303\n");
    printTree(root->left);
    // printf("line 306\n");
    switch (root->var->type)
    {
    case VARSCA:
        printf("scalar %s\n", root->var->name);
        break;

    case VARVEC:
        printf("vector %s %c %d %c\n", root->var->name, '[', root->var->fir_dim, ']');
        break;

    case VARMAT:
        printf("matrix %s %c %d %c %d %c\n", root->var->name,
               '[', root->var->fir_dim, ',', root->var->sec_dim, ']');
        break;

    default:
        break;
    }
    // printf("line 316\n");
    printTree(root->right);
}
/*
Return a pointer to the variable. Return null pointer if the requested var
do not exist.
*/
Variable *getVar(VarNode *root, char *varName)
{
    // printf("line 595\n");
    if (root == NULL)
    {
        // printf("line 598\n");
        return NULL;
    }

    int cond;

    if ((cond = strcmp(varName, root->var->name)) > 0)
    {
        // printf("line 606\n");
        return getVar(root->right, varName);
    }
    else if (cond < 0)
    {
        // printf("line 611\n");
        return getVar(root->left, varName);
    }
    else // If the searched name is in this node
    {
        // printf("line 616\n");
        // printf("line 646 %s\n", root->var->name);
        return root->var;
    }
}

/*
Create and return a linked list of expressions out of given string
between the given indices.
*/
Expression *expressify(char *str, int begin, int end)
{
    // printf("line 647 %s\n", str);
    if (begin > end)
    {
        return NULL;
    }
    // int level = 0; // Level of the subexpression in the whole expression
    int i;
    int staIn;
    char varName[MAXNAME]; // Variable name encountered
    int varIn = 0;
    // Expression *headExpr;           // Expression at the head of the linked list
    //  Prev of the head is always null
    Expression *curExpr; /* = headExpr;*/ // Current expression

    int varOperNum = 0;
    int maxLev = 0;

    // Convert it to a list of expressions
    // Curly bracket assignment will be evaluated somewhere else
    for (i = begin; str[i] != NULLCHAR && i <= end; i++)
    {
        if (isspace(str[i]))
        {
            continue;
        }
        else if (str[i] == '(')
        {
            // A new expression
            curExpr = (Expression *)malloc(sizeof(Expression));
            // Level is unnecessary in new design
            // curExpr->level = level;
            curExpr->index = i; // Start index of the expression
            // Make it -1 until you find the closing paranthesis
            curExpr->endIn = -1;
            curExpr->type = TYPAR;
            curExpr->prec = PREPAR;
            // maxLev = max(maxLev, level);

            // Find the closing paranthesis
            int parNum = 0;
            for (;; i++)
            {
                if (str[i] == NULLCHAR || i > end)
                {
                    // Raise error: unended block
                    printf("line 693 at i %d with open par at %d\n", i,
                           curExpr->index);
                    exit(1);
                }

                if (str[i] == '(')
                    parNum++;
                else if (str[i] == ')')
                {
                    parNum--;
                    // Block end is found
                    if (parNum == 0)
                    {
                        curExpr->endIn = i;
                        // i will be increased in the outer loop anyway
                        // i++;
                        // Add the subexpressions
                        curExpr->sub = expressify(str, curExpr->index + 1, curExpr->endIn - 1);
                        // Point to the next expression
                        // curExpr = curExpr->next;
                        break;
                    }
                }
            }
            break;
        }
        else if (str[i] == ')')
        {
            // Raise error: unbeginned block
            // If a closing paranthesis is encountered at here, then it
            // means that it has no corresponding beginning
            printf("line 716 at i %d\n", i);
            exit(1);
        }
        else if (str[i] == '[')
        {
            // A new expression
            curExpr = (Expression *)malloc(sizeof(Expression));
            // Level is unnecessary in new design
            // curExpr->level = level;
            curExpr->index = i; // Start index of the expression
            // Make it -1 until you find the closing paranthesis
            curExpr->endIn = -1;
            curExpr->type = TYPAR;
            curExpr->prec = PREPAR;
            // maxLev = max(maxLev, level);

            // Find the closing paranthesis
            int parNum = 0;
            for (;; i++)
            {
                if (str[i] == NULLCHAR || i > end)
                {
                    // Raise error: unended block
                }

                if (str[i] == '(')
                    parNum++;
                else if (str[i] == ')')
                {
                    parNum--;
                    // Block end is found
                    if (parNum == 0)
                    {
                        curExpr->endIn = i;
                        // i++;
                        //  Add the subexpressions
                        int comma = (int)(strchr(str, ',') - str);
                        if (curExpr->index < comma && comma < curExpr->endIn)
                        {
                            curExpr->sub = expressify(str, curExpr->index + 1,
                                                      comma - 1);
                            curExpr->secSub = expressify(str, comma + 1,
                                                         curExpr->endIn - 1);
                        }
                        else
                        {
                            curExpr->sub = expressify(str, curExpr->index + 1,
                                                      curExpr->endIn - 1);
                        }
                        // Point to the next expression
                        // curExpr = curExpr->next;
                        break;
                    }
                }
            }
            break;
        }
        else if (str[i] == ']')
        {
            // Raise error: unbeginned block
            // If a closing bracket is encountered here, then it has no
            // corresponding opening bracket
            printf("line 777\n");
            exit(1);
        }
        else if ('1' <= str[i] && str[i] <= '9') // Look for numbers
        {
            staIn = i;
            int num = 0;
            // printf("line 788 %d\n", num);
            while ('0' <= str[i] && str[i] <= '9')
            {
                // printf("line 791 %d at %d\n", num, i);
                num = 10 * num + str[i] - '0';
                i++;
            }
            // printf("line 793 %d\n", num);
            //  We incremented i one too much
            --i;
            curExpr = (Expression *)malloc(sizeof(Expression));
            curExpr->index = staIn;
            curExpr->type = TYNUM;

            curExpr->number = num;
            // Point to the next expression
            // curExpr = curExpr->next;
            break;
        }
        else if (('a' <= str[i] && str[i] <= 'z' || // Look for variables,
                  'A' <= str[i] && str[i] <= 'Z'))  // sqrt and tr
        {
            staIn = i;
            while (('a' <= str[i] && str[i] <= 'z' ||
                    'A' <= str[i] && str[i] <= 'Z') &&
                   str[i] != NULLCHAR && i <= end)
            {
                varName[varIn++] = str[i++];
            }
            varName[varIn] = NULLCHAR;

            if (strcmp(varName, "sqrt") == 0)
            {
                curExpr = (Expression *)malloc(sizeof(Expression));
                curExpr->index = staIn;
                curExpr->type = TYSQRT;
                curExpr->prec = PRETRSQRT;
                // i have increased one too much
                --i;
                // Point to the next expression
                // curExpr = curExpr->next;
            }
            else if (strcmp(varName, "tr") == 0)
            {
                curExpr = (Expression *)malloc(sizeof(Expression));
                curExpr->index = staIn;
                curExpr->type = TYTR;
                curExpr->prec = PRETRSQRT;
                // i have increased one too much
                --i;
                // Point to the next expression
                // curExpr = curExpr->next;
            }
            else // Variable
            {
                curExpr = (Expression *)malloc(sizeof(Expression));
                curExpr->index = staIn;
                // i have increased one too much
                curExpr->endIn = --i;
                curExpr->type = TYVAR;
                // Copy the variable name to the expression structure
                strcpy(curExpr->varName, varName);
                // Point to the next expression
                // curExpr = curExpr->next;
            }
            // Point to the next expression
            // curExpr = curExpr->next;
            break;
        }
        else if (str[i] == '+')
        {
            curExpr = (Expression *)malloc(sizeof(Expression));
            curExpr->index = i;
            curExpr->type = TYADD;
            curExpr->prec = PREADDSUB;
            // Point to the next expression
            // curExpr = curExpr->next;
            break;
        }
        else if (str[i] == '-')
        {
            curExpr = (Expression *)malloc(sizeof(Expression));
            curExpr->index = i;
            curExpr->type = TYSUBT;
            curExpr->prec = PREADDSUB;
            // Point to the next expression
            // curExpr = curExpr->next;
            break;
        }
        else if (str[i] == '*')
        {
            curExpr = (Expression *)malloc(sizeof(Expression));
            curExpr->index = i;
            curExpr->type = TYMULT;
            curExpr->prec = PREMULT;
            // Point to the next expression
            // curExpr = curExpr->next;
            break;
        }
        else
        {
            // Raise error: illegal character
            printf("line 874\n");
            exit(1);
        }
    }
    /*
    printf("line 878\n");
    void printExpr(Expression *);
    printExpr(headExpr);
    */
    curExpr->next = expressify(str, i + 1, end);
    return curExpr;
}

/*
Print the given expression, then its next expression until the next
is null.
*/
void printExpr(Expression *expr)
{
    // printf("line 887\n");
    if (expr == NULL)
    {
        // printf("line 890\n");
        return;
    }
    // printf("line 893\n");
    /*
    if (expr)
    {
        printf("expression exists\n");
        if (expr->type)
        {
            printf("expression type exists\n");
        }
        if (expr->index)
        {
            printf("expression index is %d\n", expr->index);
        }
    }
    */

    switch (expr->type)
    {
        // printf("line 897\n");
    case TYNUM:
        printf("%d ", expr->number);
        break;

    case TYVAR:
        printf("%s ", expr->varName);
        break;
    case TYADD:
        printf("+ ");
        break;
        //
    case TYSUBT:
        printf("- ");

        break;
    case TYMULT:
        printf("* ");

        break;
    case TYSQRT:
        printf("sqrt ");

        break;
        //
    case TYTR:
        printf("tr ");

        break;
    case TYELEM:
        printf("[ ");
        printExpr(expr->sub);
        if (expr->secSub != NULL)
        {
            printf(", ");
            printExpr(expr->secSub);
        }
        printf("] ");
        break;
    case TYPAR:
        printf("( ");
        printExpr(expr->sub);
        printf(") ");
        break;

    default:
        printf("line 936\n");
        exit(1);
        break;
    }
    // printf("line 978\n");
    printExpr(expr->next);
}

/*
Get the next word from the stream. If EOF is encountered, return NULL
*/
char *getWord(const FILE *fp, char *word, int lim)
{
    int c;
    int getch(const FILE *);
    void ungetch(int);
    char *w = word;

    int block = 0; // 1 for Paranthesis () block
                   // 2 for Bracket [] block
                   // 3 for Curly bracket {} block

    while (TRUE)
    {
        c = getch(fp);

        // We should keep record of the current line number.
        // Accept new line as a word of its own
        if (c == '\n')
        {
            lineNo++;
            *w++ = c;
            *w = '\0';
            return word;
        }
        if (!isspace(c))
        {
            break;
        }
    }

    if (c != EOF)
    {
        *w++ = c;
        if (c == '(')
        {
            block = 1;
        }
        else if (c == '[')
        {
            block = 2;
        }
        else if (c == '{')
        {
            block = 3;
        }
        else if (c == '#') // If it is a comment line, skip it and get a new word
        {
            while (TRUE)
            {
                c = getch(fp);
                if (c == '\n')
                {
                    lineNo++;
                    *--w = c;
                    *++w = '\0';
                    return word;
                }
                else if (c == EOF)
                {
                    return NULL;
                }
            }
            // return getWord(fp, word, lim);
        }
    }
    else // If c == EOF
    {
        return NULL;
    }

    if (!block)
    {
        for (; --lim > 0; w++)
        {
            c = getch(fp);

            // If you have come to a new word, unget the last char and end the
            // word
            if (isspace(c) || c == '[' || c == '(' || c == '{' ||
                c == '=' || c == '+' || c == '-' || c == '*' || c == EOF ||
                c == '#')
            {
                ungetch(c);
                *w = NULLCHAR;
                return word;
            }
            *w = c;
        }
    }
    else
    {
        for (; --lim > 0; w++)
        {
            c = getch(fp);

            // If EOF, then block is unfinished, raise error.
            // Else if the according block-ender is encountered, end the block.
            // Else if a normal char, keep getting the char.
            if (c == EOF)
            {
                // Raise error: unended block
            }
            else if (c == ')' && block == 1)
            {
                *w++ = c;
                *w = NULLCHAR;
                return word;
            }
            else if (c == ']' && block == 2)
            {
                *w++ = c;
                *w = NULLCHAR;
                return word;
            }
            else if (c == '}' && block == 3)
            {
                *w++ = c;
                *w = NULLCHAR;
                return word;
            }
            else
            {
                *w = c;
            }
        }
    }
    fprintf(stderr, "Word size limit is exceeded\n");
    return NULL;
}

#define BUFSIZE 1

char buf[BUFSIZE];
int bufP = 0;
int lineP = 0;

/*
If there is a char in buffer, get it. Else get a char from the file.
*/
int getch(const FILE *fp)
{
    if (bufP)
    {
        return buf[--bufP];
    }
    return getc(fp);
}

/*
Unget a char to the buffer. It is used when you read too many chars by accident
*/
void ungetch(int c)
{
    if (bufP)
    {
        fprintf(stderr, "Cannot unget the char, buffer is full\n");
        return;
    }
    buf[bufP++] = c;
}

/*
Raises an error related to the matlang file. Then exits the program.
*/
void raiseError(int code, ...)
{
    printf("Error (Line %d):", lineNo);

    va_list ap;
    va_start(ap, code);

    char *name;

    switch (code)
    {
    case 1:
        printf("No variable id is specified in the variable declaration");
        break;
    case 2:
        printf("Too many arguments for variable declaration\n");
        // You can maybe print out the correct syntax of variable declaration
        break;
    case 3:
        name = va_arg(ap, char *);
        printf("Variable with the id %s already declared\n", name);
        break;
    case 4:
        printf("Too few arguments for variable declaration\n");
        break;
    case 5:
        printf("Unended block\n");
        break;
    case 6:
        printf("Expected an integer, this is not an integer\n");
        break;
    default:
        break;
    }
    va_end(ap);
    // Exit with 0. Because program has run successfully, even though matlang
    // program is written with errors.
    exit(0);
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}