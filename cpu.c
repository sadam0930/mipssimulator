
#include <stdio.h>

#include <string.h>
#include <stdint.h>

//#include <stdlib.h>

extern void exit(int);

#include <assert.h>

#include "cpu.h"
#include "register_names.h"
#include "i_instructions.h"
#include "r_instructions.h"
#include "j_instructions.h"
#include "utilities.h"

#include "programming.h"

/* The Registers */


uint32_t registers[NUM_REGISTERS];

uint32_t pc, npc;

uint32_t HI, LO;

/* Memory */

#define MEMORY_SIZE_IN_BYTES (1 << 20)

uint8_t memory[MEMORY_SIZE_IN_BYTES];



//opcode is the leftmost 6 bits of the instruction
//So just need to shift right 26 bits
#define OPCODE_SHIFT 26

#define OPCODE(__x) ((uint8_t) (__x >> OPCODE_SHIFT))


//6 bit opcode, so maximum number of opcodes is 64.
#define MAX_NUMBER_OF_OPCODES 64   

//This is the type of function pointers for populating the 
//instruction dispatch table. 
typedef  void (*OPCODE_DISPATCH_FUNCTION_TYPE)(uint32_t);

//This is the instruction dispatch table. Each entry, indexed
//by opcode, points to the function for performing the 
//corresponding instruction. For R instructions, all of which have
//an opcode = 0, the 0th element of the table points to a 
//function that dispatches on the funct field to perform
//the appropriate operation.
extern OPCODE_DISPATCH_FUNCTION_TYPE opcode_dispatch_table[];

OPCODE_DISPATCH_FUNCTION_TYPE opcode_dispatch_table[MAX_NUMBER_OF_OPCODES];


BOOL exit_flag;


//All R-instructions have OPCODE = 0

#define R_INSTRUCTION_OPCODE (0x0)


//J instruction OPCODES

#define J_OPCODE (0x02)
#define JAL_OPCODE (0x03)


//I Instruction OPCODES

#define ADDI_OPCODE (0x08)
#define ADDIU_OPCODE (0x09)
#define LW_OPCODE (0x23)
#define LH_OPCODE (0x21)
#define LHU_OPCODE (0x25)
#define LB_OPCODE (0x20)
#define LBU_OPCODE (0x24)
#define SW_OPCODE (0x2b)
#define SH_OPCODE (0x29)
#define SB_OPCODE (0x28)
#define LUI_OPCODE (0x0f)
#define ANDI_OPCODE (0x0c)
#define ORI_OPCODE (0x0d)
#define SLTI_OPCODE (0x0a)
#define BEQ_OPCODE (0x04)
#define BNE_OPCODE (0x05)

//R Instruction FUNCT codes

#define ADD_FUNCT (0x20)
#define ADDU_FUNCT (0x21)
#define SUB_FUNCT (0x22)
#define SUBU_FUNCT (0x23)
#define MULT_FUNCT (0x18)
#define DIV_FUNCT (0x1a)
#define DIVU_FUNCT (0x1b)
#define MFHI_FUNCT (0x10)
#define MFLO_FUNCT (0x12)
#define AND_FUNCT (0x24)
#define OR_FUNCT (0x25)
#define XOR_FUNCT (0x26)
#define NOR_FUNCT (0x27)
#define SLT_FUNCT (0x2A)
#define SLL_FUNCT (0x00)
#define SRL_FUNCT (0x02)
#define SRA_FUNCT (0x03)
#define JR_FUNCT (0x08)
#define SYSCALL_FUNCT (0x0c)

//funct field
//0000 0000 0000 0000 0000 0000 0011 1111 
#define FUNCT_FIELD_MASK 0x0000003f
#define FUNCT_FIELD(__x) ((__x) & FUNCT_FIELD_MASK)


void illegal_r_instruction(uint32_t instruction)
{
  printf("Error. Illegal funct field for an R instruction: %d\n", FUNCT_FIELD(instruction));
  exit(1);
}

//syscall is defined in syscall.c
extern void syscall();


//6 bit funct
#define MAX_NUM_FUNCT_VALUES (1 << 6) 


typedef  void (*R_FUNCT_DISPATCH_FUNCTION_TYPE)(uint32_t);

R_FUNCT_DISPATCH_FUNCTION_TYPE r_funct_dispatch_table[MAX_NUM_FUNCT_VALUES];



void dispatch_r_instruction(uint32_t instruction)
{
  uint8_t funct = FUNCT_FIELD(instruction);
  R_FUNCT_DISPATCH_FUNCTION_TYPE r_funct_dispatch_function = r_funct_dispatch_table[funct];
  r_funct_dispatch_function(instruction);
}


