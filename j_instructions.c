#include <stdint.h> 
#include "cpu.h"

//These are the J-instructions implemented by the simulted MIPS processor

//J instruction:  opcode (6 bits)      address (26 bits)
uint32_t logicalShiftRight(uint32_t instruction, int shiftamt){
	instruction = instruction >> 1;
	uint32_t mask = 0x7fffffff;
	instruction &= mask;
	instruction = instruction >> shiftamt-1;
	return instruction;
}

uint8_t ADDR_FIELD(uint32_t instruction){ 
	instruction = instruction << 6;
	instruction = logicalShiftRight(instruction, 6);
	return (uint8_t) instruction;
}

void j(uint32_t instruction){
	uint32_t address = ADDR_FIELD(instruction);
	npc = (uint32_t) ((pc + 0x00000004) & 0xf0000000) | (address << 2);
}

void jal(uint32_t instruction){
	uint32_t address = ADDR_FIELD(instruction);
	registers[31] = (uint32_t) (pc + 0x00000004);
	npc = (uint32_t) ((pc + 0x00000004) & 0xf0000000) | (address << 2);
}