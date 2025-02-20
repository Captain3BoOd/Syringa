#define INTERPRETER
#include "function.hpp"
#include "interpreter.hpp"
#include "builtin.hpp"
#undef INTERPRETER

extern const Node* current_node;

BaseFunction::BaseFunction(
	const std::string& fun_name,
	uint64_t arg_count,
	uint64_t co_argcount,
	uint64_t default_count
):
	Object(ObjectType::FUNCTION),
	fun_name(fun_name),
	arg_count(arg_count),
	co_argcount(co_argcount),
	default_count(default_count)
{}

BaseFunction::BaseFunction(const std::string& fun_name, BuiltInFunction* fun):
	Object(ObjectType::FUNCTION),
	fun_name(fun_name),
	arg_count(fun->arg_count),
	co_argcount(fun->co_argcount),
	default_count(fun->default_count)
{
	LANG_ASSERT(fun, "fun object must have a value");

	const uint64_t overloadID = this->arg_count;
	if (this->overloading.find(overloadID) == this->overloading.end())
	{
		BaseFunctionStruct fun_struct{ arg_count, co_argcount, default_count, fun };
		this->overloading.insert({ overloadID, fun_struct });
	} else RunTimeError(current_node->start_pos, current_node->end_pos, "function '" + this->fun_name + "' this overloading was declared before");
}

BaseFunction::BaseFunction(const std::string& fun_name, Function* fun):
	Object(ObjectType::FUNCTION),
	fun_name(fun_name),
	arg_count(fun->arg_count),
	co_argcount(fun->co_argcount),
	default_count(fun->default_count)
{
	LANG_ASSERT(fun, "fun object must have a value");

	const uint64_t overloadID = this->arg_count;
	if (this->overloading.find(overloadID) == this->overloading.end())
	{
		BaseFunctionStruct fun_struct{ arg_count, co_argcount, default_count, fun };
		this->overloading.insert({ overloadID, fun_struct });
	} else RunTimeError(current_node->start_pos, current_node->end_pos, "In function '" + this->fun_name + "' this overloading was declared before");
}

void BaseFunction::add_overloading(BaseFunction* fun)
{
	LANG_ASSERT(fun, "fun object must have a value");

	const uint64_t overloadID = fun->arg_count;
	if (this->overloading.find(overloadID) == this->overloading.end())
	{
		BaseFunctionStruct fun_struct{ fun->arg_count, fun->co_argcount, fun->default_count, fun };
		this->overloading.insert({ overloadID, fun_struct });
	} else RunTimeError(current_node->start_pos, current_node->end_pos, "In function '" + this->fun_name + "' this overloading was declared before");
}

ObjectPtr BaseFunction::evaluate(ObjectVector& args, SymbolTable* parent)
{
	// TODO make function overloading better
	LANG_ASSERT(this->overloading.size(), "overloading member has 0 element");
	/************************************************************************************/
	BaseFunctionStruct* fun_struct = nullptr;
	if (this->overloading.size() > 1)
	{
		if (this->overloading.find(args.size()) == this->overloading.end())
		{
			RunTimeError(current_node->start_pos, current_node->end_pos, "");
		} else fun_struct = &this->overloading[args.size()];
	} else fun_struct = &this->overloading.begin()->second;
	LANG_ASSERT(fun_struct, "fun struct is nullptr");
	LANG_ASSERT(fun_struct->function, "fun body is nullptr");
	/***********************************************************************************/
	auto& [_arg_count, _co_argcount, _default_count, fun_ptr] = *fun_struct;
	LANG_ASSERT(fun_ptr, "fun body is nullptr");

	if (_arg_count && args.empty())
	{
		RunTimeError(current_node->start_pos, current_node->end_pos,
			"Function '" + this->fun_name + "' "
			"Expected '" + std::to_string(_arg_count)
			+ (_default_count ? (" or " + std::to_string(_arg_count - _default_count)) : "")
			+ "' argument but '0' was send"
		);
		return make_none();
	}
	/*****************************************************************************/
	ObjectVector new_args;
	const bool is_arb_count = _arg_count - (_co_argcount + _default_count);
	ObjectVecIter end_co_args = args.begin() + _co_argcount;
	/*****************************************************************************/
	if (_co_argcount) new_args.assign(args.begin(), end_co_args);
	/*****************************************************************************/
	if (is_arb_count)
	{
		ObjectPtr arb_args = make_array(ObjectVector(end_co_args, args.end()));
		/**************************************************/
		LANG_ASSERT(arb_args, "arb args must have a value");
		new_args.emplace_back(arb_args);
	}
	/*****************************************************************************/
	args.clear();
	/*****************************************************************************/
	if (new_args.size() != _arg_count)
	{
		RunTimeError(current_node->start_pos, current_node->end_pos,
			"Function '" + this->fun_name + "' "
			"Expected '" + std::to_string(_arg_count)
			+ (_default_count ? (" or " + std::to_string(_arg_count - _default_count)) : "")
			+ "' argument but '" + std::to_string(new_args.size()) + "' was send"
		);
		return make_none();
	}
	/*****************************************************************************/
	return fun_ptr->evaluate(new_args, parent);
}

