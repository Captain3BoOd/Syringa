#pragma once

#include "symboltable.hpp"
#include "types.hpp"
#include "error.hpp"
#include "node.hpp"

struct InterpreterResult
{
	ObjectPtr Result;
	bool is_return = false;
	bool is_continue = false;
	bool is_break = false;
};

class Interpreter
{
public:
	Interpreter(SymbolTable* symbol_table_parent = nullptr);
	Interpreter(Node* tree, SymbolTable* symbol_table_parent = nullptr);
	Interpreter(const NodeVector& trees, SymbolTable* symbol_table_parent = nullptr);

	ObjectPtr interpret();
	SymbolTable& get_vars();
private:
	InterpreterResult evaluate_all();
	void evaluate_fun_def(Node* tree, SymbolTable& symboltable);
	void evaluate_class_def(Node* tree, SymbolTable& symboltable);
	InterpreterResult evaluate(Node* tree);
	InterpreterResult evaluate_variable_access(Node* tree, SymbolTable& symboltable);
	InterpreterResult evaluate_variable_assign(Node* tree, SymbolTable& symboltable);
	template<bool is_class = false>
	InterpreterResult evaluate_fun_call(Node* tree, SymbolTable& symboltable, ObjectPtr self = nullptr);
	InterpreterResult evaluate_class_call(Node* tree, SymbolTable& symboltable);

	InterpreterResult evaluate_number(Node* node);
	InterpreterResult evaluate_string(Node* node);
	InterpreterResult evaluate_list(Node* node);

	InterpreterResult evaluate_expression(Node* node);

	InterpreterResult evaluate_block(Node* node);

	InterpreterResult evaluate_if(Node* node);
	InterpreterResult evaluate_for(Node* node);
	InterpreterResult evaluate_while(Node* node);
private:
	SymbolTable symbol_table;
	std::vector<Node*> trees;
};