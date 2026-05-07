#include <stdio.h>      
#include "lexer.h"      

int main(int argc, char *argv[]) 
{
    if(argc < 2)        
    {
        printf("Usage : %s <source_code.c>\n", argv[0]); 
        return 1;      
    }

    printf("Lexical Analysis Started...\n\n"); 

    // Initialize lexer and tokenize the source code file
    initializeLexer(argv[1]); 

    // Print table header for token output
    printf("\n%-3s | %-20s : %s\n", "No", "Lexeme", "Token Type");
    printf("------------------------------------------------\n");

    // Reset token index to start reading tokens from beginning
    token_index = 0;     

    // Variable to store each token
    Token token;         
    int count = 1;  
    
    // Loop until all tokens are read   
    while ((token = getNextToken()).type != UNKNOWN)  
    {
        printToken(&token); 
        count++;           
    }
    printf("\nLexical Analysis Completed...\n\n"); 
    return 0; 
}