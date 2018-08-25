#include "convert_to_c_file.h"


void print_opcode(struct opcode** opr, int len){
	printf("\n%-5s %-5s %-5s %-5s %-5s %-5s %-5s %-5s", "pc", "ic", "p1", "p2", "p3", "p4", "p5", "gc");
	for (int i = 0; i < len; i++){
		printf("\n%-5d %-5d", opr[i]->program_counter, opr[i]->instruction_count);
		for (int j = 0; j < 5; j++){
			printf(" %-5d", opr[i]->parameter[j]);
		}
		printf(" %-5d", opr[i]->goto_counter);
	}
}

void write_opcode_to_file(struct opcode** opr, int len, char* file_name){
	FILE *file_pointer = fopen(file_name,"w");
	fprintf(file_pointer,"%d\n", len);
	for (int i = 0; i < len; i++){
		fprintf(file_pointer,"%d,%d,", opr[i]->program_counter, opr[i]->instruction_count);
		for (int j = 0; j < 5; j++){
			fprintf(file_pointer," %d,", opr[i]->parameter[j]);
		}
		fprintf(file_pointer," %d\n", opr[i]->goto_counter);
	}
	fclose(file_pointer);
}

void opcode_generation(FILE* file_pointer, struct symbol_table* var, int* length, struct opcode** opr){
	char *str = (char*)calloc(100, sizeof(char));
	int register_index1,register_index2,len,instruction_counter,program_counter = 0;
	char** tokens;
	STACK* label = stack_initialize();
	STACK* if_stack = stack_initialize();
	while (1){
		fscanf(file_pointer, "%[^'\n']s", str);
		fgetc(file_pointer);
		//printf("\ninstr:----%s", str);
		tokens = tokenize_by_space(str, &len);
		instruction_counter = get_instruction_index(tokens[0]);
		if (instruction_counter >= 0){
			if (instruction_counter == 0 || instruction_counter == 1&&len==3){
				register_index1 = get_register_index(tokens[1]);
				register_index2 = get_register_index(tokens[2]);
				if (register_index1 != 1){
					int index = get_symbol_index(var, tokens[2]);
					if (index != -1){
						int* arr = (int*)calloc(8, sizeof(int));
						arr[0] = register_index1;
						arr[1] = index;
						initialize_opcode(opr, program_counter, program_counter, 1, -1, arr);
						program_counter++;
					}
					else{
						printf("\nUndeclared variable");
						exit(1);
					}
				}
				else if (register_index2 != 1){
					int index = get_symbol_index(var, tokens[1]);
					int type = get_symbol_type(var, tokens[1]);
					if (index != -1&&type!=0){
						int* arr = (int*)calloc(8, sizeof(int));
						arr[0] = index;
						arr[1] = register_index2;
						initialize_opcode(opr, program_counter, program_counter, 0, -1, arr);
						program_counter++;
					}
					else{
						printf("\nUndeclared variable");
						exit(1);
					}
				}
				else{
					printf("\nInvalid operations on memory");
					exit(1);
				}
			}
			else if (instruction_counter == 2 || instruction_counter == 3 || instruction_counter == 4&&len==4){
				int reg1 = get_register_index(tokens[1]);
				int reg2 = get_register_index(tokens[2]);
				int reg3 = get_register_index(tokens[3]);
				if (reg1 != 1 && reg2 != 1 && reg3 != 1){
					int* arr = (int*)calloc(8, sizeof(int));
					arr[0] = reg1;
					arr[1] = reg2;
					arr[2] = reg3;
					initialize_opcode(opr,program_counter,program_counter,instruction_counter,-1,arr);
					program_counter++;
				}
				else{
					printf("\nInvalid operations on memory");
					exit(1);
				}
			}
			else if (instruction_counter == 12 || instruction_counter == 13 && len==2){
				int reg1 = get_register_index(tokens[1]);
				int index = get_symbol_index(var,tokens[1]);
				if (reg1 != 1){
					int* arr = (int*)calloc(8, sizeof(int));
					arr[0] = reg1;
					initialize_opcode(opr, program_counter, program_counter, instruction_counter, -1, arr);
					program_counter++;
				}
				else if (index != -1){
					int* arr = (int*)calloc(8, sizeof(int));
					arr[0] = index;
					initialize_opcode(opr, program_counter, program_counter, instruction_counter, -1, arr);
					program_counter++;
				}
				else{
					printf("\nInvalid operations on memory");
					exit(1);
				}
			}
			else if (instruction_counter == 5&&len==2){
				int* arr = (int*)calloc(8, sizeof(int));
				initialize_opcode(opr, program_counter, program_counter, instruction_counter, -1, arr);
				program_counter++;
				stack_push(label, tokens[1], program_counter-1,1);
			}
			else if (instruction_counter == 6&&!strcmp("THEN",tokens[len-1])){
				int reg1 = get_register_index(tokens[1]);
				int reg2 = get_register_index(tokens[3]);
				int index_instruct = get_instruction_index(tokens[2]);
				if (index_instruct != -1 && reg1 != 1 && reg2 != 1){
					int* arr = (int*)calloc(8, sizeof(int));
					arr[0] = reg1;
					arr[1] = reg2;
					arr[2] = index_instruct;
					initialize_opcode(opr, program_counter, program_counter, instruction_counter, -1, arr);
					program_counter++;
					stack_push(if_stack, "IF", program_counter-1,1);
				}
				else{
					printf("\nInvalid operations on memory");
					exit(1);
				}
			}
			else if (instruction_counter == 15&&len==1){
				char* name = (char*)calloc(10, 1);
				int gc;
				stack_pop(if_stack, name, &gc);
				opr[gc]->goto_counter = program_counter+1;
				stack_push(if_stack, "ELSE", program_counter,2);
				int* arr = (int*)calloc(8, sizeof(int));
				initialize_opcode(opr, program_counter, program_counter, instruction_counter, -1, arr);
				program_counter++;
			}
			else if (instruction_counter == 14&&len==1){
				char* name = (char*)calloc(10, 1);
				int gc;
				stack_pop(if_stack, name, &gc);
				opr[gc]->goto_counter = program_counter;
			}
			else{
				printf("\nCompiler time error:invalid Token or syntax");
				exit(1);
			}
		}
		else if (tokens[0][strlen(tokens[0])-1]==':'){
			tokens[0][strlen(tokens[0]) - 1] = '\0';
			stack_push(label,tokens[0],program_counter,0);
		}
		else if (instruction_counter == -1){
			///printf("\nInvalid instructions");
			break;
		}
		for (int index = 0; index < len; index++){
			free(tokens[index]);
		}

	}
	for (int index = 0; index <= label->top; index++){
		if (label[0].type[index] == 1){
			for (int j_index = 0; j_index <= label[0].top; j_index++){
				if (j_index != index && !strcmp(label[0].names[index], label[0].names[j_index]) && label[0].type[j_index] == 0){
					opr[label[0].pc[index]]->goto_counter = label[0].pc[j_index];
				}
			}
		}
	}
	*length = program_counter;
	return;
}

