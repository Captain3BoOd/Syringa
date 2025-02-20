#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "types.hpp"
#include "token.hpp"

enum class NodeType: uint8_t
{
	NUMBER,
	STRING,
	LIST,
	FUN,
	FUN_CALL,
	ASSIGN,
	ACCESS,
	EXPRESSION,
	BLOCK,
	IF_STATEMENT,
	FOR_STATEMENT,
	WHILE_STATEMENT,
	RETURN,
	CONTINUE,
	BREAK,
	CLASS,
	CLASS_CALL,
	IMPORT,
	NONE
};

struct Node
{
	Node(NodeType type, Position start_pos, Position end_pos);
	Node(const Node* other);

	bool operator== (const NodeType node_type) const;
	virtual void destroy() = 0;
	virtual ~Node() = default;

	const Position start_pos;
	const Position end_pos;
private:
	const NodeType type;
};

/*******************************************************************************************/
/******************************************(NUMBER)*****************************************/
/*******************************************************************************************/

struct Number: public Node
{
	Number(const std::string& value, bool is_float, Position start_pos, Position end_pos);
	virtual void destroy() override {}

	bool is_float;
	std::string value;
};

/*******************************************************************************************/
/******************************************(STRING)*****************************************/
/*******************************************************************************************/

struct String: public Node
{
	String(const std::string& value, Position start_pos, Position end_pos);
	virtual void destroy() override {}

	std::string value;
};

/*******************************************************************************************/
/*******************************************(LIST)******************************************/
/*******************************************************************************************/

struct NList: public Node
{
	NList(NodeVector& elements, Position start_pos, Position end_pos);
	virtual void destroy() override;

	NodeVector elements;
};

/*******************************************************************************************/
/***********************************(FUNCTION DEFINITION)***********************************/
/*******************************************************************************************/

struct Fun: public Node
{
	struct Argument
	{
		enum ArgType : uint8_t
		{
			NONE = 0,
			NORMAL,
			ARBITARY,
			DEFAULT
		};

		Token arg;
		Argument::ArgType type = Argument::ArgType::NONE;
		Node* ddefault;
	};

	Fun(
		const std::string& name,
		uint64_t co_argcount,
		uint64_t default_count,
		const std::vector<Argument>& elements,
		const NodeVector& body,
		const std::vector<std::string>& return_types,
		bool in_class,
		Position start_pos,
		Position end_pos
	);
	virtual void destroy() override;

	std::string name;
	std::vector<Argument> elements;
	std::vector<std::string> return_types;
	NodeVector body;
	uint64_t co_argcount;
	uint64_t default_count;
	bool in_class;
};

/*******************************************************************************************/
/**************************************(FUNCTION CALL)**************************************/
/*******************************************************************************************/

struct FunCall : public Node
{
	FunCall(const std::string& name, Node* caller, NodeVector parameters, Position start_pos, Position end_pos);
	virtual void destroy() override;

	std::string name;
	NodeVector parameters;
};

/*******************************************************************************************/
/*****************************************(RETURN)******************************************/
/*******************************************************************************************/

struct Return: public Node
{
	Return(Node* expr, Position start_pos, Position end_pos);
	virtual void destroy() override;

	Node* expr;
};

/*******************************************************************************************/
/****************************************(CONTINUE)*****************************************/
/*******************************************************************************************/

struct Continue: public Node
{
	Continue(Position start_pos, Position end_pos);
	virtual void destroy() override {}
};

/*******************************************************************************************/
/******************************************(BREAK)******************************************/
/*******************************************************************************************/

struct Break: public Node
{
	Break(Position start_pos, Position end_pos);
	virtual void destroy() override {}
};

/*******************************************************************************************/
/******************************************(ACCESS)*****************************************/
/*******************************************************************************************/

struct VariableAcess: public Node
{
	VariableAcess(std::string& var_name, Position start_pos, Position end_pos);
	virtual void destroy() override {}

	std::string var_name;
};

