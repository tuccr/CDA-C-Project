#include "spimcore.h"

// Tucker Carroll
// Gianni Dragassakis
// CDA3103C C Project


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    unsigned invA = ~A + 1;
    unsigned invB = ~B + 1;
    //~X + 1 will work no matter if X is actually positive or negative

    switch(ALUControl) {
        case 0b000: //add; 000
            *ALUresult = A + B;
            break;
        case 0b001: //subtract; 001
            *ALUresult = A + invB;
            break;
        case 0b010: //if A < B, Z = 1, otherwise, Z = 0; 010
            *ALUresult = (((A + invB) & 0b1000000000000000) != 0);
            break;
        case 0b011: //slt unsigned; 011
            *ALUresult = (A < B);
            break;
        case 0b100: //A AND B; 100
            *ALUresult = (A & B);
            break;
        case 0b101: //A OR B; 101
            *ALUresult = (A | B);
            break;
        case 0b110: //shift B left by 16 bits; 110
            *ALUresult = (B << 16);
            break;
        case 0b111: //NOT A; 111
            *ALUresult = ~A;
            break;
        default:
            return;
    }
    *Zero = (*ALUresult == 0);
}


/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    // check is PC is divisible by 4 then divide by 4 (PC >> 2) to get the actual location (Mem is an array of words)
    // can halt
    
    if(PC % 4 != 0) {
        return 1;
    }
    else {
        *instruction = Mem[PC >> 2]; 
        return 0;
    }
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    // based on Step() contents in spimcore.c this needs to take instruction and simply copy parts into each thing (tear it to pieces)

    // NOTE: instead of shifting, just get binary number equal to all 1s in the respective position

    *op = (instruction & 0b11111100000000000000000000000000) >> 26;
    *r1 = (instruction & 0b00000011111000000000000000000000) >> 21;
    *r2 = (instruction & 0b00000000000111110000000000000000) >> 16;
    *r3 = (instruction & 0b00000000000000001111100000000000) >> 11;
    *offset = (instruction & 0b000000000000000001111111111111111) >> 0;
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


    switch(op) {
        case 0: //R-type instruction, 000000
            controls->RegDst = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0b111; // NOT (flag for R-type in ALU_operations)
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 1;
            break;
        case 2: //jump, 0b000010
            controls->RegDst = 2;
            controls->Jump = 1;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp = 0b000; // add (X)
            controls->MemWrite = 0;
            controls->ALUSrc = 2;
            controls->RegWrite = 0;
            break;
        case 4: //branch on equal, 0b000100
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 1;
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp = 0b001; // subtract
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            break;
        case 8: //add immediate, 0b001000
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0b000; // add
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 10: //set on less than immediate, 0b001010
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0b010; //set on less than signed
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 11: //set on less than immediate unsigned, 0b001011
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0b011; //set on less than unsigned
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 12: //AND immediate, 0b001100
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0b100; // AND
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 0b001111: //load upper immediate, 0b001111
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 2;
            controls->MemtoReg = 0;
            controls->ALUOp = 0b110; // shift left 18 bits
            controls->MemWrite = 2;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 0b100011: //load word, 0b100011
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 1;
            controls->MemtoReg = 1;
            controls->ALUOp = 0b000; // add (X)
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 0b101011: //store word, 0b101011
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0b000; // add (X)
            controls->MemWrite = 1;
            controls->ALUSrc = 1;
            controls->RegWrite = 0;
            break;
        default:
            return 1; // halt
    }
    return 0; // pass
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    // offset is input and extended_value is output

    if(((1 << 15) & offset) == (1 << 15)) {
        *extended_value = offset + 0b11111111111111110000000000000000;
    }
    else {
        *extended_value = offset;
    }
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    // look at ALUOp and determine if R-type or not
        // if yes, decode funct
    // call ALU() and determine if halt
    if(ALUOp < 0 || ALUSrc < 0 || ALUSrc > 2) {
        return 1;
    }

    if(ALUSrc == 0 && ALUOp == 0b111) {
        switch(funct) {
            case 0b100000:
                ALUOp = 0b000;//add
                break;
            case 0b100010:
                ALUOp = 0b001;//subtract
                break;
            case 0b100100:
                ALUOp = 0b100;//and
                break;
            case 0b100101:
                ALUOp = 0b101;//or
                break;
            case 0b100111:
                ALUOp = 0b111;//not
                break;
            case 42:
                ALUOp = 0b010;//slt
                break;
            case 43:
                ALUOp = 0b011;//sltu
                break;
            default:
                return 1;
        }
    }
    if(ALUSrc == 1) {
        data2 = extended_value;
    }

    ALU(data1, data2, ALUOp, ALUresult, Zero);
    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    // if MemWrite = 1, check word alignment and write into memory
    // if MemRead = 1, check word alignment and read from memory
    // IF NOT WORD ALIGNED, RETURN HALT
    
    // data2 should be register input
    // memdata should be data read from memory    

    if((MemWrite && !MemRead) && (ALUresult % 4 == 0)) { 
        // store data2 into Mem at ALUresult
        Mem[ALUresult >> 2] = data2;
        *memdata = data2;
        return 0;
    }
    if((!MemWrite && MemRead) && (ALUresult % 4 == 0)) {
        // load word into memdata from Mem
        *memdata = Mem[ALUresult >> 2];
        return 0;
    }
    if(((!MemWrite && MemRead) || (MemWrite && !MemRead)) && (ALUresult % 4 != 0)) {
        return 1;
    }
    else {
        return 0;
    }

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    // look at control signals and write as necessary

    // if RegWrite == 1 and MemtoReg == 1, then data is coming from memory
    // if RegWrite == 1, and MemtoReg == 0, then data is coming from ALUresult
    // if RegWrite == 1, then place write data into register specified by RegDst

    // load
    if(RegWrite && MemtoReg == 1) {
        if(RegDst == 1) {
            Reg[r3] = memdata;
        }
        else if(RegDst == 0) {
            Reg[r2] = memdata;
        }
    }

    // store
    if(RegWrite && MemtoReg == 0) {
        if(RegDst == 0) {
            Reg[r2] = ALUresult;
        }
        else if(RegDst == 1) {
            Reg[r3] = ALUresult;
        }
    }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    *PC = *PC + 4; // ALWAYS do this step first, no matter what

    if(Branch && Zero) {
        extended_value = (extended_value << 2);
        ALU(*PC, extended_value, 0b000, PC, &Zero);
    }

    if(Jump) {
        *PC = (*PC & 0b11110000000000000000000000000000) | (jsec << 2); 
    }
}

