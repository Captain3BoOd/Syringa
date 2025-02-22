#define INTERPRETER
#include "interpreter.hpp"
#include "objects.hpp"
#undef INTERPRETER

const Node* current_node = nullptr;
extern SymbolTable global_symbol_table;

Interpreter::Interpreter(SymbolTable* symbol_table_parent):
	symbol_table(symbol_table_parent)
{}

Interpreter::Interpreter(Node* tree, SymbolTable* symbol_table_parent):
	symbol_table(symbol_table_parent)
{
	this->trees.emplace_back(tree);
}

Interpreter::Interpreter(const NodeVector& trees, SymbolTable* symbol_table_parent):
	symbol_table(symbol_table_parent),
	trees(trees)
{}

SymbolTable& Interpreter::get_vars()
{
	return this->symbol_table;
}

ObjectPtr Interpreter::interpret()
{
	ObjectPtr Result;

	for (Node* tree : this->trees)
	{
		InterpreterResult result = this->evaluate(tree);
		Result = result.Result;
	}
	return Result;
}

inline NodeVector lex_parse(const std::string& file_name, const std::string& code);
inline bool Write_file(std::filesystem::path path, std::string_view code);
inline std::pair<std::string, bool> Read_file(std::filesystem::path path);

extern std::vector<NodeVector> all_nodes;
InterpreterResult Interpreter::evaluate_module(Node* tree) ////////////NOT FINISHED////////////
{
	current_node = tree;
	/*****************************************************************************/
	Import* import_node = static_cast<Import*>(tree);
	/*****************************************************************************/
	const std::filesystem::path& file_path = import_node->module_path;
	const std::string file_name = file_path.string();
	const std::string file_name_stem = file_path.stem().string();
	const auto[code, status] = Read_file(file_path);
	/*****************************************************************************/
	if (!status) RunTimeError(import_node->start_pos, import_node->end_pos, "file " + file_name + "not found");
	/*****************************************************************************/
	if (code.empty()) return {};
	NodeVector trees = lex_parse((file_name), code);
	if (trees.empty()) return {};
	all_nodes.emplace_back(trees);
	/*****************************************************************************/
	Interpreter module_interpreter(trees, &global_symbol_table);
	SymbolTable& module_symbols = module_interpreter.get_vars();
	module_interpreter.interpret();
	/*****************************************************************************/
	ObjectPtr module = make_module(module_symbols, file_name, file_path.string());
	this->symbol_table.ref(file_name_stem, module, true);
	/*****************************************************************************/
	return {};
}

InterpreterResult Interpreter::evaluate_all()
{
	InterpreterResult Result;

	for (Node* tree : this->trees)
		Result = this->evaluate(tree);

	return Result;
}

InterpreterResult Interpreter::evaluate(Node* tree)
{
	current_node = tree;
	if (*tree == NodeType::NUMBER)
		return this->evaluate_number(tree);
	else if (*tree == NodeType::STRING)
		return this->evaluate_string(tree);
	else if (*tree == NodeType::LIST)
		return this->evaluate_list(tree);
	else if (*tree == NodeType::EXPRESSION)
		return this->evaluate_expression(tree);

	else if (*tree == NodeType::BLOCK)
		return this->evaluate_block(tree);
	else if (*tree == NodeType::IF_STATEMENT)
		this->evaluate_if(tree);
	else if (*tree == NodeType::FOR_STATEMENT)
		this->evaluate_for(tree);
	else if (*tree == NodeType::WHILE_STATEMENT)
		this->evaluate_while(tree);

	else if (*tree == NodeType::BREAK)
		return { nullptr, false, false, true };
	else if (*tree == NodeType::CONTINUE)
		return { nullptr, false, true, false };
	else if (*tree == NodeType::RETURN)
	{
		auto [result, is_return, is_continue, is_break] = this->evaluate(static_cast<Return*>(tree)->expr);
		return { result, true, false, false };
	}

	else if (*tree == NodeType::IMPORT)
		return this->evaluate_module(tree);

	else if (*tree == NodeType::CLASS)
		this->evaluate_class_def(tree, this->symbol_table);
	else if (*tree == NodeType::CLASS_CALL)
		return this->evaluate_class_call(tree, this->symbol_table);
	else if (*tree == NodeType::FUN)
		this->evaluate_fun_def(tree, this->symbol_table);
	else if (*tree == NodeType::ACCESS)
		return this->evaluate_variable_access(tree, this->symbol_table);
	else if (*tree == NodeType::FUN_CALL)
		return this->evaluate_fun_call(tree, this->symbol_table);
	else if (*tree == NodeType::ASSIGN)
		return this->evaluate_variable_assign(tree, this->symbol_table);

	return { };
}

