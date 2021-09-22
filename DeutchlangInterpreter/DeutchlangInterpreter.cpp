// DeutchlangInterpreter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
using namespace std;

enum class TokenType
{
    none,
    eof,
    number,
    addition,
    substraction,
    multiplication,
    division,
    leftParen,
    rightParen
};

string tokenTypesAsStrings[] = { "eof" };

class Token
{
public:
   // using TokenValue = string; 

    TokenType tokenType;
    string tokenValue; //consider variant/ template

    Token(TokenType tokenType, string tokenValue) : tokenType(tokenType), tokenValue(tokenValue)
    {
    }

    string tokenRepresentation()
    {
        return "Token(" + tokenTypeToString(tokenType) + "," + tokenValue + ")";
    }

    string tokenTypeToString(TokenType tokenType)
    {
        return tokenTypesAsStrings[static_cast<int>(tokenType)];
    }
};

class Interpreter
{
public:
    string type;
    string value;
    string text;
    size_t textReadingIndex = 0;
    char currentTextChar = text[textReadingIndex];
    Token currentToken = getNextToken();

    Interpreter(string text) : text(text)
    {
    }

    Token getNextToken()
    {
        if (!isInTextsBounds())
        {
            return Token(TokenType::eof, "");
        }

        skipWhitespaces();

        if (isdigit(currentTextChar))
        {
            return Token(TokenType::number, handleNumberInput());
        }

        switch (currentTextChar)
        {
            case '+':
                advanceTextChar();
                return Token(TokenType::addition, "+");
            case '-':
                advanceTextChar();
                return Token(TokenType::substraction, "-");
            case '*':
                advanceTextChar();
                return Token(TokenType::multiplication, "*");
            case '/':
                advanceTextChar();
                return Token(TokenType::division, "/");
            case '(':
                advanceTextChar();
                return Token(TokenType::leftParen, "(");
            case ')':
                advanceTextChar();
                return Token(TokenType::rightParen, "(");
        }

        cout << "unidentified symbol at line: " << textReadingIndex;
        throw;
    }

    string handleNumberInput()
    {
        string number = string(1, currentTextChar);
        advanceTextChar();

        while (textReadingIndex < text.length() && isdigit(currentTextChar))
        {
            number += currentTextChar;
            advanceTextChar();
        }
        return number;
    }

    void advanceTextChar()
    {
        if (isInTextsBounds())
        {
            textReadingIndex++;
            currentTextChar = text[textReadingIndex];
        }
    }

    void skipWhitespaces()
    {
        while (text[textReadingIndex] == ' ' && isInTextsBounds())
        {
            advanceTextChar();
        }
    }

    void updateCurrentToken()
    {
        currentToken = getNextToken();
    }

    ///<summary>"eat" token and then update</summary>
    void eatAndUpdateCurrentToken(TokenType tokenType)
    {
        validateCurrentToken(tokenType);
        currentToken = getNextToken();
    }

    void validateCurrentToken(TokenType tokenType)
    {
        if (currentToken.tokenType != tokenType)
        {
            cout << "current token is different than given tokenType" << endl;
        }
    }

    int eatAndUpdateNumberToken()
    {
        eatAndUpdateCurrentToken(TokenType::number);
        return stoi(currentToken.tokenValue);
    }

    int exprPrecedenceTwo()
    {
        Token element = currentToken;

        if (element.tokenType == TokenType::number)
        {
            eatAndUpdateCurrentToken(TokenType::number);
            return stoi(element.tokenValue);
        }

        if (element.tokenType == TokenType::leftParen)
        {
            eatAndUpdateCurrentToken(TokenType::leftParen);
            int result = expr();
            eatAndUpdateCurrentToken(TokenType::rightParen);
            return result;
        }
    }

    int exprPrecedenceOne()
    {
        int result = exprPrecedenceTwo();
        
        while (isOperationPrecedenceOne(currentToken.tokenType))
        {
            Token operation = currentToken;

            switch (operation.tokenType)
            {
                case TokenType::multiplication:
                    eatAndUpdateCurrentToken(TokenType::multiplication);
                    result *= exprPrecedenceTwo();
                    break;
                case TokenType::division:
                    eatAndUpdateCurrentToken(TokenType::division);
                    result /= exprPrecedenceTwo();
                    break;
                default:
                    cout << "unknown operation" << endl;
                    throw;
            }
        }

        return result;
    }

    int expr()
    {
        int result = exprPrecedenceOne();

        while (isOperationPrecedenceZero(currentToken.tokenType)) //refactor not to use boilerplate method
        {
            Token operation = currentToken;

            switch (operation.tokenType)
            {
                case TokenType::addition:
                    eatAndUpdateCurrentToken(TokenType::addition);
                    result += exprPrecedenceOne();
                    break;
                case TokenType::substraction:
                    eatAndUpdateCurrentToken(TokenType::substraction);
                    result -= exprPrecedenceOne();
                    break;
                default:
                    cout << "unknown operation" << endl;
                    throw;
            }
        }

        return result;
    }

    bool isOperationPrecedenceOne(TokenType tokenType)
    {
        return             
            tokenType == TokenType::multiplication ||
            tokenType == TokenType::division;
    }

    bool isOperationPrecedenceZero(TokenType tokenType)
    {
        return
            tokenType == TokenType::addition ||
            tokenType == TokenType::substraction;
    }

    bool isInTextsBounds()
    {
        return textReadingIndex < text.length();
    }
};

int main()
{
    while (true)
    {
        string text; getline(cin, text);
        Interpreter interpreter = Interpreter(text);
        cout << interpreter.expr() << endl;
    }
}

/*Token left = currentToken;
updateCurrentToken(TokenType::number);

Token operation = currentToken;
switch (operation.tokenType)
{
    case TokenType::addition:
        updateCurrentToken();
        break;
    case TokenType::substraction:
        updateCurrentToken();
        break;
    case TokenType::multiplication:
        updateCurrentToken();
        break;
    case TokenType::division:
        updateCurrentToken();
        break;
    default:
        cout << "unknown operation" << endl;
        throw;
}

Token right = currentToken;
updateCurrentToken(TokenType::number);

int leftValue = stoi(left.tokenValue);
int rightValue = stoi(right.tokenValue);

switch (operation.tokenType)
{
    case TokenType::addition:
        return leftValue + rightValue;
    case TokenType::substraction:
        return leftValue - rightValue;
    case TokenType::multiplication:
        return leftValue * rightValue;
    case TokenType::division:
        return leftValue / rightValue;
}*/