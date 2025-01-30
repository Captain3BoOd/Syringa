#define INTERPRETER
#include "object.hpp"
#include "others.hpp"
#include "help.hpp"
#include "builtin.hpp"
#include "interpreter.hpp"
#undef INTERPRETER

extern const Node* current_node;

Object::Object(const ObjectType type):
	type(type)
{}

Object::Object(const Object& other):
	type(other.type),
	symbols(other.symbols)
{}

bool Object::operator==(const ObjectType object_type) const
{
	return (
		(this->type == object_type)
		&& (this->type != ObjectType::NONe)
	);
}

ObjectType Object::get_type() const
{
	return this->type;
}

const Object& Object::operator=(const Object& other)
{
	this->type = other.type;
	return *this;
}

Object::operator bool() const
{
	return this->type != ObjectType::NONe;
}

/*******************************************************************************************/
/*****************************************(MODULE)******************************************/
/*******************************************************************************************/

Module::Module(const std::string& module_name, const std::string& module_path):
	Object(ObjectType::MODULE),
	module_name(module_name),
	module_path(module_path)
{}

std::string Module::__info__() const
{
	return std::string();
}

std::string Module::__str__() const
{
	return std::string();
}

/*******************************************************************************************/
/******************************************(CLASS)******************************************/
/*******************************************************************************************/

Class::Class(const std::string& class_name):
	Object(ObjectType::CLASS),
	class_name(class_name)
{}

std::string Class::__str__() const
{
	return this->class_name;
}

std::string Class::__info__() const
{
	return "class name: " + this->class_name;
}

/*******************************************************************************************/
/***************************************(User_Object)***************************************/
/*******************************************************************************************/

User_Object::User_Object(const Class& _class):
	Object(ObjectType::OBJECT),
	_class(_class)
{
	SymbolTable& self_symbols = this->get_symbols();
	const SymbolTable& class_symbols = this->_class.get_symbols();

	self_symbols.add(class_symbols);
	self_symbols.set_parent(class_symbols.get_parent());
}

User_Object::User_Object(const User_Object& other):
	Object(ObjectType::OBJECT),
	_class(other._class)
{
	this->get_symbols().add(other.get_symbols());
}

std::string User_Object::__str__() const
{
	return "object from user class '"+ this->_class.__str__() + "'";
}

std::string User_Object::__info__() const
{
	return
		"object from user class"
		"\nclass name '" + this->_class.__str__() + "'"
		"\nAddress in memory: " + std::to_string(uint64_t(this)) +
		"\nsize in memory = " + std::to_string(sizeof(*this));
}

/*******************************************************************************************/
/********************************************(INT)******************************************/
/*******************************************************************************************/

Int::Int(int64_t number):
	Object(ObjectType::INT),
	number(number)
{}

void Int::set(int64_t number)
{
	this->number = number;
}

int64_t Int::get() const
{
	return this->number;
}

std::string Int::__str__() const
{
	return std::to_string(this->number);
}

bool Int::__bool__() const
{
	return this->number;
}

std::string Int::__info__() const
{
	return
		"class name 'Int'"
		"\nvalue = " + this->__str__() +
		"\nAddress in memory: " + std::to_string(uint64_t(this)) +
		"\nsize in memory = " + std::to_string(sizeof(*this));
}

/*******************************************************************************************/
/*******************************************(FLOAT)*****************************************/
/*******************************************************************************************/

Float::Float(long double number):
	Object(ObjectType::FLOAT),
	number(number)
{}

void Float::set(const long double& number)
{
	this->number = number;
}

long double Float::get() const
{
	return this->number;
}

std::string Float::__str__() const
{
	return std::to_string(this->number);
}

bool Float::__bool__() const
{
	return this->number;
}

std::string Float::__info__() const
{
	return
		"class name 'Float'"
		"\nvalue = " + this->__str__() +
		"\naddress = " + std::to_string(uint64_t(this)) +
		"\nsize in memory = " + std::to_string(sizeof(*this));
}

/*******************************************************************************************/
/*******************************************(STRING)****************************************/
/*******************************************************************************************/

Str::Str(const std::string& str):
	Object(ObjectType::STRING),
	str(str)
{}

std::string Str::__str__() const
{
	return this->str;
}

bool Str::__bool__() const
{
	return !this->str.empty();
}

std::string Str::__info__() const
{
	return
		"class name 'Str'"
		"\nvalue = " + this->__str__() +
		"\naddress = " + std::to_string(uint64_t(this)) +
		"\nsize in memory = " + std::to_string(sizeof(*this));
}