/*******************************************************************************************/
/******************************************(ASSIGN)*****************************************/
/*******************************************************************************************/

struct VariableAssign: public Node
{
	VariableAssign(const std::string& var_name, Node* value, Position start_pos, Position end_pos, bool declare, bool is_const);
	virtual void destroy() override;

	std::string var_name;
	Node* value;
	bool declare;
	bool is_const;
};

/*******************************************************************************************/
/****************************************(EXPRESSION)***************************************/
/*******************************************************************************************/

enum class ExpressionType: uint8_t
{
	UNARY_OPERATOR,
	BIN_OPERATOR,
	NONE
};

struct Expression: public Node
{
	Expression(Node* left, TokenType op, Node* right, Position start_pos, Position end_pos, ExpressionType expr_type = ExpressionType::NONE);
	bool operator==(const ExpressionType expression_type) const;
	virtual void destroy() override;

	Node* left;
	Node* right;
	TokenType op;
private:
	ExpressionType expr_type;
};

// Binary Operator //
struct BinOp: public Expression
{
	BinOp(Node* left, TokenType op, Node* right, Position start_pos, Position end_pos);
};

// Unary Operator //
struct UnaryOp : public Expression
{
	UnaryOp(Node* left, TokenType op, Position start_pos, Position end_pos, bool after = false);
	const bool after;
};

/*******************************************************************************************/
/*******************************************(BLOCK)*****************************************/
/*******************************************************************************************/

struct Block: public Node
{
	Block(const NodeVector& expressions, Position start_pos, Position end_pos);
	virtual void destroy() override;

	NodeVector expressions;
};

/*******************************************************************************************/
/***************************************(IF STATEMENT)**************************************/
/*******************************************************************************************/

struct IfStatment: public Node
{
	IfStatment(Node* condition, Node* block, Node* elsee, Position start_pos, Position end_pos);
	virtual void destroy() override;

	Node* condition;
	Node* block;
	Node* elsee;
};

/*******************************************************************************************/
/*************************************(WHILE STATEMENT)*************************************/
/*******************************************************************************************/

struct WhileStatment: public Node
{
	WhileStatment(Node* condition, Node* block, Position start_pos, Position end_pos);
	virtual void destroy() override;

	Node* condition;
	Node* block;
};

/*******************************************************************************************/
/**************************************(FOR STATEMENT)**************************************/
/*******************************************************************************************/

struct ForStatment: public Node
{
	ForStatment(Node* counter, Node* condition, Node* increase, Node* block, Position start_pos, Position end_pos);
	virtual void destroy() override;

	Node* counter;
	Node* condition;
	Node* increase;
	Node* block;
};

/*******************************************************************************************/
/******************************************(CLASS)******************************************/
/*******************************************************************************************/

struct NClass: public Node
{
	NClass(const std::string& class_name, const std::vector<Token>& inh, const NodeVector& variables, const NodeVector& functions, const NodeVector& classes, Position start_pos, Position end_pos);
	virtual void destroy() override;

	std::string class_name;
	std::vector<Token> inh;
	NodeVector variables;
	NodeVector functions;
	NodeVector classes;
};

/*******************************************************************************************/
/***************************************(CLASS CALL)****************************************/
/*******************************************************************************************/

struct ClassCall : public Node
{
	ClassCall(Node* class_node, Node* member_node, Position start_pos, Position end_pos);
	virtual void destroy() override;

	Node* class_node;
	Node* member_node;
};

/*******************************************************************************************/
/*****************************************(IMPORT)******************************************/
/*******************************************************************************************/

#include <filesystem>

struct Import: public Node
{
	Import(const std::string& path, Position start_pos, Position end_pos);
	virtual void destroy() override {};

	std::filesystem::path module_path;
};

/*******************************************************************************************/
/***************************************(CLASS CALL)****************************************/
/*******************************************************************************************/

inline void DestroyNodes(NodeVector& nodes)
{
	for (Node* node: nodes)
	{
		if (node)
		{
			node->destroy();
			delete node;
		}
	}
}
