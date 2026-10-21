#include <iostream>
#include "Parser.hpp"
#include "Tokens.hpp"
#include "ASTNode.hpp"


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
    else if (subtree_head.type_ == TokenType::POWER)
    {
        const auto& current_node = static_cast<const OperatorNode&>(subtree_head);
        flatten_postfix_helper(*current_node.left_, postfix_array);
        flatten_postfix_helper(*current_node.right_, postfix_array);

        const auto& current_right = static_cast <const NumberNode&>(*current_node.right_);
        const auto& current_left = static_cast <const NumberNode&> (*current_node.left_);
        if (current_right.value_ > 1 || current_right.value_ < -1)
        {
            postfix_array.pop_back();
        }
        if (current_right.value_ == 0)
        {
            postfix_array.pop_back();
            postfix_array.pop_back();
            postfix_array.emplace_back(TokenType::NUMBER, 1.0);

        } 
        else if (current_right.value_ == -1) 
        //incomplete for power -1
        {
            postfix_array.emplace_back(TokenType::DIV);
        }
        else if (current_right.value_ < 0)
        {
            for (double i=0; i > current_right.value_; i--)
            {
                
                postfix_array.emplace_back(TokenType::DIV);
                postfix_array.emplace_back(TokenType::NUMBER, current_left.value_);
            };
        }
        else if (current_right.value_ > 1)
        {
        
            for (double i=1; i < current_right.value_; i++)
            {
                postfix_array.emplace_back(TokenType::NUMBER, current_left.value_);
            }
            for (double i=1; i < current_right.value_; i++)
            {
                postfix_array.emplace_back(TokenType::MULT);
            }
        };
        return postfix_array;
        
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



//flatten input equation to array
std::vector<Token> eq_to_postfix(std::string input_formula)
{
    //innits
    std::vector <Token> tokens;
    tokens = parse_equation(input_formula);
    parser parsed_tree(tokens);
    auto head = parsed_tree.get_head_node();
    std::vector<Token> flattened_vector;
    
    //if to call helpers
    if (head)
    {
        flattened_vector.reserve(parsed_tree.get_nodes_length());
        flattened_vector = flatten_postfix(*head, parsed_tree.get_nodes_length());
        return flattened_vector;
    };  
    return flattened_vector;
};

//simplify tree by math

//prep for integration, derivation, etc 

