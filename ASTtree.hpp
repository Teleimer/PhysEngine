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

