//Brandon Royal
//Ryan Beasley
//Stephen Ulmer
//Becca Parramore
//Bryan Hart

//Lexer Program

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void stringTokenizer(char* str, int printSource, int printClean);
void printSourceCode(char* source);
void printSourceCodeWithoutComments(char* source);
void printTable(char tokens[][50], int tokenCount);
int checkIdenBegin(char* str);
int checkIdenLength(char* str);
int checkNum(int num);
int validToken(char* str);
int getSystem(char* str);
void stringTokenizer();
char* deleteComments(char* str);
void deleteNewLine(char* source);
int checkSpecial(char c1, char c2);

//array of operators and special characters
char spec[] = {'+', '-', '*', '/', '=', '<', '>', ',', ';', '.', '(', ')', ':'};

//the primary function that handles tokenizing the input
void stringTokenizer(char* str, int printSource, int printClean){

    //open the given
    FILE* ptr;
    ptr = fopen(str, "r");
    if(ptr == NULL){
        printf("failed to open file\n");
        return;
    }

    //this takes the whole file that was given and puts it into a string
    //this allows us to go through the string and tokenize it
    char* fileContents = 0;
    fseek(ptr, 0, SEEK_END);
    int length = ftell(ptr);
    fseek(ptr, 0, SEEK_SET);
    fileContents = malloc (length);
    fread(fileContents, 1, length, ptr);

    int i;
    int tokenCount = 0;
    int curLength = 0;
    char tokens[1000][50];

    //based on the given input flags will print the source code
    if(printSource == 1){
        printSourceCode(fileContents);
    }

    //based on the given input flags will print the source
    if(printClean == 1){
        printSourceCodeWithoutComments(fileContents);

    }

    //if the clean source is not printed then the comments have not been deleted
    //so we must delete them so they are not tokenized
    if(printClean == 0)
        deleteComments(fileContents);
    //deletes new line characters from the input
    deleteNewLine(fileContents);

    //this for loop goes over the string that holds the file contents and tokenizes each part
    for(i=0; i<strlen(fileContents); i++){
        char curChar = fileContents[i];
        if(curChar != ' ' && curChar != '\n'){
            if(checkSpecial(curChar, fileContents[i+1]) == 0){
                tokens[tokenCount][curLength] = curChar;
                curLength++;
            }
            else if(checkSpecial(curChar, fileContents[i+1]) == 1){
                if(curLength > 0){
                    tokens[tokenCount][curLength] = '\0';
                    curLength = 0;
                    tokenCount++;
                }
                tokens[tokenCount][curLength] = curChar;
                curLength++;
                tokens[tokenCount][curLength] = '\0';
                curLength = 0;
                tokenCount++;
            }
            else if(checkSpecial(curChar, fileContents[i+1]) == 2){
                if(curLength > 0){
                    tokens[tokenCount][curLength] = '\0';
                    curLength = 0;
                    tokenCount++;
                }
                tokens[tokenCount][curLength] = curChar;
                curLength++;
                tokens[tokenCount][curLength] = fileContents[i+1];
                i++;
                curLength++;
                tokens[tokenCount][curLength] = '\0';
                curLength = 0;
                tokenCount++;
            }
        }
        else if(curChar == ' ' || curChar != '\n'){
            if(curLength > 0){
                tokens[tokenCount][curLength] = '\0';
                curLength = 0;
                tokenCount++;
            }
        }
    }

    //this will print the table of tokens and print out any errors because of invalid token issues
    printTable(tokens, tokenCount);

    //closes the file
    fclose(ptr);
}


//this function goes through and deletes any section of the input that is comments
//pretty much deletes everything from "/*" to "*/" and replaces it with spaces
//so that the clean source can keep its format
char* deleteComments(char* str){
    int i;
    for(i=0; i<strlen(str); i++){
        if(str[i] == '/' && str[i+1] == '*'){
            int j = i;
            while(1){
                if(str[j] == '\n'){
                    j++;
                    continue;
                }
                if(str[j] == '*' && str[j+1] == '/'){
                    str[j] = ' ';
                    str[j+1] = ' ';
                    break;
                }
                str[j] = ' ';
                j++;
            }
        }
    }
    return str;
}