inline std::ostream& operator<<(std::ostream& out, const Str& str)
{
	out << str.str;
	return out;
}

std::string Str::get() const
{
	return this->str;
}

/*******************************************************************************************/
/******************************************(FUNCTIONS)**************************************/
/*******************************************************************************************/

List::List(const ObjectVector& objects):
	Object(ObjectType::LIST)
{
	for (const ObjectPtr& object : objects)
		this->objects.emplace_back(AssignObjects(object));
}

void List::append(ObjectPtr& obj)
{
	this->objects.emplace_back(AssignObjects(obj));
}

void List::append(ObjectPtr obj)
{
	this->objects.emplace_back(AssignObjects(obj));
}

void List::remove(uint64_t index)
{
	this->objects.erase(this->objects.begin() + index);
}

std::string List::__str__() const
{
	std::string result = "[";
	for (auto& object : this->objects)
		result.append(object->__str__() + ", ");

	result.pop_back();
	result.pop_back();
	result.push_back(']');

	return result;
}

bool List::__bool__() const
{
	return !this->objects.empty();
}

std::string List::__info__() const
{
	return
		"class name 'List'"
		"\nvalue = "+ this->__str__() +
		"\naddress = " + std::to_string(uint64_t(this)) +
		"\nsize in memory = " + std::to_string(sizeof(*this));
}

/*******************************************************************************************/
/*******************************************(TUPLE)*****************************************/
/*******************************************************************************************/

Tuple::Tuple(const ObjectVector& objects):
	Object(ObjectType::TUPLE)
{
	for (const ObjectPtr& object : objects)
		this->objects.emplace_back(AssignObjects(object));
}

std::string Tuple::__str__() const
{
	std::string result = "[";
	for (auto& element : this->objects)
		result.append(element->__str__() + ", ");

	result.pop_back();
	result.pop_back();
	result.push_back(']');

	return result;
}

bool Tuple::__bool__() const
{
	return !this->objects.empty();
}

std::string Tuple::__info__() const
{
	return
		"class name 'List'"
		"\nvalue = " + this->__str__() +
		"\naddress = " + std::to_string(uint64_t(this)) +
		"\nsize in memory = " + std::to_string(sizeof(*this));
}

/*******************************************************************************************/
/*******************************************(ARRAY)*****************************************/
/*******************************************************************************************/

Array::Array(const ObjectVector& objects):
	Object(ObjectType::ARRAY)
{
	for (const ObjectPtr& object : objects)
		this->objects.emplace_back(AssignObjects(object));
}

const ObjectVector& Array::get_data() const
{
	return this->objects;
}

std::string Array::__str__() const
{
	std::string result = "[";
	for (auto& element : this->objects)
		result.append(element->__str__() + ", ");

	result.pop_back();
	result.pop_back();
	result.push_back(']');

	return result;
}

bool Array::__bool__() const
{
	return !this->objects.empty();
}

std::string Array::__info__() const
{
	return
		"class name 'List'"
		"\nvalue = " + this->__str__() +
		"\naddress = " + std::to_string(uint64_t(this)) +
		"\nsize in memory = " + std::to_string(sizeof(*this));
}

/*******************************************************************************************/
/********************************************(BOOL)*****************************************/
/*******************************************************************************************/

Bool::Bool(bool data):
	Object(ObjectType::BOOL),
	data(data)
{}

bool Bool::get() const
{
	return this->data;
}

std::string Bool::__str__() const
{
	return this->data ? "True" : "False";
}

bool Bool::__bool__() const
{
	return this->data;
}

std::string Bool::__info__() const
{
	return
		"class name 'Bool'"
		"\nvalue = " + this->__str__() +
		"\naddress = " + std::to_string(uint64_t(this)) +
		"\nsize in memory = " + std::to_string(sizeof(*this));
}

/*******************************************************************************************/
/********************************************(NONE)*****************************************/
/*******************************************************************************************/

None::None():
	Object(ObjectType::NONE)
{}

std::string None::__str__() const
{
	return "None";
}

bool None::__bool__() const
{
	return false;
}

std::string None::__info__() const
{
	return
		"class name 'None'"
		"\nvalue = " + this->__str__() +
		"\naddress = " + std::to_string(uint64_t(this)) +
		"\nsize in memory = " + std::to_string(sizeof(*this));
}

/*******************************************************************************************/
/*******************************************(ERROR)*****************************************/
/*******************************************************************************************/

Error::Error(Position start, Position end, ErrorType type, std::string details):
	Object(ObjectType::ERROR),
	Errorr(start, end, type, details)
{}

