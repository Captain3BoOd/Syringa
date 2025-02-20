#define INTERPRETER
#include "help.hpp"
#include "objects.hpp"
#include "builtin.hpp"
#include "interpreter.hpp"
#undef INTERPRETER

extern const Node* current_node;

#define SELF_DEF(name) this->define(#name, name)
#define MAKE_FUN(fun_name, params, co_arg, defaults, code)\
	const ObjectPtr fun_name = make_function(\
		#fun_name,\
		std::function([](ObjectVector& args) {\
			ObjectPtr self = args[0];\
			auto _this = static_cast<decltype(this)>(self.get());\
			LANG_ASSERT(self, "self is nullptr");\
			LANG_ASSERT(_this, "casting failed");\
			code\
		}),\
		params, co_arg, defaults\
	)
#define MAKE_CLASS(class_name) const ObjectPtr class_name = make_class(#class_name)
/*********************************************************************************/
#define MAKE_CLASS_DEF(class_name)\
	MAKE_CLASS(class_name);\
	SELF_DEF(fun_name)
#define MAKE_FUN_DEF(fun_name, params, co_arg, defaults, code)\
	MAKE_FUN(fun_name, params, co_arg, defaults, code);\
	SELF_DEF(fun_name)

/*******************************************************************************************************************/
/*******************************************************************************************************************/
/*******************************************************************************************************************/

void Object::init_class_built_in()
{
	MAKE_FUN_DEF(__pos__, 1, 1, {},
		return make_pointer(self);
	);
	MAKE_FUN_DEF(__bool__, 1, 1, {},
		return make_bool(_this->__bool__());
	);
	MAKE_FUN_DEF(__str__, 1, 1, {},
		return make_str(_this->__str__());
	);
	MAKE_FUN_DEF(__info__, 1, 1, {},
		return make_str(_this->__info__());
	);
}

void Module::init_class_built_in()
{
	// TODO Fixing
	Object::init_class_built_in();
	/***************************************************/

}

void Class::init_class_built_in()
{
	Object::init_class_built_in();
	/***************************************************/

}

void Instance::init_class_built_in()
{
	Object::init_class_built_in();
	/***************************************************/

}

