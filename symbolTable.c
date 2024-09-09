#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char keywords[32][15] = {
    "auto", "break", "case", "char", "const", "continue", "default",
    "do", "double", "else", "enum", "extern", "float", "for", "goto",
    "if", "int", "long", "register", "return", "short", "signed",
    "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while"};

char mapper[9][20] = {"NIL", "Keyword", "Identifier", "Constant", "Operator", "Special character", "Space", "Semi-colon", "Bracket"};
// Certain definitions

int currentState = -1;

int len(char *str)
{
    int i = 0;
    for (; str[i]; i++)
        ;
    return i;
}

int isAlpha(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
    {
        return 1;
    }
    return 0;
}

int isDigit(char c)
{
    if (c >= '0' && c <= '9')
    {
        return 1;
    }
    return 0;
}

int isOp(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=' || c == '<' || c == '>')
    {
        return 1;
    }
    return 0;
}

int isTrio(char c)
{
    if (c == '+' || c == '-' || c == '=')
    {
        return 1;
    }
    return 0;
}

int isKeyword(char *inputString, int start, int end)
{
    char buffer[100];
    int j = 0;
    for (int i = start; i <= end; i++)
    {
        buffer[j++] = inputString[i];
    }

    buffer[j] = '\0';
    // printf("Current buffer element : %s\n", buffer);
    for (int i = 0; i < 32; i++)
    {
        if (strcmp(keywords[i], buffer) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int isIdentifier(char *inputString, int start, int end)
{
    char buffer[100];
    int j = 0;
    for (int i = start; i <= end; i++)
    {
        buffer[j++] = inputString[i];
    }
    buffer[j] = '\0';

    if (!isAlpha(buffer[0]))
    {
        return 0;
    }
    for (int i = 1; i < len(buffer); i++)
    {
        if (!isAlpha(buffer[i]) && !isDigit(buffer[i]))
        {
            return 0;
        }
    }
    return 1;
}
int isConstant(char *inputString, int start, int end)
{
    char buffer[100];
    int j = 0;
    for (int i = start; i <= end; i++)
    {
        buffer[j++] = inputString[i];
    }
    buffer[j] = '\0';
    // printf("Current buffer element : %s\n", buffer);
    for (int i = 0; i < len(buffer); i++)
    {
        if (!isDigit(buffer[i]))
        {
            return 0;
        }
    }
    return 1;
}

int isBracket(char *inputString, int start, int end)
{
    if (start == end)
    {
        if (inputString[start] == '(' || inputString[start] == ')')
        {
            return 1;
        }
    }
    return 0;
}
int isOperator(char *inputString, int start, int end)
{
    if (start == end)
    {
        if (isOp(inputString[start]))
        {
            return 1;
        }
        return 0;
    }
    else if (start + 1 == end)
    {
        if (isOp(inputString[start]) && inputString[start] == inputString[end])
        {
            return 1;
        }
        else if (isOp(inputString[start]) && inputString[end] == '=')
        {
            return 1;
        }
        return 0;
    }
    return 0;
}

int returnState(char *inputString, int start, int end)
{
    if (inputString[end] == ' ')
    {

        return 6;
    }
    else if (isKeyword(inputString, start, end))
    {
        return 1;
    }
    else if (isIdentifier(inputString, start, end))
    {
        return 2;
    }
    else if (isConstant(inputString, start, end))
    {
        return 3;
    }
    else if (isOperator(inputString, start, end))
    {
        return 4;
    }
    else if (start == end && inputString[start] == ';')
    {
        return 7;
    }
    else if (isBracket(inputString, start, end))
    {
        return 8;
    }
    else
    {
        return 5;
    }
}

int stateDefn(char *inputString, int start, int end)
{
    int activeState = returnState(inputString, start, end);
    // printf("active State : %d\n", activeState);
    if (activeState != currentState)
    {
        return 0;
    }
    return 1;
}

char *getSubString(char *inputString, int start, int end, char *buffer)
{

    int j = 0;
    for (int i = start; i <= end; i++)
    {
        buffer[j++] = inputString[i];
    }
    return buffer;
}
void clean(char *buffer)
{
    for (int i = 0; buffer[i]; i++)
    {
        buffer[i] = '\0';
    }
}

void display(char *symbol, int type, char *description)
{
    printf("| %-15s | %-10d | %-20s |\n", symbol, type, description);
}

void newLine()
{
    char buffer[100];
    int i;
    for (i = 0; i < 55; i++)
    {
        buffer[i] = '-';
    }
    buffer[i] = '\0';
    printf("%s\n", buffer);
}
void parseFunc(char *inputString)
{
    int start = 0, end = 0;
    char buffer[100];
    newLine();
    char symbol[10] = "Symbol";
    char type[10] = "Type";
    char description[15] = "Description";
    printf("| %-15s | %-10s | %-20s |\n", symbol, type, description);
    newLine();
    while (end < len(inputString))
    {
        clean(buffer);
        // printf("%s\n", buffer);
        if (currentState == -1)
        {
            currentState = returnState(inputString, start, end);
            end++;
            continue;
        }

        else if (!stateDefn(inputString, start, end) && returnState(inputString, start, end) == 1)
        {
            getSubString(inputString, start, end, buffer);
            int activeState = returnState(inputString, start, end);

            // printf("The section : %s is %s \n", buffer, mapper[activeState]);

            display(buffer, activeState, mapper[activeState]);
            newLine();
            end++;
            start = end;
            currentState = -1;
        }
        else if (!stateDefn(inputString, start, end))
        {
            getSubString(inputString, start, end - 1, buffer);
            if (!(currentState == 6 || currentState == 5))
            {
                // printf("The section : %s is %s \n", buffer, mapper[currentState]);
                display(buffer, currentState, mapper[currentState]);
                newLine();
            }
            start = end;
            currentState = -1;
        }
        else
        {
            currentState = returnState(inputString, start, end);
            end++;
        }
    }
    if (start < end)
    {
        getSubString(inputString, start, end, buffer);
        display(buffer, currentState, mapper[currentState]);
        newLine();
        currentState = -1;
    }
}

void trimNewline(char *str)
{
    int len = strlen(str);

    // Check for trailing newline characters and remove them
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r'))
    {
        str[len - 1] = '\0';
        len--;
    }
}

int main()
{
    char inputString[100];
    int flag = 1, i = 1;
    do
    {
        printf("Enter an input string to parse through : ");
        if (i != 1)
        {
            getchar();
        }
        fgets(inputString, sizeof(inputString), stdin);
        trimNewline(inputString);
        printf("%s\n", inputString);
        parseFunc(inputString);
        printf("Enter 1 to continue 0 to stop : ");
        // getchar();
        scanf("%d", &flag);
        i++;
    } while (flag);
}