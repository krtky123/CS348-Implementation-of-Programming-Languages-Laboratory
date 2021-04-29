Assignment 01 - CS 348 IPL Lab

2-Pass-Assembler

Student: Kartikay Goel
Rollno: 180101033

Input format:

1. Every label-less line of code must begin with '*' followed by exactly one space before the mnemonic.
Eg: * JEQ RLOOP

2. In case operand is not present, '*' should be present in place of operand.
Eg: * RSUB *

3. Comments must begin with '.'
Eg: . this is a comment

4. Every new instruction must begin on new line

COMPILE: g++ 180101033_Assign01.cpp
	
RUN: 
1. ./a.out
2. The name of the input file is input.asm which is present in the zip folder.
3. 3 output files will be generated - output.o (main object code file), intermediate_file.txt and line_by_line_object_codes.txt
4. Please refresh the directory after running the code.
