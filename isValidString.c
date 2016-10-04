#include <stdio.h>
#include <stdlib.h>

int loadInvalidTokens(FILE* tokenp, int* tokens);
void printTokens(int* tokens);
int isValidIdent(char* word, int* tokens);
int isValidNum(char* word);
int getSize(char* word);

int main(){
	int tokens[128];
	FILE* tokenp;
	char* test[] = {"x", "a0", "0a", "abcde", "abcde*", "abcde#", "5983", "28#54"};

	loadInvalidTokens(tokenp, tokens);
	printTokens(tokens);
	
	for (int i = 0; i < 8; i++){
		if (test[i][0] < 58 && test[i][0] > 47){
			if (isValidNum(test[i]) == 1){
				printf("%s is a valid number\n", test[i]);
			}
		}
		else {
			if (isValidIdent(test[i], tokens) == 1){
				printf("%s is a valid identifier\n", test[i]);
			}
		}		
	}	
}

int loadInvalidTokens(FILE* tokenp, int* tokens){
	char ch;
	int value;

	for (int i = 0; i < 128; i++){
		tokens[i] = 0;
	}
	
	tokenp = fopen("tokens.txt", "r");
	if (tokenp == NULL) {
		printf("Can't open tokens.txt\n");
		return 0;
	}

	while((ch = fgetc(tokenp)) != EOF) {
		value = fgetc(tokenp) - 48;
		tokens[ch] = value;
	}

	fclose(tokenp);
	return 1;
}

void printTokens(int* tokens){
	for (int i = 33; i < 127; i++){
		printf("%c----%d\n", i, tokens[i]);
	}
}

int isValidIdent(char* word, int* tokens){
	int size = getSize(word);

	for (int i = 1; i < size; i++){
		if (tokens[word[i]] == 1){
			printf("Invalid token in identifier located at char %d for %s\n", i, word);
			return 0;
		}
		else if (tokens[word[i]] == 2){
			printf("Not properly tokenized at char %d for %s\n", i, word);
			return 0;
		}
	}
	return 1;
}

int getSize(char* word){
	char* c;
	for (c = word; *c != '\0'; c++){}
	return c - word;
}

int isValidNum(char* word){
	int size = getSize(word);

	for (int i = 1; i < size; i++){
		if (word[i] >= 58 || word[i] <= 47){
			printf("Invalid token in number located at char %d for %s\n", i, word);
			return 0;
		}
	}
	return 1;
}