std::string Error::__str__() const
{
	const std::string error_name = error_.at(this->get__type());
	std::string result;

	if (
		this->pos_start.is_default()
		|| this->pos_end.is_default()
		) {
		result = std::format("{}: {}\n", error_name, this->details);
	}
	else
	{
		result = std::format("{}: {}\n", error_name, this->details);
		result += std::format("File {} at Line {}\n", this->pos_start.file_name, this->pos_start.line + 1);
		result += "\n\n" + string_with_arrows(this->pos_start.text.data(), this->pos_start, this->pos_end);
	}

	return result;
}

bool Error::__bool__() const
{
	return this->get__type() != ErrorType::NONE;
}

std::string Error::__info__() const
{
	return "";
}

/*******************************************************************************************/
/******************************************(FUNCTION)***************************************/
/*******************************************************************************************/

Pointer::Pointer(const ObjectPtr& object):
	Object(ObjectType::POINTER),
	object(object)
{}

bool Pointer::__bool__() const
{
	return this->object.expired(); //
}

std::string Pointer::__info__() const
{
	return std::string();
}

std::string Pointer::__str__() const
{
	return std::string();
}

/*******************************************************************************************/
/******************************************(FUNCTION)***************************************/
/*******************************************************************************************/

BaseFunction::BaseFunction(
	const std::string& fun_name,
	uint64_t arg_count,
	uint64_t co_argcount,
	uint64_t default_count
):
	Object(ObjectType::FUNCTION),
	fun_name(fun_name),
	arg_count(arg_count),
	co_argcount(co_argcount),
	default_count(default_count),
	function(nullptr)
{}

BaseFunction::BaseFunction(const std::string& fun_name, BuiltInFunction* fun):
	Object(ObjectType::FUNCTION),
	fun_name(fun_name),
	arg_count(fun->arg_count),
	co_argcount(fun->co_argcount),
	default_count(fun->default_count),
	function(fun)
{
	LANG_ASSERT(this->function, "fun object must have a value");
}

BaseFunction::BaseFunction(const std::string& fun_name, Function* fun):
	Object(ObjectType::FUNCTION),
	fun_name(fun_name),
	arg_count(fun->arg_count),
	co_argcount(fun->co_argcount),
	default_count(fun->default_count),
	function(fun)
{
	LANG_ASSERT(this->function, "fun object must have a value");
}

ObjectPtr BaseFunction::evaluate(SymbolTable& parent, ObjectVector& args)
{
	if (this->arg_count && args.empty())
	{
		RunTimeError(current_node->start_pos, current_node->end_pos,
			"Function '" + this->fun_name + "' "
			"Expected '" + std::to_string(this->arg_count)
			+ (this->default_count ? (" or " + std::to_string(this->arg_count - this->default_count)) : "")
			+ "' argument but '0' was send"
		);
		return make_none();
	}
	/*****************************************************************************/
	ObjectVector new_args;
	const bool is_arb_count = this->arg_count - (this->co_argcount + this->default_count);
	ObjectVecIter end_co_args = args.begin() + this->co_argcount;
	/*****************************************************************************/
	if (this->co_argcount) new_args.assign(args.begin(), end_co_args);
	/*****************************************************************************/
	if (is_arb_count)
	{
		ObjectPtr arb_args = make_array(ObjectVector(end_co_args, args.end()));
		/**************************************************/
		LANG_ASSERT(arb_args, "arb args must have a value");
		new_args.emplace_back(arb_args);
	}
	/*****************************************************************************/
	args.clear();
	/*****************************************************************************/
	if (new_args.size() != this->arg_count)
	{
		RunTimeError(current_node->start_pos, current_node->end_pos,
			"Function '" + this->fun_name + "' "
			"Expected '" + std::to_string(this->arg_count)
			+ (this->default_count ? (" or " + std::to_string(this->arg_count - this->default_count)) : "")
			+ "' argument but '" + std::to_string(new_args.size()) + "' was send"
		);
		return make_none();
	}
	/*****************************************************************************/
	return this->function->evaluate(parent, new_args);
}

// Built-In //
BuiltInFunction::BuiltInFunction(
	const std::string& fun_name,
	const FunctionType& function,
	uint64_t params_number,
	uint64_t co_argcount,
	const ObjectVector& defaults
):
	BaseFunction(fun_name, params_number, co_argcount, defaults.size()),
	defaults(defaults),
	function(function)
{
	LANG_ASSERT(this->function, "fun object must have a value");
}

ObjectPtr BuiltInFunction::evaluate(SymbolTable& parent, ObjectVector& args)
{
	return this->function(args);
}

std::string BuiltInFunction::__str__() const
{
	return "Built-In Function '" + this->fun_name + "' at <" + std::to_string((uint64_t)this) + '>';
}

