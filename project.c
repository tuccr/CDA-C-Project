#include "spimcore.h"

// BINARY CAN BE DONE WITH 0b001 AND HEX CAN BE DONE WITH 0x001



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
           if(A < B) {*ALUresult = 1; *Zero = 0;}
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
    // check is PC is divisible by 4 then divide by 4 (PC >> 4) to tget the actual location (Mem is an array of words)
    // spimcore.c -> #define MEM(addr) MEM[(addr >> 4)] should do this
    // can halt
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    // see projectDetails.pptx

    //based on Step() contents in spimcore.c this needs to take instruction and simply copy parts into each thing (tear it to pieces)

    int n = 0;

    // NOTE: instead of shifting, just get binary number equal to all 1s in the respective position
    *op = (instruction & 0b11111100000000000000000000000000) >> 26;
    *r1 = (instruction & 0b00000011111000000000000000000000) >> 21;
    *r2 = (instruction & 0b00000000000111110000000000000000) >> 16;
    *r3 = (instruction & 0b00000000000000001111100000000000) >> 11;
    *offset = (instruction & 0b000000000000000000000011111000000) >> 6;
    *funct = (instruction & 0b00000000000000000000000000111111) >> 0;
    *jsec = (instruction & 0b00000011111111111111111111111111) >> 0;
}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
    // taking opcode then setting all vales of controls to whatever required
    // 0 or 1, 2 means don't care
    // return 1 if halt, 0 otherwise

    // currently hardcoded (table route), make sure all values are correct 

    switch(op) {
        case 0: //R-type instruction, 000000
            // filled from r-type pdf
            controls->RegDst = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0b000; // double check this
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 1;
            return 0;
        case 2: //jump, 0b000010
            controls->RegDst = 2;
            controls->Jump = 1;
            controls->Branch = 0;
            controls->MemRead = 1;
            controls->MemtoReg = 2;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 2;
            controls->RegWrite = 0;
            return 0;
        case 4: //branch on equal, 0b000100
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 1;
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp = 0b001; // needs to be subtraction for comparison
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            return 0;
        case 8: //add immediate, 0b001000
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
        case 10: //set on less than immediate, 0b001010
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
        case 11: //set on less than immediate unsigned, 0b001011
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
        case 12: //and immediate, 0b001100
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
        case 13: //or immediate, 0b001101; may not be needed as instructions say only OR literal with opcode 0b000000
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
        case 15: //load upper immediate, 0b001111
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
        case 0b100101: //load word, 0b100011
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 1;
            controls->MemtoReg = 1;
            controls->ALUOp = 0b000;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            return 0;
        case 0b101011: //store word, 0b101011
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0b000;
            controls->MemWrite = 1;
            controls->ALUSrc = 1;
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
    // do not need to be word aligned, doesn't return a halt
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    // unsigned long long int is 8 bytes, not sure we need to use long long int but definitely need to keep sign 
    // offset is input and extended_value is output
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    // look at ALUOp and determine if R-type or not
        // if yes, decode funct
    // call ALU() and determine if halt
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    // if MemWrite = 1, check word alignment and write into memory
    // if MemRead = 1, check word alignment and read from memory
    // IF NOT WORD ALIGNED, RETURN HALT
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    // look at control signals and write as necessary
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

