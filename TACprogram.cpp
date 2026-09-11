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
    std::vector<std::vector<double>> registers;
    std::vector<std::vector<double>> variable_values;
    std::vector<double> evaluated_solutions;
    double number_loops;
    TACProgram(std::vector<Token>postfix_tokens, 
               std::vector<std::vector<double>> variable_values) :
                    postfix_vector(std::move(postfix_tokens)),
                    variable_values(variable_values)
    {
        //innits and trackers
        instructions.reserve(postfix_vector.size());
        register_map.reserve(postfix_vector.size());
        registers.reserve(postfix_vector.size());
        number_loops = variable_values[0].size();
        evaluated_solutions.resize(number_loops);
        size_t current = 0;
        size_t register_id = 0;
        size_t current_var = 0;
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
                int operand = get_or_assign(value, current, true, false);
                eval_stack.push_back(operand);
            }
            else if (current_token.type == TokenType::VARIABLE)
            {
                std::string value = std::get<std::string>(current_token.value);
                int operand = get_or_assign(value, current_var, false, true);
                current_var ++;
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
                int destination_i = get_or_assign("t"+std::to_string(register_id),current, false, false);
                
                // update storage and attributes
                TACInstruction current_inst = {operation, left_i, right_i, destination_i};
                instructions.push_back(current_inst);
                eval_stack.push_back(destination_i);
                register_id ++;
            };
            current ++;
        };
    };

    std::vector<double> execute_tac()
    {
        for (const TACInstruction& instruction : instructions)
        {
            double left = instruction.left_i;
            double right = instruction.right_i;

            for (size_t i=0; i < number_loops; i++)
            {
                switch(instruction.operation)
                {
                    case OpCode::ADD: 
                        registers[instruction.dest_i][i] = 
                            registers[left][i] + registers[right][i];
                        break;

                    case OpCode::SUB:
                        registers[instruction.dest_i][i] = 
                            registers[left][i] - registers[right][i];
                        break;

                    case OpCode::MULT:
                        registers[instruction.dest_i][i] = 
                            registers[left][i] * registers[right][i];
                        break;

                    case OpCode::DIV:
                        registers[instruction.dest_i][i] = 
                            registers[left][i] / registers[right][i];
                        break;
                    
                    case OpCode::POWER:
                        // needs optimizing
                        registers[instruction.dest_i][i] = 
                            std::pow(registers[left][i], registers[right][i]);
                        break;
                };
            };
        };
        return registers[instructions.back().dest_i];
      };

    



    private:
        int get_or_assign(std::string value, size_t& current_var, bool is_num=false, 
                          bool is_var=false)
        {
            auto key = register_map.find(value);
            
            //if  found 
            if (key != register_map.end())
            {
                auto& [reg_key, reg_index] = *key;
                return reg_index;
            }
            
            //if not found
            else if (key == register_map.end())
            {
                int reg_index = static_cast<int>(registers.size());
                register_map[value] = reg_index;
                if (is_num)
                {
                    std::vector<double> val;
                    val.resize(number_loops);
                    std::fill(val.begin(),val.end(),std::stod(value));
                    registers.push_back(val);
                }
                else if (is_var)
                {
                    std::vector<double> var_vals = variable_values[current_var];
                    registers.push_back(var_vals);
                    
                }

                else
                {
                    std::vector<double> val;
                    val.resize(number_loops);
                    std::fill(val.begin(), val.end(),0);
                    registers.push_back(val);
                };
                return reg_index;
            };
        };
};







//flatten to TACInstruction -- vectorized for eval


//evaluate the TAC vector



int main()
{
    std::string input_formula = "3x+2";
    std::vector<Token> postfix_vector;
    std::vector<std::vector<double>> var_vals;
    std::vector<double> input_var = {0,1,2,3,4,5,6,7};
    var_vals.push_back(input_var);
    postfix_vector = eq_to_postfix(input_formula);
    TACProgram current_program = TACProgram (postfix_vector, var_vals);
    std::vector<double> solution;
    solution = current_program.execute_tac();
    std::cout << "All done generating nodes \n\n";

}