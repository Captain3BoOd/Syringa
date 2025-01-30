#include <unordered_map>
#include <unordered_set>

#include "lexer.hpp"
#include "help.hpp"

const std::unordered_set<std::string> KEYWORDS = {
	"if",
	"elif",
	"else",
	"while",
	"for",
	"var",
	"const",
	"this",
	"return",
	"continue",
	"break",
	"fun",
	"lambda",
	"class"
};

Lexer::Lexer(const std::string& file_name, const std::string& text):
	pos(-1, 0, -1, file_name, text),
	text(text),
	current_char(0)
{
	LANG_ASSERT(!this->text.empty(), "Lexer hates empty files");
	this->pos.text = this->text;
	this->advance();
}

void Lexer::advance()
{
	this->pos.advance(this->current_char);
	this->current_char = this->pos.index < this->text.size() ? this->text[this->pos.index] : static_cast<char>(0);
}

inline void orderingErrors();
std::vector<Token>& Lexer::makeTokens()
{
	while (this->current_char)
	{
		if (std::isdigit(this->current_char))
		{
			this->makeNumber();
		}
		else if (std::isalpha(this->current_char) || this->current_char == '_')
		{
			this->makeIdentifier();
		}
		else if (this->current_char == '"')
		{
			this->makeString();
		}
		else if (this->current_char == ';')
		{
			this->Tokens.emplace_back(TokenType::TT_SEMI_COLON, ";", this->pos);
			this->advance();
		}
		else if (this->current_char == ']')
		{
			this->Tokens.emplace_back(TokenType::TT_RSQUARE, "]", this->pos);
			this->advance();
		}
		else if (this->current_char == '[')
		{
			this->Tokens.emplace_back(TokenType::TT_LSQUARE, "[", this->pos);
			this->advance();
		}
		else if (this->current_char == '}')
		{
			this->Tokens.emplace_back(TokenType::TT_RBRACE, "}", this->pos);
			this->advance();
		}
		else if (this->current_char == '{')
		{
			this->Tokens.emplace_back(TokenType::TT_LBRACE, "{", this->pos);
			this->advance();
		}
		else if (this->current_char == ',')
		{
			this->Tokens.emplace_back(TokenType::TT_COMMA, ",", this->pos);
			this->advance();
		}
		else if (this->current_char == '.')
		{
			this->Tokens.emplace_back(TokenType::TT_DOT, ".", this->pos);
			this->advance();
		}
		else if (this->current_char == '%')
		{
			this->Tokens.emplace_back(TokenType::TT_MODULO, "%", this->pos);
			this->advance();
		}
		else if (this->current_char == '~')
		{
			this->Tokens.emplace_back(TokenType::TT_BIT_NOT, "~", this->pos);
			this->advance();
		}
		else if (this->current_char == '^')
		{
			this->Tokens.emplace_back(TokenType::TT_NOR, "^", this->pos);
			this->advance();
		}
		else if (this->current_char == '>')
		{
			Position pos(this->pos);
			this->advance();
			if (this->current_char == '=')
			{
				this->Tokens.emplace_back(TokenType::TT_GREATER_THAN_EQUAL, ">=", pos, this->pos);
				this->advance();
			}
			else this->Tokens.emplace_back(TokenType::TT_GREATER_THAN, ">", pos);
		}
		else if (this->current_char == '<')
		{
			Position pos(this->pos);
			this->advance();
			if (this->current_char == '=')
			{
				this->Tokens.emplace_back(TokenType::TT_LESS_THAN_EQUAL, "<=", pos, this->pos);
				this->advance();
			}
			else this->Tokens.emplace_back(TokenType::TT_LESS_THAN, "<", pos);
		}
		else if (this->current_char == '&')
		{
			Position pos(this->pos);
			this->advance();
			if (this->current_char == '&')
			{
				this->Tokens.emplace_back(TokenType::TT_AND, "&&", pos, this->pos);
				this->advance();
			}
			else this->Tokens.emplace_back(TokenType::TT_BIT_AND, "&", pos);
		}
		else if (this->current_char == '!')
		{
			Position pos(this->pos);
			this->advance();
			if (this->current_char == '=')
			{
				this->Tokens.emplace_back(TokenType::TT_NOT_EQUAL, "!=", pos, this->pos);
				this->advance();
			}
			else this->Tokens.emplace_back(TokenType::TT_NOT, "!", pos);
		}
		else if (this->current_char == '|')
		{
			Position pos(this->pos);
			this->advance();
			if (this->current_char == '|')
			{
				this->Tokens.emplace_back(TokenType::TT_OR, "||", pos, this->pos);
				this->advance();
			}
			else this->Tokens.emplace_back(TokenType::TT_BIT_OR, "|", pos);
		}
		else if (this->current_char == '=')
		{
			Position pos(this->pos);
			this->advance();
			if (this->current_char == '=')
			{
				this->Tokens.emplace_back(TokenType::TT_EQUAL, "==", pos, this->pos);
				this->advance();
			}
			else this->Tokens.emplace_back(TokenType::TT_ASSIGN, "=", pos);
		}
		else if (this->current_char == '*')
		{
			Position pos(this->pos);
			this->advance();
			if (this->current_char == '=')
			{
				this->Tokens.emplace_back(TokenType::TT_MUL_EQUAL, "*=", pos, this->pos);
				this->advance();
			}
			else this->Tokens.emplace_back(TokenType::TT_MUL, "*", pos);
		}
		else if (this->current_char == '/')
		{
			Position pos(this->pos);
			this->advance();
			if (this->current_char == '=')
			{
				this->Tokens.emplace_back(TokenType::TT_MUL_EQUAL, "/=", pos, this->pos);
				this->advance();
			}
			else if (this->current_char == '/')
			{
				this->advance();
				this->makeComment();
			}
			else this->Tokens.emplace_back(TokenType::TT_DIV, "/", this->pos);
		}
		else if (this->current_char == '+')
		{
			Position pos(this->pos);
			this->advance();
			if (this->current_char == '=')
			{
				this->Tokens.emplace_back(TokenType::TT_PLUS_EQUAL, "+=", pos, this->pos);
				this->advance();
			}
			else if (this->current_char == '+')
			{
				this->Tokens.emplace_back(TokenType::TT_PLUS_PLUS, "++", pos, this->pos);
				this->advance();
			}
			else this->Tokens.emplace_back(TokenType::TT_PLUS, "+", this->pos);
		}
		else if (this->current_char == '-')
		{
			Position pos(this->pos);
			this->advance();
			if (this->current_char == '=')
			{
				this->Tokens.emplace_back(TokenType::TT_MINUS_EQUAL, "-=", pos, this->pos);
				this->advance();
			}
			else if (this->current_char == '-')
			{
				this->Tokens.emplace_back(TokenType::TT_MINUS_MINUS, "--", pos, this->pos);
				this->advance();
			}
			else this->Tokens.emplace_back(TokenType::TT_MINUS, "-", this->pos);
		}
		else if (this->current_char == '(')
		{
			this->Tokens.emplace_back(TokenType::TT_LPAREN, "(", this->pos);
			this->advance();
		}
		else if (this->current_char == ')')
		{
			this->Tokens.emplace_back(TokenType::TT_RPAREN, ")", this->pos);
			this->advance();
		}
		else if (in(this->current_char, { ' ', '\t' }))
		{
			this->Tokens.emplace_back(TokenType::TT_WHITE_SPACE, "", this->pos);
			this->advance();
		}
		else if (this->current_char == '\n')
		{
			this->Tokens.emplace_back(TokenType::TT_NEW_LINE, "", this->pos);
			this->advance();
		}
		else
		{
			Position pos(this->pos);
			const char c = this->current_char;
			this->advance();
			IllegalCharError(pos, this->pos, std::format("'{}'", c));
		}
	}
	this->Tokens.emplace_back(TokenType::TT_EOF, "", this->pos);
	orderingErrors();

	return Tokens;
}

