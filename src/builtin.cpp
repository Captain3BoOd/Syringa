#include <iostream>

#include "lexer.hpp"
#include "parser.hpp"
#include "others.hpp"
#include "builtin.hpp"
#include "interpreter.hpp"

extern const Node* current_node;

namespace BuiltIn
{
	ObjectPtr print(ObjectVector& params)
	{
		Array* arr = static_cast<Array*>(params[0].get());
		const ObjectVector& texts = arr->get_data();

		for (const ObjectPtr& text: texts)
			std::cout << text->__str__();

		return std::make_shared<None>();
	}

	ObjectPtr input(ObjectVector& params)
	{
		std::string input;
		std::cout << params[0]->__str__();

		std::cin >> input;
		return std::make_shared<Str>(input);
	}

	ObjectPtr id(ObjectVector& params)
	{
		return make_pointer(params[0]);
	}

	ObjectPtr info(ObjectVector& params)
	{
		return std::make_shared<Str>(params[0]->__info__());
	}
}