void Interpreter::evaluate_class_def(Node* tree, SymbolTable& symboltable)
{
	current_node = tree;
	/*************************************************************/
	NClass* class_def_node = static_cast<NClass*>(tree);
	ObjectPtr classs = make_class(class_def_node->class_name, &symboltable);
	SymbolTable& class_symbol_table = classs->get_symbols();
	class_symbol_table.set_parent(&symboltable);

	if (symbol_table.find(class_def_node->class_name))
	{
		RunTimeError(class_def_node->start_pos,
			class_def_node->end_pos,
			class_def_node->class_name + " was declared before"
		);
	}

	/* evaluate inhr */
	// TODO

	/* evaluate symbol table */
	for (auto& variable: class_def_node->variables)
		this->evaluate_variable_assign(variable, class_symbol_table);

	for (auto& function: class_def_node->functions)
		this->evaluate_fun_def(function, class_symbol_table);

	for (auto& _classs: class_def_node->classes)
		this->evaluate_class_def(_classs, class_symbol_table);
	/***********************************************/
	symboltable.add(class_def_node->class_name, classs, true);
}

InterpreterResult Interpreter::evaluate_variable_access(Node* tree, SymbolTable& symboltable)
{
	current_node = tree;
	/*************************************************************/
	VariableAcess* access_node = static_cast<VariableAcess*>(tree);
	ObjectPtr value = symboltable.get(access_node->var_name);
	if (!value)
		RunTimeError(access_node->start_pos, access_node->end_pos, "Variable '" + access_node->var_name + "' not declared");

	return { value };
}

InterpreterResult Interpreter::evaluate_variable_assign(Node* tree, SymbolTable& symboltable)
{
	current_node = tree;
	/*************************************************************/
	VariableAssign* assign_node = static_cast<VariableAssign*>(tree);
	ObjectPtr result = make_none();

	if (assign_node->value)
	{
		auto [res, _, __, ___] = this->evaluate(assign_node->value);
		result = res;
	}

	if (assign_node->declare)
	{
		if (symboltable.find(assign_node->var_name))
		{
			RunTimeError(
				assign_node->start_pos, assign_node->end_pos, assign_node->var_name + " was declared before"
			);
		} else symboltable.add(assign_node->var_name, result, assign_node->is_const);
	}
	else
	{
		SymbolTable::AssignResult status = symboltable.set(assign_node->var_name, result);
		if (status == SymbolTable::AssignResult::NOT_FOUNDED)
		{
			RunTimeError(
				assign_node->start_pos,
				assign_node->end_pos,
				"variable " + assign_node->var_name + " not declared"
			);
		}
		else if (status != SymbolTable::AssignResult::SUCCESS)
		{
			static const std::unordered_map<SymbolTable::AssignResult, std::string> types = {
				{ SymbolTable::AssignResult::FUNCTION, "function" },
				{ SymbolTable::AssignResult::MODULE, "module" },
				{ SymbolTable::AssignResult::CONST, "const" }
			};

			RunTimeError(
				assign_node->start_pos,
				assign_node->end_pos,
				"variable " + assign_node->var_name + " is a " + types.at(status)
			);
		}
		return { symboltable.get(assign_node->var_name) };
	}
	return { };
}

void Interpreter::evaluate_fun_def(Node* tree, SymbolTable& symboltable)
{
	current_node = tree;
	/*************************************************************/
	Fun* fun_def_node = static_cast<Fun*>(tree);

	if (symboltable.find_fun(fun_def_node->name))
	{
		ObjectPtr function = symboltable.get(fun_def_node->name);
		BaseFunction* _function = static_cast<BaseFunction*>(function.get());
		LANG_ASSERT(_function, "function didn't casted");
		/*******************************************************************************/
		Function* user_fun = new Function(
			fun_def_node->name,
			fun_def_node->elements,
			fun_def_node->co_argcount,
			fun_def_node->default_count,
			fun_def_node->body
		);
		LANG_ASSERT(user_fun, "fun object must have a value");
		BaseFunction* overloaded_part = new BaseFunction(fun_def_node->name, user_fun);
		/*******************************************************************************/
		_function->add_overloading(overloaded_part);
	}
	else
	{
		ObjectPtr user_fun = make_function(
			fun_def_node->name,
			fun_def_node->elements,
			fun_def_node->co_argcount,
			fun_def_node->default_count,
			fun_def_node->body
		);
		LANG_ASSERT(user_fun, "fun object must have a value");
		symboltable.add(fun_def_node->name, user_fun, true);
	}
}

