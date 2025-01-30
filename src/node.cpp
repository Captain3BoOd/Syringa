#include "node.hpp"

Node::Node(NodeType type, Position start_pos, Position end_pos):
	type(type),
	start_pos(start_pos),
	end_pos(end_pos)
{}

Node::Node(const Node* other):
	type((other ? (*other).type : NodeType::NONE))
{}

bool Node::operator==(const NodeType node_type) const
{
	return ((this->type == node_type) && (this->type != NodeType::NONE));
}

/*******************************************************************************************/
/******************************************(NUMBER)*****************************************/
/*******************************************************************************************/

Number::Number(const std::string& value, bool is_float, Position start_pos, Position end_pos):
	Node(NodeType::NUMBER, start_pos, end_pos),
	value(value),
	is_float(is_float)
{
	LANG_ASSERT(!this->value.empty(), "object must have a value");
}

/*******************************************************************************************/
/******************************************(STRING)*****************************************/
/*******************************************************************************************/

String::String(const std::string& value, Position start_pos, Position end_pos):
	Node(NodeType::STRING, start_pos, end_pos),
	value(value)
{}

/*******************************************************************************************/
/*******************************************(LIST)******************************************/
/*******************************************************************************************/

NList::NList(NodeVector& elements, Position start_pos, Position end_pos):
	Node(NodeType::LIST, start_pos, end_pos),
	elements(elements)
{}

void NList::destroy()
{
	for (Node* node : this->elements)
	{
		if (node)
		{
			node->destroy();
			delete node;
		}
	}
}

/*******************************************************************************************/
/***********************************(FUNCTION DEFINITION)***********************************/
/*******************************************************************************************/

Fun::Fun(
	const std::string& name,
	uint64_t co_argcount,
	uint64_t default_count,
	const std::vector<Argument>& elements,
	const NodeVector& body,
	bool in_class,
	Position start_pos,
	Position end_pos
):
	Node(NodeType::FUN, start_pos, end_pos),
	name(name),
	co_argcount(co_argcount),
	default_count(default_count),
	elements(elements),
	body(body),
	in_class(in_class)
{
	LANG_ASSERT(!(in_class && this->elements.empty()), "if fun in class\nit must have a self argument");
}

void Fun::destroy()
{
	for (Fun::Argument& arg : this->elements)
	{
		if (arg.ddefault)
		{
			arg.ddefault->destroy();
			delete arg.ddefault;
		}
	}

	for (Node* expression: this->body)
	{
		if (expression)
		{
			expression->destroy();
			delete expression;
		}
	}
}

/*******************************************************************************************/
/**************************************(FUNCTION CALL)**************************************/
/*******************************************************************************************/

FunCall::FunCall(const std::string& name, Node* caller, NodeVector parameters, Position start_pos, Position end_pos):
	Node(NodeType::FUN_CALL, start_pos, end_pos),
	name(name),
	parameters(parameters)
{}

void FunCall::destroy()
{
	for (Node* arg : this->parameters)
	{
		if (arg)
		{
			arg->destroy();
			delete arg;
		}
	}
}

/*******************************************************************************************/
/*****************************************(RETURN)******************************************/
/*******************************************************************************************/


Return::Return(Node* expr, Position start_pos, Position end_pos):
	Node(NodeType::RETURN, start_pos, end_pos),
	expr(expr)
{}

void Return::destroy()
{
	if (!this->expr) return;

	this->expr->destroy();
	delete this->expr;
}

/*******************************************************************************************/
/****************************************(CONTINUE)*****************************************/
/*******************************************************************************************/

Continue::Continue(Position start_pos, Position end_pos):
	Node(NodeType::CONTINUE, start_pos, end_pos)
{}

/*******************************************************************************************/
/******************************************(BREAK)******************************************/
/*******************************************************************************************/

Break::Break(Position start_pos, Position end_pos):
	Node(NodeType::BREAK, start_pos, end_pos)
{}

/*******************************************************************************************/
/******************************************(ACCESS)*****************************************/
/*******************************************************************************************/

VariableAcess::VariableAcess(std::string& var_name, Position start_pos, Position end_pos):
	Node(NodeType::ACCESS, start_pos, end_pos),
	var_name(var_name)
{}

/*******************************************************************************************/
/******************************************(ASSIGN)*****************************************/
/*******************************************************************************************/

VariableAssign::VariableAssign(const std::string& var_name, Node* value, Position start_pos, Position end_pos, bool declare, bool is_const):
	Node(NodeType::ASSIGN, start_pos, end_pos),
	var_name(var_name),
	declare(declare),
	is_const(is_const),
	value(value)
{}

void VariableAssign::destroy()
{
	if (!this->value) return;

	this->value->destroy();
	delete this->value;
}

/*******************************************************************************************/
/****************************************(EXPRESSION)***************************************/
/*******************************************************************************************/

