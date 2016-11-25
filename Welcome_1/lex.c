#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"

struct Error* front = NULL;
struct Error* rear = NULL;
Word code[MAX_NUM_WORDS];
int wordCount = 0;

void lex(int argc, char* argv[]){
    SepToken separators[NUM_SEP_TOKENS];
    char line[MAX_NUM_LINES][MAX_CHARS_PER_LINE];
    char rawLine[MAX_NUM_LINES][MAX_CHARS_PER_LINE];
    FILE *fp;
    int numLines = 0;
    
    if((fp = fopen(argv[1], "r")) == NULL){
        printf("%s cannot be found\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    
    numLines = readInCode(line, fp);
    fclose(fp);
    if(readInToks(separators) == 0){
        return;
    }    
    uncommentCode(line, rawLine, numLines);

    if (argc == 3){
        if (strcmp(argv[2], "--clean") == 0){
            printf("\nsource code without comments:\n");
            printf("-----------------------------\n");
            testReadInCode(rawLine, numLines);
        }
        else if (strcmp(argv[2], "--source") == 0){
            printf("\nsource code:\n");
            printf("------------\n");
            testReadInCode(line, numLines);
        }
    }
    
    if (argc == 4){
        if (strcmp(argv[2], "--clean") == 0){
            printf("\nsource code without comments:\n");
            printf("-----------------------------\n");
            testReadInCode(rawLine, numLines);
        }
        else if (strcmp(argv[2], "--source") == 0){
            printf("\nsource code:\n");
            printf("------------\n");
            testReadInCode(line, numLines);
        }
        if (strcmp(argv[3], "--clean") == 0){
            printf("\nsource code without comments:\n");
            printf("-----------------------------\n");
            testReadInCode(rawLine, numLines);
        }
        else if (strcmp(argv[3], "--source") == 0){
            printf("\nsource code:\n");
            printf("------------\n");
            testReadInCode(line, numLines);
        }
    }
    PrintErrors(argv[1]);
    separateCode(separators, rawLine, numLines);
}

//Reads in separator tokens from tokentypes.txt file and assigns their
//appropriate symbol values.

Boolean readInToks(SepToken separators[NUM_SEP_TOKENS]){
    char buff[MAX_CHARS_PER_LINE];
    FILE* fp;
    int i = 0;
    
    if((fp = fopen("tokentypes.txt", "r")) == NULL){
        printf("Error!  Could not find tokentypes.txt in this directory\n");
        return FALSE;
    }
    
    strcpy(buff, "");
    while (fgets(buff, sizeof(buff), fp) != NULL){
        int j = 1;
        separators[i].pval = buff[0];
        if (buff[1] != '~'){
            separators[i].sval = buff[j];
            j++;
        }
        else{
            separators[i].sval = '\0';
        }
        
        j++;
        
        int num = 0;
        for(; buff[j]!='\0' && buff[j]!=13; j++){
            num = num * 10 + buff[j] - 48;
        }
        
        separators[i].sym = num;
        i++;
        strcpy(buff, "");
    }
    
    fclose(fp);
    return TRUE;
}

//Test for readInToks

void testSepToks(SepToken separators[NUM_SEP_TOKENS]){
    for (int i = 0; i < NUM_SEP_TOKENS; i++){
        if (separators[i].sval != '\0'){
            printf("%c%c = %d\n", separators[i].pval, separators[i].sval, separators[i].sym);
        }
        else{
            printf(" %c = %d\n", separators[i].pval, separators[i].sym);
        }
    }
}

//Reads code from argument file into 'line' returns the number of lines of code

int readInCode(char line[MAX_NUM_LINES][MAX_CHARS_PER_LINE], FILE *fp){
    int i = 0;
    while(fgets(line[i], sizeof(line[i]), fp) != NULL){
        i++;
    }
    
    return i + 1;
}

//Removes comments from each 'line'

void uncommentCode(char line[MAX_NUM_LINES][MAX_CHARS_PER_LINE], char rawLine[MAX_NUM_LINES][MAX_CHARS_PER_LINE], int numLines){
    Boolean isInComment = FALSE;

    for (int i = 0; i < numLines; i++){
        int j = 0;
        while (line[i][j] != '\0'){
            if (isInComment){
                if (line[i][j] == '*' && line[i][j+1] == '/'){
                    rawLine[i][j] = ' ';
                    j++;
                    rawLine[i][j] = ' ';
                    isInComment = FALSE;
                }
                else{
                    rawLine[i][j] = ' ';
                    if (line[i][j] == '\n')
                        rawLine[i][j] = '\n';
                }
            }
            else{
                if (line[i][j] == '/' && line[i][j+1] == '*'){
                    rawLine[i][j] = ' ';
                    j++;
                    rawLine[i][j] = ' ';
                    isInComment = TRUE;
                }
                else {
                    rawLine[i][j] = line[i][j];
                }
            }
            j++;
        }
    }
}

//Test for readInCode and uncommentCode

void testReadInCode(char line[MAX_NUM_LINES][MAX_CHARS_PER_LINE], int numLines){
    for(int i = 0; i < numLines; i++){
        printf("%s", line[i]);
    }
    printf("\n");
}

//Separates the code into separate words and stores separator tokens into 'code'
//also determines if an identifier is too long

void separateCode(SepToken separators[NUM_SEP_TOKENS], char line[MAX_NUM_LINES][MAX_CHARS_PER_LINE], int numLines){
    ReservedWord reserved[NUM_RESERVED_WORDS];
    int tokens[128];
    char buffer[MAX_IDENT_SIZE];
    int bufCount = 0;
    int dummy = 0;
    int sym = 0;
    int numChars = 0;
    int i;
    
    loadReservedWords(reserved);
    loadInvalidTokens(tokens);
    resetBuffer(&bufCount, buffer);
    
    for (i = 0; i < numLines; i++){
        numChars = 0;
        Boolean report = FALSE;
        
        for (int j = 0; j < strlen(line[i]); j++){
            if (line[i][j] == ' ' || line[i][j] == '\t' || line[i][j] == '\n' || line[i][j] == '\r'){
                if (line[i][j] == '\r'){
                    continue;
                }

                if(eatWord(bufCount, buffer, reserved, tokens)){
                    code[wordCount].line = i + 1;
                    code[wordCount].col = numChars;
                    numChars++;
                    wordCount++;
                    report = FALSE;
                }                
                resetBuffer(&bufCount, buffer);
            }
            else if ((dummy = isSepToken(separators, line[i][j], line[i][j+1], &sym)) != 0){                                
                if(eatWord(bufCount, buffer, reserved, tokens)){
                    code[wordCount].line = i + 1;
                    code[wordCount].col = numChars;
                    numChars++;
                    wordCount++;
                    report = FALSE;
                }
                
                code[wordCount].val.s[0] = line[i][j];
                code[wordCount].sym = sym;
                if (dummy == 2){
                    code[wordCount].val.s[1] = line[i][j+1];
                    j++;
                }

                code[wordCount].line = i + 1;
                code[wordCount].col = numChars;
                numChars++;
                resetBuffer(&bufCount, buffer);
                wordCount++;
                report = FALSE;
            }
            else{
                if (bufCount == MAX_IDENT_SIZE){
                    if (report == FALSE){
                        EnqueueError(&code[wordCount], idlengtherr);
                        report = TRUE;
                    }
                    continue;
                }
                buffer[bufCount] = line[i][j];
                bufCount++;
                numChars++;
                code[wordCount].sym = 0;
            }
        }
    }
    
    if (buffer[0] != '\0'){
        if(eatWord(bufCount, buffer, reserved, tokens)){
            code[wordCount].line = i + 1;
            code[wordCount].col = numChars;
            numChars++;
            wordCount++;
        }
    }
}

//loads the reserved words into struct 'reserved'

Boolean loadReservedWords(ReservedWord reserved[NUM_RESERVED_WORDS]){
    FILE* fp;
    int num;
    if ((fp = fopen("reservedWords.txt", "r")) == NULL){
        printf("Could not find reservedWords.txt\n");
        return FALSE;
    }
    for (int i = 0; i < NUM_RESERVED_WORDS; i++){
        fscanf(fp, "%s %d", reserved[i].s, &num);
        reserved[i].sym = num;
    }
    
    return TRUE;
}

//loads invalid tokens into struct 'tokens'

Boolean loadInvalidTokens(int* tokens){
    FILE *fp;
    char ch;
    int value;

    for (int i = 0; i < 128; i++){
        tokens[i] = 0;
    }

    fp = fopen("tokens.txt", "r");
    if (fp == NULL) {
        printf("Can't open tokens.txt\n");
        return FALSE;
    }

    while((ch = fgetc(fp)) != EOF) {
        value = fgetc(fp) - 48;
        tokens[ch] = value;
    }

    fclose(fp);
    return TRUE;
}

//resets the 'buffer'

void resetBuffer(int* bufCount, char* buffer){
    memset(buffer, 0, sizeof(buffer));
    *bufCount = 0;
}

//takes a word that could be an identifier, number, or reserved word and
//adds it to code with its proper type

Boolean eatWord(int bufCount, char buffer[MAX_IDENT_SIZE], 
        ReservedWord reserved[NUM_RESERVED_WORDS], int* tokens){
    int dummy;
    ErrorType e;
    
    if (bufCount > 0){
        if (buffer[0] > 47 && buffer[0] < 58){
            
            if ((dummy = isValidNum(buffer, bufCount, &e)) != -1){
                code[wordCount].val.d = dummy;
                code[wordCount].sym = numbersym;
            }
            else{
                strncpy(code[wordCount].val.s, buffer, bufCount);
                code[wordCount].sym = 0;
                EnqueueError(&code[wordCount], e);
            }
        }
        else{
            strncpy(code[wordCount].val.s, buffer, bufCount);
            if ((dummy = isReserved(buffer, reserved)) != 0){
                code[wordCount].sym = dummy;
            }
            else if(isValidIdent(buffer, tokens, bufCount)){
                code[wordCount].sym = identsym;
            }
            else {
                EnqueueError(&code[wordCount], invalidtokenerr);
            }
        }
        return TRUE;
    }
    return FALSE;
}

//tests whether 'word' is a valid number returns '0' and an error type if it is
//not

int isValidNum(char word[MAX_IDENT_SIZE], int bufCount, ErrorType* e){
    int num = 0;

    for (int i = 0; i < bufCount; i++){
        if (word[i] >= 58 || word[i] <= 47){
            *e = idstarterr;
            return 0;
        }
        num = num * 10 + word[i] - 48;
    }
    
    if (num > MAX_NUM_SIZE){
        *e = overflowerr;
        return -1;
    }
    return num;
}

//tests whether 'word' is a valid identifier

int isValidIdent(char word[MAX_IDENT_SIZE], int* tokens, int bufCount){
    for (int i = 0; i < bufCount; i++){
        if (tokens[word[i]] != 0){
            return 0;
        }        
    }
    return 1;
}

//tests whether 'word' is considered a reserved word and returns the appropriate
//sym if it is.  If it is not returns 0.

int isReserved(char word[MAX_IDENT_SIZE], ReservedWord reserved[NUM_RESERVED_WORDS]){
    for (int i = 0; i < NUM_RESERVED_WORDS; i++){
        if (strncmp(word, reserved[i].s, strlen(reserved[i].s)) == 0){
            return reserved[i].sym;
        }
    }
    return 0;
}

//tests whether the passed in chars form a separator combination and returns how
//many tokens are in the separator. Also edits the 'sym' of the separator byref.
//Returns 0 if the chars don't form a separator.

int isSepToken(SepToken separators[NUM_SEP_TOKENS], char p, char s, int* sym){
    for (int i = 0; i < NUM_SEP_TOKENS; i++){
        if (p == separators[i].pval && s == separators[i].sval){
            *sym = separators[i].sym;
            return 2;
        }        
    }
    for (int i = 0; i < NUM_SEP_TOKENS; i++){
        if (p == separators[i].pval && separators[i].sval == '\0'){
            *sym = separators[i].sym;
            return 1;
        }
    }
    return 0;
}

//Prints out the classification of the words in the appropriate format

void testSeparate(){
    printf("\ntokens:\n");
    printf("-------\n");
    
    for (int i = 0; i < wordCount; i++){
        if (code[i].sym != numbersym){
            printf("%-12.12s %d\n", code[i].val.s, code[i].sym);
        }
        else{
            printf("%-12d %d\n", code[i].val.d, code[i].sym);
        }
    }
}

//Used to test the state of the lexer throughout coding, not used in final.

void testWord(Word word){
    if (word.sym != numbersym){
        printf("%-12.12s%d\n", word.val.s, word.sym);
    }
    else{
        printf("%-12d%d\n", word.val.d, word.sym);
    }
}

//Enqueues an error

void EnqueueError(Word* w, ErrorType e){
    struct Error* temp = (struct Error*)malloc(sizeof(struct Error));
    temp->error = w;
    temp->type = e;
    temp->next = NULL;
    if(front == NULL && rear == NULL){
        front = temp;
        rear = temp;
        return;
    }
    rear->next = temp;
    rear = temp;
}

//Prints all enqueued errors

void PrintErrors(char* fileName){
    struct Error* temp = front;
    while (temp != NULL){
        printf("%s:%d:%d: ", fileName, temp->error->line, temp->error->col);
        switch (temp->type){
            case idstarterr:
                printf("Identifier does not start with a letter.\n");
                break;
            case idlengtherr:
                printf("Identifier is too long\n");
                break;
            case invalidtokenerr:
                printf("Invalid token in identifier\n");
                break;
            case overflowerr:
                printf("Number is greater than 2^16-1.\n");
                break;
            case wrongeqerr:
                printf("Use = instead of :=.\n");
                break;
            case eqlbynumerr:
                printf("= must be followed by a number.\n");
                break;
            case idfolleqlerr:
                printf("Identifier must be followed by =.\n");
                break;
            case varidenterr:
                printf("const, var, procedure must be followed by identifier.\n");
                break;
            case misssemierr:
                printf("Semicolon or comma missing.\n");
                break;
            case procedecerr:
                printf("Incorrect symbol after procedure declaration.\n");
                break;
            case expstatementerr:
                printf("Statement expected.\n");
                break;
            case incorrsymaftstaterr:
                printf("Incorrect symbol after statement part in block.\n");
                break;
            case perexperr:
                printf("Period expected.\n");
                break;
            case semibetstatementserr:
                printf("Semicolon between statements missing.\n");
                break;
            case undecidenterr:
                printf("Undeclared identifier.\n");
                break;
            case assignconstprocerr:
                printf("Assignment to constant or procedure is not allowed.\n");
                break;
            case assignoperexperr:
                printf("Assignment operator expected.\n");
                break;
            case idaftercallerr:
                printf("Missing identifier.\n");
                break;
            case cantcallconstvarerr:
                printf("Call of a constant or variable is meaningless.\n");
                break;
            case thenexperr:
                printf("then expected.\n");
                break;
            case semibrackerr:
                printf("Semicolon or } expected.\n");
                break;
            case doexperr:
                printf("do expected.\n");
                break;
            case incsymaftstateerr:
                printf("Incorrect symbol following statement.\n");
                break;
            case exprelaterr:
                printf("Relational operator expected.\n");
                break;
            case missingrparenterr:
                printf("Missing right parenthesis.\n");
                break;
            case expcantbeginwitherr:
                printf("An expression cannot begin with this symbol.\n");
                break;
            case idaftercallprocerr:
                printf("call must be followed by a procedure identifier.\n");
                break;
        }
        temp = temp->next;
    }
}