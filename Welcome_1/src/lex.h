#ifndef LEX_H
#define LEX_H

#define MAX_NUM_LINES       500
#define MAX_CHARS_PER_LINE  500
#define MAX_NUM_WORDS       1000
#define NUM_SEP_TOKENS      16
#define MAX_IDENT_SIZE      12
#define MAX_NUM_SIZE        65535
#define MAX_RESERVED_SIZE   9
#define NUM_RESERVED_WORDS  14


typedef enum{	nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5, 
		multsym = 6, slashsym = 7, oddsym = 8,eqlsym = 9, neqsym = 10, lessym = 11, 
		leqsym = 12, gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, 
		commasym = 17, semicolonsym = 18, periodsym = 19, becomessym = 20, 
		beginsym = 21, endsym = 22, ifsym = 23, thensym = 24, whilesym = 25, 
		dosym = 26, callsym = 27, constsym = 28, varsym = 29, procsym = 30, 
		writesym = 31, readsym = 32, elsesym = 33} Type;              
                
typedef enum{   FALSE = 0, TRUE = 1} Boolean;

typedef enum{   idstarterr = 1, idlengtherr = 2, invalidtokenerr = 3, 
                overflowerr = 4, wrongeqerr = 5, eqlbynumerr = 6, 
                idfolleqlerr = 7, varidenterr = 8, misssemierr = 9, 
                procedecerr = 10, expstatementerr = 11, incorrsymaftstaterr = 12,
                perexperr = 13, semibetstatementserr = 14, undecidenterr = 15,
                assignconstprocerr = 16, assignoperexperr = 17, idaftercallerr = 18,
                cantcallconstvarerr = 19, thenexperr = 20, semibrackerr = 21, 
                doexperr = 22, incsymaftstateerr = 23, exprelaterr = 24,
                missingrparenterr = 25, expcantbeginwitherr = 26} ErrorType;

typedef struct{
    union {
        char s[MAX_IDENT_SIZE];
        int d;
    } val;
    int line;
    int col;
    Type sym;
} Word;

typedef struct{
    char pval;
    char sval;
    Type sym;
} SepToken;

typedef struct{
    char s[MAX_RESERVED_SIZE];
    Type sym;
} ReservedWord;

struct Error{
    Word* error;
    ErrorType type;
    struct Error* next;
};

extern struct Error* front;
extern struct Error* rear;
extern Word code[MAX_NUM_WORDS];
extern int wordCount;

void lex(int argc, char* argv[]);

void testSepToks(SepToken separators[NUM_SEP_TOKENS]);
void testReadInCode(char line[MAX_NUM_LINES][MAX_CHARS_PER_LINE], int numLines);
void testSeparate();

Boolean readInToks(SepToken separators[NUM_SEP_TOKENS]);
int readInCode(char line[MAX_NUM_LINES][MAX_CHARS_PER_LINE], FILE *fp);
void uncommentCode(char line[MAX_NUM_LINES][MAX_CHARS_PER_LINE], char rawLine[MAX_NUM_LINES][MAX_CHARS_PER_LINE], int numLines);
void separateCode(SepToken separators[NUM_SEP_TOKENS], char line[MAX_NUM_LINES][MAX_CHARS_PER_LINE], int numLines);

Boolean loadReservedWords(ReservedWord reserved[NUM_RESERVED_WORDS]);
Boolean loadInvalidTokens(int* tokens);
void resetBuffer(int* bufCount, char* buffer);
Boolean eatWord(int bufCount, char buffer[MAX_IDENT_SIZE],
        ReservedWord reserved[NUM_RESERVED_WORDS], int* tokens);
int isValidNum(char word[MAX_IDENT_SIZE], int bufCount, ErrorType* e);
int isValidIdent(char word[MAX_IDENT_SIZE], int* tokens, int bufCount);
int isReserved(char word[MAX_IDENT_SIZE], ReservedWord reserved[NUM_RESERVED_WORDS]);
int isSepToken(SepToken separators[NUM_SEP_TOKENS], char p, char s, int* sym);

void testWord(Word word);

void EnqueueError(Word* w, ErrorType e);
void PrintErrors(char* fileName);
#endif	// LEX_H

