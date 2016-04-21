#include <stdint.h> 
#include <stdio.h>
#include "cpu.h"
#include "register_names.h"

void syscall(){
	printf("in syscall\n");
	switch(REG_V0){
		printf("REG_V0 is %d\n", REG_V0);
		case 0x00000001:
		printf("in 1\n");
			//print_int
			printf("%lu\n", (unsigned long) REG_A0);
			break;
		case 0x00000002:
		printf("in 2\n");
			//print_float
			printf("Error: print_float not supported\n");
			cpu_exit(1);
			break;
		case 0x00000003:
		printf("in 3\n");
			//print_double
			printf("Error: print_double not supported\n");
			cpu_exit(1);
			break;
		case 0x00000004:
		printf("in 4\n");
			//print_string
			printf("%s\n", &memory[REG_A0]);
			break;
		case 0x00000005:
		printf("in 5\n");
			//read_int
			scanf("%d", &REG_V0);
			break;
		case 0x00000006:
		printf("in 6\n");
			//read_float
			printf("Error: read_float not supported\n");
			cpu_exit(1);
			break;
		case 0x00000007:
		printf("in 7\n");
			//read_double
			printf("Error: read_double not supported\n");
			cpu_exit(1);
			break;
		case 0x00000008:
		printf("in 8\n"); 
			// {
			// 	//read_string
			// 	uint32_t max_num_bytes = REG_A1;
			// 	uint32_t mem_location = REG_A0;
			// 	uint32_t byte_i = 0;

			// 	char temp[max_num_bytes];
			// 	scanf("%s", temp);

			// 	while(byte_i <= max_num_bytes){ 
			// 		memory[mem_location] = (uint8_t) temp[byte_i];
			// 		if(temp[byte_i] == 0) {
			// 			break;
			// 		} else {
			// 			byte_i += sizeof(char);
			// 			mem_location += sizeof(char);
			// 		}
			// 	}
			// }
			break;
		case 0x00000009:
		printf("in 9\n");
			//sbrk
			printf("Error: sbrk not supported\n");
			cpu_exit(1);
			break;
		case 0x0000000a:
		printf("in 10\n");
			//exit
			cpu_exit(0);
			break;
		default:
		printf("in other\n");
			printf("Error: Unsupported operation\n");
			cpu_exit(1);
	}
}