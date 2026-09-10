#include <iostream>
#include <string>
#include "Parser.hpp"
#include "Tokens.hpp"
#include "ASTNode.hpp"
#include "ASTtree.hpp"


enum class OpCode 
{
    ADD,
    SUB,
    MULT,
    DIV,
    POWER
};


struct TACInstruction
{
    OpCode operation;
    int left_i;
    int right_i;
    int dest_i;
    TACInstruction(OpCode operation, int left_i, int right_i, int dest_i):
        operation(operation), left_i(left_i), right_i(right_i), dest_i(dest_i){};
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
};


struct TACProgram
{
    std::vector<Token> postfix_vector;
    std::vector<TACInstruction> instructions;
    std::unordered_map<std::string, int> register_map;
    std::vector<double> registers;
    
    TACProgram(std::vector<Token>postfix_tokens) : postfix_vector(std::move(postfix_tokens))
    {
        //innits and trackers
        instructions.reserve(postfix_vector.size());
        register_map.reserve(postfix_vector.size());
        registers.reserve(postfix_vector.size());
        size_t current = 0;
        size_t register_id = 0;
        std::vector <int> eval_stack;
        eval_stack.reserve(postfix_vector.size());

        //build instructions 
        while (current < postfix_vector.size())
        {
            const auto current_token = postfix_vector[current];

            if (current_token.type == TokenType::NUMBER)
            {
                double num_val = std::get<double>(current_token.value); //unbox for key to_string
                std::string value = std::to_string(num_val);
                int operand = get_or_assign(value, true);
                eval_stack.push_back(operand);
            }
            else if (current_token.type == TokenType::VARIABLE)
            {
                std::string value = std::get<std::string>(current_token.value);
                int operand = get_or_assign(value);
                eval_stack.push_back(operand);
            }

            else
            //current_token is operator
            {
                // generate attributes
                OpCode operation = convert_operation_type(current_token.type);
                int right_i = eval_stack.back();
                eval_stack.pop_back();
                int left_i = eval_stack.back();
                eval_stack.pop_back();
                int destination_i = get_or_assign("t"+std::to_string(register_id));
                
                // update storage and attributes
                TACInstruction current_inst = {operation, left_i, right_i, destination_i};
                instructions.push_back(current_inst);
                eval_stack.push_back(destination_i);
                register_id ++;
            };
            current ++;
        };
    }

    private:
        int get_or_assign(std::string value, bool is_num=false)
        {
            auto it = register_map.find(value);
            
            //if  found 
            if (it != register_map.end())
            {
                auto& [reg_key, reg_index] = *it;
                return reg_index;
            }
            
            //if found
            else if (it == register_map.end())
            {
                int reg_index = static_cast<int>(registers.size());
                register_map[value] = reg_index;
                if (is_num)
                {
                    registers.push_back(std::stod(value));
                }
                else
                {
                    registers.push_back(0.0);
                };
                return reg_index;
            };
        };
};







//flatten to TACInstruction -- vectorized for eval


//evaluate the TAC vector



int main()
{
    std::string input_formula = "3x+2^3*(-1+666)";
    std::vector<Token> postfix_vector;
    postfix_vector = eq_to_postfix(input_formula);
    TACProgram current_program = TACProgram (postfix_vector);
    std::cout << "All done generating nodes \n\n";

}