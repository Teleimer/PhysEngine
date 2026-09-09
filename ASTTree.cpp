#include <iostream>
#include "Parser.hpp"
#include "Tokens.hpp"
#include "ASTNode.hpp"

enum class OpCode 
{
    ADD,
    SUB,
    MULT,
    DIV,
    POWER
};

enum class OperandType
{
    NUMBER,
    VARIABLE,
    DESTINATION
};

struct Operand
{
    OperandType type;
    std::variant<std::string, double> value;
    Operand(OperandType type, std::variant<std::string, double> value) :
        type(type), value(value){};
};


struct Instruction
{
    OpCode operation;
    Operand source_1;
    Operand source_2;
    Operand destination;
    Instruction(OpCode operation, Operand s1, Operand s2, Operand destination):
        operation(operation), source_1(s1), source_2(s2), destination(destination){};
};

OpCode convert_operation_type(TokenType toketype)
{
    switch(toketype)
    {
        case TokenType::ADD: return OpCode::ADD;
        case TokenType::SUB: return OpCode::SUB;
        case TokenType::MULT: return OpCode::MULT;
        case TokenType::DIV: return OpCode::DIV;
        case TokenType::POWER: return OpCode::POWER;
    }
}
// postfixarray helper
std::vector<Token>& flatten_postfix_helper(
    const ASTNode& subtree_head, 
    std::vector<Token>& postfix_array)
{
    if (subtree_head.type_ == TokenType::NUMBER)
    {
        //static cast, allowing to access .value_ --> makes current_ndoe take NumberNode type and give up ASTNode
        const auto& current_node = static_cast<const NumberNode&>(subtree_head);
        postfix_array.emplace_back(TokenType::NUMBER, current_node.value_);
    }
    else if (subtree_head.type_ == TokenType::VARIABLE)
    {
        const auto& current_node = static_cast<const VariableNode&>(subtree_head);
        postfix_array.emplace_back(TokenType::VARIABLE, current_node.value_);
    }
    else if (subtree_head.type_ == TokenType::NEG) //if unary negative
    {
        const auto& current_node = static_cast<const UnaryNode&>(subtree_head);
        flatten_postfix_helper(*current_node.left_, postfix_array);
        postfix_array.emplace_back(TokenType::NUMBER, -1.0);
        postfix_array.emplace_back(TokenType::MULT);
    }
    else 
    // operators
    {
        const auto& current_node = static_cast<const OperatorNode&>(subtree_head);
        
        // traverse left, no emplace back, because post-order traversla
        if (current_node.left_)
        {  
                flatten_postfix_helper(*current_node.left_, postfix_array); 
        }
        
        //traverse right
        if (current_node.right_)
        {
            flatten_postfix_helper(*current_node.right_, postfix_array);
        };

        postfix_array.emplace_back(current_node.type_);
    };

    return postfix_array;
};


//flatten to postfix token array
std::vector<Token> flatten_postfix(const ASTNode& head_node, size_t len_nodes)
{
    std::vector<Token> postfix_array;
    postfix_array.reserve(len_nodes);
    flatten_postfix_helper(head_node, postfix_array);
    return postfix_array;
};



//flatten to TAC (three address code) array
std::vector<Instruction> generate_tac(const std::vector<Token>& postfix_array)
{
    std::stack<Operand> containers;
    //containers.reserve(postfix_array.size());
    std::vector<Instruction> tac_array;
    tac_array.reserve(postfix_array.size());
    size_t current = 0;
    size_t dest_id = 0;

    while (current < postfix_array.size())
    {
        const Token current_token = postfix_array[current];
        if (current_token.type == TokenType::VARIABLE)
        {
            Operand current_operand = {OperandType::VARIABLE, current_token.value};
            containers.push(current_operand);
        }
        else if (postfix_array[current].type == TokenType::NUMBER)
        {
            Operand current_operand = {OperandType::NUMBER, current_token.value};
            containers.push(current_operand);
        }
        else
        {
            Operand right = containers.top();
            containers.pop();
            Operand left = containers.top();
            containers.pop();
            Operand destination = Operand (
                OperandType::DESTINATION, 
                "t"+std::to_string(dest_id));
                dest_id ++;
            OpCode op = convert_operation_type(current_token.type);
            Instruction next_instruction = 
            {
                op,
                left,
                right,
                destination
            };
            containers.push(destination);
            tac_array.push_back(next_instruction);
        }
        current++;
    }
    return tac_array;
};

//evaluate the TAC vector
std::variant<std::string, double> eval_tac(const std::vector<Instruction>& tac_vector)
{

}




//simplify tree by math

//prep for integration, derivation, etc 



//flatten input equation to array
std::vector<Instruction> eq_to_postfix(std::string input_formula)
//still need to clean up, just a draft to make main small as possible
{
    //innits
    std::vector <Token> tokens;
    tokens = parse_equation(input_formula);
    parser parsed_tree(tokens);
    auto head = parsed_tree.get_head_node();
    std::vector<Instruction> tac_arr;
    
    //if to call helpers
    if (head)
    {
        std::vector<Token> flattened_vector;
        flattened_vector.reserve(parsed_tree.get_nodes_length());
        flattened_vector = flatten_postfix(*head, parsed_tree.get_nodes_length());
        const auto& next = generate_tac(flattened_vector);
        return next;
    }
    else
    {
        return tac_arr;
    };
    
};


int main(){
    std::string input_formula = "3x+2^3*(-1+666)";
    std::vector<Instruction> postfix_vector;
    postfix_vector = eq_to_postfix(input_formula);
    //postfix_vector = eq_to_postfix(input_formula);
    std::cout << "All done generating nodes \n\n";
    return 10;
    }