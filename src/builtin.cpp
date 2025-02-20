#include <iostream>

#define INTERPRETER
#include "lexer.hpp"
#include "parser.hpp"
#include "objects.hpp"
#include "builtin.hpp"
#include "interpreter.hpp"
#undef INTERPRETER

extern const Node* current_node;

namespace BuiltIn
{
	ObjectPtr print(ObjectVector& params)
	{
		Array* arr = static_cast<Array*>(params[0].get());
		const ObjectVector& texts = arr->get_data();

		for (const ObjectPtr& text: texts)
			std::cout << text->__str__();

		return make_none();
	}

	ObjectPtr input(ObjectVector& params)
	{
		std::string input;
		std::cout << params[0]->__str__();

		std::cin >> input;
		return make_str(input);
	}

	ObjectPtr id(ObjectVector& params)
	{
		return make_pointer(params[0]);
	}

	ObjectPtr info(ObjectVector& params)
	{
		return make_str(params[0]->__info__());
	}
}