/*
Various "define"s, typedefs and structs reside here
*/

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
        VARSCA,
        VARVEC,
        VARMAT
    } type;                 // Type of the variable
    char name[MAXNAME + 1]; // Name of the variable
    int fir_dim;            // First dimension
    int sec_dim;            // Second dimension
    int sca_val;            // Value of the scalar variable
    int *arr;               // Vector or matrix array
    int arr_step;           // Length of each row (or column) if
                            // variable is a matrix
} Variable;

typedef struct Prints
{
    enum
    {
        PRINT,
        PRISEP
    } type;                 // Print() or printsep()
    char name[MAXNAME + 1]; // Name of the variable
} Print;

typedef struct TrSqs
{
    enum
    {
        MTR,
        MSQRT
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
    struct LineBlocks *lines; // Lines in the for loop
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