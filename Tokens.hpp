#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <variant>

//Enumerate class for token types and core data
enum class TokenType
{
    NUMBER,
    VARIABLE,
    FUNCTION,
    ADD,
    SUB,
    MULT,
    DIV,
    NEG,
    POWER,
    OPAR,
    CLPAR,
    END_FILE
};
const char* NUMBERS = "0123456789.";
const char* VARIABLES = "abcdfghjklmnopqrstuvwxyzABCDFGHIKLMNOPQRSTUVWXYZ";

//Will be used later
std::string FUNCTIONS[6] = {"cos", "sin", "tan", "arccos", "arcsin", "arctan"};

//Unordered_map type is a dictionary in c++
std::unordered_map <char, TokenType> operator_map = 
{
    {'+', TokenType::ADD},
    {'-', TokenType::SUB},
    {'*', TokenType::MULT},
    {'/', TokenType::DIV},
    {'^', TokenType::POWER},
    {'(', TokenType::OPAR},
    {')', TokenType::CLPAR}
};



//create the token struct using double or string depending on type
struct Token 
{
    TokenType type;
    std::variant<std::string, double> value;
    Token(TokenType type, std::string value="") : type(type), value(value){};
    Token(TokenType type, double value): type(type), value(value){};
    Token(TokenType type, char value ) : type(type), value(std::string(1,value)){};
};

//function to get type for the token as we are parsing
TokenType get_type(char current_char)
{
    if (std::strchr(NUMBERS, current_char) != nullptr)
    {
        return TokenType::NUMBER;
    }
    if (std::strchr(VARIABLES, current_char) != nullptr)
    {
        return TokenType::VARIABLE;
    }
    if (operator_map.count (current_char) > 0) 
    {
        return operator_map.at(current_char);
    }
    else
    {
        return TokenType::FUNCTION;
    }
};

//generate tokens from each element of the string. Use token-type to determine when to submit
std::vector<Token> parse_equation(std::string equation)
{
    const char* current_char = equation.data();
    const char* first_char = equation.data();
    const char* last_char = equation.data() + equation.size();
    const char* token_start = current_char;
    std::vector <Token> tokens;
    
    while (current_char < last_char)
    {
        TokenType current_type = get_type(*current_char);
        
        std::string current_val;
        if (current_type == TokenType::NUMBER)
        {
            while (current_char+1 < last_char && get_type(*(current_char+1)) == TokenType::NUMBER)
            {
                current_char ++;
            }
            
            current_val = std::string (token_start, current_char+1);
        }
            
        else if (current_type == TokenType::VARIABLE || operator_map.count(*current_char) > 0)
        {
            current_val = *current_char;
        };
        
        if (current_type == TokenType::OPAR && current_char > first_char)
        {
            TokenType prev_type = get_type(*(current_char-1));
            if (prev_type == TokenType::VARIABLE || prev_type == TokenType::NUMBER || prev_type == TokenType::CLPAR)
            {
            tokens.emplace_back(TokenType::MULT, "*");
            };
        };
        
        //emplace sends instance of token to the back of the vecotr
        if (current_type == TokenType::NUMBER)
        {
            double nums = std::stod(current_val);
            tokens.emplace_back(current_type, nums);
        }
        else
        {
            tokens.emplace_back(current_type, current_val);
        }
        
        //handle numvar i.e. 5x
        if ((get_type(*(current_char+1)) == TokenType::VARIABLE) && (current_type == TokenType::NUMBER)) 
        {
                tokens.emplace_back(TokenType::MULT, "*");
        }
        else if ((get_type(*(current_char+1)) == TokenType::NUMBER) && (current_type == TokenType::VARIABLE)) 
        {
                tokens.emplace_back(TokenType::MULT, "*");
        }
        else if (current_type == TokenType::CLPAR && current_char < last_char)
        {
            TokenType next_type = get_type(*(current_char+1));
            if (next_type == TokenType::VARIABLE || next_type == TokenType::NUMBER || next_type == TokenType::OPAR )
            {
                tokens.emplace_back(TokenType::MULT, "*");
            };
        }
        current_char ++;
        token_start = current_char;

    };
    return tokens;
};






