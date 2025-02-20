#define INTERPRETER
#include "objects.hpp"
#include "help.hpp"
#include "builtin.hpp"
#include "interpreter.hpp"
#include "object.hpp"
#undef INTERPRETER

extern const Node* current_node;

Object::Object(const ObjectType type, const Class* _class):
	type(type),
	_class(_class)
{
	//LANG_ASSERT(this->_class, "instance type is nullptr");
}

const Object& Object::operator=(const Object& other)
{
	this->type = other.type;
	return *this;
}

ObjectPtr Object::evaluate(ObjectVector& args, SymbolTable* parent)
{
	LANG_ASSERT(false, "this function mustn't be called");
	return nullptr;
}

/*******************************************************************************************/
/******************************************(CLASS)******************************************/
/*******************************************************************************************/

Class::Class(const std::string& class_name):
	Object(ObjectType::CLASS, this),
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
/*****************************************(Instance)****************************************/
/*******************************************************************************************/

Instance::Instance(const Class& _class):
	Object(ObjectType::OBJECT, &_class)
{
	SymbolTable& self_symbols = this->get_symbols();
	const SymbolTable& class_symbols = _class.get_symbols();

	self_symbols.add(class_symbols);
	self_symbols.set_parent(class_symbols.get_parent());
}

Instance::Instance(const Instance& other):
	Object(ObjectType::OBJECT, other.get_class())
{
	this->get_symbols().add(other.get_symbols());
}

std::string Instance::__str__() const
{
	return "object from user class '"+ this->get_class()->__str__() + "'";
}

std::string Instance::__info__() const
{
	return
		"object from user class"
		"\nclass name '" + this->get_class()->__str__() + "'"
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
/****************************************(FILE STREAM)**************************************/
/*******************************************************************************************/

const std::unordered_map<std::string, std::ios_base::openmode> FileStream::mode_map = {
	{ "wb", std::ios::out | std::ios::binary },
	{ "ab", std::ios::app | std::ios::binary },
	{ "rb", std::ios::in | std::ios::binary },
	{ "r+", std::ios::in | std::ios::out },
	{ "w+", std::ios::in | std::ios::out },
	{ "a+", std::ios::in | std::ios::app },
	{ "w", std::ios::out },
	{ "a", std::ios::app },
	{ "r", std::ios::in }
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
	return "class name 'FileStream'"
		+ (this->file.is_open()) ? "file name: " + this->file_name
		+ "\nmode is " + this->mode : "file open"
		"\naddress = " + std::to_string(uint64_t(this))
		+ "\nsize in memory = " + std::to_string(sizeof(*this));
}

std::string FileStream::__str__() const
{
	return
		"<FileStream object>"
		+ (this->file.is_open()) ? "file name: " + this->file_name + "\nmode is " + this->mode : "file open";
}
