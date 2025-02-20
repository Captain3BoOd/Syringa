#define INTERPRETER
#include "module.hpp"
#include "builtin.hpp"
#undef INTERPRETER

Module::Module(const SymbolTable& symbol_table, const std::string& module_name, const std::string& module_path) :
	Object(ObjectType::MODULE),
	symbols(&Object::get_symbols()),
	module_name(module_name),
	module_path(module_path)
{
	this->symbols.ref(symbol_table);
	this->symbols.set_parent(symbol_table.get_parent());
}

std::string Module::__info__() const
{
	return std::string();
}

std::string Module::__str__() const
{
	return std::string();
}
