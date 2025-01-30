#define INTERPRETER
#include "object.hpp"
#include "others.hpp"
#include "help.hpp"
#include "builtin.hpp"
#include "interpreter.hpp"
#undef INTERPRETER

extern const Node* current_node;

#define SELF_DEF(name) this->define(#name, name)
#define MAKE_FUN(fun_name, params, co_arg, defaults, code) const ObjectPtr fun_name = make_function(#fun_name, std::function([](ObjectVector& args){code}), params, co_arg, defaults)
#define MAKE_FUN_(fun_name, params, co_arg, defaults, code)\
	const ObjectPtr fun_name = make_function(#fun_name,\
	std::function([&](ObjectVector& args) {\
		ObjectPtr self = args[0];\
		code\
	}\
), params, co_arg, defaults)

#define MAKE_CLASS(class_name) const ObjectPtr class_name = make_class(#class_name)

// TODO Fixing
void Module::init_class_built_in()
{
}

void Class::init_class_built_in()
{
}

void User_Object::init_class_built_in()
{
	/*********************************/
	// this->define("__class__", this->_class);
}

void Int::init_class_built_in()
{
	MAKE_FUN_(__plus__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(this->number + num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(this->number + num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(this->number + _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__minus__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(this->number - num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(this->number - num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(this->number - _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__mul__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(this->number * num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(this->number * num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(this->number * _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__div__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(this->number / num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(this->number / num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(this->number / _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__mod__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(this->number % num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			long double f_num = num.get();
			int64_t int_ = static_cast<int64_t>(num.get());
			return make_int(this->number % int_);
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(this->number % _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__plus_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(this->number += num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(this->number += num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(this->number += _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__b_and__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(this->number & num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			long double f_num = num.get();
			int64_t int_ = static_cast<int64_t>(num.get());
			return make_int(this->number & int_);
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(this->number & _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__b_or__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(this->number | num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			long double f_num = num.get();
			int64_t int_ = static_cast<int64_t>(num.get());
			return make_int(this->number | int_);
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(this->number | _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__nor__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(this->number ^ num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			long double f_num = num.get();
			int64_t int_ = static_cast<int64_t>(num.get());
			return make_int(this->number ^ int_);
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(this->number ^ _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__and__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(this->number && num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(this->number && num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(this->number && _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__or__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(this->number || num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(this->number || num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(this->number || _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(this->number == num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(this->number == num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(this->number == _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__not_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(this->number != num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(this->number != num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(this->number != _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__less__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(this->number < num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(this->number < num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(this->number < _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__less_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(this->number <= num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(this->number <= num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(this->number <= _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__gr__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(this->number > num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(this->number > num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(this->number > _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__gr_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(this->number >= num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(this->number >= num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(this->number >= _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__minus_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(this->number -= num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(this->number -= num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(this->number -= _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__div_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(this->number /= num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(this->number /= num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(this->number /= _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__mul_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(this->number *= num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(this->number *= num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(this->number *= _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__b_not__, 1, 1, {},
		return make_int(~this->number);
	);
	MAKE_FUN_(__not__, 1, 1, {},
		return make_bool(!this->number);
	);
	MAKE_FUN_(__plus_plus__, 1, 1, {},
		return make_int(this->number++);
	);
	MAKE_FUN_(__min_min__, 1, 1, {},
		return make_int(this->number--);
	);
	MAKE_FUN_(__min__, 1, 1, {},
		return make_int(-this->number);
	);
	/***************************/
	MAKE_FUN_(__float__, 1, 1, {},
		return make_float(this->number);
	);
	MAKE_FUN_(__bool__, 1, 1, {},
		return make_bool(this->number);
	);
	MAKE_FUN_(__str__, 1, 1, {},
		return make_str(this->__str__());
	);
	MAKE_FUN_(__info__, 1, 1, {},
		return make_str(this->__info__());
	);
	/*******************************************************************/
	SELF_DEF(__plus_plus__);
	SELF_DEF(__minus_eq__);
	SELF_DEF(__min_min__);
	SELF_DEF(__plus_eq__);
	SELF_DEF(__less_eq__);
	SELF_DEF(__not_eq__);
	SELF_DEF(__div_eq__);
	SELF_DEF(__mul_eq__);
	SELF_DEF(__gr_eq__);
	SELF_DEF(__minus__);
	SELF_DEF(__b_and__);
	SELF_DEF(__b_not__);
	SELF_DEF(__less__);
	SELF_DEF(__plus__);
	SELF_DEF(__b_or__);
	SELF_DEF(__not__);
	SELF_DEF(__mul__);
	SELF_DEF(__div__);
	SELF_DEF(__and__);
	SELF_DEF(__nor__);
	SELF_DEF(__mod__);
	SELF_DEF(__min__);
	SELF_DEF(__or__);
	SELF_DEF(__gr__);
	SELF_DEF(__eq__);
	/*****************************/
	SELF_DEF(__float__);
	SELF_DEF(__bool__);
	SELF_DEF(__info__);
	SELF_DEF(__str__);
}

void Float::init_class_built_in()
{
	MAKE_FUN_(__plus__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_float(this->number + num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(this->number + num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_float(this->number + _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__minus__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_float(this->number - num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(this->number - num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_float(this->number - _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__mul__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_float(this->number * num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(this->number * num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_float(this->number * _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__div__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_float(this->number / num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(this->number / num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_float(this->number / _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__mod__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		/*if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			int64_t i_num = num.get();
			long double float_ = static_cast<long double>(i_num);
			return make_float(this->number % float_);
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(this->number % num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_float(this->number % _bool.get());
		}
		/*******************************************************/
		/*ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();*/
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__plus_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(this->number += num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(this->number += num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(this->number += _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__and__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(this->number && num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(this->number && num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(this->number && _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__or__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(this->number || num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(this->number || num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(this->number || _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(this->number == num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(this->number == num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(this->number == _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__not_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(this->number != num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(this->number != num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(this->number != _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__less__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(this->number < num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(this->number < num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(this->number < _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__less_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(this->number <= num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(this->number <= num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(this->number <= _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__gr__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(this->number > num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(this->number > num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(this->number > _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__gr_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(this->number >= num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(this->number >= num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(this->number >= _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__minus_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_float(this->number -= num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(this->number -= num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_float(this->number -= _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__div_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_float(this->number /= num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(this->number /= num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_float(this->number /= _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__mul_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_float(this->number *= num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(this->number *= num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_float(this->number *= _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->run(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_(__not__, 1, 1, {},
		return make_bool(!this->number);
	);
	MAKE_FUN_(__plus_plus__, 1, 1, {},
		return make_float(this->number++);
	);
	MAKE_FUN_(__min_min__, 1, 1, {},
		return make_float(this->number--);
	);
	MAKE_FUN_(__min__, 1, 1, {},
		return make_float(-this->number);
	);
	/***************************/
	MAKE_FUN_(__int__, 1, 1, {},
		return make_int(this->number);
	);
	MAKE_FUN_(__bool__, 1, 1, {},
		return make_bool(this->number);
	);
	MAKE_FUN_(__str__, 1, 1, {},
		return make_str(this->__str__());
	);
	MAKE_FUN_(__info__, 1, 1, {},
		return make_str(this->__info__());
	);
	/*******************************************************************/
	SELF_DEF(__plus_plus__);
	SELF_DEF(__minus_eq__);
	SELF_DEF(__min_min__);
	SELF_DEF(__plus_eq__);
	SELF_DEF(__less_eq__);
	SELF_DEF(__not_eq__);
	SELF_DEF(__div_eq__);
	SELF_DEF(__mul_eq__);
	SELF_DEF(__gr_eq__);
	SELF_DEF(__minus__);
	SELF_DEF(__less__);
	SELF_DEF(__plus__);
	SELF_DEF(__not__);
	SELF_DEF(__mul__);
	SELF_DEF(__div__);
	SELF_DEF(__and__);
	SELF_DEF(__mod__);
	SELF_DEF(__min__);
	SELF_DEF(__or__);
	SELF_DEF(__gr__);
	SELF_DEF(__eq__);
	/*****************************/
	SELF_DEF(__bool__);
	SELF_DEF(__info__);
	SELF_DEF(__int__);
	SELF_DEF(__str__);
}

void Str::init_class_built_in()
{
	MAKE_FUN_(__str__, 1, 1, {},
		return make_str(this->__str__());
	);
	MAKE_FUN_(__info__, 1, 1, {},
		return make_str(this->__info__());
	);
	MAKE_FUN_(__append__, 2, 1, {},
		const Array* a = static_cast<Array*>(args[1].get());
		const ObjectVector & objects = a->get_data();

		for (const ObjectPtr& object : objects)
			this->str += object->get("__str__")->__str__();

		return make_none();
	);
	MAKE_FUN_(__plus__, 2, 2, {},
		const std::string arg_str = args[1]->get("__str__")->__str__();
		return make_str(this->str + arg_str);
	);
	MAKE_FUN_(__plus_eq__, 2, 2, {},
		const std::string arg_str = args[1]->get("__str__")->__str__();
		this->str += arg_str;

		return std::make_shared<Str>(*this);
	);
	MAKE_FUN_(__eq__, 2, 2, {},
		const std::string arg_str = args[1]->__str__();
		return make_bool(this->str == arg_str);
	);
	MAKE_FUN_(__index__, 2, 2, {},
		Tuple& range = *static_cast<Tuple*>(args[1].get());
		const ObjectVector& data = range.get();

		return make_str("");
	);
	/***********************************************************/
	this->define("__plus_eq__", __plus_eq__);
	this->define("__info__", __info__);
	this->define("append", __append__);
	this->define("__plus__", __plus__);
	this->define("__str__", __str__);
	this->define("__eq__", __eq__);
}

void Pointer::init_class_built_in()
{
}

void BaseFunction::init_class_built_in()
{
}

void List::init_class_built_in()
{
}

void Tuple::init_class_built_in()
{
}

void Array::init_class_built_in()
{
}

void Bool::init_class_built_in()
{
}

void None::init_class_built_in()
{
}

void Error::init_class_built_in()
{
}

void FileStream::init_class_built_in()
{
	MAKE_FUN_(is_open, 0, 0, {},
		return make_bool(this->file.is_open());
	);
	MAKE_FUN_(open, 2, 2, {},
		ObjectPtr file_name_ = args[1];
		ObjectPtr mode_ = args[1];
		if (*file_name_ != ObjectType::STRING) RunTimeError(current_node->start_pos, current_node->end_pos, "file name must be a string");
		if (*mode_ != ObjectType::STRING) RunTimeError(current_node->start_pos, current_node->end_pos, "mode must be a string");
		/*****************************************/
		const std::string file_name = static_cast<Str*>(file_name_.get())->get();
		const std::string mode = static_cast<Str*>(mode_.get())->get();
		/*****************************************/
		this->file.open(file_name, FileStream::mode_map.at(mode)); // mode
		/*****************************************/
		return make_none();
	);
	MAKE_FUN_(close, 0, 0, {},
		this->file.close();
		return make_none();
	);
	MAKE_FUN_(write, 1, 1, {},
		ObjectPtr text = args[1];
		if (*text != ObjectType::STRING) RunTimeError(current_node->start_pos, current_node->end_pos, "arg must be a string");

		const std::string& str = static_cast<Str*>(text.get())->get();
		this->file.write(str.c_str(), str.size());
		return make_none();
	);
	MAKE_FUN_(read, 0, 0, {},
		std::string text;
		std::string line;

		while (std::getline(this->file, line))
			text += line + '\n';

		if (!text.empty()) text.pop_back();
		return make_str(text);
	);
	/**************************************************/
	this->define("is_open", is_open);
	this->define("write", write);
	this->define("close", close);
	this->define("open", open);
	this->define("read", read);
}
