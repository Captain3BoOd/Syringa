#define INTERPRETER
#include "make.hpp"
#include "help.hpp"
#include "objects.hpp"
#undef INTERPRETER

extern const Node* current_node;

ObjectPtr AssignObjects(const ObjectPtr& obj)
{
	LANG_ASSERT(obj, "Object must have a value");

	static constexpr std::initializer_list<ObjectType> ref_types = {
		ObjectType::FUNCTION,
		ObjectType::MODULE,
		ObjectType::STRING,
		ObjectType::CLASS,
		ObjectType::TUPLE,
		ObjectType::ARRAY,
		ObjectType::LIST
	};
	ObjectPtr new_obj = nullptr;

	if (*obj == ObjectType::BOOL)
	{
		const Bool& bool_obj = *static_cast<const Bool*>(obj.get());
		new_obj = make_bool(bool_obj.get());
	}
	else if (*obj == ObjectType::FLOAT)
	{
		const Float& float_obj = *static_cast<const Float*>(obj.get());
		new_obj = make_float(float_obj.get());
	}
	else if (*obj == ObjectType::INT)
	{
		const Int& int_obj = *static_cast<const Int*>(obj.get());
		new_obj = make_int(int_obj.get());
	}
	else if (*obj == ObjectType::ERROR)
	{
		const Error& error_obj = *static_cast<const Error*>(obj.get());
		new_obj = make_error(error_obj.get_start_pos(), error_obj.get_end_pos(), error_obj.get__type(), error_obj.get_details());
	}
	else if (*obj == ObjectType::FILE_STREAM)
	{
		const FileStream& file_stream_obj = *static_cast<const FileStream*>(obj.get());
		new_obj = make_file_stream(file_stream_obj.get_file_name(), file_stream_obj.get_mode());
	}
	else if (*obj == ObjectType::POINTER)
	{
		const Pointer& pointer_obj = *static_cast<const Pointer*>(obj.get());
		new_obj = make_pointer(pointer_obj.get());
	}
	else if (in(*obj, ref_types))
	{
		new_obj = obj;
	}
	else if (*obj == ObjectType::NONE)
	{
		new_obj = make_none();
	}
	else if (*obj == ObjectType::OBJECT)
	{
		const Instance& user_obj = *static_cast<const Instance*>(obj.get());
		new_obj = make_instance(user_obj);
	}
	else TypeError(current_node->start_pos, current_node->end_pos, "Can not assign this type");
	LANG_ASSERT(new_obj, "new object must have a value");

	return new_obj;
}
ObjectPtr make_function(
	const std::string& fun_name,
	const FunctionType& function,
	uint64_t params_number,
	uint64_t co_argcount,
	const ObjectVector& defaults
) {
	BuiltInFunction* fun = new BuiltInFunction(fun_name, function, params_number, co_argcount, defaults);
	LANG_ASSERT(fun, "fun object must have a value");

	return std::make_shared<BaseFunction>(fun_name, fun);
}
ObjectPtr make_function(
	const std::string& fun_name,
	const std::vector<Fun::Argument>& parameters,
	uint64_t co_argcount,
	uint64_t default_count,
	const NodeVector& body
) {
	Function* fun = new Function(fun_name, parameters, co_argcount, default_count, body);
	LANG_ASSERT(fun, "fun object must have a value");

	return std::make_shared<BaseFunction>(fun_name, fun);
}
ObjectPtr make_module(const SymbolTable& symbol_table, const std::string& module_name, const std::string& module_path)
{
	ObjectPtr object = std::make_shared<Module>(symbol_table, module_name, module_path);
	object->init_class_built_in();
	return object;
}
ObjectPtr make_error(const Position& start, const Position& end, ErrorType type, std::string details)
{
	ObjectPtr object = std::make_shared<Error>(start, end, type, details);
	object->init_class_built_in();
	return object;
}
ObjectPtr make_file_stream(const std::string& file_name, const std::string& mode)
{
	ObjectPtr object = std::make_shared<FileStream>(file_name, mode);
	object->init_class_built_in();
	return object;
}
ObjectPtr make_instance(const ObjectPtr& _class)
{
	if (*_class != ObjectType::CLASS)
		RunTimeError(current_node->start_pos, current_node->end_pos, "Can't create a User Object from Non-Class type");
	/*****************************************************************************************/
	const Class& klass = *static_cast<Class*>(_class.get());
	ObjectPtr object = std::make_shared<Instance>(klass);
	object->init_class_built_in();
	return object;
}
ObjectPtr make_instance(const Instance& user_object)
{
	ObjectPtr object = std::make_shared<Instance>(user_object);
	object->init_class_built_in();
	return object;
}
ObjectPtr make_class(const std::string& class_name, SymbolTable* parent)
{
	ObjectPtr object = std::make_shared<Class>(class_name);
	object->get_symbols().set_parent(parent);
	object->init_class_built_in();
	return object;
}
ObjectPtr make_array(const ObjectVector& elements)
{
	ObjectPtr object = std::make_shared<Array>(elements);
	object->init_class_built_in();
	return object;
}
ObjectPtr make_tuple(const ObjectVector& elements)
{
	ObjectPtr object = std::make_shared<Tuple>(elements);
	object->init_class_built_in();
	return object;
}
ObjectPtr make_list(const ObjectVector& elements)
{
	ObjectPtr object = std::make_shared<List>(elements);
	object->init_class_built_in();
	return object;
}
ObjectPtr make_pointer(const WeakObject& _object)
{
	ObjectPtr object = std::make_shared<Pointer>(_object);
	object->init_class_built_in();
	return object;
}
ObjectPtr make_pointer(const ObjectPtr& _object)
{
	ObjectPtr object = std::make_shared<Pointer>(_object);
	object->init_class_built_in();
	return object;
}
ObjectPtr make_float(const long double& num)
{
	ObjectPtr object = std::make_shared<Float>(num);
	object->init_class_built_in();
	return object;
}
ObjectPtr make_str(const std::string& str)
{
	ObjectPtr object = std::make_shared<Str>(str);
	object->init_class_built_in();
	return object;
}
ObjectPtr make_bool(bool data)
{
	ObjectPtr object = std::make_shared<Bool>(data);
	object->init_class_built_in();
	return object;
}
ObjectPtr make_int(int64_t num)
{
	ObjectPtr object = std::make_shared<Int>(num);
	object->init_class_built_in();
	return object;
}
ObjectPtr make_none()
{
	ObjectPtr object = std::make_shared<None>();
	object->init_class_built_in();
	return object;
}
