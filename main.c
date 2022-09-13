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

VarNode root;   // Root of the variable tree
int lineNo = 0; // Current number of line that is being read

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
        lineNo++;
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

char *getWord(FILE *fp, char *word, int lim)
{
    int c;
    int getch(FILE *);
    void ungetch(int);
    char *w = word;

    int block = 0; // 1 for Paranthesis () block
                   // 2 for Bracket [] block
                   // 3 for Curly bracket {} block

    while (isspace(c = getch(fp)))
        ;

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
    }

    if (!block)
    {
        for (; --lim > 0; w++)
        {
            c = getch(fp);

            // If you have come to a new word, unget the last char and end the
            // word
            if (isspace(c) || c == '[' || c == '(' || c == '{' ||
                c == '=' || c == '+' || c == '-' || c == '*' || c == EOF)
            {
                ungetch(c);
                *w = '\0';
                return word;
            }
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
            else if (c == '(' && block == 1)
            {
                *w++ = c;
                *w = '\0';
                return word;
            }
            else if (c == '[' && block == 2)
            {
                *w++ = c;
                *w = '\0';
                return word;
            }
            else if (c == '{' && block == 3)
            {
                *w++ = c;
                *w = '\0';
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

/*
If there is a char in buffer, get it. Else get a char from the file.
*/
int getch(FILE *fp)
{
    if (bufP)
    {
        return buf[--bufP];
    }
    return fgetc(fp);
}

/*
Unget a char to the buffer. It is used when you read too many chars by accident
*/
void ungetch(int c)
{
    if (bufP >= BUFSIZE)
    {
        fprintf(stderr, "Cannot unget the char, buffer limit is exceeded\n");
        return;
    }
    buf[bufP++] = c;
}