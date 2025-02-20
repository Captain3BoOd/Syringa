#include "parser.hpp"
#include "help.hpp"

Parser::Parser(std::vector<Token>& tokens):
	tokens(&tokens),
	current_token(nullptr),
	pos(0)
{
	this->advance();
}

constexpr std::initializer_list<NodeType> non_semi_colon = {
	NodeType::WHILE_STATEMENT,
	NodeType::FOR_STATEMENT,
	NodeType::IF_STATEMENT,
	NodeType::CLASS,
	NodeType::BLOCK,
	NodeType::FUN,
};

constexpr std::initializer_list<TokenType> Allowed = {
	TokenType::TT_INT,
	TokenType::TT_FLOAT,
	TokenType::TT_STRING,
	TokenType::TT_LPAREN,
	TokenType::TT_MINUS,
	TokenType::TT_IDENTIFIER,
	TokenType::TT_KEYWORD,
	TokenType::TT_LSQUARE,
	TokenType::TT_LBRACE
};

inline bool operator==(Token* token, TokenType type)
{
	if (token) return (*token) == type;
	return false;
}

NodeVector& Parser::Parse()
{
	if (in(this->current_token->type, Allowed))
		this->make_statements();
	else
		InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Invalid Token");

	return this->Trees;
}

static constexpr std::initializer_list<TokenType> BinOP = {
	TokenType::TT_GREATER_THAN_EQUAL,
	TokenType::TT_LESS_THAN_EQUAL,
	TokenType::TT_GREATER_THAN,
	TokenType::TT_LESS_THAN,
	TokenType::TT_BIT_AND,
	TokenType::TT_BIT_OR,
	TokenType::TT_EQUAL,
	TokenType::TT_AND,
	TokenType::TT_NOR,
	TokenType::TT_OR
};

Node* Parser::make_expression()
{
	Position start_pos(this->current_token->pos_start);
	Node* left = this->arithmetic();

	while (in(*this->current_token, BinOP))
	{
		TokenType op = this->current_token->type;
		this->advance();
		Node* right = this->term();
		if (!right) InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected expression");

		left = new BinOp(left, op, right, start_pos, this->current_token->pos_end);
	}
	return left;
}

Node* Parser::make_list()
{
	NodeVector elements;
	Position start_pos(this->current_token->pos_start);

	if (*this->current_token != TokenType::TT_RSQUARE)
	{
		if (*this->current_token == TokenType::TT_EOF)
		{
			InvalidSyntaxError(start_pos, this->current_token->pos_end, "Expected ']'");
			return nullptr;
		}

		Node* expr = this->make_expression();
		elements.emplace_back(expr);

		while (*this->current_token == TokenType::TT_COMMA)
		{
			this->advance();

			Node* expr = this->make_expression();
			elements.emplace_back(expr);
		}
		if (*this->current_token != TokenType::TT_RSQUARE)
		{
			InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected ']'");
		} else this->advance();
	} else this->advance();

	Position end_pos(this->current_token->pos_end);
	if ((!elements.empty()) && elements[0] == nullptr)
	{
		Position end_pos2(start_pos);
		end_pos2.advance(',');

		InvalidSyntaxError(start_pos, end_pos2, "Expected element");
		return nullptr;
	}
	return new NList(elements, start_pos, this->current_token->pos_end);
}