//this function checks if something is a special character so that if you are given for example "3+3"
//the program will know "3" "+" and "3" are all separate characters
int checkSpecial(char c1, char c2){

    int i;
    for(i=0; i<sizeof(spec); i++){
        if(c1 == spec[i]){
            if(c2 == '=' || c2 == '>'){
                return 2;
            }
            else{
                return 1;
            }
        }
    }
return 0;
}

//deletes new line characters from the input
void deleteNewLine(char* source){
    int i;
    for(i=0; i<strlen(source); i++){
        if(source[i] == '\n' || source[i] == '\r'){
            source[i] = ' ';
        }
    }
}

//simply prints out the original source code
void printSourceCode(char* source){
    printf("source code:\n");
    printf("------------\n");
    printf("%s\n", source);
    printf("\n");
}

//first removes the comments and then prints out the clean source code
void printSourceCodeWithoutComments(char* source){
    source = deleteComments(source);
    printf("source code without comments:\n");
    printf("------------\n");
    printf("%s\n", source);
    printf("\n");
}

//this function is more complicated
//it first goes through the array of tokens and checks if they are valid
//if the token is valid it will be printed out along with its System number
//if the token is not valid then it will be placed in the "invalidTokens" array
//next the invalidTokens array is looped over and for each invalid token it
//prints what the error with the token is
//this function utilizes the validToken function which returns an int 1-4 depending on if there is an error and the type of error
void printTable(char tokens[][50], int tokenCount){

    char invalidTokens[1000][50];
    int invalidCount = 0;

    printf("tokens:\n");
    printf("-------\n");
    int i;
    for(i=0; i<tokenCount; i++){

        if(strcmp(tokens[i], "") == 0 || strcmp(tokens[i], " ") == 0 || strcmp(tokens[i], "\n") == 0 || strcmp(tokens[i], "\r\n") == 0){
                continue;
        }

        //printf("\nToken: %s and Error value: %d and System: %d", tokens[i], validToken(tokens[i]), getSystem(tokens[i]));
        if(validToken(tokens[i]) != 1 && getSystem(tokens[i]) == 2){
            strcpy(invalidTokens[invalidCount], tokens[i]);
            invalidCount++;
        }
        else if(validToken(tokens[i]) != 1 && getSystem(tokens[i]) == 3){
            strcpy(invalidTokens[invalidCount], tokens[i]);
            invalidCount++;
        }
        else{

            if(i == 0){
                printf("%-13s%d", tokens[i], getSystem(tokens[i]));
            }
            else{
                printf("\n%-13s%d", tokens[i], getSystem(tokens[i]));
            }
        }
    }

    //printf("Invalid count is : %d", invalidCount);

    //prints out all of the errors caused by invalid tokens
    if(invalidCount > 0){
        printf("\n");
        int error = 0;
        for(i=0; i<invalidCount; i++){
            printf("\n");
            error = validToken(invalidTokens[i]);

            if(error == 2){
                printf("Error : invalid token, number(%s) MAX_NUMBER_VALUE(65535)", invalidTokens[i]);
            }
            else if(error == 3){
                printf("Error : Invalid Identifier '%s', identifier must start with a letter", invalidTokens[i]);
            }
            else if(error == 4){
                printf("Error : Identifier '%s' is longer than 12 char (%d)", invalidTokens[i], strlen(invalidTokens[i]));

            }
        }
    }



}