std::string BaseFunction::__str__() const
{
	std::string result;
	for (size_t counter = 0; const auto& [_, fun] : this->overloading)
	{
		result += "number: " + std::to_string(counter) + "\n"
			+ fun.function->__str__() + "\n";

		counter++;
	}
	return result;
}

std::string BaseFunction::__info__() const
{
	const size_t size = this->overloading.size();
	std::string result = "class name 'Function'"
		"\nfunction name '" + this->fun_name + "'"
		"\nnumber: " + std::to_string(size) + "\n";

	for (size_t counter = 0; const auto & [_, fun] : this->overloading)
	{
		result += "number: " + std::to_string(counter) + "\n"
			+ fun.function->__info__() + "\n";

		counter++;
	}

	return result;
}

// Built-In //
BuiltInFunction::BuiltInFunction(
	const std::string& fun_name,
	const FunctionType& function,
	uint64_t params_number,
	uint64_t co_argcount,
	const ObjectVector& defaults
) :
	BaseFunction(fun_name, params_number, co_argcount, defaults.size()),
	defaults(defaults),
	function(function)
{
	LANG_ASSERT(this->function, "fun object must have a value");
}

ObjectPtr BuiltInFunction::evaluate(ObjectVector& args, SymbolTable* parent)
{
	return this->function(args);
}

std::string BuiltInFunction::__str__() const
{
	return "Built-In Function '" + this->fun_name + "' at <" + std::to_string((uint64_t)this) + '>';
}

std::string BuiltInFunction::__info__() const
{
	return
		"class name 'Function'"
		"\nfunction name '" + this->fun_name + "'"
		"\nfunction type : 'Built in Funtion'"
		"\nnumber of arguments = " + std::to_string(this->arg_count) +
		"\naddress = " + std::to_string(uint64_t(this)) +
		"\nsize in memory = " + std::to_string(sizeof(*this));
}

// Normal //
Function::Function(
	const std::string& fun_name,
	const std::vector<Fun::Argument>& parameters,
	uint64_t co_argcount,
	uint64_t default_count,
	const NodeVector& body
) :
	BaseFunction(fun_name, parameters.size(), co_argcount, default_count),
	parameters(parameters),
	body(body)
{}

ObjectPtr Function::evaluate(ObjectVector& args, SymbolTable* parent)
{
	Interpreter interpreter(this->body, parent);
	SymbolTable& fun_symbols = interpreter.get_vars();

	for (size_t i = 0; i < this->parameters.size(); i++)
	{
		if (this->parameters[i].arg == TokenType::TT_IDENTIFIER)
		{
			if (this->parameters[i].arg.value == "self") fun_symbols.ref(this->parameters[i].arg.value, args[i], true);
			else fun_symbols.add(this->parameters[i].arg.value, args[i], false);
		}
	}

	return interpreter.interpret();
}

std::string Function::__str__() const
{
	return "Function '" + this->fun_name + "' at <" + std::to_string((uint64_t)this) + '>';
}

std::string Function::__info__() const
{
	std::string arguments_name = "[";
	for (const Fun::Argument& parameter : this->parameters)
		arguments_name += parameter.arg.value + ", ";

	if (!this->parameters.empty()) arguments_name.pop_back(), arguments_name.pop_back();
	arguments_name += "]";

	return
		"class name 'Function'"
		"\nfunction name '" + this->fun_name + "'"
		"\nfunction type : 'User Funtion'"
		"\nnumber of arguments = " + std::to_string(this->arg_count) +
		"\narguments name" + arguments_name +
		"\naddress = " + std::to_string(uint64_t(this)) +
		"\nsize in memory = " + std::to_string(sizeof(*this));
}