Node* Parser::make_class_def()
{
	Position start_pos = this->current_token->pos_start;

	if (*this->current_token != TokenType::TT_IDENTIFIER)
	{	
		InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected class name");
		return nullptr;
	}
	std::string class_name = this->current_token->value;
	std::vector<Token> inh;
	LANG_ASSERT(!class_name.empty(), "class must have a name");
	this->advance();

	if (*this->current_token == TokenType::TT_LPAREN)
	{
		this->advance();

		while (*this->current_token != TokenType::TT_RPAREN)
		{
			if (*this->current_token == TokenType::TT_EOF)
			{		
				InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected ')'");
				break;
			}

			if (*this->current_token == TokenType::TT_IDENTIFIER)
			{
				inh.emplace_back(*this->current_token);
				this->advance();
			}
			else
			{
				InvalidSyntaxError(
					start_pos, this->current_token->pos_end, "Expected class name"
				);
			}

			if (*this->current_token != TokenType::TT_COMMA)
			{	
				InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected ','");

				if (!inh.empty()) Errors.pop_back();
			} else this->advance();
		} this->advance();
	}

	if (*this->current_token != TokenType::TT_LBRACE)
	{
		InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected '{'");
	} else this->advance();

	NodeVector classes;
	NodeVector variables;
	NodeVector functions;
	while (*this->current_token != TokenType::TT_RBRACE)
	{
		if (*this->current_token == TokenType::TT_EOF)
		{
			InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected '}'");
			break;
		}

		if (*this->current_token == KeyWord::VAR)
		{
			this->advance();

			bool is_const = false;
			if (*this->current_token == KeyWord::CONST)
			{
				this->advance();
				is_const = true;
			}

			if (*current_token != TokenType::TT_IDENTIFIER)
			{		
				InvalidSyntaxError(
					this->current_token->pos_start, this->current_token->pos_end, "Expected Identifier"
				);
				return nullptr;
			}

			std::string var_name = this->current_token->value;
			this->advance();

			Node* expr = nullptr;
			if (*this->current_token == TokenType::TT_ASSIGN)
			{
				this->advance();
				expr = this->make_expression();
				if (!expr)
				{
					InvalidSyntaxError(
						this->current_token->pos_start, this->current_token->pos_end, "Expected expression"
					);
					return nullptr;
				}
			}
			variables.emplace_back(new VariableAssign(var_name, expr, start_pos, this->current_token->pos_end, true, is_const));
		}
		else if (*this->current_token == KeyWord::FUN)
		{
			this->advance();
			Node* fun = this->make_fun_def<true>();

			functions.emplace_back(fun);
		}
		else if (*this->current_token == KeyWord::CLASS)
		{
			this->advance();
			Node* classs = this->make_class_def();

			classes.emplace_back(classs);
		}
		else
		{
			InvalidSyntaxError(
				this->current_token->pos_start, this->current_token->pos_end, "Expected class or function"
			);
		}


		if (*this->current_token != TokenType::TT_SEMI_COLON && *this->current_token != TokenType::TT_NEW_LINE)
		{
			InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected ';'");

			if (!functions.empty()) Errors.pop_back();
		} else this->advance();
	}
	this->advance();

	return new NClass(class_name, inh, variables, functions, classes, start_pos, this->current_token->pos_end);
}

