#pragma once

#include <unordered_map>
#include <string>

#include "symboltable.hpp"
#include "object.hpp"
#include "types.hpp"
#include "error.hpp"
#include "token.hpp"
#include "node.hpp"

/*******************************************************************************************/
/******************************************(Module)*****************************************/
/*******************************************************************************************/

class Module: public Object
{
public:
	Module(const std::string& module_name, const std::string& module_path);

	bool __bool__() const override { return true; }
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	std::string module_name;
	std::string module_path;
};

/*******************************************************************************************/
/*******************************************(Class)*****************************************/
/*******************************************************************************************/
class Class: public Object
{
public:
	Class(const std::string& class_name = "");
	inline void set(const std::string& class_name) { this->class_name = class_name;  };
	inline const std::string& get() { return this->class_name; };

	bool __bool__() const override { return true; }
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	std::string class_name;
};

/*******************************************************************************************/
/***************************************(User_Object)***************************************/
/*******************************************************************************************/

class User_Object: public Object
{
public:
	User_Object(const Class& _class);
	User_Object(const User_Object& other);

	bool __bool__() const override { return true; }
	const Class& get_class() const { return this->_class; }
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	const Class& _class;
};

ObjectPtr make_user_object(const User_Object& user_object);

/*******************************************************************************************/
/********************************************(INT)******************************************/
/*******************************************************************************************/

class Int : public Object
{
public:
	Int(int64_t number = 0);

	void set(int64_t number);
	int64_t get() const;
	/************************************************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	int64_t number;
};

/*******************************************************************************************/
/*******************************************(FLOAT)*****************************************/
/*******************************************************************************************/

class Float : public Object
{
public:
	Float(long double number = 0.0);

	void set(const long double& number);
	long double get() const;
	/************************************************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	long double number;
};

/*******************************************************************************************/
/*******************************************(STRING)****************************************/
/*******************************************************************************************/

class Str : public Object
{
public:
	Str(const std::string& str = "");

	friend inline std::ostream& operator<<(std::ostream& out, const Str& str);
	std::string get() const;
	/*************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	std::string str;
};

/*******************************************************************************************/
/********************************************(BOOL)*****************************************/
/*******************************************************************************************/

class Bool : public Object
{
public:
	Bool(bool data);

	bool get() const;
	/************************************************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	bool data;
};

/*******************************************************************************************/
/********************************************(LIST)*****************************************/
/*******************************************************************************************/

class List: public Object
{
public:
	List(const ObjectVector& objects);
	/************************************************************************/
	void append(ObjectPtr& obj);
	void append(ObjectPtr obj);
	void remove(uint64_t index);
	/************************************************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	ObjectVector objects;
};

/*******************************************************************************************/
/*******************************************(TUPLE)*****************************************/
/*******************************************************************************************/

class Tuple: public Object
{
public:
	Tuple(const ObjectVector& elements);
	inline const ObjectVector& get() const { return this->objects; }
	/************************************************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	ObjectVector objects;
};

/*******************************************************************************************/
/*******************************************(ARRAY)*****************************************/
/*******************************************************************************************/

class Array : public Object
{
public:
	Array(const ObjectVector& objects);
	/************************************************************************/
	const ObjectVector& get_data() const;
	/************************************************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	ObjectVector objects;
};

/*******************************************************************************************/
/********************************************(NONE)*****************************************/
/*******************************************************************************************/

class None : public Object
{
public:
	None();
	/************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
};

/*******************************************************************************************/
/*******************************************(ERROR)*****************************************/
/*******************************************************************************************/

class Error : public Object, public Errorr
{
public:
	Error(Position start, Position end, ErrorType type, std::string details);
	/************************************/
	Position get_start_pos() const { return this->pos_start; };
	Position get_end_pos() const { return this->pos_end; };
	std::string get_details() const { return this->details; };
	/************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
};

/*******************************************************************************************/
/*******************************************(POINTER)***************************************/
/*******************************************************************************************/

class Pointer: public Object
{
public:
	Pointer(const ObjectPtr& object);
	/************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	WeakObject object;
};

/*******************************************************************************************/
/******************************************(FUNCTION)***************************************/
/*******************************************************************************************/

struct FinalResult;
class BuiltInFunction;
class Function;

class BaseFunction : public Object
{
public:
	BaseFunction(
		const std::string& fun_name,
		uint64_t arg_count,
		uint64_t co_argcount,
		uint64_t default_count
	);
	BaseFunction(const std::string& fun_name, BuiltInFunction* fun);
	BaseFunction(const std::string& fun_name, Function* fun);
	~BaseFunction() { if (this->function) delete this->function; }

	virtual inline ObjectPtr run(ObjectVector& args) { return this->function->evaluate(this->get_symbols(), args); };
	virtual ObjectPtr evaluate(SymbolTable& parent, ObjectVector& args);
	virtual std::string __str__() const override { return this->function->__str__(); }
	virtual std::string __info__() const override { return this->function->__info__(); }
	bool __bool__() const override { return true; }
	void init_class_built_in() override;
public:
	std::string fun_name;
protected:
	uint64_t arg_count;
	uint64_t co_argcount;
	uint64_t default_count;
private:
	BaseFunction* function;
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
	ObjectPtr evaluate(SymbolTable& parent, ObjectVector& params);
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
	ObjectPtr evaluate(SymbolTable& parent, ObjectVector& params) override;
public:
	std::string __str__() const override;
	std::string __info__() const override;
private:
	std::vector<Fun::Argument> parameters;
	NodeVector body;
};

/*******************************************************************************************/
/****************************************(FILE STREAM)**************************************/
/*******************************************************************************************/

#include <fstream>

class FileStream : public Object
{
public:
	FileStream(const std::string& file_name, const std::string& mode);
	/************************************/
	inline std::string get_file_name() const { return this->file_name; };
	inline std::string get_mode() const { return this->mode; };
	/************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	std::fstream file;
	std::string file_name;
	std::string mode;
	static const std::unordered_map<std::string, std::ios_base::openmode> mode_map;
};
