
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNAME 20
#define MAXLINE 100
#define NULLCHAR '\0'
#define WORDLEN 50 // Maximum word length, including blocks

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
    Variable *var;               // Variable of this node
    struct VariableNodes *left;  // VarNode on the left
    struct VariableNodes *right; // VarNode on the right. Variables on
                                 // the left has names that come earlier
                                 // on the dictionary
} VarNode;

VarNode root;   // Root of the variable tree
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

    if ((fp = fopen(fileName, "r")) == NULL)
    {
        fprintf(stderr, "%s: cannot open %s", progName, fileName);
        exit(2);
    }
    printf("line 53\n");
    char word[WORDLEN];

    // Get each line one by one
    while (getWord(fp, word, WORDLEN) != NULL)
    {
        // lineNo++;
        // printf("line 60\n");
        // processLine(fp, line);
        printf("%s\n", word);
        // printf("line 62\n");
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
char *getWord(const FILE *fp, char *word, int lim)
{
    int c;
    int getch(const FILE *);
    void ungetch(int);
    char *w = word;

    int block = 0; // 1 for Paranthesis () block
                   // 2 for Bracket [] block
                   // 3 for Curly bracket {} block

    while (isspace(c = getch(fp)))
        ;

    if (c != EOF )
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
    }else{
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
                c == '=' || c == '+' || c == '-' || c == '*' || c == EOF)
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