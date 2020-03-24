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
         g++ Main_Phase1.cpp
        ./a.out
        
        On the GUI:-
            Editor pane is working.And Machine Code, Basic Code and Original Code is being displayed

Phase 1:-
Parsed the input file
                input.txt to input1.txt for assembler directives
                
                input1.txt to basicCode.txt for the basic code 
                
                basicCode.txt to machineCode.txt for phase 2
                
                basicCode.txt to machineCode1.txt for the machine code
                
Error(s) Handled in the instruction:
- If "lw x10 x20" was given and no offset/immediate field, Our Assembler pointed out
- If, "addi x10, x20, x30" was given. It is pointed out.
- If, "jalr x1, loc1" was given but loc1 was not defined, it is pointed out.
- The immediate/offset value given in the instruction is beyond the limit, it is pointed out.
- x33 and x-1 is an invalid instruction as we have registers from 0 to 31 and this is pointed out
-If any number of parameters are given less in the instruction then it is pointed out

If all the errors are not cleared, Error message will be written on the terminal and complete machine code will not be available.
Our assembler accepts instructions without bracket for offset 

lb,lh,lw assumes the offset of 8

Ex: lw x10, 10(x20) can be accepted as lw x10 x20 10 by your assembler.

Address of text segment : 0x00
Address of data segment : 0x10000000

Contribution:
        R_Type : Vishal
        IType and Main_Phase1 : Ishika
        UType and UJType: Harsha
        SType and SBType: Sujit
        MemoryAcess and bintohex: Laksya
        Error Handling in Instructions : Harsha and Ishika
        Complete debugging and Running of Phase1 : Harsha and Ishika

Test Program:Factorial 
