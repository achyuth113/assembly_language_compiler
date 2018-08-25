#include "Initializations.h"

char** tokenize_by_space(char *string, int *len){
	if (string == NULL)return NULL;
	char **tokens1; int count = 1;
	tokens1 = (char**)calloc(50, sizeof(char*));
	int i = 0, j = 0, k = 0;
	for (i = 0; i < 20; i++)tokens1[i] = (char*)calloc(20, sizeof(char));
	i = 0, j = 0, k = 0;
	while (string[i] != '\0'){
		if (string[i] == ' ' || string[i] == ','||string[i]=='\t'){
			count++; j++; k = 0;
			i++;
		}
		else{
			char ch = string[i++];
			tokens1[j][k++] = ch;
		}
	}
	char **tokens;
	int count2 = 0;
	tokens = (char**)calloc(20, sizeof(char*));
	for (int x = 0; x < count; x++){
		if (tokens1[x][0] != '\0'){
			tokens[count2] = (char*)calloc(20, sizeof(char));
			strcpy(tokens[count2], tokens1[x]); count2++;
		}
		free(tokens1[x]);
	}
	*len = count2;
	return tokens;

}

int convertint(char *string){

	int num = 0, i = 10, j = 0;
	while (string[j] != '\0'&&isdigit(string[j])){
		num *= i;
		num += (string[j++] - '0');
	}
	if (string[j] != '\0')return -1;
	return num;
}