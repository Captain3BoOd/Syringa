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

class Class;

class Object
{
public:
	Object(const ObjectType type, const Class* _class = nullptr);
	virtual ~Object() = default;
	/************************************************************************/
	const Object& operator=(const Object& other);
	inline ObjectType get_type() const { return this->type; }
	inline const Class* get_class() const { return this->_class; }
	inline operator bool() const { return this->type != ObjectType::NONe; }
	inline bool operator==(const ObjectType object_type) const { return (this->type == object_type) && (this->type != ObjectType::NONe); }
	/************************************************************************/
	template<bool is_const = true>
	inline void define(const std::string& name, ObjectPtr value) { this->symbols.add(name, value, is_const); };
	inline virtual void set(const std::string& name, ObjectPtr value) { this->symbols.set(name, value); };
	inline virtual const ObjectPtr get(const std::string& name) { return this->symbols.get(name); };
	inline virtual void remove(const std::string& name) { this->symbols.remove(name); };
	inline virtual const SymbolTable& get_symbols() const { return this->symbols; }
	inline virtual SymbolTable& get_symbols() { return this->symbols; }
	/************************************************************************/
	virtual ObjectPtr evaluate(ObjectVector& args, SymbolTable* parent = nullptr);
	virtual std::string __info__() const = 0;
	virtual std::string __str__() const = 0;
	virtual void init_class_built_in();
	virtual bool __bool__() const = 0;
private:
	const Class* _class;
	SymbolTable symbols;
	ObjectType type;
};