template<bool is_class>
Node* Parser::make_fun_def()
{
	Position start_pos = this->current_token->pos_start;

	if (*this->current_token != TokenType::TT_IDENTIFIER)
	{
		InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected function name");
		return nullptr;
	}
	std::string fun_name = this->current_token->value;
	const bool is_constructor = fun_name == "__init__" && is_class;
	LANG_ASSERT(!fun_name.empty(), "function must have a name");

	this->advance();
	if (*this->current_token != TokenType::TT_LPAREN)
	{
		InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected '('");
	} else this->advance();

	// Return type(s)
	std::vector<std::string> return_types;
	if (*this->current_token == TokenType::TT_ARROW)
	{
		this->advance();
		if (*this->current_token == TokenType::TT_IDENTIFIER)
		{
			return_types.emplace_back(this->current_token->value);
			this->advance();

			while (*this->current_token == TokenType::TT_BIT_OR)
			{
				this->advance();
				if (*this->current_token == TokenType::TT_IDENTIFIER)
				{
					return_types.emplace_back(this->current_token->value);
					this->advance();
				} else InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected type");
			}
		} else InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected type");
	}

	// Parameters
	std::vector<Fun::Argument> parameters;
	uint64_t co_argcount = 0;
	uint64_t default_count = 0;
	if constexpr (is_class)
	{
		Fun::Argument arg_this{ Token(TokenType::TT_IDENTIFIER, "self"), Fun::Argument::ArgType::NORMAL, nullptr };
		parameters.emplace_back(arg_this);
		co_argcount++;
	}

	if (*this->current_token != TokenType::TT_RPAREN)
	{
		if (*this->current_token == TokenType::TT_EOF)
		{
			InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected ')'");
			return nullptr;
		}
		bool has_arbitary = false;
		bool has_default = false;

		Fun::Argument arg = this->make_arg();
		parameters.emplace_back(arg);
		this->advance();
		/***********************************************************************************/
		if (arg.type == Fun::Argument::ARBITARY) has_arbitary = true;
		if (arg.ddefault) { default_count++; has_default = true; }
		if (arg.type == Fun::Argument::NORMAL) co_argcount++;
		if (arg.type == Fun::Argument::ARBITARY && arg.ddefault)
			InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Arbitary argment mustn't have a default value");
		/***********************************************************************************/
		while (*this->current_token == TokenType::TT_COMMA)
		{
			this->advance();
			arg = this->make_arg();
			parameters.emplace_back(arg);
			/***********************************************************************************/
			if (arg.ddefault) default_count++;
			if (arg.type == Fun::Argument::NORMAL) co_argcount++;
			if (has_arbitary && (arg.type == Fun::Argument::ARBITARY))
				InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Can't be two arbitary arguments");
			if (has_default && !arg.ddefault)
				InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Default argments must be last arguments");
			if (arg.type == Fun::Argument::ARBITARY && arg.ddefault)
				InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Arbitary argment mustn't have a default value");
			if (has_arbitary && !arg.ddefault)
				InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "after Arbitary argment all arguments must have a default value");
			/***********************************************************************************/
			this->advance();
		}

		if (*this->current_token != TokenType::TT_RPAREN)
		{
			InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected ')'");
		} else this->advance();
	} else this->advance();

	// Body
	if (*this->current_token != TokenType::TT_LBRACE)
	{
		InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected '{'");
	} else this->advance();

	Node* body = this->block<true>();
	NodeVector& expressions = static_cast<Block*>(body)->expressions;

	for (Node* expression: expressions)
	{
		if ((*expression) == NodeType::RETURN)
		{
			Return* return_node = static_cast<Return*>(expression);
			if (return_node->expr && is_constructor)
				InvalidSyntaxError(return_node->start_pos, return_node->end_pos, "'__init__' function mustn't return a value");
		}
	}

	return new Fun(fun_name, co_argcount, default_count, parameters, expressions, return_types, is_class, start_pos, this->current_token->pos_end);
}

Node* Parser::make_lambda()
{
	Position start_pos = this->current_token->pos_start;

	if (*this->current_token != TokenType::TT_LPAREN)
	{	
		InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected '('");
	} else this->advance();

	// Return type(s)
	std::vector<std::string> return_types;
	if (*this->current_token == TokenType::TT_ARROW)
	{
		this->advance();
		if (*this->current_token == TokenType::TT_IDENTIFIER)
		{
			return_types.emplace_back(this->current_token->value);
			this->advance();

			while (*this->current_token == TokenType::TT_BIT_OR)
			{
				this->advance();
				if (*this->current_token == TokenType::TT_IDENTIFIER)
				{
					return_types.emplace_back(this->current_token->value);
					this->advance();
				} else InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected type");
			}
		} else InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected type");
	}

	// Parameters
	std::vector<Fun::Argument> parameters;
	uint64_t co_argcount = 0;
	uint64_t default_count = 0;
	if (*this->current_token != TokenType::TT_RPAREN)
	{
		if (*this->current_token == TokenType::TT_EOF)
		{
			InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected ')'");
			return nullptr;
		}
		bool has_arbitary = false;
		bool has_default = false;

		Fun::Argument arg = this->make_arg();
		parameters.emplace_back(arg);
		this->advance();
		/***********************************************************************************/
		if (arg.type == Fun::Argument::ARBITARY) has_arbitary = true;
		if (arg.ddefault) { default_count++; has_default = true; }
		if (arg.type == Fun::Argument::NORMAL) co_argcount++;
		if (arg.type == Fun::Argument::ARBITARY && arg.ddefault)
			InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Arbitary argment mustn't have a default value");
		/***********************************************************************************/
		while (*this->current_token == TokenType::TT_COMMA)
		{
			this->advance();
			arg = this->make_arg();
			parameters.emplace_back(arg);
			/***********************************************************************************/
			if (arg.ddefault) default_count++;
			if (arg.type == Fun::Argument::NORMAL) co_argcount++;
			if (has_arbitary && (arg.type == Fun::Argument::ARBITARY))
				InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Can't be two arbitary arguments");
			if (has_default && !arg.ddefault)
				InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Default argments must be last arguments");
			if (arg.type == Fun::Argument::ARBITARY && arg.ddefault)
				InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Arbitary argment mustn't have a default value");
			if (has_arbitary && !arg.ddefault)
				InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "after Arbitary argment all arguments must have a default value");
			/***********************************************************************************/
			this->advance();
		}

		if (*this->current_token != TokenType::TT_RPAREN)
		{
			InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected ')'");
		} else this->advance();
	} else this->advance();

	// Body
	if (*this->current_token != TokenType::TT_LBRACE)
	{
		InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected '{'");
	} else this->advance();

	Node* body = this->block<true>();
	NodeVector& expressions = static_cast<Block*>(body)->expressions;

	return new Fun("<anonymous>", co_argcount, default_count, parameters, expressions, return_types, false, start_pos, this->current_token->pos_end);
}