//this function returns the system number based on the token it is given
int getSystem(char* str){

    if(str == NULL){
        return 1;
    }
    else if(strcmp(str, "+") == 0){
        return 4;
    }
    else if(strcmp(str, "-") == 0){
        return 5;
    }
    else if(strcmp(str, "*") == 0){
        return 6;
    }
    else if(strcmp(str, "/") == 0){
        return 7;
    }
    else if(strcmp(str, "odd") == 0){
        return 8;
    }
    else if(strcmp(str, "=") == 0){
        return 9;
    }
    else if(strcmp(str, "<>") == 0){
        return 10;
    }
    else if(strcmp(str, "<") == 0){
        return 11;
    }
    else if(strcmp(str, "<=") == 0){
        return 12;
    }
    else if(strcmp(str, ">") == 0){
        return 13;
    }
    else if(strcmp(str, ">=") == 0){
        return 14;
    }
    else if(strcmp(str, "(") == 0){
        return 15;
    }
    else if(strcmp(str, ")") == 0){
        return 16;
    }
    else if(strcmp(str, ",") == 0){
        return 17;
    }
    else if(strcmp(str, ";") == 0){
        return 18;
    }
    else if(strcmp(str, ".") == 0){
        return 19;
    }
    else if(strcmp(str, ":=") == 0){
        return 20;
    }
    else if(strcmp(str, "begin") == 0){
        return 21;
    }
    else if(strcmp(str, "end") == 0){
        return 22;
    }
    else if(strcmp(str, "if") == 0){
        return 23;
    }
    else if(strcmp(str, "then") == 0){
        return 24;
    }
    else if(strcmp(str, "while") == 0){
        return 25;
    }
    else if(strcmp(str, "do") == 0){
        return 26;
    }
    else if(strcmp(str, "call") == 0){
        return 27;
    }
    else if(strcmp(str, "const") == 0){
        return 28;
    }
    else if(strcmp(str, "var") == 0){
        return 29;
    }
    else if(strcmp(str, "procedure") == 0){
        return 30;
    }
    else if(strcmp(str, "write") == 0){
        return 31;
    }
    else if(strcmp(str, "odd") == 0){
        return 32;
    }
    else if(strcmp(str, "else") == 0){
        return 33;
    }
    else{
        if(str[0] >= '0' && str[0] <= '9'){
            return 3;
        }
        else{
            return 2;
        }

    }


    return -1;
}

//this is used for the validToken function
//checks if the number is too large
int checkNum(int num){

    if (num >= (2 << 16)){
        return 0;
    }
    return 1;
}

//this function is used for the validToken function
//checks if the identifier is too long
int checkIdenLength(char* str){

    if(strlen(str) > 12){
        return 0;
    }

    return 1;
}

//this function is used for the validToken function
//checks if the identifier is valid
int checkIdenBegin(char* str){

    char begin = str[0];
    if(begin < 65 || begin > 122){
        return 0;
    }

    return 1;
}

int isadigit(char c){

    if(c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9'){
        return 1;
    }
    return 0;
}
// if 1 is returned then it is a valid token
// if 2 is returned the number is too long
// if 3 is returned the identifier is invalid
// if 4 is returned the identifier is too long
int validToken(char* str){

    int i;
    int num = 1;

    //first check if its a number or an identifier
    for(i=0; i<strlen(str); i++){

        if(!isadigit(str[i])){
            num = 0;
            break;
        }
    }

    //if its a number check the length
    if(num == 1){
        int number = atoi(str);
        if(checkNum(number) == 0){
            return 2;
        }
    }
    //if not a number check the identifier for its length and if its a valid identifier
    else if(num == 0){
        if(checkIdenBegin(str) == 0){
            return 3;
        }
        if(checkIdenLength(str) == 0){
            return 4;
        }
    }

    return 1;
}

//main function that handles the args and passes them to stringTokenizer
int main(int argc, char** argv)
{
    int printSource = 0;
    int printClean = 0;

    //following ifs determine if the flags --clean and --source are given arguments
    if(argv[2] != 0){
        if(strcmp(argv[2], "--source") == 0){
            printSource = 1;
        }
        else if(strcmp(argv[2], "--clean") == 0){
            printClean = 1;
        }
    }
    if(argv[3] != 0){
        if(strcmp(argv[3], "--source") == 0){
            printSource = 1;
        }
        else if(strcmp(argv[3], "--clean") == 0){
            printClean = 1;
        }
    }

    stringTokenizer(argv[1], printSource, printClean);

    return 0;
}
