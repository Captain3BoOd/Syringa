#pragma once

#include "object.hpp"
#include "make.hpp"

class Pointer : public Object
{
public:
	Pointer(const ObjectPtr& object);
	Pointer(const WeakObject& object);
	inline WeakObject get() const { return this->object; }
	/************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	WeakObject object;
};