Fun::Argument Parser::make_arg()
{
	Fun::Argument::ArgType type = Fun::Argument::ArgType::NORMAL;
	Node* Default = nullptr;
	Token tok;

	if (*this->current_token == TokenType::TT_MUL)
	{
		type = Fun::Argument::ArgType::ARBITARY;
		this->advance();
	}
	if (*this->current_token != TokenType::TT_IDENTIFIER)
	{
		InvalidSyntaxError(
			this->current_token->pos_start, this->current_token->pos_end, "Expected Identefier"
		);
	} else tok = *this->current_token;

	if (*this->current_token == TokenType::TT_ASSIGN)
	{
		if (type == Fun::Argument::ArgType::ARBITARY)
		{
			InvalidSyntaxError(
				this->current_token->pos_start, this->current_token->pos_end, "Arbitary Arguments don't supprt default values"
			);
		} this->advance();

		Default = this->make_expression();

		if (!Default)
		{
			InvalidSyntaxError(
				this->current_token->pos_start, this->current_token->pos_end, "Expected expression"
			);
		}
	}

	return { tok, type, Default };
}

Node* Parser::make_import()
{
	Position pos(this->current_token->pos_start);
	std::string module_path = this->current_token->value + "." exten;
	this->advance();


	return new Import(module_path, pos, this->current_token->pos_end);
}

void Parser::make_statements()
{
	while (*this->current_token != TokenType::TT_EOF && (*this->current_token) && this->current_token)
	{
		this->Trees.emplace_back(this->make_statement());
		if (*this->current_token != TokenType::TT_SEMI_COLON)
		{
			InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected ';'");
			if (!this->Trees.empty()) if (in(*(this->Trees.back()), non_semi_colon)) Errors.pop_back();
		} else this->advance();
	}
}

template<bool is_return, bool is_con_break>
Node* Parser::make_statement()
{
	Position start_pos = this->current_token->pos_start;

	if (*this->current_token == KeyWord::VAR)
	{
		this->advance();

		bool is_const = false;
		if (*this->current_token == KeyWord::CONST)
		{
			this->advance();
			is_const = true;
		}

		if (*current_token != TokenType::TT_IDENTIFIER)
		{
			InvalidSyntaxError(
				this->current_token->pos_start, this->current_token->pos_end, "Expected Identifier"
			);
			return nullptr;
		}

		std::string var_name = this->current_token->value;
		this->advance();

		Node* expr = nullptr;
		if (*this->current_token == TokenType::TT_ASSIGN)
		{
			this->advance();
			expr = this->make_expression();
			if (!expr)
			{
				InvalidSyntaxError(
					this->current_token->pos_start, this->current_token->pos_end, "Expected expression"
				);
				return nullptr;
			}
		}
		return new VariableAssign(var_name, expr, start_pos, this->current_token->pos_end, true, is_const);
	}
	else if (*this->current_token == KeyWord::FUN)
	{
		this->advance();
		Node* fun = this->make_fun_def();

		return fun;
	}
	else if (*this->current_token == KeyWord::CLASS)
	{
		this->advance();
		Node* classs = this->make_class_def();

		return classs;
	}
	else if (*this->current_token == KeyWord::IF)
	{
		this->advance();
		Node* If = this->make_if<is_return, is_con_break>();

		return If;
	}
	else if (*this->current_token == KeyWord::WHILE)
	{
		this->advance();
		Node* While = this->make_while<is_return>();

		return While;
	}
	else if (*this->current_token == KeyWord::FOR)
	{
		this->advance();
		Node* For = this->make_for<is_return>();

		return For;
	}
	else if (*this->current_token == KeyWord::IMPORT)
	{
		this->advance();
		Node* import_module = this->make_import();

		return import_module;
	}

	return this->make_expression();
}

