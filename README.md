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