Expression::Expression(Node* left, TokenType op, Node* right, Position start_pos, Position end_pos, ExpressionType expr_type):
	Node(NodeType::EXPRESSION, start_pos, end_pos),
	expr_type(expr_type),
	op(op),
	left(left),
	right(right)
{}

bool Expression::operator==(const ExpressionType expression_type) const
{
	return this->expr_type == expression_type;
}

void Expression::destroy()
{
	if (this->left)
	{
		this->left->destroy();
		delete this->left;
	}

	if (this->right)
	{
		this->right->destroy();
		delete this->right;
	}
}

// Bin Operator //
BinOp::BinOp(Node* left, TokenType op, Node* right, Position start_pos, Position end_pos):
	Expression(left, op, right, start_pos, end_pos, ExpressionType::BIN_OPERATOR)
{}

// Unary Operator //
UnaryOp::UnaryOp(Node* left, TokenType op, Position start_pos, Position end_pos, bool after):
	Expression(left, op, nullptr, start_pos, end_pos, ExpressionType::UNARY_OPERATOR),
	after(after)
{}

/*******************************************************************************************/
/*******************************************(BLOCK)*****************************************/
/*******************************************************************************************/

Block::Block(const NodeVector& expressions, Position start_pos, Position end_pos):
	Node(NodeType::BLOCK, start_pos, end_pos),
	expressions(expressions)
{}

void Block::destroy()
{
	for (Node* expr : this->expressions)
	{
		if (expr)
		{
			expr->destroy();
			delete expr;
		}
	}
}

/*******************************************************************************************/
/***************************************(IF STATEMENT)**************************************/
/*******************************************************************************************/

IfStatment::IfStatment(Node* condition, Node* block, Node* elsee, Position start_pos, Position end_pos):
	Node(NodeType::IF_STATEMENT, start_pos, end_pos),
	condition(condition),
	block(block),
	elsee(elsee)
{}

void IfStatment::destroy()
{
	if (this->block)
	{
		this->block->destroy();
		delete this->block;
	}

	if (this->condition)
	{
		this->condition->destroy();
		delete this->condition;
	}

	if (this->elsee)
	{
		this->elsee->destroy();
		delete this->elsee;
	}
}

/*******************************************************************************************/
/*************************************(WHILE STATEMENT)*************************************/
/*******************************************************************************************/

WhileStatment::WhileStatment(Node* condition, Node* block, Position start_pos, Position end_pos):
	Node(NodeType::WHILE_STATEMENT, start_pos, end_pos),
	condition(condition),
	block(block)
{}

void WhileStatment::destroy()
{
	if (this->block)
	{
		this->block->destroy();
		delete this->block;
	}

	if (this->condition)
	{
		this->condition->destroy();
		delete this->condition;
	}
}

/*******************************************************************************************/
/**************************************(FOR STATEMENT)**************************************/
/*******************************************************************************************/

ForStatment::ForStatment(Node* counter, Node* condition, Node* increase, Node* block, Position start_pos, Position end_pos):
	Node(NodeType::FOR_STATEMENT, start_pos, end_pos),
	counter(counter),
	condition(condition),
	increase(increase),
	block(block)
{}

void ForStatment::destroy()
{
	if (this->block)
	{
		this->block->destroy();
		delete this->block;
	}

	if (this->condition)
	{
		this->condition->destroy();
		delete this->condition;
	}

	if (this->increase)
	{
		this->increase->destroy();
		delete this->increase;
	}

	if (this->counter)
	{
		this->counter->destroy();
		delete this->counter;
	}
}

/*******************************************************************************************/
/******************************************(CLASS)******************************************/
/*******************************************************************************************/

NClass::NClass(
		const std::string& class_name,
		const std::vector<Token>& inh,
		const NodeVector& variables,
		const NodeVector& functions,
		const NodeVector& classes,
		Position start_pos,
		Position end_pos
	):
	Node(NodeType::CLASS, start_pos, end_pos),
	class_name(class_name),
	inh(inh),
	functions(functions),
	variables(variables),
	classes(classes)
{}

void NClass::destroy()
{
	for (Node* fun : this->functions)
	{
		if (fun)
		{
			fun->destroy();
			delete fun;
		}
	}

	for (Node* var : this->variables)
	{
		if (var)
		{
			var->destroy();
			delete var;
		}
	}

	for (Node* cls : this->classes)
	{
		if (cls)
		{
			cls->destroy();
			delete cls;
		}
	}
}

/*******************************************************************************************/
/***************************************(CLASS CALL)****************************************/
/*******************************************************************************************/

ClassCall::ClassCall(Node* class_node, Node* member_node, Position start_pos, Position end_pos):
	Node(NodeType::CLASS_CALL, start_pos, end_pos),
	class_node(class_node),
	member_node(member_node)
{}

void ClassCall::destroy()
{
	if (this->class_node)
	{
		this->class_node->destroy();
		delete this->class_node;
	}

	if (this->member_node)
	{
		this->member_node->destroy();
		delete this->member_node;
	}
}
