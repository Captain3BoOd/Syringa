#include "symboltable.hpp"
#include "objects.hpp"
#include "help.hpp"

SymbolTable::SymbolTable(SymbolTable* parent):
	parent(parent)
{}

SymbolTable::SymbolTable(const SymbolTable& other):
	parent(other.parent)
{
	for (auto& [name, var]: other.variables)
		this->variables.insert({ name, var });
}

void SymbolTable::add(const std::string& var_name, ObjectPtr value, bool is_const)
{
	LANG_ASSERT(value, "ObjectPtr must have a value");

	SymbolInfo::VarType var_type = is_const ? SymbolInfo::VarType::CONST : SymbolInfo::VarType::VARIABLE;
	if (value->get_type() == ObjectType::FUNCTION) var_type = SymbolInfo::VarType::FUNCTION;
	else if (value->get_type() == ObjectType::CLASS) var_type = SymbolInfo::VarType::CLASS;
	else if (value->get_type() == ObjectType::MODULE) var_type = SymbolInfo::VarType::MODULE;

	SymbolInfo symbol{ var_type, AssignObjects(value) };
	this->variables.insert({ var_name, symbol });
}

void SymbolTable::ref(const std::string& var_name, ObjectPtr value, bool is_const)
{
	LANG_ASSERT(value, "ObjectPtr must have a value");

	SymbolInfo::VarType var_type = is_const ? SymbolInfo::VarType::CONST : SymbolInfo::VarType::VARIABLE;
	if (value->get_type() == ObjectType::FUNCTION) var_type = SymbolInfo::VarType::FUNCTION;
	else if (value->get_type() == ObjectType::CLASS) var_type = SymbolInfo::VarType::CLASS;

	SymbolInfo symbol{ var_type, value };
	this->variables.insert({ var_name, symbol });
}

const ObjectPtr SymbolTable::get(const std::string& var_name)
{
	ObjectPtr result = nullptr;

	if (this->variables.find(var_name) != this->variables.end()) result = this->variables[var_name].object;
	else if (this->parent) result = this->parent->get(var_name);

	return result;
}

SymbolTable::AssignResult SymbolTable::set(const std::string& var_name, ObjectPtr value)
{
	LANG_ASSERT(value, "var must have a value");
	LANG_ASSERT(!var_name.empty(), "var must have a name");

	AssignResult result = AssignResult::NOT_FOUNDED;
	if (this->variables.find(var_name) != this->variables.end())
	{
		SymbolInfo& var = this->variables[var_name];
		if (var.type == SymbolInfo::VarType::VARIABLE)
		{
			var.object = AssignObjects(value);
			result = AssignResult::SUCCESS;
		}
		else if (var.type == SymbolInfo::VarType::CONST) result = AssignResult::CONST;
		else if (var.type == SymbolInfo::VarType::CLASS) result = AssignResult::CLASS;
		else if (var.type == SymbolInfo::VarType::MODULE) result = AssignResult::MODULE;
		else if (var.type == SymbolInfo::VarType::FUNCTION) result = AssignResult::FUNCTION;
	} else if (this->parent) result = this->parent->set(var_name, value);

	return result;
}

void SymbolTable::add(const SymbolTable& other)
{
	for (auto& [name, var] : other.variables)
	{
		LANG_ASSERT(var.object, "object must have a value");

		SymbolInfo symbol = var;
		symbol.object = AssignObjects(var.object);

		LANG_ASSERT(symbol.object, "object must have a value");
		this->variables.insert({ name, symbol });
	}
}

void SymbolTable::ref(const SymbolTable& other)
{
	for (auto& [name, var] : other.variables)
	{
		LANG_ASSERT(var.object, "object must have a value");

		SymbolInfo symbol = var;
		symbol.object = var.object;

		LANG_ASSERT(symbol.object, "object must have a value");
		this->variables.insert({ name, symbol });
	}
}

bool SymbolTable::remove(const std::string& var_name)
{
	if (this->variables.find(var_name) != this->variables.end())
	{
		this->variables.erase(var_name);
		return true;
	}

	return false;
}

bool SymbolTable::find(const std::string& var_name) const
{
	return this->variables.find(var_name) != this->variables.end();
}

bool SymbolTable::find_class(const std::string& var_name) const
{
	const auto& symbol = this->variables.find(var_name);
	if (symbol == this->variables.end()) return false;
	return symbol->second.type == SymbolInfo::VarType::CLASS;
}

bool SymbolTable::find_var(const std::string& var_name) const
{
	const auto& symbol = this->variables.find(var_name);
	if (symbol == this->variables.end()) return false;
	return (symbol->second.type == SymbolInfo::VarType::VARIABLE)
		|| (symbol->second.type == SymbolInfo::VarType::CONST);
}

bool SymbolTable::find_fun(const std::string& var_name) const
{
	const auto& symbol = this->variables.find(var_name);
	if (symbol == this->variables.end()) return false;
	return symbol->second.type == SymbolInfo::VarType::FUNCTION;
}
