#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

// List of keywords in C
const char *keywords[] = {"int", "return", "if", "else", "for", "while", "do", "switch", "case", "break", "continue", "default", "const", "char", "float", "double", "void"};
int numKeywords = 16;

// Function to check if a string is a keyword
int isKeyword(char *str) {
    for (int i = 0; i < numKeywords; i++) {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

// Function to check if a character is an operator and classify it
void identifyOperator(char c, char nextChar) {
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
        printf("'%c' is an arithmetic operator\n", c);
    } else if (c == '=' && nextChar == '=') {
        printf("'==' is a relational operator\n");
    } else if (c == '=' && nextChar != '=') {
        printf("'=' is an assignment operator\n");
    } else if (c == '+' && nextChar == '+') {
        printf("'++' is an increment operator\n");
    } else if (c == '-' && nextChar == '-') {
        printf("'--' is a decrement operator\n");
    } else if (c == '&' && nextChar == '&') {
        printf("'&&' is a logical AND operator\n");
    } else if (c == '|' && nextChar == '|') {
        printf("'||' is a logical OR operator\n");
    } else if (c == '!' && nextChar == '=') {
        printf("'!=' is a relational operator\n");
    } else if (c == '!') {
        printf("'!' is a logical NOT operator\n");
    }
}

// Function to identify tokens in a given string
void identifyTokens(char *str) {
    int i = 0, j = 0;
    char buffer[MAX];

    while (str[i] != '\0') {
        if (isalpha(str[i])) {
            // Collect identifiers/keywords
            j = 0;
            while (isalnum(str[i])) {
                buffer[j++] = str[i++];
            }
            buffer[j] = '\0';
            if (isKeyword(buffer)) {
                printf("'%s' is a keyword\n", buffer);
            } else {
                printf("'%s' is an identifier\n", buffer);
            }
        } else if (isdigit(str[i])) {
            // Collect numbers
            j = 0;
            while (isdigit(str[i])) {
                buffer[j++] = str[i++];
            }
            buffer[j] = '\0';
            printf("'%s' is a number\n", buffer);
        } else if (ispunct(str[i])) {
            // Handle operators and punctuations
            char nextChar = str[i+1];
            identifyOperator(str[i], nextChar);
            
            // Move to the next character
            if ((str[i] == '+' && nextChar == '+') || (str[i] == '-' && nextChar == '-') || 
                (str[i] == '=' && nextChar == '=') || (str[i] == '&' && nextChar == '&') || 
                (str[i] == '|' && nextChar == '|') || (str[i] == '!' && nextChar == '=')) {
                i++; // Skip next char if it's part of a compound operator
            }
            i++; // Move to the next character
        } else {
            i++; // Move to the next character if it's whitespace or unhandled
        }
    }
}

int main() {
    char str[MAX];
    
    // Example string
    printf("Enter the string: ");
    fgets(str, MAX, stdin);
    
    identifyTokens(str);

    return 0;
}