template<bool is_return, bool is_con_break>
Node* Parser::make_if()
{
	Position start_pos = this->current_token->pos_start;

	if (*this->current_token != TokenType::TT_LPAREN)
	{
		InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected '('");
	} else this->advance();

	Node* condition = this->make_expression();
	if (!condition)
		InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected expression");

	if (*this->current_token != TokenType::TT_RPAREN)
	{
		InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected ')'");
	} else this->advance();

	Node* body;
	if (*this->current_token == TokenType::TT_LBRACE)
	{
		this->advance();
		body = this->block<is_return, is_con_break>();
	}
	else
	{
		Node* st = this->make_statement<is_return, is_con_break>();
		if (!st)
			InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected statement");

		NodeVector expressions;
		expressions.emplace_back(st);

		body = new Block(expressions, start_pos, this->current_token->pos_end);
	}

	Node* elsee = nullptr;
	if (*this->current_token == KeyWord::ELIF)
	{
		this->advance();
		elsee = this->make_if<is_return, is_con_break>();
	}
	else if (*this->current_token == KeyWord::ELSE)
	{
		this->advance();
		if (*this->current_token == TokenType::TT_LBRACE)
		{
			this->advance();
			elsee = this->block<is_return, is_con_break>();
		}
		else
		{
			Position st_start(this->current_token->pos_start);
			elsee = this->make_statement<is_return, is_con_break>();
			if (!elsee)
				InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected statement");
		}
	}

	return new IfStatment(condition, body, elsee, start_pos, this->current_token->pos_end);
}

template<bool is_return>
Node* Parser::make_while()
{
	Position start_pos = this->current_token->pos_start;

	if (*this->current_token != TokenType::TT_LPAREN)
	{
		InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected '('");
	} else this->advance();

	Node* condition = this->make_expression();
	if (!condition)
		InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected expression");

	if (*this->current_token != TokenType::TT_RPAREN)
	{
		InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected ')'");
	} else this->advance();

	Node* body;
	if (*this->current_token == TokenType::TT_LBRACE)
	{
		this->advance();
		body = this->block<is_return, true>();
	}
	else
	{
		Node* st = this->make_statement<is_return, true>();
		if (!st)
			InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected statement");

		NodeVector expressions;
		expressions.emplace_back(st);

		body = new Block(expressions, start_pos, this->current_token->pos_end);
	}

	return new WhileStatment(condition, body, start_pos, this->current_token->pos_end);
}

template<bool is_return>
Node* Parser::make_for()
{
	Position start_pos = this->current_token->pos_start;

	if (*this->current_token != TokenType::TT_LPAREN)
	{
		InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected '('");
	} else this->advance();

	Node* counter = this->make_statement();
	if (counter)
	{
		if (!in(*counter, { NodeType::ACCESS, NodeType::ASSIGN }))
		{
			InvalidSyntaxError(
				this->current_token->pos_start, this->current_token->pos_end,
				"Expected Counter Definition or Assign"
			);
		}
	}
	if (*this->current_token != TokenType::TT_SEMI_COLON)
	{
		InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected ';'");
	} else this->advance();

	Node* condition = this->make_expression();
	if (*this->current_token != TokenType::TT_SEMI_COLON)
	{
		InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected ';'");
	} else this->advance();

	Node* increase = this->make_expression();

	if (*this->current_token != TokenType::TT_RPAREN)
	{
		InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected ')'");
	} else this->advance();


	Node* body;
	if (*this->current_token == TokenType::TT_LBRACE)
	{
		this->advance();
		body = this->block<is_return, true>();
	}
	else
	{
		Node* st = this->make_statement<is_return, true>();
		if (!st)
			InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected statement");

		NodeVector expressions;
		expressions.emplace_back(st);

		body = new Block(expressions, start_pos, this->current_token->pos_end);
	}

	return new ForStatment(counter, condition, increase, body, start_pos, this->current_token->pos_end);
}

