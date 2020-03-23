# CA_RISCV_PROJECT
README

CS-204 project, RISC-V SIMULATOR, GROUP1:-

Ishika Soni-2017eeb1145
Harsha Garg-2017meb1008
Vishal Lunia-2017eeb1177
Sujit Jaiswaliya-2017csb1115
Lakshya-2017eeb1145

Language:- C++


HOW TO RUN:-
        On the terminal :-
        

Phase 1:-
Parsed the input file
                input.txt to input1.txt for assembler directives
                input1.txt to basicCode.txt for the basic code 
                basicCode.txt to machineCode.txt for phase 2
                basicCode.txt to machineCode1.txt for the machine code
                
Error(s) Handled in the instruction:
- If "lw x10 x20" was given and no offset/immediate field, Our Assembler pointed out
- Say, "addi x10, x20, x30" was given. It should be pointed out.
- Say, "jalr x1, loc1" was given but loc1 was not defined, it should be pointed out.
- The immediate/offset value given in the instruction is beyond the limit, it should be pointed out.

All others, errors should be pointed to the user. Until all the errors are cleared, complete machine code will not be available.
f your assembler accepts instructions without special characters like comma, bracket, etc., you need to clearly mention that in the "README" file for the user to know.

Ex: lw x10, 10(x20) can be accepted as lw x10 x20 10 by your assembler.
                
Our assembler accepts instructions without special characters like bracket, etc.
