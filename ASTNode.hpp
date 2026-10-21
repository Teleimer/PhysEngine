#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>


struct ASTNode 
{
    TokenType type_;
    explicit ASTNode(TokenType type) : type_(type){};
    
    virtual ~ASTNode() = default;
};

struct NumberNode : public ASTNode
{
    double value_;
    NumberNode(double value) : ASTNode(TokenType::NUMBER),value_(value){}; //constructor for NumberNode -- takes one argument a value of type double
    // instruction for set a new value in a number node. need .set for multiple node types on simplification
};

struct VariableNode: public ASTNode
{
    std::string value_;
    VariableNode(std::string value) : ASTNode(TokenType::VARIABLE), value_(value){};
};

struct OperatorNode : public ASTNode
{
    std::unique_ptr<ASTNode> left_; //initialized to a unique ASTNode pointer, ensuring fidelity across calls
    std::unique_ptr<ASTNode> right_;
    
    //constructor
    OperatorNode(TokenType type, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
    : ASTNode(type), left_(std::move(left)), right_(std::move(right)){};
};

struct UnaryNode : public ASTNode
{
    std::unique_ptr<ASTNode> left_;
    UnaryNode(std::unique_ptr<ASTNode> neg_value) : 
        ASTNode(TokenType::NEG), left_(std::move(neg_value)){};
};