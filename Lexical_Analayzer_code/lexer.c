#include <stdio.h>      
#include <string.h>     
#include <ctype.h>     
#include "lexer.h"

// Array to store all tokens extracted
Token tokens[MAX_TOKENS]; 

// Counter for total tokens stored
int token_count = 0;       
int token_index = 0;   

int paren_stack[100];
int paren_top = -1;

int brace_stack[100];
int brace_top = -1;


// List of C language keywords
static const char* keywords[MAX_KEYWORDS] = {
    "int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
    "char", "double", "void", "switch", "case", "default", "const", "static", "sizeof", 
    "struct", "auto", "enum", "extern", "goto", "long", "register", "short", "signed",
    "typedef", "union", "unsigned", "volatile"
};

// Single-character operators
static const char* operators = "+-*/%.=!<>|&^~?:";

// Special characters
static const char* special_characters = ",;{}()[]";

// Multi-character operators
static const char* multi_operators[] = {"==", "!=", "<=", ">=", "++", "--", 
    "+=", "-=", "*=", "/=", "%=", "<<", ">>", "&=", "|=", "^=", "->", "&&", "||"};

const int num_multi_operators = sizeof(multi_operators)/sizeof(multi_operators[0]);

// Function: Check if string is a keyword
int isKeyword(const char *str)
{
    for(int i = 0; i < MAX_KEYWORDS; i++)
    {
        // Compare string with keyword
        if(strcmp(str, keywords[i]) == 0) 
            return 1; 
    }
    return 0; 
}

// Function: Check if a character is an operator
int isOperator(char ch)
{
    for(int i = 0; operators[i] != '\0'; i++)
    {
        // Compare with each operator
        if(ch == operators[i]) 
            return 1; 
    }
    return 0; 
}

// Function: Check if a character is a special symbol
int isSpecialCharacter(char ch)
{
    for(int i = 0; special_characters[i] != '\0'; i++)
    {
        if(ch == special_characters[i])
            return 1; 
    }
    return 0;
} 

// Function: Check if string is a valid identifier
int isIdentifier(const char *str)
{
    // First char must be letter or underscore
    if(!isalpha(str[0]) || str[0] != '_') 
        return 0;

    for(int i = 1; str[i] != '\0'; i++)
    {
        // Only letters, digits, underscore
        if(!isalnum(str[i]) && str[i] != '_') 
            return 0;
    }
    return 1;
}


/* Function: Initialize lexer by reading file and tokenizing each line
Working:
- Opens the specified source code file (e.g., source_code.c) for reading.
- Reads each line using fgets() and passes it to Tokenizer() along with the line number.
- Closes the file after processing all lines and prepares tokens for later retrieval by getNextToken().
*/
void initializeLexer(const char* filename)
{
    /* stores each line of code */
    char input[MAX_INPUT_SIZE];
    int line_number = 1;

    // Open source code file
    FILE *fp = fopen(filename, "r"); 
    if(fp == NULL)
    {
        printf("ERROR : Unable to open a file %s\n", filename);
        return;
    }
    printf("%s opened successfully.\n\n", filename);

    while(fgets(input, sizeof(input), fp) != NULL)
    {
        // send line to tokenizer
        Tokenizer(input, line_number); 
        line_number++;
    }
    

    /* check for unmatched parenthesis */
    for(int i = 0; i <= paren_top; i++)
    {
        printf("\n %d | ( : Error(Missing closing parenthesis ')')\n", paren_stack[i]);
    }
    for(int i = 0; i <= brace_top; i++)
    {
        printf("\n %d | { : Error(Missing closing brace '}')\n", brace_stack[i]);
    }
    fclose(fp);
}


/* Function: Tokenizer - splits input line into tokens
Working:
- Scans the input line character-by-character using a loop.
- Identifies token types — keywords, identifiers, constants, operators, special symbols, strings, and characters.
- Saves each recognized token using saveToken() and skips whitespace/comments.
*/