template<bool is_class>
InterpreterResult Interpreter::evaluate_fun_call(Node* tree, SymbolTable& symboltable, ObjectPtr self)
{
	current_node = tree;
	/*************************************************************/
	if constexpr (is_class) LANG_ASSERT(self, "self object must have a value if fun was called by a class");

	FunCall* fun_call_node = static_cast<FunCall*>(tree);
	ObjectPtr classs = nullptr;
	ObjectPtr function = symboltable.get(fun_call_node->name);
	bool is_constructor = false;
l:
	if (!function)
	{
		if (is_constructor) return { classs };
		RunTimeError(fun_call_node->start_pos, fun_call_node->end_pos, "Function '" + fun_call_node->name + "' not declared");
	}

	if (*function == ObjectType::CLASS)
	{
		classs = make_instance(function);
		function = function->get("__init__");
		is_constructor = true;
		goto l;
	}
	else if (*function != ObjectType::FUNCTION)
		RunTimeError(fun_call_node->start_pos, fun_call_node->end_pos, "'" + fun_call_node->name + "' not a function");

	BaseFunction* fun = static_cast<BaseFunction*>(function.get());
	if (!fun)
		RunTimeError(fun_call_node->start_pos, fun_call_node->start_pos, "Function " + fun_call_node->name + " doesn't have body");

	ObjectVector params;
	if constexpr (is_class) params.emplace_back(self);
	if (classs) params.emplace_back(classs);

	for (auto& parameter: fun_call_node->parameters)
	{
		InterpreterResult res = this->evaluate(parameter);
		params.emplace_back(res.Result);
	}
	ObjectPtr result = fun->evaluate(params, &symboltable);
	if (is_constructor) result = classs;

	return { result };
}

InterpreterResult Interpreter::evaluate_class_call(Node* tree, SymbolTable& symboltable)
{
	current_node = tree;
	/*************************************************************/
	ClassCall* class_call_node = static_cast<ClassCall*>(tree);
	ObjectPtr self = this->evaluate(class_call_node->class_node).Result;
	if (!self) RunTimeError(class_call_node->start_pos, class_call_node->end_pos, "class must have self");
	if (!self->get_symbols().get_parent() && *self == ObjectType::OBJECT) self->get_symbols().set_parent(&symboltable); // temp solution

	SymbolTable& class_vars = self->get_symbols();
	ObjectPtr result = nullptr;
	if (*class_call_node->member_node == NodeType::ACCESS)
		result = this->evaluate_variable_access(class_call_node->member_node, class_vars).Result;
	else if (*class_call_node->member_node == NodeType::ASSIGN)
		result = this->evaluate_variable_assign(class_call_node->member_node, class_vars).Result;
	else if (*class_call_node->member_node == NodeType::CLASS_CALL)
		result = this->evaluate_class_call(class_call_node->member_node, class_vars).Result;
	else if (*class_call_node->member_node == NodeType::FUN_CALL)
		if (*self == ObjectType::MODULE) result = this->evaluate_fun_call(class_call_node->member_node, class_vars).Result;
		else result = this->evaluate_fun_call<true>(class_call_node->member_node, class_vars, self).Result;
	else if (*class_call_node->member_node == NodeType::EXPRESSION)
		result = this->evaluate_expression(class_call_node->member_node).Result;
	else LANG_ASSERT(false, "a new Node type");

	return { result };
}

InterpreterResult Interpreter::evaluate_number(Node* node)
{
	Number* number_node = static_cast<Number*>(node);

	if (number_node->is_float) return { make_float(std::atof(number_node->value.c_str())) };
	return { make_int(std::atoll(number_node->value.c_str())) };
}

InterpreterResult Interpreter::evaluate_string(Node* node)
{
	String* string_node = static_cast<String*>(node);
	return { make_str(string_node->value) };
}

InterpreterResult Interpreter::evaluate_list(Node* node)
{
	NList* list_node = static_cast<NList*>(node);
	ObjectVector elements;

	for (const auto& element : list_node->elements)
	{
		InterpreterResult result = this->evaluate(element);
		elements.emplace_back(result.Result);
	}

	return { make_list(elements) };
}

static const std::unordered_map<TokenType, std::string> operators = {
	{ TokenType::TT_MINUS, "__minus__" },
	{ TokenType::TT_PLUS, "__plus__" },
	{ TokenType::TT_MUL, "__mul__" },
	{ TokenType::TT_DIV, "__div__" },
	{ TokenType::TT_PLUS_EQUAL, "__plus_eq__" },
	{ TokenType::TT_MINUS_EQUAL, "__minus_eq__" },
	{ TokenType::TT_EQUAL, "__eq__" },
	{ TokenType::TT_NOT_EQUAL, "__not_eq__" },
	{ TokenType::TT_DIV_EQUAL, "__div_eq__" },
	{ TokenType::TT_MUL_EQUAL, "__mul_eq__" },
	{ TokenType::TT_AND, "__and__" },
	{ TokenType::TT_OR, "__or__" },
	{ TokenType::TT_NOR, "__nor__" },
	{ TokenType::TT_BIT_OR, "__b_or__" },
	{ TokenType::TT_BIT_AND, "__b_and__" },
	{ TokenType::TT_GREATER_THAN, "__gr__" },
	{ TokenType::TT_GREATER_THAN_EQUAL, "__gr_eq__" },
	{ TokenType::TT_LESS_THAN, "__less__" },
	{ TokenType::TT_LESS_THAN_EQUAL, "__less_eq__" },
	{ TokenType::TT_MINUS_MINUS, "__min_min__" },
	{ TokenType::TT_PLUS_PLUS, "__plus_plus__" },
	{ TokenType::TT_BIT_NOT, "__b_not__" },
	{ TokenType::TT_MODULO, "__mod__" },
	{ TokenType::TT_NOT, "__not__" }
};

