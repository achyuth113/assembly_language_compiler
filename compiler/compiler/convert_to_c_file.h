#include "Opcode_execution.h"

void put_prev_data(FILE* file_pointer){
	fprintf(file_pointer, "#include<stdio.h>\n#include<stdlib.h>\n");
	fprintf(file_pointer, "int main(){\nint AX,BX,CX,DX,EX,FX,GX,HX;\n");
	return;
}

void opcode_generation_and_write_into_file(FILE* file_pointer2, FILE* file_pointer, struct symbol_table* variable, int* length, struct opcode** opr){
	char *str = (char*)calloc(100, sizeof(char));
	int program_counter = 0;
	STACK* label = stack_initialize();
	STACK* if_stack = stack_initialize();
	while (1){
		fscanf(file_pointer, "%[^'\n']s", str);
		fgetc(file_pointer);
		int len;
		char** tokens = tokenize_by_space(str, &len);
		int instruction_counter = get_instruction_index(tokens[0]);
		if (instruction_counter >= 0){
			if (instruction_counter == 0 || instruction_counter == 1 && len == 3){
				fprintf(file_pointer2, "%s=%s;\n", tokens[1], tokens[2]);
			}
			else if (instruction_counter == 2 || instruction_counter == 3 || instruction_counter == 4 && len == 4){
				fprintf(file_pointer2, "%s%s%s%s%s;\n",tokens[1],"=",tokens[2],valid_instructions_conversions[instruction_counter],tokens[3]);
			}
			else if (instruction_counter == 12 && len == 2){
				int reg1 = get_register_index(tokens[1]);
				char s[10];
				s[0] = '('; s[1] = '"'; s[2] = '%'; s[3] = 'd'; s[4] = '"'; s[5] = ','; s[6] = '\0';
				fprintf(file_pointer2, "printf%s%s);\n", s, tokens[1]);
			}
			else if (instruction_counter == 13 && len == 2){
				char s[10];
				s[0] = '('; s[1] = '"'; s[2] = '%'; s[3] = 'd'; s[4] = '"'; s[5] = ','; s[6] = '\0';
				fprintf(file_pointer2, "scanf%s&%s);\n", s, tokens[1]);
			}
			else if (instruction_counter == 5 && len == 2){
				fprintf(file_pointer2, "goto %s;\n", tokens[1]);
			}
			else if (instruction_counter == 6 && !strcmp("THEN", tokens[len - 1])){
				int reg1 = get_register_index(tokens[1]);
				int reg2 = get_register_index(tokens[3]);
				int index_instruct = get_instruction_index(tokens[2]);
				fprintf(file_pointer2, "if(%s%s%s){\n", tokens[1], valid_instructions_conversions[index_instruct], tokens[3]);
			}
			else if (instruction_counter == 15 && len == 1){
				fprintf(file_pointer2, "}\nelse{\n");
			}
			else if (instruction_counter == 14 && len == 1){
				fprintf(file_pointer2, "}\n");
			}
			else{
				printf("\nCompiler time error:invalid Token or syntax");
				exit(1);
			}
		}
		else if (tokens[0][strlen(tokens[0]) - 1] == ':'){
			fprintf(file_pointer2, "%s\n", tokens[0]);
		}
		else if (instruction_counter == -1){
			printf("\nins::%s", str);
			break;
		}
		for (int index = 0; index < len; index++){
			free(tokens[index]);
		}
	}
	fprintf(file_pointer2, "return 0;\n}\n");
	fclose(file_pointer);
	return;
}


void convert_to_c(FILE* file_pointer, struct symbol_table* variable){
	char* file_name = (char*)calloc(100, sizeof(sizeof(char)));
	printf("\nEnter .c file name\n>>");
	scanf("%s", file_name);
	FILE* file_pointer2 = fopen(file_name, "w");
	char** tokens;
	int len;
	put_prev_data(file_pointer2);
	char *str = (char*)calloc(100, sizeof(char));
	while (1){
		fscanf(file_pointer, "%[^'\n']s", str);
		fgetc(file_pointer);
	    tokens = tokenize_by_space(str, &len);
		if (!strcmp(tokens[0], "DATA")){
			fprintf(file_pointer2, "int %s;", tokens[1]);
			//insert_symbol(variable, tokens[1], 1);
		}
		else if (!strcmp(tokens[0], "CONST")){
			int index;
			int value = convertint(tokens[3]);
			char *name = get_name_and_index(tokens[1], &index);
			fprintf(file_pointer2, "const int %s=%d;", tokens[1],value);
			//insert_symbol(variable, tokens[1], 0);
		}
		else{
			int length;
			struct opcode** opr = initialize_opcode_table();
			opcode_generation_and_write_into_file(file_pointer2,file_pointer, variable, &length, opr);
			fclose(file_pointer2);
			fclose(file_pointer);
			return;
		}
		for (int index = 0; index < len; index++){
			free(tokens[index]);
		}
	}
}