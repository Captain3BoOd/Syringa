#include "pointer.hpp"
#include "builtin.hpp"

extern const Node* current_node;

Pointer::Pointer(const ObjectPtr& object):
	Object(ObjectType::POINTER),
	object(object)
{}

Pointer::Pointer(const WeakObject& object):
	Object(ObjectType::POINTER),
	object(object)
{}

bool Pointer::__bool__() const
{
	return this->object.expired();
}

std::string Pointer::__info__() const
{
	return "Class name 'Pointer'";
}

std::string Pointer::__str__() const
{
	return std::to_string((uint64_t)this->object.lock().get());
}
