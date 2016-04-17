#include <stdint.h> 
#include "r_instructions.h"

//These are the R-instructions implemented by the simulted MIPS processor
//You will need to define the following macros that use masking and
//shifting to extract the specified bits of the instruction:

uint32_t logicalShiftRight(uint32_t instruction, int shiftamt){
	instruction = instruction >> 1;
	uint32_t mask = 0x7fffffff;
	instruction &= mask;
	instruction = instruction >> shiftamt-1;
	return instruction;
}

//extracts the value of the RS field bits (bits 21-25)
uint8_t RS_FIELD(uint32_t instruction){ 
	instruction = instruction << 6;
	instruction = logicalShiftRight(instruction, 27);
	return (uint8_t) instruction;
}

//extracts the value of the RT field bits (bits 16-20)
uint8_t RT_FIELD(uint32_t instruction){
	instruction = instruction << 11;
	instruction = logicalShiftRight(instruction, 27);
	return (uint8_t) instruction;
}

//extracts the value of the RD field bits (bits 11-15)
uint8_t RD_FIELD(uint32_t instruction){
	instruction = instruction << 16;
	instruction = logicalShiftRight(instruction, 27);
	return (uint8_t) instruction;
}

//Here's the simulated mips add instruction.

void add(uint32_t instruction){
	uint8_t rs = RS_FIELD(instruction);
	uint8_t rt = RT_FIELD(instruction);
	uint8_t rd = RD_FIELD(instruction);
	registers[rd] = (uint32_t) ((int32_t) registers[rs] + (int32_t) registers[rt]);
}

void addu(uint32_t instruction){

} 

void sub(uint32_t instruction){} 

void subu(uint32_t instruction){} 

void mult(uint32_t instruction){} 

void multu(uint32_t instruction){} 

void div(uint32_t instruction){} 

void divu(uint32_t instruction){} 

void and(uint32_t instruction){} 

void nor(uint32_t instruction){} 

void or(uint32_t instruction){} 

void xor(uint32_t instruction){} 

void sll(uint32_t instruction){} 

void sllv(uint32_t instruction){} 

void srl(uint32_t instruction){} 

void sra(uint32_t instruction){} 

void srlv(uint32_t instruction){} 

void srav(uint32_t instruction){} 

void slt(uint32_t instruction){} 

void sltu(uint32_t instruction){} 

void jr(uint32_t instruction){} 

void jalr(uint32_t instruction){} 

void mfhi(uint32_t instruction){} 

void mflo(uint32_t instruction){} 