void Tokenizer(char *input, int line_number)
{
    int i = 0;
    char buffer[MAX_TOKEN_SIZE]; 
    int index = 0;
    static int multiline_comment = 0;

    while(input[i] != '\0')
    {
        // Ignore whitespace
        if(isspace(input[i])) 
        {
            i++;
            continue;
        }

        // Handle multiline comments
        else if (multiline_comment || (input[i] == '/' && input[i + 1] == '*'))
        {
            if(input[i] == '/' && input[i + 1] == '*')
            {
                // Enter comment mode
                multiline_comment = 1; 
                i += 2;  // Skip '/*'
            }
            while (input[i] != '\0' && !(input[i] == '*' && input[i + 1] == '/'))
                i++; // Skip until '*/'
            if (input[i] != '\0')
            {
                multiline_comment = 0; 
                i += 2; // Skip closing '*/'
                // Exit comment mode
            }
            continue;
        }

        // Skip single-line comments
        else if(input[i] == '/' && input[i + 1] == '/')
            break;
        
        // Character literal
        else if(input[i] == '\'')
        {
            i++;
            index = 0;

            while(input[i] != '\0' && input[i] != '\'' && input[i] != '\n')
            {
                if(index < MAX_TOKEN_SIZE - 1)
                    buffer[index++] = input[i++];
            }
            if(input[i] == '\'')
            {
                i++;
                buffer[index] = '\0';
                saveToken(buffer, CHAR_LITERAL, line_number);
            }
            else    
            {
                printf("%3d | Error(Unterminated char literal)\n", line_number);
            }
            continue;
        }

        // Multi-character operators
        else
        {
            int flag = 0;
            for(int j = 0; j < num_multi_operators; j++)
            {
                if(input[i] == multi_operators[j][0] && input[i + 1] == multi_operators[j][1])
                {
                    saveToken(multi_operators[j], OPERATOR, line_number);
                    i = i + 2;
                    flag = 1;
                    break;
                }
            }
            if(flag)
                continue;

            // Single-character operators
            else if(isOperator(input[i]))
            {
                buffer[0] = input[i];
                buffer[1] = '\0';
                saveToken(buffer, OPERATOR, line_number); 
                i++;
                continue;
            }
            
            // Special characters
            else if(isSpecialCharacter(input[i]))
            {
                buffer[0] = input[i];
                buffer[1] = '\0';
                saveToken(buffer, SPECIAL_CHARACTER, line_number); 

                if(input[i] == '(')
                {
                    paren_stack[++paren_top] = line_number;
                }
                else if(input[i] == ')')
                {
                    if(paren_top >= 0)
                        paren_top--;
                    else   
                        printf("%3d | ) : Error(Unmatched Parenthesis)\n", line_number);
                }
                else if(input[i] == '{')
                {
                    brace_stack[++brace_top] = line_number;
                }
                else if(input[i] == '}')
                {
                    if(brace_top >= 0)
                        brace_top--;
                    else 
                        printf("%3d | } : Error(Unmatched Brace)\n", line_number);
                }
                i++;
                continue;
            }

            // Constants (numbers)
            else if(isdigit(input[i]))
            {   
                index = 0;

                /* hexadecimal */
                if(input[i] == '0' && (input[i+1] == 'x' || input[i+1] == 'X'))
                {
                    buffer[index++] = input[i++];
                    buffer[index++] = input[i++];

                    while(isxdigit(input[i]))
                    {
                        if(index < MAX_TOKEN_SIZE-1)
                            buffer[index++] = input[i++];
                    }
                    buffer[index] = '\0';
                    saveToken(buffer, CONSTANT, line_number);
                }
                /* decimal and float */
                else
                {
                    while(isdigit(input[i]) || input[i] == '.')
                    {
                        if(index < MAX_TOKEN_SIZE-1)
                            buffer[index++] = input[i++];
                    }
                    buffer[index] = '\0';
                    saveToken(buffer, CONSTANT, line_number);
                }
                continue;
            }

            // Keywords or identifiers
            else if(isalpha(input[i]) || input[i] == '_')
            {
                index = 0;
                while(isalnum(input[i]) || input[i] == '_')
                {
                    if(index < MAX_TOKEN_SIZE-1)
                        buffer[index++] = input[i++];
                }
                buffer[index] = '\0';

                if(isKeyword(buffer))
                    saveToken(buffer, KEYWORD, line_number);

                else if(isIdentifier(buffer))
                    saveToken(buffer, IDENTIFIER, line_number);
                continue;
            }

            // String literal
            else if(input[i] == '"')
            {
                i++;
                index = 0;

                while(input[i] != '"' && input[i] != '\0' && input[i] != '\n')
                {
                    if(index < MAX_TOKEN_SIZE-1)
                        buffer[index++] = input[i++];
                }

                if(input[i] == '"')
                {
                    buffer[index] = '\0';
                    saveToken(buffer, STRING_LITERAL, line_number);
                    i++;
                }
                else
                {
                    buffer[index] = '\0';
                    printf("%3d | %-10s : Error(Unterminated String)\n", line_number, buffer);
                }
                continue;
            }

            // Invalid characters
            else
            {
                printf("%3d | %c : Error(Invalid Character)\n", line_number, input[i]);
                i++;
            }
        }
    }
}


/* Function: Return next token sequentially
Working: 
- Uses a global index (token_index) to track the current
 token in the token array.
- Each time it’s called, it returns the next token and increments the index.
- When all tokens are returned, it sets the type to UNKNOWN to signal completion.
*/
Token getNextToken()
{
    if(token_index >= token_count)
    {
        Token token;
        strcpy(token.lexeme, "");
        token.type = UNKNOWN;
        return token;
    }
    else
        return tokens[token_index++]; 
}


/* Function: Print token details
Working: 
- Prints the token number, lexeme (actual word), and token type in tabular format.
- Helps visualize the token stream during lexical analysis.
*/
void printToken(Token *token)
{
    const char* typeStr;
    switch(token->type)
    {
        case KEYWORD: 
            typeStr = "Keyword"; 
            break;
        case IDENTIFIER: 
            typeStr = "Identifier"; 
            break;
        case CONSTANT: 
            typeStr = "Constant"; 
            break;
        case OPERATOR: 
            typeStr = "Operator"; 
            break;
        case SPECIAL_CHARACTER: 
            typeStr = "Special Character"; 
            break;
        case STRING_LITERAL: 
            typeStr = "String Literal"; 
            break;       
        case CHAR_LITERAL: 
            typeStr = "Character Literal"; 
            break; 
        default: 
            typeStr = "Unknown"; 
            break;
    }

    printf("%3d | %-20s : %s\n", token->line, token->lexeme, typeStr);
}


/* Function: Save token to token array and update counters
Working: 
- Copies the token string (lexeme) and its type into a global array (e.g., tokens[]).
- Records the line number where the token was found.
*/
void saveToken(const char* lexeme, TokenType type, int line)
{
    if(token_count >= MAX_TOKENS) 
        return; 
    strncpy(tokens[token_count].lexeme, lexeme, MAX_TOKEN_SIZE - 1); 
    tokens[token_count].type = type;           
    tokens[token_count].line = line;           
    token_count++;
}


