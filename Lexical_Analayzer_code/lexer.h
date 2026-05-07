#ifndef LEXER_H
#define LEXER_H

#define MAX_KEYWORDS 32        
#define MAX_TOKEN_SIZE 100     
#define MAX_INPUT_SIZE 1024    
#define MAX_TOKENS 1000       

// Enum for token types
typedef enum 
{
    KEYWORD,                 
    OPERATOR,                
    SPECIAL_CHARACTER,      
    CONSTANT,                
    IDENTIFIER,              
    STRING_LITERAL,         
    CHAR_LITERAL,              
    UNKNOWN                  
} TokenType;

// Struct to represent a token
typedef struct 
{
    char lexeme[MAX_TOKEN_SIZE];  
    TokenType type;               
    int line;                     
} Token;


extern int token_count;                        
extern int token_index;           

// Function prototypes

// Initializes lexer by reading file and tokenizing
void initializeLexer(const char* filename);

// Returns the next token sequentially
Token getNextToken();

// Tokenizes a single line of input
void Tokenizer(char *input,int line_number);

// Checks if a string is a keyword
int isKeyword(const char* str);

// Checks if a character is an operator
int isOperator(char ch);

// Checks if a character is a special symbol
int isSpecialCharacter(char ch);

// Checks if a string is a valid identifier
int isIdentifier(const char* str);

// Saves a token into global array and updates counters
void saveToken(const char* lexeme, TokenType type, int line);

// Prints token information
void printToken(Token *token);

#endif  