void Int::init_class_built_in()
{
	Object::init_class_built_in();
	/***************************************************/
	MAKE_FUN_DEF(__plus__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(_this->number + num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(_this->number + num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(_this->number + _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__minus__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(_this->number - num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(_this->number - num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(_this->number - _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__mul__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(_this->number * num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(_this->number * num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(_this->number * _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__div__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(_this->number / num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(_this->number / num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(_this->number / _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__mod__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(_this->number % num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			long double f_num = num.get();
			int64_t int_ = static_cast<int64_t>(num.get());
			return make_int(_this->number % int_);
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(_this->number % _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__plus_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(_this->number += num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(_this->number += num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(_this->number += _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__b_and__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(_this->number & num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			long double f_num = num.get();
			int64_t int_ = static_cast<int64_t>(num.get());
			return make_int(_this->number & int_);
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(_this->number & _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__b_or__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(_this->number | num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			long double f_num = num.get();
			int64_t int_ = static_cast<int64_t>(num.get());
			return make_int(_this->number | int_);
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(_this->number | _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__nor__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(_this->number ^ num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			long double f_num = num.get();
			int64_t int_ = static_cast<int64_t>(num.get());
			return make_int(_this->number ^ int_);
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(_this->number ^ _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__and__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(_this->number && num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(_this->number && num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(_this->number && _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__or__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(_this->number || num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(_this->number || num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(_this->number || _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(_this->number == num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(_this->number == num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(_this->number == _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__not_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(_this->number != num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(_this->number != num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(_this->number != _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__less__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(_this->number < num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(_this->number < num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(_this->number < _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__less_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(_this->number <= num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(_this->number <= num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(_this->number <= _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__gr__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(_this->number > num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(_this->number > num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(_this->number > _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__gr_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(_this->number >= num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(_this->number >= num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(_this->number >= _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__minus_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(_this->number -= num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(_this->number -= num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(_this->number -= _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__div_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(_this->number /= num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(_this->number /= num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(_this->number /= _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__mul_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(_this->number *= num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(_this->number *= num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(_this->number *= _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__int__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::INT))
			{
				Int& result = *static_cast<Int*>(args[1].get());
				return make_int(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__b_not__, 1, 1, {},
		return make_int(~_this->number);
	);
	MAKE_FUN_DEF(__not__, 1, 1, {},
		return make_bool(!_this->number);
	);
	MAKE_FUN_DEF(__plus_plus__, 1, 1, {},
		return make_int(_this->number++);
	);
	MAKE_FUN_DEF(__min_min__, 1, 1, {},
		return make_int(_this->number--);
	);
	MAKE_FUN_DEF(__min__, 1, 1, {},
		return make_int(-_this->number);
	);
	/***************************/
	MAKE_FUN_DEF(__float__, 1, 1, {},
		return make_float(_this->number);
	);
}

void Float::init_class_built_in()
{
	Object::init_class_built_in();
	/***************************************************/
	MAKE_FUN_DEF(__plus__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_float(_this->number + num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(_this->number + num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_float(_this->number + _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__minus__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_float(_this->number - num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(_this->number - num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_float(_this->number - _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__mul__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_float(_this->number * num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(_this->number * num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_float(_this->number * _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__div__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_float(_this->number / num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(_this->number / num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_float(_this->number / _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__mod__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		/*if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			int64_t i_num = num.get();
			long double float_ = static_cast<long double>(i_num);
			return make_float(_this->number % float_);
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(_this->number % num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_float(_this->number % _bool.get());
		}
		/*******************************************************/
		/*ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();*/
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__plus_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_int(_this->number += num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(_this->number += num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_int(_this->number += _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__and__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(_this->number && num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(_this->number && num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(_this->number && _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__or__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(_this->number || num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(_this->number || num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(_this->number || _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(_this->number == num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(_this->number == num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(_this->number == _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__not_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(_this->number != num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(_this->number != num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(_this->number != _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__less__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(_this->number < num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(_this->number < num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(_this->number < _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__less_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(_this->number <= num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(_this->number <= num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(_this->number <= _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__gr__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(_this->number > num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(_this->number > num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(_this->number > _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__gr_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_bool(_this->number >= num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_bool(_this->number >= num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_bool(_this->number >= _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__minus_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_float(_this->number -= num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(_this->number -= num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_float(_this->number -= _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__div_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_float(_this->number /= num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(_this->number /= num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_float(_this->number /= _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__mul_eq__, 2, 2, {},
		ObjectType type = args[1]->get_type();
		if (type == ObjectType::INT)
		{
			Int& num = *static_cast<Int*>(args[1].get());
			return make_float(_this->number *= num.get());
		}
		else if (type == ObjectType::FLOAT)
		{
			Float& num = *static_cast<Float*>(args[1].get());
			return make_float(_this->number *= num.get());
		}
		else if (type == ObjectType::BOOL)
		{
			Bool& _bool = *static_cast<Bool*>(args[1].get());
			return make_float(_this->number *= _bool.get());
		}
		/*******************************************************/
		ObjectPtr type_op = args[1]->get("__float__");
		if (type_op && check_type(type_op, ObjectType::FUNCTION))
		{
			ObjectVector arr = { args[1] };
			ObjectPtr result = type_op->evaluate(arr);
			if (check_type(result, ObjectType::FLOAT))
			{
				Float& result = *static_cast<Float*>(args[1].get());
				return make_float(_this->number + result.get());
			}
		}
		RunTimeError(current_node->start_pos, current_node->end_pos, "UnSupported type");
		return make_none();
	);
	MAKE_FUN_DEF(__not__, 1, 1, {},
		return make_bool(!_this->number);
	);
	MAKE_FUN_DEF(__plus_plus__, 1, 1, {},
		return make_float(_this->number++);
	);
	MAKE_FUN_DEF(__min_min__, 1, 1, {},
		return make_float(_this->number--);
	);
	MAKE_FUN_DEF(__min__, 1, 1, {},
		return make_float(-_this->number);
	);
	/***************************/
	MAKE_FUN_DEF(__int__, 1, 1, {},
		return make_int(_this->number);
	);
}

void Str::init_class_built_in()
{
	Object::init_class_built_in();
	/***************************************************/
	MAKE_FUN_DEF(__append__, 2, 1, {},
		const Array* a = static_cast<Array*>(args[1].get());
		const ObjectVector & objects = a->get_data();

		for (const ObjectPtr& object : objects)
			_this->str += object->get("__str__")->__str__();

		return make_none();
	);
	MAKE_FUN_DEF(__plus__, 2, 2, {},
		const std::string arg_str = args[1]->get("__str__")->__str__();
		return make_str(_this->str + arg_str);
	);
	MAKE_FUN_DEF(__plus_eq__, 2, 2, {},
		const std::string arg_str = args[1]->get("__str__")->__str__();
		_this->str += arg_str;

		return make_str(_this->str);
	);
	MAKE_FUN_DEF(__eq__, 2, 2, {},
		const std::string arg_str = args[1]->__str__();
		return make_bool(_this->str == arg_str);
	);
	MAKE_FUN_DEF(__index__, 2, 2, {},
		Tuple& range = *static_cast<Tuple*>(args[1].get());
		const ObjectVector& data = range.get();

		return make_str("");
	);
}

void Pointer::init_class_built_in()
{
	Object::init_class_built_in();
	/***********************************************************/

	/***********************************************************/

}

void BaseFunction::init_class_built_in()
{
	Object::init_class_built_in();
	/***************************************************/

}

void List::init_class_built_in()
{
	Object::init_class_built_in();
	/***************************************************/

}

void Tuple::init_class_built_in()
{
	Object::init_class_built_in();
	/***************************************************/

}

void Array::init_class_built_in()
{
	Object::init_class_built_in();
	/***************************************************/

}

void Bool::init_class_built_in()
{
	Object::init_class_built_in();
	/***************************************************/

}

void None::init_class_built_in()
{
	Object::init_class_built_in();
	/***************************************************/

}

void Error::init_class_built_in()
{
	Object::init_class_built_in();
	/***************************************************/

}

void FileStream::init_class_built_in()
{
	Object::init_class_built_in();
	/***************************************************/
	MAKE_FUN_DEF(is_open, 0, 0, {},
		return make_bool(_this->file.is_open());
	);
	MAKE_FUN_DEF(open, 2, 2, {},
		ObjectPtr file_name_ = args[1];
		ObjectPtr mode_ = args[1];
		if (*file_name_ != ObjectType::STRING) RunTimeError(current_node->start_pos, current_node->end_pos, "file name must be a string");
		if (*mode_ != ObjectType::STRING) RunTimeError(current_node->start_pos, current_node->end_pos, "mode must be a string");
		/*****************************************/
		const std::string file_name = static_cast<Str*>(file_name_.get())->get();
		const std::string mode = static_cast<Str*>(mode_.get())->get();
		/*****************************************/
		_this->file.open(file_name, FileStream::mode_map.at(mode)); // mode
		/*****************************************/
		return make_none();
	);
	MAKE_FUN_DEF(close, 0, 0, {},
		_this->file.close();
		return make_none();
	);
	MAKE_FUN_DEF(write, 1, 1, {},
		ObjectPtr text = args[1];
		if (*text != ObjectType::STRING) RunTimeError(current_node->start_pos, current_node->end_pos, "arg must be a string");

		const std::string& str = static_cast<Str*>(text.get())->get();
		_this->file.write(str.c_str(), str.size());
		return make_none();
	);
	MAKE_FUN_DEF(read, 0, 0, {},
		std::string text;
		std::string line;

		while (std::getline(_this->file, line))
			text += line + '\n';

		if (!text.empty()) text.pop_back();
		return make_str(text);
	);
}
