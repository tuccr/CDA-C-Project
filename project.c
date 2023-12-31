#include "spimcore.h"

/* 
    Group 1
    Tucker Carroll, Gianni Dragassakis
    CDA3103C
    C Project
*/

/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    //~X + 1 will work no matter if X is actually positive or negative

    switch(ALUControl) {
        case 0b000: //add
            *ALUresult = A + B;
            break;
        case 0b001: //subtract
            B = ~B + 1;
            *ALUresult = A + B;
            break;
        case 0b010: //if A < B, Z = 1, otherwise, Z = 0
            B = ~B + 1;
            *ALUresult = (((A + B) & 0b1000000000000000) != 0);
            break;
        case 0b011: //slt unsigned
            *ALUresult = (A < B);
            break;
        case 0b100: //A AND B
            *ALUresult = (A & B);
            break;
        case 0b101: //A OR B
            *ALUresult = (A | B);
            break;
        case 0b110: //shift B left by 16 bits
            *ALUresult = (B << 16);
            break;
        case 0b111: //NOT A; 111
            *ALUresult = ~A;
            break;
        default:
            return; // invalid ALUControl signal
    }
    *Zero = (*ALUresult == 0);
}


/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    // check if PC is divisible by 4 (word alignment) then divide by 4 (PC >> 2) to get the actual location
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
    // Taking opcode then setting all vales of controls to whatever required
    // Signals 0 or 1
        // 2 means don't care
        // ALUOp don't care is add (0b000)
        // ALUOp r-type flag is NOT (0b111)

    switch(op) {
        case 0b000000: //R-type instruction
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
        case 0b000010: //jump
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
        case 0b000100: //branch on equal
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
        case 0b001000: //add immediate
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
        case 0b001010: //set on less than immediate
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0b010; // set on less than signed
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 0b001011: //set on less than immediate unsigned
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0b011; // set on less than unsigned
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 0b001100: //AND immediate
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
        case 0b001111: //load upper immediate
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
        case 0b100011: //load word
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
        case 0b101011: //store word
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
        *extended_value = offset | 0b11111111111111110000000000000000;
    }
    else {
        *extended_value = offset;
    }
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    // decode funct if needed and halt if invalid inputs
    
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
            case 0b00101010:
                ALUOp = 0b010;//slt
                break;
            case 0b00101011:
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

    // store data2 into Mem at ALUresult
    if((MemWrite && !MemRead) && (ALUresult % 4 == 0)) { 
        Mem[ALUresult >> 2] = data2;
        *memdata = data2;
        return 0;
    }

    // load word into memdata from Mem
    if((!MemWrite && MemRead) && (ALUresult % 4 == 0)) {
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

    // if RegWrite == 1, then place write data into register specified by RegDst
        // if MemtoReg == 1, then data is coming from memory
        // if MemtoReg == 0, then data is coming from ALUresult
    
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

    // beq
    if(Branch && Zero) {
        extended_value = (extended_value << 2);
        ALU(*PC, extended_value, 0b000, PC, &Zero);
    }

    // jump
    if(Jump) {
        *PC = (*PC & 0b11110000000000000000000000000000) | (jsec << 2); 
    }
}

