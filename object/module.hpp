#pragma once

#include "object.hpp"
#include "symboltable.hpp"

class Module : public Object
{
public:
	Module(const SymbolTable& symbol_table, const std::string& module_name, const std::string& module_path);

	inline void set(const std::string& name, ObjectPtr value) override { this->symbols.set(name, value); };
	inline const ObjectPtr get(const std::string& name) override { return this->symbols.get(name); };
	inline void remove(const std::string& name) override { this->symbols.remove(name); };
	inline const SymbolTable& get_symbols() const override { return this->symbols; }
	inline SymbolTable& get_symbols() override { return this->symbols; }

	bool __bool__() const override { return true; }
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	SymbolTable symbols;
	std::string module_name;
	std::string module_path;
};
