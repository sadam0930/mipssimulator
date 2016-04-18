#include <stdint.h> 
#include "cpu.h"

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

uint8_t SHAMT_FIELD(uint32_t instruction){
	instruction = instruction << 21;
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
	uint8_t rs = RS_FIELD(instruction);
	uint8_t rt = RT_FIELD(instruction);
	uint8_t rd = RD_FIELD(instruction);
	registers[rd] = (uint32_t) ((uint32_t) registers[rs] + (uint32_t) registers[rt]);
} 

void sub(uint32_t instruction){
	uint8_t rs = RS_FIELD(instruction);
	uint8_t rt = RT_FIELD(instruction);
	uint8_t rd = RD_FIELD(instruction);
	registers[rd] = (uint32_t) ((int32_t) registers[rs] - (int32_t) registers[rt]);
} 

void subu(uint32_t instruction){
	uint8_t rs = RS_FIELD(instruction);
	uint8_t rt = RT_FIELD(instruction);
	uint8_t rd = RD_FIELD(instruction);
	registers[rd] = (uint32_t) ((uint32_t) registers[rs] - (uint32_t) registers[rt]);
} 

void mult(uint32_t instruction){
	uint8_t rs = RS_FIELD(instruction);
	uint8_t rt = RT_FIELD(instruction);
	int64_t result = (int64_t)((int32_t) registers[rs]) * (int64_t)((int32_t) registers[rt]);
	LO = (uint32_t) result;
	HI = (uint32_t) logicalShiftRight(result, 32);
} 

void multu(uint32_t instruction){
	uint8_t rs = RS_FIELD(instruction);
	uint8_t rt = RT_FIELD(instruction);
	uint64_t result = (uint64_t) registers[rs] * (uint64_t) registers[rt]; 
	LO = (uint32_t) result;
	HI = (uint32_t) logicalShiftRight(result, 32);
} 

void div(uint32_t instruction){
	uint8_t rs = RS_FIELD(instruction);
	uint8_t rt = RT_FIELD(instruction);
	LO = (uint32_t) ((int32_t) registers[rs] / (int32_t) registers[rt]);
	HI = (uint32_t) ((int32_t) registers[rs] % (int32_t) registers[rt]);
} 

void divu(uint32_t instruction){
	uint8_t rs = RS_FIELD(instruction);
	uint8_t rt = RT_FIELD(instruction);
	LO = (uint32_t) ((uint32_t) registers[rs] / (uint32_t) registers[rt]);
	HI = (uint32_t) ((uint32_t) registers[rs] % (uint32_t) registers[rt]);
} 

void and(uint32_t instruction){
	uint8_t rs = RS_FIELD(instruction);
	uint8_t rt = RT_FIELD(instruction);
	uint8_t rd = RD_FIELD(instruction);
	registers[rd] = (uint32_t) ((uint32_t) registers[rs] & (uint32_t) registers[rt]);
} 

void nor(uint32_t instruction){
	uint8_t rs = RS_FIELD(instruction);
	uint8_t rt = RT_FIELD(instruction);
	uint8_t rd = RD_FIELD(instruction);
	registers[rd] = (uint32_t) ~((uint32_t) registers[rs] | (uint32_t) registers[rt]);
} 

void or(uint32_t instruction){
	uint8_t rs = RS_FIELD(instruction);
	uint8_t rt = RT_FIELD(instruction);
	uint8_t rd = RD_FIELD(instruction);
	registers[rd] = (uint32_t) ((uint32_t) registers[rs] | (uint32_t) registers[rt]);
} 

void xor(uint32_t instruction){
	uint8_t rs = RS_FIELD(instruction);
	uint8_t rt = RT_FIELD(instruction);
	uint8_t rd = RD_FIELD(instruction);
	registers[rd] = (uint32_t) ((uint32_t) registers[rs] ^ (uint32_t) registers[rt]);
} 

void sll(uint32_t instruction){
	uint8_t rt = RT_FIELD(instruction);
	uint8_t rd = RD_FIELD(instruction);
	uint8_t shamt = SHAMT_FIELD(instruction);
	registers[rd] = (uint32_t) ((uint32_t) registers[rt] << shamt);
} 

void sllv(uint32_t instruction){
	uint8_t rs = RT_FIELD(instruction);
	uint8_t rt = RT_FIELD(instruction);
	uint8_t rd = RD_FIELD(instruction);
	registers[rd] = (uint32_t) ((uint32_t) registers[rs] << (uint32_t) registers[rt]);
} 

void srl(uint32_t instruction){
	uint8_t rt = RT_FIELD(instruction);
	uint8_t rd = RD_FIELD(instruction);
	uint8_t shamt = SHAMT_FIELD(instruction);
	registers[rd] = (uint32_t) logicalShiftRight((uint32_t) registers[rt], shamt);
} 

void sra(uint32_t instruction){
	uint8_t rt = RT_FIELD(instruction);
	uint8_t rd = RD_FIELD(instruction);
	uint8_t shamt = SHAMT_FIELD(instruction);
	registers[rd] = (uint32_t) ((int32_t) registers[rt] >> shamt);
} 

void srlv(uint32_t instruction){
	uint8_t rs = RT_FIELD(instruction);
	uint8_t rt = RT_FIELD(instruction);
	uint8_t rd = RD_FIELD(instruction);
	registers[rd] = (uint32_t) logicalShiftRight((uint32_t) registers[rs], registers[rt]);
} 

void srav(uint32_t instruction){
	uint8_t rs = RT_FIELD(instruction);
	uint8_t rt = RT_FIELD(instruction);
	uint8_t rd = RD_FIELD(instruction);
	registers[rd] = (uint32_t) ((int32_t) registers[rs] >> (uint32_t) registers[rt]);
} 

void slt(uint32_t instruction){
	uint8_t rs = RT_FIELD(instruction);
	uint8_t rt = RT_FIELD(instruction);
	uint8_t rd = RD_FIELD(instruction);
	if((int32_t) registers[rs] < (int32_t) registers[rt]){
		registers[rd] = 1;
	}
	else {
		registers[rd] = 0;
	}
} 

void sltu(uint32_t instruction){
	uint8_t rs = RT_FIELD(instruction);
	uint8_t rt = RT_FIELD(instruction);
	uint8_t rd = RD_FIELD(instruction);
	if((uint32_t) registers[rs] < (uint32_t) registers[rt]){
		registers[rd] = 1;
	}
	else {
		registers[rd] = 0;
	}
} 

void jr(uint32_t instruction){
	uint8_t rs = RT_FIELD(instruction);
	npc = (uint32_t) registers[rs];
} 

void jalr(uint32_t instruction){
	uint8_t rs = RT_FIELD(instruction);
	r[31] = pc << 2;
	npc = registers[rs];
} 

void mfhi(uint32_t instruction){
	uint8_t rd = RD_FIELD(instruction);
	registers[rd] = HI;
} 

void mflo(uint32_t instruction){
	uint8_t rd = RD_FIELD(instruction);
	registers[rd] = LO;
} 