template<bool is_return, bool is_con_break>
Node* Parser::block()
{
	Position start_pos = this->current_token->pos_start;

	NodeVector expressions;
	while (*this->current_token != TokenType::TT_RBRACE)
	{
		if (*this->current_token == TokenType::TT_EOF)
		{
			InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected '}'");
			break;
		}

		if (*this->current_token == KeyWord::RETURN)
		{
			if constexpr(!is_return)
			{
				InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "");
				this->advance();
			}
			else
			{
				this->advance();
				Node* expr = this->make_expression();

				expressions.emplace_back(new Return(expr, start_pos, this->current_token->pos_end));
			}
		}
		else if (*this->current_token == KeyWord::CONTINUE)
		{
			if constexpr (!is_con_break)
			{
				InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "");
				this->advance();
			}
			else
			{
				this->advance();
				expressions.emplace_back(new Continue(start_pos, this->current_token->pos_end));
			}
		}
		else if (*this->current_token == KeyWord::BREAK)
		{
			if constexpr (!is_con_break)
			{
				InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "");
				this->advance();
			}
			else
			{
				this->advance();
				expressions.emplace_back(new Break(start_pos, this->current_token->pos_end));
			}
		}

		Node* st = this->make_statement<is_return, is_con_break>();
		if (st) expressions.emplace_back(st);

		if (*this->current_token != TokenType::TT_SEMI_COLON && *this->current_token != TokenType::TT_NEW_LINE)
		{
			InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected ';'");

			if (!expressions.empty())
			{
				if (in(*(expressions.back()), {
						NodeType::WHILE_STATEMENT,
						NodeType::FOR_STATEMENT,
						NodeType::IF_STATEMENT,
						NodeType::BLOCK,
						NodeType::FUN,
					}
				)){
					Errors.pop_back();
				}
			}
		} else this->advance();
	}
	this->advance();

	return new Block(expressions, start_pos, this->current_token->pos_end);
}

Node* Parser::arithmetic()
{
	Position start_pos(this->current_token->pos_start);

	Node* left = this->term();
	while (*this->current_token == TokenType::TT_PLUS || *this->current_token == TokenType::TT_MINUS)
	{
		TokenType op = this->current_token->type;
		this->advance();
		Node* right = this->term();
		if (!right) InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected expression");

		left = new BinOp(left, op, right, start_pos, this->current_token->pos_end);
	}
	return left;
}

static constexpr std::initializer_list<TokenType> arth = {
	TokenType::TT_MINUS_EQUAL,
	TokenType::TT_PLUS_EQUAL,
	TokenType::TT_MUL_EQUAL,
	TokenType::TT_DIV_EQUAL,
	TokenType::TT_MODULO,
	TokenType::TT_MUL,
	TokenType::TT_DIV
};

Node* Parser::term()
{
	Node* left = this->factor();
	Position start_pos(this->current_token->pos_start);

	while (in(*this->current_token, arth))
	{
		TokenType op = this->current_token->type;
		this->advance();
		Node* right = this->factor();

		if (!right)
			InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected expression");

		left = new BinOp(left, op, right, start_pos, this->current_token->pos_end);
	}
	return left;
}

static constexpr std::initializer_list<TokenType> UnOp = {
	TokenType::TT_MINUS_MINUS,
	TokenType::TT_PLUS_PLUS,
	TokenType::TT_BIT_NOT,
	TokenType::TT_BIT_AND,
	TokenType::TT_MINUS,
	TokenType::TT_NOT
};