void read_data(FILE* file_pointer, struct symbol_table* var){
	char *str = (char*)calloc(100, sizeof(char));
	while (1){
		fscanf(file_pointer, "%[^'\n']s", str);
		fgetc(file_pointer);
		int len;
		char** tokens = tokenize_by_space(str, &len);
		if (!strcmp(tokens[0], "DATA")){
			insert_symbol(var, tokens[1],1,0);
		}
		else if (!strcmp(tokens[0], "CONST")){
			int index;
			int value = convertint(tokens[3]);
			char *name = get_name_and_index(tokens[1], &index);
			insert_symbol(var, tokens[1], 0,value);
		}
		else{
			printf("\nReading done");
			int length;
			struct opcode** opr=initialize_opcode_table();
			opcode_generation(file_pointer, var,&length,opr);
			fclose(file_pointer);
			//print_opcode(opr, length);
			printf("\nif you want to write opcode into file enter 1 else 0\n");
			int temp;
			scanf("%d", &temp);
			if (temp == 1){
				char* dest_file_name = (char*)calloc(30, sizeof(char));
				scanf("%s", dest_file_name);
				write_opcode_to_file(opr, length, dest_file_name);
			}
			execute_opcode(var, opr, length);
			return;
		}
		for (int index = 0; index < len; index++){
			free(tokens[index]);
		}
	}
}



int main(){
	struct symbol_table* var = initialize_symbol_table();
	int type;
	printf("Enter 1 to execute code file or\nEnter 2 to convert code to c code or\nEnter 3 to run opcode file\n>>");
	scanf("%d", &type);
	char* file_name = (char*)calloc(100, 1);
	printf("Enter file name:");
	scanf("%s", file_name);
	FILE *file_pointer = fopen(file_name, "r");
	if (file_pointer == NULL){
		printf("\nFile not found");
		exit(1);
	}
	if(type==1)read_data(file_pointer, var);
	else if (type == 2)convert_to_c(file_pointer, var);
	else if (type == 3)read_opcode(var, file_name);
	return 0;
}