void Lexer::makeNumber()
{
	std::string number;
	bool is_float = false;
	Position pos(this->pos);

	while (std::isdigit(this->current_char) || this->current_char == '.')
	{
		if (this->current_char == '.')
		{
			if (!is_float) is_float = true;
			else
			{
				Position dot_pos(this->pos);
				number.push_back(this->current_char);
				this->advance();
				number.push_back(this->current_char);
				this->advance();

				
				UnKnownTypeError(dot_pos, this->pos, number);
				return;
			}
		}

		number.push_back(this->current_char);
		this->advance();
	}
	if (number.back() == '.')
	{
		InvalidSyntaxError(pos, this->pos, "Expected number");
		return;
	}

	this->Tokens.emplace_back(is_float ? TokenType::TT_FLOAT : TokenType::TT_INT, number, pos, this->pos);
}

void Lexer::makeComment()
{
	while (this->current_char != '\n' && this->current_char)
		this->advance();
	this->advance();
}

void Lexer::makeIdentifier()
{
	std::string identifier;
	Position pos(this->pos);
	while (std::isalnum(this->current_char) || this->current_char == '_')
	{
		identifier.push_back(this->current_char);
		this->advance();
	}
	this->Tokens.emplace_back(
		in(identifier, KEYWORDS) ? TokenType::TT_KEYWORD : TokenType::TT_IDENTIFIER,
		identifier, pos, this->pos
	);
}

static const std::unordered_map<char, std::string> escape_chars = {
	{ 'n', "\n" },
	{ 't', "\t" },
	{ 'a', "\a" },
	{ 'r', "\r" },
	{ '\\', "\\" }
};

void Lexer::makeString()
{
	std::string string;
	Position pos(this->pos);
	bool escape = false;

	this->advance();
	while (this->current_char != '"')
	{
		if (this->current_char == '\n' || this->current_char == 0)
		{
			InvalidSyntaxError(pos, this->pos, "");
			return;
		}

		if (escape)
		{
			if (!in(this->current_char, { 'n', 't', '\\', 'r', 'a' }))
			{
				Position escape_start_pos(this->pos);
				this->advance();
				Position escape_end_pos(this->pos);
				this->advance();

				
				InvalidSyntaxError(escape_start_pos, escape_end_pos, "Invalid excape charecter");
				return;
			}

			string += escape_chars.at(this->current_char);
			escape = false;
			this->advance();
			continue;
		}

		if (this->current_char == '\\')
		{
			escape = true;
			this->advance();
			continue;
		}
		string.push_back(this->current_char);
		this->advance();
	}
	this->advance();

	this->Tokens.emplace_back(TokenType::TT_STRING, string, pos, this->pos);
}

void orderingErrors()
{
	Errorr* first_error = nullptr;
	for (size_t i = 0; i < Errors.size(); i++)
	{
		if (first_error = Errors[i].get__type() == ErrorType::IllegalChar? &Errors[i] : nullptr)
		{
			while (i < Errors.size() && Errors[i].get__type() == ErrorType::IllegalChar)
			{
				first_error->pos_end = Errors[i].pos_end;
				first_error->details.insert(first_error->details.end() - 1, Errors[i].details[1]);
				i++;
			}
			first_error->details.erase(first_error->details.begin() + 1);

			auto start = Errors.begin() + (first_error - &Errors[0]) + 1;
			auto end = Errors.begin() + i;
			Errors.erase(start, end);
		}
	}
}