Node* Parser::factor()
{
	Position start_pos = this->current_token->pos_start;
	Node* result = nullptr;

	if (*this->current_token == TokenType::TT_IDENTIFIER)
	{
		result = this->identifier();
		if ((*this->current_token == TokenType::TT_MINUS_MINUS) || (*this->current_token == TokenType::TT_PLUS_PLUS))
		{
			result = new UnaryOp(result, this->current_token->type, start_pos, this->current_token->pos_end, true);
			this->advance();
		}
	}
	else if (in(*this->current_token, UnOp))
	{
		TokenType op = this->current_token->type;
		this->advance();
		Node* left = this->factor();

		if (op == TokenType::TT_BIT_AND && *left != NodeType::ACCESS)
			InvalidSyntaxError(left->start_pos, left->end_pos, "Expected Variable name");

		result = new UnaryOp(left, op, start_pos, this->current_token->pos_end);
	}
	else if (*this->current_token == TokenType::TT_INT || *this->current_token == TokenType::TT_FLOAT)
	{
		LANG_ASSERT(!(this->current_token->value.empty()), "object must have a value");

		const std::string value = this->current_token->value;
		const bool is_float = (*this->current_token) == TokenType::TT_FLOAT;
		this->advance();

		result = new Number(value, is_float, start_pos, this->current_token->pos_end);
	}
	else if (*this->current_token == TokenType::TT_LPAREN)
	{
		this->advance();
		Node* expr = this->make_expression();

		if (*this->current_token != TokenType::TT_RPAREN)
			InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected ')'");
		this->advance();

		result = expr;
	}
	else if (*this->current_token == TokenType::TT_STRING)
	{
		std::string value = this->current_token->value;
		this->advance();
		result = new String(value, start_pos, this->current_token->pos_end);
	}
	else if (*this->current_token == TokenType::TT_LSQUARE)
	{
		this->advance();
		result = this->make_list();
	}
	else if (*this->current_token == KeyWord::LAMBDA)
	{
		this->advance();
		result = this->make_lambda();
	}

	if (*this->current_token == TokenType::TT_DOT)
	{
		this->advance();
		result = this->make_class_call(result, start_pos);
	}

	return result;
}

Node* Parser::identifier(const bool is_const, const bool declare)
{
	Position start_pos = this->current_token->pos_start;
	std::string var_name = this->current_token->value;
	Node* result = nullptr;
	this->advance();

	if (*this->current_token == TokenType::TT_ASSIGN)
	{
		this->advance();
		Node* expr = this->make_expression();

		result = new VariableAssign(var_name, expr, start_pos, this->current_token->pos_end, declare, is_const);
	}
	else if (*this->current_token == TokenType::TT_LPAREN)
	{
		this->advance();

		if (*this->current_token == TokenType::TT_RPAREN)
		{
			this->advance();
			return new FunCall(var_name, nullptr, NodeVector(), start_pos, this->current_token->pos_end);
		}

		NodeVector params;
		Position expr_start_pos(this->current_token->pos_start);
		Node* expr = this->make_expression();
		if (!expr) InvalidSyntaxError(expr_start_pos, this->current_token->pos_end, "Expected expression");


		params.emplace_back(expr);
		while (*this->current_token == TokenType::TT_COMMA)
		{
			Position expr_start_pos(this->current_token->pos_start);
			this->advance();
			expr = this->make_expression();
			if (!expr) InvalidSyntaxError(expr_start_pos, this->current_token->pos_end, "Expected expression");

			params.emplace_back(expr);
		}

		if (*this->current_token != TokenType::TT_RPAREN)
		{
			InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Expected ')'");
		} else this->advance();

		Position end(this->current_token->pos_end);
		result = new FunCall(var_name, nullptr, params, start_pos, this->current_token->pos_end);
	}
	else
	{
		Position end(this->current_token->pos_end);
		result = new VariableAcess(var_name, start_pos, this->current_token->pos_end);
	}

	return result;
}

Node* Parser::make_class_call(Node* class_node, const Position& start_pos)
{
	Node* member_node = this->factor();
	if (!member_node) RunTimeError(start_pos, this->current_token->pos_end, "Expected token");

	return new ClassCall(class_node, member_node, start_pos, this->current_token->pos_end);
}

template<bool skip>
void Parser::advance()
{
	this->current_token = this->pos < (*this->tokens).size() ? &((*tokens)[this->pos++]) : nullptr;

	if (this->current_token)
	{
		if (*this->current_token == TokenType::TT_LBRACE)
			this->br.push(TokenType::TT_LBRACE);
		else if (*this->current_token == TokenType::TT_RBRACE)
		{
			if (this->br.empty())
			{
				InvalidSyntaxError(this->current_token->pos_start, this->current_token->pos_end, "Unexpected '}'");
			} else this->br.pop();
		}

		if (((*this->current_token == TokenType::TT_WHITE_SPACE) || *this->current_token == TokenType::TT_NEW_LINE) && skip)
			this->advance();
	}
}
