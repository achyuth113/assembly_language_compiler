#include "stack_operatons.h"


void execute_opcode(struct symbol_table* variable,struct opcode** op_code, int length){
	printf("\n\nOutput:\n\n");
	long long int high = 0;
	int program_counter = 0;
	while (program_counter < length && high<(10*length)){
		high++;
		//printf("\n:pc=%d ",program_counter);
		if (op_code[program_counter]->instruction_count == 0){
			//variable->Symbol[variable->Symbols_len]->type = type;
			*(int *)(variable->memory + op_code[program_counter]->parameter[1]) = variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[0]))];
			program_counter++;
		}
		else if (op_code[program_counter]->instruction_count == 1){
			variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[0]))] = *(int *)(variable->memory + op_code[program_counter]->parameter[1]);
			program_counter++;
		}
		else if (op_code[program_counter]->instruction_count == 2){
			variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[0]))] = variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[1]))] + variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[2]))];
			program_counter++;
		}
		else if (op_code[program_counter]->instruction_count == 3){
			variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[0]))] = variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[1]))] - variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[2]))];
			program_counter++;
		}
		else if (op_code[program_counter]->instruction_count == 4){
			variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[0]))] = variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[1]))] * variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[2]))];
			program_counter++;
		}
		else if (op_code[program_counter]->instruction_count == 5){
			program_counter = op_code[program_counter]->goto_counter;
		}
		else if (op_code[program_counter]->instruction_count == 6){
			if (op_code[program_counter]->parameter[2] == 7){
				if (variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[0]))] == variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[1]))]){
					program_counter++;
				}
				else{
					program_counter = op_code[program_counter]->goto_counter;
				}
			}
			else if (op_code[program_counter]->parameter[2] == 8){
				if (variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[0]))] < variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[1]))]){
					program_counter++;
				}
				else{
					program_counter = op_code[program_counter]->goto_counter;
				}
			}
			else if (op_code[program_counter]->parameter[2] == 9){
				if (variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[0]))] > variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[1]))]){
					program_counter++;
				}
				else{
					program_counter = op_code[program_counter]->goto_counter;
				}
			}
			else if (op_code[program_counter]->parameter[2] == 10){
				if (variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[0]))] <= variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[1]))]){
					program_counter++;
				}
				else{
					program_counter = op_code[program_counter]->goto_counter;
				}
			}
			else if (op_code[program_counter]->parameter[2] == 11){
				if (variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[0]))] >= variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[1]))]){
					program_counter++;
				}
				else{
					program_counter = op_code[program_counter]->goto_counter;
				}
			}
		}
		else if (op_code[program_counter]->instruction_count == 12){
			printf("\n%s=%d\n", register_names[(-1 + (-1 * op_code[program_counter]->parameter[0]))], variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[0]))]);
			program_counter++;
		}
		else if (op_code[program_counter]->instruction_count == 13){
			//printf("%d\n", (-1 + (-1 * op_code[program_counter]->parameter[0])));
			printf("\nEnter a value\n");
			scanf("%d", &(variable->registers[(-1 + (-1 * op_code[program_counter]->parameter[0]))]));
			//printf("end");
			program_counter++;
		}
		else if (op_code[program_counter]->instruction_count == 15){
			program_counter = op_code[program_counter]->goto_counter;
		}
	}

}

void read_opcode(struct symbol_table* variable,char* file_name){
	FILE* fp = fopen(file_name, "r");
	if (fp == NULL){
		printf("\nFile is empty");
		exit(1);
	}
	int len;
	fscanf(fp, "%d\n", &len);
	struct opcode** opr;
	opr = (struct opcode**)calloc(100, sizeof(struct opcode*));
	for (int index = 0; index < len; index++){
		opr[index] = (struct opcode*)calloc(100, sizeof(struct opcode));
	}
	for (int i = 0; i < len; i++){
		fscanf(fp, "%d,%d,", &(opr[i]->program_counter), &(opr[i]->instruction_count));
		for (int j = 0; j < 5; j++){
			fscanf(fp, " %d,", &(opr[i]->parameter[j]));
		}
		fscanf(fp, " %d\n", &(opr[i]->goto_counter));
	}
	fclose(fp);
	execute_opcode(variable, opr, len);
}
