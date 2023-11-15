#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    unsigned invA = ~A + 1;
    unsigned invB = ~B + 1;
    //~X + 1 will work no matter if X is actually positive or negative

    switch(ALUControl) {
        case 0: //add; 000
           *ALUresult = A + B;
           break;
        case 1: //subtract; 001
           *ALUresult = A + invB;
           break;
        case 2: //if A < B, Z = 1; otherwise, Z = 0; 010
            if(A + invB < 0) {*ALUresult = 1; *Zero = 0;}
            else{*ALUresult = 0; *Zero = 1;}
            break;
        case 3: //slt unsigned; 011
           if((A << 1) < (B << 1)) {*ALUresult = 1; *Zero = 0;}
           else {*ALUresult = 0; *Zero = 1;}
           break; 
        case 4: //A AND B; 100
           *ALUresult = (A & B);
           break;
        case 5: //A OR B; 101
           *ALUresult = (A | B);
           break;
        case 6: //shift B left by 16 bits; 110
           *ALUresult = (B << 16);
           break;
        case 7: //NOT A; 111
           *ALUresult = ~A;
           break;
        default:
           *ALUresult = 0;
           break;
    }
}


//_____________________________________________________________________________________________________________________________________________________________________


/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    // see projectDetails.pptx

    //based on Step() contents in spimcore.c this needs to take instruction and simply copy parts into each thing (tear it to pieces)

    int n = 0; //placeholder for sake of example, change to actual number

    // NOTE: instead of shifting, just get binary number equal to all 1s in the respective position
    *op = (instruction & n);
    *r1 = (instruction & n);
    *r2 = (instruction & n);
    *r3 = (instruction & n);
    *offset = (instruction & n);
    *funct = (instruction & 63); // 63 is 111111
    *jsec = (instruction & 33554431); // this number is 26 ones
}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
    // taking opcode then setting all vales of controls to whatever required
    // 0 or 1, 2 means don't care
    // return 1 if halt, 0 otherwise

    // currently hardcoded, maybe find way to make it not hard coded

    switch(op) {
        case 0: //R-type instruction, operations will be determined by funct so return nothing
            controls->RegDst = 2;
            controls->Jump = 2;
            controls->Branch = 2;
            controls->MemRead = 2;
            controls->MemtoReg = 2;
            controls->ALUOp = 2;
            controls->MemWrite = 2;
            controls->ALUSrc = 2;
            controls->RegWrite = 2;
            return 0;
        case 2: //jump, 000010
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            return 0;
        case 4: //branch eq, 000100
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            return 0;
        case 8: //add immediate, 001000
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            return 0;
        case 10: //set on less than immediate, 001010
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            return 0;
        case 11: //set on less than immediate unsigned, 001011
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            return 0;
        case 12: //and immediate, 001100
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            return 0;
        case 13: //or immediate, 001101
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            return 0;
        case 15: //load upper immediate, 001111
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            return 0;
        case 131: //load word, 100011
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            return 0;
        case 163: //store word, 101011
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            return 0;
        default:
            return 1;
    }


}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{

}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    // unsigned long long int is 8 bytes, not sure if needed
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

