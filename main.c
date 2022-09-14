
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define MAXEXPR 100
#define MAXNAME 20
#define MAXLINE MAXEXPR
#define NULLCHAR '\0'
#define TRUE 1
#define WORDLEN 50 // Maximum word length, including blocks

typedef struct Variables
{
    enum
    {
        SCA,
        VEC,
        MAT
    } type;                 // Type of the variable
    char name[MAXNAME + 1]; // Name of the variable
    int fir_dim;            // First dimension
    int sec_dim;            // Second dimension
} Variable;

typedef struct Prints
{
    enum
    {
        PRI,
        SEP
    } type;                 // Print() or printsep()
    char name[MAXNAME + 1]; // Name of the variable
} Print;

typedef struct TrSqs
{
    enum
    {
        TR,
        SQRT
    } type;
    char expr[MAXEXPR + 1]; // Argument expression
} TrSq;

typedef struct Chooses
{
    char expr1[MAXEXPR + 1]; // Test argument
    char expr2[MAXEXPR + 1]; // Returns expr2 if expr1 == 0
    char expr3[MAXEXPR + 1]; // Returns expr3 if expr1 > 0
    char expr4[MAXEXPR + 1]; // Returns expr4 if expr1 < 0

} Choose;

typedef struct Forls
{
    LineBlock *lines; // Lines in the for loop
} Forl;

typedef struct Assigns
{
    char leftSide[MAXEXPR + 1];
    char rightSide[MAXEXPR + 1];
} Assign;

typedef struct VariableNodes
{
    Variable *var;               // Variable of this node
    struct VariableNodes *left;  // VarNode on the left
    struct VariableNodes *right; // VarNode on the right. Variables on
                                 // the left has names that come earlier
                                 // on the dictionary
} VarNode;

typedef struct LineBlocks
{
    enum
    {
        SCA,  // 0: Scalar definition
        VEC,  // 1: Vector definition
        MAT,  // 2: Matrix definition
        TR,   // 3: Tr function call
        SQRT, // 4: Sqrt function call
        CHS,  // 5: Choose function call
        FORL, // 6: For loop
        PRI,  // 7: Print function call
        SEP,  // 8: Printsep function call
        ASN   // 9: Assignment
    } type;

    union
    {
        Variable *var;     // If scalar, vector, or matrix definition
        Print *printArg;   // For print()
        TrSq *trsqArg;     // For tr() and sq()
        Choose *chooseArg; // For choose()
        Forl *forlArg;     // For for()
        Assign *assignArg; // For assignments '='
    } statement;
} LineBlock;

VarNode *root;  // Root of the variable tree
int lineNo = 0; // Current number of line that is being read

int main(int argc, char const *argv[])
{
    void readLines(const char *, const char *);

    char fileName[] = "/home/said/school/spring_semester_2022/cmpe_230/homeworks/hw_1/ex1.txt";

    readLines(fileName, argv[0]);
    return 0;
}

void readLines(const char fileName[], const char progName[])
{
    // void processLine(const FILE *, char *);
    char *getWord(const FILE *, char *, int);

    FILE *fp;

    // Allocate storage to root
    root = (VarNode *)malloc(sizeof(VarNode));

    if ((fp = fopen(fileName, "r")) == NULL)
    {
        fprintf(stderr, "%s: cannot open %s", progName, fileName);
        exit(2);
    }
    printf("line 53\n");
    char word[WORDLEN];

    int prevLine = 0;

    // Get each line one by one
    while (getWord(fp, word, WORDLEN) != NULL)
    {
        if (strcmp(word, "scalar") == 0)
        {
            // Scalar definition
            scalDef(fp);
        }
        else if (strcmp(word, "vector") == 0)
        {
            // Vector definition
        }
        else if (strcmp(word, "matrix") == 0)
        {
            // Matrix definition
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
        else
        {
            // Possible assignment statement, must check for '='
        }

        printf("%s\t", word);
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
Checks whether the current line is a scalar definition. Returns 0 if so.
Else returns a positive integer denoting the error type
*/
int scalDef(const FILE *fp)
{
    char *getWord(const FILE *fp, char *word, int lim);
    int addTree(VarNode * root, Variable * var);

    char name[MAXNAME + 1];
    char word[MAXEXPR + 1];

    if (getWord(fp, word, MAXNAME) == NULL || strcmp(name, "\n") == 0)
    {
        return 1; // Error: no variable name specified
    }
    strcpy(name, word);

    if (getWord(fp, word, MAXEXPR) != NULL && strcmp(word, "\n") != 0)
    {
        return 2; // Error: too many arguments for variable declaration
    }

    // Allocate storage for the variable
    Variable *varPtr = (Variable *)malloc(sizeof(Variable));

    // Put the type and name
    varPtr->type = SCA;
    strcpy(varPtr->name, name);

    // Add the node to the tree
    return addTree(root, varPtr);
}

int addTree(VarNode *root, Variable *var)
{
    if (!root)
    {
        root = (VarNode *)malloc(sizeof(VarNode));
        root->var = var;
        return 0; // Successfully added
    }

    if (strcmp(var->name, root->var->name) > 0) // Go to right child
    {
        return addTree(root->right, var);
    }
    else if (strcmp(var->name, root->var->name) < 0) // Go to left
    {
        return addTree(root->left, var);
    }
    else // If they are equal
    {
        return 3; // Error: a variable with same id is already declared
    }
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
                else if (c == 'EOF')
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