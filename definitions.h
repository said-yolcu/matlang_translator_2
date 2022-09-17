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
    Variable *destVar; // Id of the destination variable
    int fir_pos;       // Position in the first dimension
    int sec_pos;       // Position in the second dimension
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

typedef struct Expressions
{
    enum
    {
        VAR,                      // Variable
        ADD,                      // Addition
        MULT,                     // Multiplication
        SUBT,                     // Subtraction
        ELEM,                     // Getting element( with brackets)
        SQRT,                     // Sqrt()
        TR,                       // Tr()
        PAR                       // Parantheses
    } function;                   // Type of the expression is its operator
                                  // or function
    struct Expressions fir_child; // Almost all the expressions have a child
    struct Expressions sec_child; // Some of the expressions have a second child

} Expression;

typedef struct OperFuns
{
    enum
    {
        TYADD,
        TYSUBT,
        TYMULT,
        TYSQRT,
        TYTR,
        TYELEM,
        TYPAR
    } type;

    enum
    {
        PREADDSUB, // Addition and subtraction have the lowest precision
        PREMULT,   // Multiplication
        PRETRSQRT, // Tr and sqrt
        PREELEM,   // Element get Var[1,2]
        PREPAR,    // Parantheses. Function calls' parantheses count parantheses
                   // also
    } prec;        // Precedence of the operator/function
    int level;     // Paranthesization level of the function .
                   // As parantesization incease, precedence increase
    int index;     // Index of the opFun in the string, or start index if
                   // the opFun takes multiple indices
    int endIn;     // End index of the opFun

} OperFun;

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

    LineBlock *next;
} LineBlock;