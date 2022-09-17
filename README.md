# matlang_translator_2

A translator c program that takes a file written in matlang 
and creates a c program that outputs what the original 
matlang file outputs. Certain checks will be made during the
translation phase.

1. Variables
There are three types of variables: scalar, vector, and 
matrix. Vectors can be regarded as matrices with the second 
dimension equal to one. So as far as we are concerned, there 
are two variables: scalar and matrix. During the translation 
certain checks must be made regarding the variable type, 
name ,and dimensions. Thus I will create binary tree to 
store all the variables. When I need to access a variable's 
info, I will do it in on average O(log(n)) time complexity. 
Also I should create a struct for variables.

2. Statements
There are 5 types of statements:
1- Variable declarations
2- Variable assignment
3- Print calls
4- Choose call
5- Print loop
Each of these statements consists of multiple expressions. We 
should keep a record of this expressions, their relation to 
statements, and the order of the statements. Either we should 
create a linked list to keep a record of these, or we can 
instantly write them to the outfile. If we use a linked list 
to keep the record, we can use that structure in a compiler 
project, but it may be too space consuming for this project.

3. Errors
Errors are raised by a raiseError() function. This function 
prints the error declaration, then exits the program. Each 
error declaration specifies the line number. Some error 
declarations may print out the correct usage (syntax) of the 
source of the error.
Error codes:
0- No error. Successfull execution.
1- No variable id specified in variable declaration
2- Too many arguments for variable declaration 
3- Variable with the same id already declared
4- Too few arguments for variable declaration
5- Unended block

4. Arrays
Array indices in matlang are counted from 1 onwards.
