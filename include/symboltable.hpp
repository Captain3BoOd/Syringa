#pragma once

#include "types.hpp"

struct SymbolInfo
{
	enum class VarType : uint8_t
	{
		NONE = 0,
		VARIABLE,
		FUNCTION,
		CONST,
		CLASS
	};

	VarType type = VarType::NONE;
	ObjectPtr object;
};

class SymbolTable
{
public:
	enum class AssignResult: uint8_t
	{
		NOT_FOUNDED,
		FUNCTION,
		SUCCESS,
		CLASS,
		CONST
	};
public:
	SymbolTable(SymbolTable* parent = nullptr);
	SymbolTable(const SymbolTable& other);

	const ObjectPtr get(const std::string& var_name);
	AssignResult set(const std::string& var_name, ObjectPtr value);
	inline void set_parent(SymbolTable* parent) { this->parent = parent; };
	inline SymbolTable* get_parent() const { return this->parent; };

	void add(const std::string& var_name, ObjectPtr value, bool is_const);
	void add(const SymbolTable& other);

	void ref(const std::string& var_name, ObjectPtr value, bool is_const);
	void ref(const SymbolTable& other);

	bool remove(const std::string& var_name);
	bool find(const std::string& var_name) const;
	bool find_class(const std::string& var_name) const;
	bool find_var(const std::string& var_name) const;
	bool find_fun(const std::string& var_name) const;
private:
	std::unordered_map<std::string, SymbolInfo> variables;
	SymbolTable* parent;
};