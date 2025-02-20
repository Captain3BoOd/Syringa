#pragma once

#include "object.hpp"
#include "make.hpp"

struct FinalResult;
class BuiltInFunction;
class Function;

class BaseFunction : public Object
{
public:
	struct BaseFunctionStruct
	{
		uint64_t arg_count;
		uint64_t co_argcount;
		uint64_t default_count;
		//std::vector<std::string> return_types;
		BaseFunction* function;
	};
	typedef std::unordered_map<uint64_t, BaseFunctionStruct> OverLoadingMap;
public:
	BaseFunction(
		const std::string& fun_name,
		uint64_t arg_count,
		uint64_t co_argcount,
		uint64_t default_count
	);
	BaseFunction(const std::string& fun_name, BuiltInFunction* fun);
	BaseFunction(const std::string& fun_name, Function* fun);
	inline ~BaseFunction()
	{
		for (auto& [_, fun] : this->overloading)
			if (fun.function) delete fun.function;
	}

	void add_overloading(BaseFunction* fun);
	virtual ObjectPtr evaluate(ObjectVector& args, SymbolTable* parent);
	virtual std::string __str__() const override;
	virtual std::string __info__() const override;
	bool __bool__() const override { return true; }
	void init_class_built_in() override;
public:
	std::string fun_name;
	uint64_t arg_count;
	uint64_t co_argcount;
	uint64_t default_count;
private:
	OverLoadingMap overloading;
};

// Built-in //
class BuiltInFunction : public BaseFunction
{
public:
	BuiltInFunction(
		const std::string& fun_name,
		const FunctionType& function,
		uint64_t params_number,
		uint64_t co_argcount,
		const ObjectVector& defaults
	);
	ObjectPtr evaluate(ObjectVector& args, SymbolTable* parent) override;
public:
	std::string __str__() const override;
	std::string __info__() const override;
private:
	const ObjectVector defaults;
	FunctionType function;
};

// Normal //
class Function : public BaseFunction
{
public:
	Function(
		const std::string& fun_name,
		const std::vector<Fun::Argument>& parameters,
		uint64_t co_argcount,
		uint64_t default_count,
		const NodeVector& body
	);
	ObjectPtr evaluate(ObjectVector& args, SymbolTable* parent) override;
public:
	std::string __str__() const override;
	std::string __info__() const override;
private:
	std::vector<Fun::Argument> parameters;
	NodeVector body;
};

