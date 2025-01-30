#pragma once

#include "object.hpp"

namespace BuiltIn
{
	ObjectPtr evaluate(ObjectVector& params);
	ObjectPtr print(ObjectVector& params);
	ObjectPtr input(ObjectVector& params);
	ObjectPtr info(ObjectVector& params);
	ObjectPtr id(ObjectVector& params);
}