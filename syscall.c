#include <stdint.h> 
#include <stdio.h>
#include "cpu.h"
#include "register_names.h"

void syscall(){
	switch(REG_V0){
		case 0x00000001:
			//print_int
			printf("%d\n", REG_A0);
			break;
		case 0x00000002:
			//print_float
			printf("Error: print_float not supported\n");
			cpu_exit(1);
			break;
		case 0x00000003:
			//print_double
			printf("Error: print_double not supported\n");
			cpu_exit(1);
			break;
		case 0x00000004:
			//print_string
			printf("%s", &memory[REG_A0]);
			break;
		case 0x00000005:
			//read_int
			scanf("%d", &REG_V0);
			break;
		case 0x00000006:
			//read_float
			printf("Error: read_float not supported\n");
			cpu_exit(1);
			break;
		case 0x00000007:
			//read_double
			printf("Error: read_double not supported\n");
			cpu_exit(1);
			break;
		case 0x00000008:
			{
				//read_string
				uint32_t max_num_bytes = REG_A1;
				uint32_t mem_location = REG_A0;
				uint32_t byte_i = 0;

				char temp[max_num_bytes];
				scanf("%s", temp);

				while(byte_i <= max_num_bytes){ 
					memory[mem_location] = (uint8_t) temp[byte_i];
					if(temp[byte_i] == 0) {
						break;
					} else {
						byte_i += sizeof(char);
						mem_location += sizeof(char);
					}
				}
			}
			break;
		case 0x00000009:
			//sbrk
			printf("Error: sbrk not supported\n");
			cpu_exit(1);
			break;
		case 0x0000000a:
			//exit
			cpu_exit(0);
			break;
		default:
			printf("Error: Unsupported operation\n");
			cpu_exit(1);
	}
}
