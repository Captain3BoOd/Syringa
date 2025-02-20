#pragma once

#include <vector>
#include <memory>
#include <stack>

#include "token.hpp"
#include "error.hpp"
#include "node.hpp"

class Parser
{
public:
	Parser(std::vector<Token>& tokens);
	NodeVector& Parse();
private:
	Node* make_expression();
	Node* make_list();
	Node* make_class_def();
	/***********************/
	template<bool is_class = false>
	Node* make_fun_def();
	Node* make_lambda();
	Fun::Argument make_arg();
	/***********************/
	Node* make_import();
	/***********************/
	void make_statements();
	/**********************************************************/
	template<bool is_return = false, bool is_con_break = false>
	Node* make_statement();
	template<bool is_return = false, bool is_con_break = false>
	Node* make_if();
	template<bool is_return = false>
	Node* make_while();
	template<bool is_return = false>
	Node* make_for();
	template<bool is_return = false, bool is_con_break = false>
	Node* block();
	/**********************************************************/
	Node* arithmetic();
	Node* term();
	Node* factor();
	/***************/
	Node* identifier(const bool is_const = false, const bool declare = false);
	Node* make_class_call(Node* class_node, const Position& start_pos);
	/***************/
	template<bool skip = true>
	void advance();
private:
	NodeVector Trees;
	std::vector<Token>* tokens;
	std::stack<TokenType> br;
	Token* current_token;
	size_t pos;
};
