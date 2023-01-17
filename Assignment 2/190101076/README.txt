Name - Ritish Bansal
Roll No - 190101076
Email id - rbansal@iitg.ac.in

This folder contains 3 more files as 190101076_Assign02_assembler.cpp, 190101076_Assign02_linkloader.cpp, input.txt

Instructions to run code --
Environment to run code is Linux g++
Firstly assembler will be compiled
command to compile assembler - g++ 190101076_Assign02_assembler.cpp
command to run it - ./a.out

Next linker loader will be compiled
command to compile assembler - g++ 190101076_Assign02_linkloader.cpp
command to run it - ./a.out

Assumptions and Properties of following programs 

1. Assumed that input.txt is input file and it must be present in same directory as of assembler.
2. Four files will be produced by assembler which are intermediate.txt, listing.txt, output.txt, caret.txt
3. Now for linker loader output.txt and listing.txt will be serving as input so they must be present in that directory as off linker loader.
4. Two more files will be produced by linkloader which are memory.txt, line_listing.txt.
5. intermediate.txt - file that is passed in between 2 passes of assembler
6. listing.txt - contains object code for each instruction
7. caret.txt - has output with caret character   
8. output.txt - Actual output produced by assembler without caret
9. line_listing.txt - print each instruction in the file
10. memory.txt - It has final output of memory map
11. Also assumed that comments begin with . character.
12. Errors will be printed on terminal.
13. External refrences and csect names will be printed on terminal.(for linkloader only)
14. Also all modification values will be printed. (for linkloader only)
15. Program uses c++ file I/O. If any error is encountered it is printed on terminal and execution stops.
16. FIELD SIZE is 10 for label and opcode and 30 for the operands. Extra column is provided for '+', '=' and '#'.
17. Make sure there are no spaces in the expressions in the input file.
18. Assumed that linker loader loads the code starting from 0 memory address.