std::string BuiltInFunction::__info__() const
{
	return
		"class name 'Function'"
		"\nfunction name '" + this->fun_name + "'"
		"\nfunction type : 'Built in Funtion'"
		"\nnumber of arguments = " + std::to_string(this->arg_count) +
		"\naddress = " + std::to_string(uint64_t(this)) +
		"\nsize in memory = " + std::to_string(sizeof(*this));
}

// Normal //
Function::Function(
	const std::string& fun_name,
	const std::vector<Fun::Argument>& parameters,
	uint64_t co_argcount,
	uint64_t default_count,
	const NodeVector& body
):
	BaseFunction(fun_name, parameters.size(), co_argcount, default_count),
	parameters(parameters),
	body(body)
{}

ObjectPtr Function::evaluate(SymbolTable& parent, ObjectVector& params)
{
	Interpreter interpreter(this->body, &parent);
	SymbolTable& fun_symbols = interpreter.get_vars();

	for (size_t i = 0; i < this->parameters.size(); i++)
	{
		if (this->parameters[i].arg == TokenType::TT_IDENTIFIER)
		{
			if (this->parameters[i].arg.value == "self") fun_symbols.ref(this->parameters[i].arg.value, params[i], true);
			else fun_symbols.ref(this->parameters[i].arg.value, params[i], false);
		}
	}

	return interpreter.interpret();
}

std::string Function::__str__() const
{
	return "Function '" + this->fun_name + "' at <" + std::to_string((uint64_t)this) + '>';
}

std::string Function::__info__() const
{
	std::string arguments_name = "[";
	for (const Fun::Argument& parameter : this->parameters)
		arguments_name += parameter.arg.value + ", ";

	if (!this->parameters.empty()) arguments_name.pop_back(), arguments_name.pop_back();
	arguments_name += "]";

	return
		"class name 'Function'"
		"\nfunction name '" + this->fun_name + "'"
		"\nfunction type : 'User Funtion'"
		"\nnumber of arguments = " + std::to_string(this->arg_count) +
		"\narguments name" + arguments_name +
		"\naddress = " + std::to_string(uint64_t(this)) +
		"\nsize in memory = " + std::to_string(sizeof(*this));
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

/*******************************************************************************************/
/****************************************(FILE STREAM)**************************************/
/*******************************************************************************************/

const std::unordered_map<std::string, std::ios_base::openmode> FileStream::mode_map = {
	{ "r", std::ios::in },
	{ "w", std::ios::out },
	{ "w+", std::ios::in | std::ios::out },
	{ "a", std::ios::app },
	{ "a+", std::ios::in | std::ios::app },
	{ "rb", std::ios::in | std::ios::binary },
	{ "wb", std::ios::out | std::ios::binary },
	{ "ab", std::ios::app | std::ios::binary },
	{ "r+", std::ios::in | std::ios::out }
};

FileStream::FileStream(const std::string& file_name, const std::string& mode):
	Object(ObjectType::FILE_STREAM),
	mode(mode),
	file_name(file_name),
	file(file_name, FileStream::mode_map.at(mode))
{}

bool FileStream::__bool__() const
{
	return this->file.is_open();
}

std::string FileStream::__info__() const
{
	return "";
}

std::string FileStream::__str__() const
{
	return "";
}

/*******************************************************************************************/
/**************************************(MAKES FUNCTIONS)************************************/
/*******************************************************************************************/

ObjectPtr AssignObjects(const ObjectPtr& obj)
{
	LANG_ASSERT(obj, "Object must have a value");

	static constexpr std::initializer_list<ObjectType> ref_types = {
		ObjectType::FUNCTION,
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
		const User_Object& user_obj = *static_cast<const User_Object*>(obj.get());
		new_obj = make_user_object(user_obj);
	} else TypeError(current_node->start_pos, current_node->end_pos, "Can not assign this type");
	LANG_ASSERT(new_obj, "new object must have a value");

	return new_obj;
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
ObjectPtr make_user_object(const ObjectPtr& _class)
{
	if (*_class != ObjectType::CLASS)
		RunTimeError(current_node->start_pos, current_node->end_pos, "Can't create a User Object from Non-Class type");
	/*****************************************************************************************/
	const Class& klass = *static_cast<Class*>(_class.get());
	ObjectPtr object = std::make_shared<User_Object>(klass);
	object->init_class_built_in();
	return object;
}
ObjectPtr make_user_object(const User_Object& user_object)
{
	ObjectPtr object = std::make_shared<User_Object>(user_object);
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
/*****************************************************************************************/