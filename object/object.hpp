#pragma once

#include <string>

#include "symboltable.hpp"
#include "types.hpp"
#include "node.hpp"

enum class ObjectType: uint8_t
{
	NONe = 0,
	OBJECT,
	BOOL,
	INT,
	POINTER,
	STRING,
	MODULE,
	LIST,
	TUPLE,
	ARRAY,
	FLOAT,
	FUNCTION,
	ERROR,
	CLASS,
	FILE_STREAM,
	NONE
};

class Object
{
public:
	Object(const ObjectType type);
	Object(const Object& other);
	virtual ~Object() = default;
	/************************************************************************/
	bool operator==(const ObjectType object_type) const;
	ObjectType get_type() const;
	const Object& operator=(const Object& other);
	operator bool() const;
	/************************************************************************/
	template<bool is_const = true>
	inline void define(const std::string& name, ObjectPtr value) { this->symbols.add(name, value, is_const); };
	inline void set(const std::string& name, ObjectPtr value) { this->symbols.set(name, value); };
	inline const ObjectPtr get(const std::string& name) { return this->symbols.get(name); };
	inline void remove(const std::string& name) { this->symbols.remove(name); };
	inline const SymbolTable& get_symbols() const { return this->symbols; }
	inline SymbolTable& get_symbols() { return this->symbols; }
	/************************************************************************/
	virtual inline ObjectPtr run(ObjectVector& args) { return nullptr; }
	virtual void init_class_built_in() = 0;
	virtual bool __bool__() const = 0;
	virtual std::string __info__() const = 0;
	virtual std::string __str__() const = 0;
private:
	SymbolTable symbols;
	ObjectType type;
};

/*******************************************************************************************/
/*****************************************(FUNCTIONS)***************************************/
/*******************************************************************************************/
ObjectPtr AssignObjects(const ObjectPtr& obj);
inline bool check_type(const ObjectPtr& obj, ObjectType type) { return (*obj) == type; }
inline void AssignObjects(ObjectPtr& obj1, const ObjectPtr& obj2) { obj1 = AssignObjects(obj2); };
/******************************/
ObjectPtr make_function(const std::string& fun_name, const FunctionType& function, uint64_t params_number = 0, uint64_t co_argcount = 0, const ObjectVector& defaults = {});
ObjectPtr make_function(const std::string& fun_name, const std::vector<Fun::Argument>& parameters, uint64_t co_argcount, uint64_t default_count, const NodeVector& body);
ObjectPtr make_error(const Position& start, const Position& end, ErrorType type, std::string details);
ObjectPtr make_class(const std::string& class_name, SymbolTable* parent = nullptr);
ObjectPtr make_file_stream(const std::string& file_name, const std::string& mode);
ObjectPtr make_user_object(const ObjectPtr& _class);
ObjectPtr make_array(const ObjectVector& elements);
ObjectPtr make_tuple(const ObjectVector& elements);
ObjectPtr make_list(const ObjectVector& elements);
ObjectPtr make_pointer(const ObjectPtr& object);
ObjectPtr make_float(const long double& num);
ObjectPtr make_str(const std::string& str);
ObjectPtr make_bool(bool data);
ObjectPtr make_int(int64_t num);
ObjectPtr make_none();