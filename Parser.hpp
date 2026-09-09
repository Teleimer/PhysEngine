#pragma once
#include <iostream>
#include <variant>
#include "Tokens.hpp"
#include "ASTNode.hpp"


//execute with recursive descent through the tokens vecotr
class parser
{
    public:
        const std::vector<Token>& tokens_;
        //constructor
        parser(const std::vector<Token>& tokens): tokens_(tokens)
        {
            head_node_ = parse_tokens();
        };
        const ASTNode* get_head_node() const
        {
            return head_node_.get();
        };

        size_t get_nodes_length() const
        {
            return nodes_length_;
        };

    private:
    size_t current_ = 0;
    std::unique_ptr<ASTNode> head_node_ = nullptr;
    size_t nodes_length_ = 0;

    bool const is_end()
    {
        if (tokens_[current_].type == TokenType::END_FILE || current_ >= tokens_.size())
        {
            return true;
        }
        return false;
    };


    //peek method
    Token const peek() 
    {
        if (is_end())
        {
            return Token(TokenType::END_FILE);
        }
        else
        {
            return tokens_[current_];
        }
    };
    //advance to next token to peek, returns the one we were just working on. Holds 2 values, operator, and right side
    //of the equation
    Token const advance()
    {
        if (is_end() != true)
        {
            current_ ++;
            return tokens_[current_-1];
        }
        return Token(TokenType::END_FILE);
        };


    //helper to call the parsing recursion
            std::unique_ptr<ASTNode> parse_tokens()
            {
                return parse_priority_4();
            };

   

    //Handle each layer of priority
    
    //handle parsing for lowest(highest score) priority operators +/-
    std::unique_ptr<ASTNode> parse_priority_4()
    {
        std::unique_ptr<ASTNode> left = parse_priority_3();

        //iterate to find operator
        while (peek().type == TokenType::ADD || peek().type == TokenType::SUB)
        {
            std::unique_ptr<ASTNode> right;
            TokenType operation = advance().type;
            right = parse_priority_3();
            nodes_length_ ++;
            left = std::make_unique<OperatorNode>(operation, std::move(left), std::move(right));
        };
        return left;
        
    };
    
    
    //handle parsing for * and /
    std::unique_ptr<ASTNode> parse_priority_3()
    {
        std::unique_ptr<ASTNode> left = parse_priority_2();

        //iterate to find operator
        while (peek().type == TokenType::MULT|| peek().type == TokenType::DIV)
        {
            std::unique_ptr<ASTNode> right;
            TokenType operation = advance().type;
            right = parse_priority_2();
            nodes_length_ ++;
            left = std::make_unique<OperatorNode>(operation, std::move(left), std::move(right));
        }
        return left;
    };
    
    //handle parsing for (-)unary operator 
    //calls itself rather than next level. on 'right' 
    std::unique_ptr<ASTNode> parse_priority_2()
    {
        while (peek().type == TokenType::SUB)
        {
            advance();
            std::unique_ptr<ASTNode> value;
            value = parse_priority_2();
            nodes_length_ ++;
            return std::make_unique<UnaryNode>(std::move(value));
        };
        return parse_priority_1();
    };

    //handle parsing for ^ power, calls itself on right rather than parse_leaf
    std::unique_ptr<ASTNode> parse_priority_1()
    {
        std::unique_ptr<ASTNode> left = parse_leaf();
        while (peek().type == TokenType::POWER)
        {
            std::unique_ptr<ASTNode> right;
            TokenType operation = advance().type;
            right = parse_priority_1();
            nodes_length_ ++;
            return std::make_unique<OperatorNode>(operation, std::move(left), std::move(right));
        };
        return left;
    };


    //handle parsing for leaf/recursion/reset on par
    std::unique_ptr<ASTNode> parse_leaf()
    {
        Token current_token = advance();
        switch (current_token.type)
        {
            case TokenType::NUMBER : 
                nodes_length_ ++;
                return std::make_unique<NumberNode> (std::get<double>(current_token.value));

            case TokenType::VARIABLE :
                nodes_length_ ++;
                return std::make_unique<VariableNode> (std::get<std::string>(current_token.value));

            case TokenType::OPAR : 
            {
                std::unique_ptr<ASTNode> new_head = parse_priority_4();
                current_token = advance();
                if (current_token.type == (TokenType::CLPAR)){
                    advance();
                }
                else {return nullptr;};
                
                return new_head;
            }

            
            default : return nullptr;
            
        };
    };
};