void populate_r_funct_dispatch_table()
{
  int i;
  for(i=0; i < MAX_NUM_FUNCT_VALUES; i++)
    r_funct_dispatch_table[i] = illegal_r_instruction;
  
  r_funct_dispatch_table[ADD_FUNCT] = add;
  r_funct_dispatch_table[ADDU_FUNCT] = addu;
  r_funct_dispatch_table[SUB_FUNCT] = sub;
  r_funct_dispatch_table[SUBU_FUNCT] = subu;
  r_funct_dispatch_table[MULT_FUNCT] = mult;
  r_funct_dispatch_table[DIV_FUNCT] = div;
  r_funct_dispatch_table[DIVU_FUNCT] = divu;
  r_funct_dispatch_table[MFHI_FUNCT] = mfhi;
  r_funct_dispatch_table[MFLO_FUNCT] = mflo;
  r_funct_dispatch_table[AND_FUNCT] = and;
  r_funct_dispatch_table[OR_FUNCT] = or;
  r_funct_dispatch_table[XOR_FUNCT] = xor;
  r_funct_dispatch_table[NOR_FUNCT] = nor;
  r_funct_dispatch_table[SLT_FUNCT] = slt;
  r_funct_dispatch_table[SLL_FUNCT] = sll;
  r_funct_dispatch_table[SRL_FUNCT] = srl;
  r_funct_dispatch_table[SRA_FUNCT] = sra;
  r_funct_dispatch_table[JR_FUNCT] = jr;
  r_funct_dispatch_table[SYSCALL_FUNCT] = syscall;
}


void populate_opcode_dispatch_table_for_r_instructions()
{
  opcode_dispatch_table[R_INSTRUCTION_OPCODE] = dispatch_r_instruction;
}



void illegal_opcode(uint32_t instruction)
{
  printf("Error. Illegal opcode: %d\n", OPCODE(instruction));
  exit_flag = TRUE;
}



void populate_opcode_dispatch_table_for_j_instructions()
{
  opcode_dispatch_table[J_OPCODE] = j;
  opcode_dispatch_table[JAL_OPCODE] = jal;
}


void populate_opcode_dispatch_table_for_i_instructions()
{
  opcode_dispatch_table[ADDI_OPCODE] = addi;
  opcode_dispatch_table[ADDIU_OPCODE] = addiu;
  opcode_dispatch_table[LW_OPCODE] = lw;
  opcode_dispatch_table[LH_OPCODE] = lh;
  opcode_dispatch_table[LHU_OPCODE] = lhu;
  opcode_dispatch_table[LB_OPCODE] = lb;
  opcode_dispatch_table[LBU_OPCODE] = lbu;
  opcode_dispatch_table[SW_OPCODE] = sw;
  opcode_dispatch_table[SH_OPCODE] = sh;
  opcode_dispatch_table[SB_OPCODE] = sb;
  opcode_dispatch_table[LUI_OPCODE] = lui;
  opcode_dispatch_table[ANDI_OPCODE] = andi;
  opcode_dispatch_table[ORI_OPCODE] = ori;
  opcode_dispatch_table[SLTI_OPCODE] = slti;
  opcode_dispatch_table[BEQ_OPCODE] = beq;
  opcode_dispatch_table[BNE_OPCODE] = bne;
}



void populate_opcode_dispatch_table()
{
  int i;
  for(i=0;i<MAX_NUMBER_OF_OPCODES;i++)
    opcode_dispatch_table[i] = illegal_opcode;

  populate_opcode_dispatch_table_for_r_instructions();
  populate_opcode_dispatch_table_for_i_instructions();
  populate_opcode_dispatch_table_for_j_instructions();
}



void cpu_initialize()
{
  exit_flag = FALSE;
  pc = npc = CODE_STARTING_ADDRESS;
  REG_SP = STACK_STARTING_ADDRESS;
  REG_FP = REG_SP;
  registers[0] = 0;  //constant value 0
  populate_opcode_dispatch_table();
  populate_r_funct_dispatch_table();
}



void cpu_execute()
{
  uint32_t current_instruction;
  uint8_t opcode;
  OPCODE_DISPATCH_FUNCTION_TYPE dispatch_function;
  while (!exit_flag) {
    pc = npc;
    npc = pc + 4;
    current_instruction = *((uint32_t *) &(memory[pc]));
    opcode = OPCODE(current_instruction);

#ifdef DEBUG
    printf("At pc = %d, current_instruction = %x = ", pc, current_instruction);
    print_bits(current_instruction);
    printf("opcode = %d\n", (int) opcode);
#endif

    dispatch_function = opcode_dispatch_table[opcode];
    dispatch_function(current_instruction);
  }
}
    
  

void cpu_exit(int errcode)
{
  if (!errcode)
    printf("\nProgram terminated normally\n");
  else
    printf("\nProgram terminated with error code = %d\n", errcode);
  exit_flag = TRUE;
}



