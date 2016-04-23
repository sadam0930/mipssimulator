#include <stdint.h> 
#include "cpu.h"

//These are the i-instructions implemented by the simulted MIPS processor

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

//extracts the value of the immediate field bits
uint16_t IMM_FIELD(uint32_t instruction){
	instruction = instruction << 16;
	instruction = logicalShiftRight(instruction, 16);
	return (uint16_t) instruction;
}

int32_t sign_extend(uint16_t immediate){
	return ((int32_t) immediate << 16) >> 16;
}

int32_t sign_extend_8(uint8_t immediate){
	return ((int32_t) immediate << 24) >> 24;
}

uint32_t zero_extend(uint16_t immediate){
	return ((uint32_t) immediate << 16) >> 16;
}

uint32_t zero_extend_8(uint8_t immediate){
	return ((uint32_t) immediate << 24) >> 24;
}

void addi(uint32_t instruction){
	uint32_t rs = RS_FIELD(instruction);
	uint32_t rt = RT_FIELD(instruction);
	uint16_t imm = IMM_FIELD(instruction);
	registers[rt] = (uint32_t) ((int32_t) registers[rs] + (int32_t) sign_extend(imm)); 
}

void addiu(uint32_t instruction){
	uint32_t rs = RS_FIELD(instruction);
	uint32_t rt = RT_FIELD(instruction);
	uint16_t imm = IMM_FIELD(instruction);
	registers[rt] = (uint32_t) ((uint32_t) registers[rs] + (uint32_t) sign_extend(imm));
}

void andi(uint32_t instruction){
	uint32_t rs = RS_FIELD(instruction);
	uint32_t rt = RT_FIELD(instruction);
	uint16_t imm = IMM_FIELD(instruction);
	registers[rt] = (uint32_t) ((uint32_t) registers[rs] & (uint32_t) zero_extend(imm));
}

void ori(uint32_t instruction){
	uint32_t rs = RS_FIELD(instruction);
	uint32_t rt = RT_FIELD(instruction);
	uint16_t imm = IMM_FIELD(instruction);
	registers[rt] = (uint32_t) ((uint32_t) registers[rs] | (uint32_t) zero_extend(imm));
}

void xori(uint32_t instruction){
	uint32_t rs = RS_FIELD(instruction);
	uint32_t rt = RT_FIELD(instruction);
	uint16_t imm = IMM_FIELD(instruction);
	registers[rt] = (uint32_t) ((uint32_t) registers[rs] ^ (uint32_t) zero_extend(imm));
}

void slti(uint32_t instruction){
	uint32_t rs = RS_FIELD(instruction);
	uint32_t rt = RT_FIELD(instruction);
	uint16_t imm = IMM_FIELD(instruction);
	if((int32_t) registers[rs] < (int32_t) sign_extend(imm)){
		registers[rt] = (uint32_t) 0x00000001;
	} else {
		registers[rt] = (uint32_t) 0x00000000;
	}
}

void sltiu(uint32_t instruction){
	uint32_t rs = RS_FIELD(instruction);
	uint32_t rt = RT_FIELD(instruction);
	uint16_t imm = IMM_FIELD(instruction);
	if((uint32_t) registers[rs] < (uint32_t) sign_extend(imm)){
		registers[rt] = (uint32_t) 0x00000001;
	} else {
		registers[rt] = (uint32_t) 0x00000000;
	}
}

void beq(uint32_t instruction){
	uint32_t rs = RS_FIELD(instruction);
	uint32_t rt = RT_FIELD(instruction);
	uint16_t imm = IMM_FIELD(instruction);
	if((uint32_t) registers[rs] == (uint32_t) registers[rt]){
		npc = pc + (uint32_t) 0x00000004 + (uint32_t) (sign_extend(imm) << 2);
	}
}

void bne(uint32_t instruction){
	uint32_t rs = RS_FIELD(instruction);
	uint32_t rt = RT_FIELD(instruction);
	uint16_t imm = IMM_FIELD(instruction);
	if((uint32_t) registers[rs] != (uint32_t) registers[rt]){
		npc = pc + (uint32_t) 0x00000004 + (uint32_t) (sign_extend(imm) << 2);
	}
}

void lb(uint32_t instruction){
	uint32_t rs = RS_FIELD(instruction);
	uint32_t rt = RT_FIELD(instruction);
	uint16_t imm = IMM_FIELD(instruction);
	registers[rt] = (uint32_t) sign_extend_8(memory[(registers[rs] + (uint32_t) sign_extend(imm))]); 
}

void lbu(uint32_t instruction){
	uint32_t rs = RS_FIELD(instruction);
	uint32_t rt = RT_FIELD(instruction);
	uint16_t imm = IMM_FIELD(instruction);
	registers[rt] = (uint32_t) zero_extend_8(memory[(registers[rs] + (uint32_t) sign_extend(imm))]); 
}

void lh(uint32_t instruction){
	uint32_t rs = RS_FIELD(instruction);
	uint32_t rt = RT_FIELD(instruction);
	uint16_t imm = IMM_FIELD(instruction);
	uint16_t * temp = (uint16_t *) &memory[(registers[rs] + (uint32_t) sign_extend(imm))];
	registers[rt] = (uint32_t) sign_extend((uint16_t) *temp);
}

void lhu(uint32_t instruction){
	uint32_t rs = RS_FIELD(instruction);
	uint32_t rt = RT_FIELD(instruction);
	uint16_t imm = IMM_FIELD(instruction);
	uint16_t * temp = (uint16_t *) &memory[(registers[rs] + (uint32_t) sign_extend(imm))];
	registers[rt] = (uint32_t) zero_extend((uint16_t) *temp);
}

void lw(uint32_t instruction){
	uint32_t rs = RS_FIELD(instruction);
	uint32_t rt = RT_FIELD(instruction);
	uint16_t imm = IMM_FIELD(instruction);
	uint32_t * temp = (uint32_t *) &memory[(registers[rs] + (uint32_t) sign_extend(imm))];
	registers[rt] = (uint32_t) *temp;
}

void lui(uint32_t instruction){
	uint32_t rt = RT_FIELD(instruction);
	uint16_t imm = IMM_FIELD(instruction);
	registers[rt] = (uint32_t) (imm << 16);
}

void sb(uint32_t instruction){
	uint32_t rs = RS_FIELD(instruction);
	uint32_t rt = RT_FIELD(instruction);
	uint16_t imm = IMM_FIELD(instruction);
	memory[(registers[rs] + (uint32_t) sign_extend(imm))] = (uint8_t) registers[rt];
}

void sh(uint32_t instruction){
	uint32_t rs = RS_FIELD(instruction);
	uint32_t rt = RT_FIELD(instruction);
	uint16_t imm = IMM_FIELD(instruction);
	uint16_t * mlocation = (uint16_t *) memory[(registers[rs] + (uint32_t) sign_extend(imm))];
	*mlocation = (uint16_t) registers[rt];
}

void sw(uint32_t instruction){
	uint32_t rs = RS_FIELD(instruction);
	uint32_t rt = RT_FIELD(instruction);
	uint16_t imm = IMM_FIELD(instruction);
	uint32_t * mlocation = (uint32_t *) memory[(registers[rs] + (uint32_t) sign_extend(imm))];
	*mlocation = (uint32_t) registers[rt]
}
