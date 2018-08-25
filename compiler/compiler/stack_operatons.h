#include "Basecodes.h"
struct stack
{
	char** names;
	int *pc;
	int *type;
	int top;
};

typedef struct stack STACK;

STACK* stack_initialize(){
	STACK* s;
	s = (STACK*)calloc(1, sizeof(STACK));
	s->top = -1;
	s->names = (char**)calloc(100, sizeof(char*));
	for (int i = 0; i < 100; i++){
		s->names[i] = (char*)calloc(10, sizeof(char));
	}
	s->pc = (int*)calloc(10, sizeof(int));
	s->type = (int*)calloc(10, sizeof(int));
	return s;
}

void stack_push(STACK* s, char* name, int pc,int type){
	if (s[0].top == (100 - 1))
	{
		printf("Stack is Full\n");
		return;
	}
	else
	{
		s[0].top = s[0].top + 1;
		strcpy(s[0].names[s[0].top], name);
		s[0].pc[s[0].top] = pc;
		s[0].type[s[0].top] = type;
	}
	return;
}


void stack_pop(STACK* s,char* name, int *pc){
	if (s[0].top == -1)
	{
		printf("Stack is Empty\n");
		return;
	}
	else
	{
		strcpy(name,s[0].names[s[0].top]);
		//free(s[0].names[s[0].top]);
		*pc=s[0].pc[s[0].top];
		s[0].top--;
	}
	return;
}

void display_stack(STACK* S){
	printf("\n\n%-10s%-10s%-10s", "name", "pc", "type");
	for (int i = 0; i <= S[0].top; i++){
		printf("\n%-10s%-10d%-10d", S[0].names[i], S[0].pc[i], S[0].type[i]);
	}
}
