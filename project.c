#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    unsigned invA = ~A + 1;
    unsigned invB = ~B + 1;
    //~X + 1 will work no matter if X is actually positive or negative

    switch(ALUControl) {
        case '000': //add
           *ALUresult = A + B;
           break;
        case '001': //subtract
           *ALUresult = A + invB;
           break;
        case '010': //if A < B, Z = 1; otherwise, Z = 0
            if(A + invB < 0) {*ALUresult = 1; *Zero = 0;}
            else{*ALUresult = 0; *Zero = 1;}
            break;
        case '011': //slt unsigned
           if((A << 1) < (B << 1)) {*ALUresult = 1; *Zero = 0;}
           else {*ALUresult = 0; *Zero = 1;}
           break; 
        case '100': //A AND B
           *ALUresult = (A & B);
           break;
        case '101': //A OR B
           *ALUresult = (A | B);
           break;
        case '110': //shift B left by 16 bits
           *ALUresult = (B << 16);
           break;
        case '111': //NOT A
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
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

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
    // unsigned long long int is 8 bytes; not sure if needed
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

