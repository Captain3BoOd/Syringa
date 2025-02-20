#pragma once

#include "object.hpp"
#include "make.hpp"

#define MAKE_CLASS(class_name) inline const ObjectPtr _##class_name = make_class(#class_name, nullptr)
namespace BuiltIn
{
	ObjectPtr evaluate(ObjectVector& params);
	ObjectPtr print(ObjectVector& params);
	ObjectPtr input(ObjectVector& params);
	ObjectPtr info(ObjectVector& params);
	ObjectPtr id(ObjectVector& params);
	/**************************************************************************/
	MAKE_CLASS(float);
	MAKE_CLASS(int);
	MAKE_CLASS(str);
	MAKE_CLASS(array);
	MAKE_CLASS(tuple);
	MAKE_CLASS(bool);
	MAKE_CLASS(None);
	MAKE_CLASS(error);
	MAKE_CLASS(list);
	MAKE_CLASS(function);
	MAKE_CLASS(module);
	MAKE_CLASS(file_stream);
	MAKE_CLASS(pointer);
}
#undef MAKE_CLASS(class_name)