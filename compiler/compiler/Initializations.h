#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define maxsize 200

struct symbol{
	char *name;
	int size;
	int from_block;
	int end_block;
	int type;
};

struct symbol_table{
	char *memory;
	struct symbol** Symbol;
	int Symbols_len;
	int last_index;
	int* registers;
};

struct opcode{
	int program_counter;
	int instruction_count;
	int parameter[8];
	int goto_counter;
};

char valid_instructions[20][30] = { "MOV", "MOV", "ADD", "SUB", "MUL", "JUMP", "IF", "EQ", "LT", "GT", "LTEQ", "GTEQ", "PRINT", "READ", "ENDIF","ELSE" };

char valid_instructions_conversions[20][30] = { "=", "=", "+", "-", "*", "goto ", "if", "==", "<", ">", "<=", ">=", "printf", "scanf", "}", "else" };

char register_names[10][5] = { "AX", "BX", "CX", "DX", "EX", "FX", "GX", "HX" };

int get_register_index(char* name){
	for (int index = 0; index < 8; index++){
		if (!strcmp(name, register_names[index])){
			return (-1*index)-1;
		}
	}
	return 1;
}


int get_instruction_index(char* name){
	for (int index = 0; index < 16; index++){
		if (!strcmp(name, valid_instructions[index])){
			return index;
		}
	}
	return -1;
}


struct symbol_table* initialize_symbol_table(){
	struct symbol_table* variable;
	variable = (struct symbol_table*)calloc(1, sizeof(struct symbol_table));
	variable->memory = (char*)calloc(100, 1);
	variable->Symbols_len = 0;
	variable->last_index = 1;
	variable->Symbol = (struct symbol**)calloc(100, sizeof(struct symbol*));
	variable->registers = (int*)calloc(10, sizeof(int));
	return variable;
}

struct opcode** initialize_opcode_table(){
	struct opcode** variable;
	variable = (struct opcode**)calloc(100, sizeof(struct opcode*));
	return variable;
}

void initialize_opcode(struct opcode** variable, int index, int program_counter, int instruction_counter, int goto_counter, int* array){
	variable[index] = (struct opcode*)calloc(100, sizeof(struct opcode));
	variable[index]->goto_counter = goto_counter;
	for (int i = 0; i < 8; i++){
		variable[index]->parameter[i] = array[i];
	}
	variable[index]->instruction_count = instruction_counter;
	variable[index]->program_counter = program_counter;
	
}

char* get_name_and_index(char* string, int* index){
	char* name = (char*)calloc(100, sizeof(char));
	int temp = 0;
	while (string[temp] != '\0' && string[temp] != '['){
		name[temp] = string[temp];
		temp++;
	}
	if (string[temp] == '\0'){
		name[temp++] = '\0';
		*index = 1;
		return name;
	}
	name[temp++] = '\0';
	int temp2 = 0;
	while (string[temp] != '\0' && string[temp] != ']'){
		temp2 = (temp2 * 10) + string[temp] - '0';
		temp++;
	}
	*index = temp2;
	//free(string);
	return name;
}

char* get_name_and_index_2(char* string, int* index){
	char* name = (char*)calloc(100, sizeof(char));
	int temp = 0;
	while (string[temp] != '\0' && string[temp] != '['){
		name[temp] = string[temp];
		temp++;
	}
	if (string[temp] == '\0'){
		name[temp++] = '\0';
		*index = 0;
		return name;
	}
	name[temp++] = '\0';
	int temp2 = 0;
	while (string[temp] != '\0' && string[temp] != ']'){
		temp2 = (temp2 * 10) + string[temp] - '0';
		temp++;
	}
	*index = temp2;
	//free(string);
	return name;
}


void insert_symbol(struct symbol_table* variable, char *str, int type,int value){
	int size;
	char *name = get_name_and_index(str, &size);
	for (int temp = 0; temp < variable->Symbols_len; temp++){
		if (!strcmp(variable->Symbol[temp]->name, name)){
			printf("\nName exists");
			return;
		}
	}
	*(int *)(variable->memory + variable->last_index) = value;
	variable->Symbol[variable->Symbols_len] = (struct symbol*)calloc(1, sizeof(struct symbol));
	variable->Symbol[variable->Symbols_len]->name = (char*)calloc(100, sizeof(char));
	strcpy(variable->Symbol[variable->Symbols_len]->name, name);
	variable->Symbol[variable->Symbols_len]->from_block = variable->last_index;
	variable->Symbol[variable->Symbols_len]->end_block = variable->last_index+(size*4);
	variable->Symbol[variable->Symbols_len]->type = type;
	variable->Symbol[variable->Symbols_len]->size = size;
	variable->last_index += (size * 4);
	variable->Symbols_len++;
}

void get_symbol(struct symbol_table* variable, int index){
	printf("%d\n", *(int *)(variable->memory + index));
	return;
}


int get_symbol_index(struct symbol_table* variable, char* str){
	int index = 0;
	char *name = get_name_and_index_2(str, &index);
	for (int temp = 0; temp < variable->Symbols_len; temp++){
		if (!strcmp(variable->Symbol[temp]->name, name)){
			if (variable->Symbol[temp]->size > index){
				return variable->Symbol[temp]->from_block + (4 * index);
			}
			else{
				printf("\nsize out of bound");
				return -1;
			}
		}
	}
	//printf("\nNot found of variableiable");
	return -1;
}

int get_symbol_type(struct symbol_table* variable, char* str){
	int index = 0;
	char *name = get_name_and_index_2(str, &index);
	for (int temp = 0; temp < variable->Symbols_len; temp++){
		if (!strcmp(variable->Symbol[temp]->name, name)){
			if (variable->Symbol[temp]->size > index){
				return variable->Symbol[temp]->type;
			}
			else{
				printf("\nsize out of bound");
				return -1;
			}
		}
	}
	printf("\nNot found of variable");
	return -1;
}