InterpreterResult Interpreter::evaluate_expression(Node* node)
{
	Expression* expression_node = static_cast<Expression*>(node);
	Node* left = expression_node->left;
	Node* right = expression_node->right;
	TokenType op = expression_node->op;
	/************************************************************************************/
	LANG_ASSERT(operators.find(op) != operators.end(), "operator token not in operators list");
	/************************************************************************************/
	InterpreterResult result;
	InterpreterResult left_result = this->evaluate(left);
	ObjectPtr left_result_obj = left_result.Result;
	ObjectPtr op_fun = left_result_obj->get(operators.at(op));
	if (*expression_node == ExpressionType::UNARY_OPERATOR)
	{
		if (op == TokenType::TT_MINUS) op_fun = left_result_obj->get("__min__");
		else if (op == TokenType::TT_BIT_AND) op_fun = left_result_obj->get("__pos__");
		else LANG_ASSERT(false, "Unknown operator");
	}
	/************************************************************************************/
	if (!op_fun)
		RunTimeError(left->start_pos, left->end_pos, "Unsupported operator for this type");
	if (*op_fun != ObjectType::FUNCTION)
		RunTimeError(left->start_pos, left->end_pos, "operator function must be function");
	/************************************************************************************/
	BaseFunction& _op_fun = *static_cast<BaseFunction*>(op_fun.get());
	if (*expression_node == ExpressionType::BIN_OPERATOR)
	{
		InterpreterResult right_result = this->evaluate(right);
		ObjectPtr right_result_obj = right_result.Result;
		ObjectVector params = { left_result_obj, right_result_obj };
		/*********************************************************************************/
		result.Result = _op_fun.evaluate(params, &left_result_obj->get_symbols());
	}
	else if (*expression_node == ExpressionType::UNARY_OPERATOR)
	{
		ObjectVector params = { left_result_obj };
		/*********************************************************************************/
		result.Result = _op_fun.evaluate(params, &left_result_obj->get_symbols());
	} else LANG_ASSERT(false, "Unsupported expression");

	return result;
}

InterpreterResult Interpreter::evaluate_block(Node* node)
{
	Block* block_node = static_cast<Block*>(node);
	Interpreter block_interpreter(block_node->expressions, &this->symbol_table);

	return { block_interpreter.interpret(), false, false, false};
}

InterpreterResult Interpreter::evaluate_if(Node* node)
{
	IfStatment* if_node = static_cast<IfStatment*>(node);
	Block* block_node = static_cast<Block*>(if_node->block);
	Interpreter local_interpreter(block_node->expressions, &this->symbol_table);

	if (local_interpreter.evaluate(if_node->condition).Result->__bool__()) return local_interpreter.evaluate_all();
	else if (if_node->elsee) local_interpreter.evaluate(if_node->elsee);

	return {};
}

InterpreterResult Interpreter::evaluate_for(Node* node)
{
	ForStatment* for_node = static_cast<ForStatment*>(node);
	Block* block_node = static_cast<Block*>(for_node->block);

	Interpreter local_interpreter(block_node->expressions, &this->symbol_table);
	auto [counter, is_return, is_continue, is_break] = local_interpreter.evaluate(for_node->counter);

	while (local_interpreter.evaluate(for_node->condition).Result->__bool__())
	{
		auto [result, is_return, is_continue, is_break] = local_interpreter.evaluate_all();

		if (is_return) return { result, is_return };
		else if (is_continue) continue;
		else if (is_break) break;

		local_interpreter.evaluate(for_node->increase);
	}

	return {};
}

InterpreterResult Interpreter::evaluate_while(Node* node)
{
	WhileStatment* while_node = static_cast<WhileStatment*>(node);
	Block* block_node = static_cast<Block*>(while_node->block);

	Interpreter local_interpreter(block_node->expressions, &this->symbol_table);
	bool is_return = false;

	while (local_interpreter.evaluate(while_node->condition).Result->__bool__())
	{
		auto [result, is_return, is_continue, is_break] = local_interpreter.evaluate_all();

		if (is_return) return { result, is_return };
		else if (is_continue) continue;
		else if (is_break) break;
	}

	return { nullptr, is_return };
}
