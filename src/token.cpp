#include <unordered_map>
#include <string>

#include "token.hpp"

std::unordered_map<TokenType, std::string> tokentype = {
	{ TokenType::TT_INT, "INT" },
	{ TokenType::TT_FLOAT, "FLOAT" },
	{ TokenType::TT_PLUS, "PLUS" },
	{ TokenType::TT_MINUS, "MINUS" },
	{ TokenType::TT_IDENTIFIER, "IDENTIFIER" },
	{ TokenType::TT_STRING, "STRING" },
	{ TokenType::TT_KEYWORD, "KEYWORD" },
	{ TokenType::TT_MUL, "MUL" },
	{ TokenType::TT_DIV, "DIV" },
	{ TokenType::TT_LPAREN, "LPAREN" },
	{ TokenType::TT_RPAREN, "RPAREN" },
	{ TokenType::TT_ASSIGN, "ASSIGN" },
	{ TokenType::TT_EQUAL, "EQUAL" },
	{ TokenType::TT_EOF, "EOF" },
	{ TokenType::TT_LBRACE, "LBRACE" },
	{ TokenType::TT_RBRACE, "RBRACE" },
	{ TokenType::TT_COMMA, "COMMA" },
	{ TokenType::TT_DOT, "DOT" },
	{ TokenType::TT_RSQUARE, "RSQUARE" },
	{ TokenType::TT_LSQUARE, "LSQUARE" },
	{ TokenType::TT_SEMI_COLON, "SEMI_COLON" },
	{ TokenType::TT_NEW_LINE, "NEW_LINE" },
	{ TokenType::TT_WHITE_SPACE, "WHITE SPACE" },
	{ TokenType::TT_NONE, "" }
};

static const std::unordered_map<std::string, KeyWord> Keywords = {
	{ "if", KeyWord::IF },
	{ "elif", KeyWord::ELIF },
	{ "else", KeyWord::ELSE },
	{ "while", KeyWord::WHILE },
	{ "for", KeyWord::FOR },
	{ "var", KeyWord::VAR },
	{ "const", KeyWord::CONST },
	{ "this", KeyWord::THIS },
	{ "return", KeyWord::RETURN },
	{ "continue", KeyWord::CONTINUE },
	{ "break", KeyWord::BREAK },
	{ "fun", KeyWord::FUN },
	{ "lambda", KeyWord::LAMBDA },
	{ "class", KeyWord::CLASS },
	{ "import", KeyWord::IMPORT },
	// { "null", KeyWord::NULl },
	{ "", KeyWord::NONE },
};

Token::Token():
	type(TokenType::TT_NONE),
	keyword(KeyWord::NONE)
{}

Token::Token(TokenType type, const std::string& value, const Position& pos_start):
	type(type),
	keyword(type == TokenType::TT_KEYWORD? Keywords.at(value) : KeyWord::NONE),
	value(value),
	pos_start(pos_start),
	pos_end(pos_start)
{
	for (const char& curr_char : this->value)
		this->pos_end.advance(curr_char);
}

Token::Token(const Token& other):
	type(other.type),
	pos_start(other.pos_start),
	pos_end(other.pos_end),
	value(other.value),
	keyword(other.keyword)
{}

Token::Token(TokenType type, const std::string& value):
	type(type),
	value(value),
	keyword(KeyWord::NONE)
{}

Token::Token(TokenType type):
	type(type),
	keyword(type == TokenType::TT_KEYWORD ? Keywords.at(value) : KeyWord::NONE)
{}

Token::Token(TokenType type, const std::string& value, const Position& pos_start, const Position& pos_end):
	type(type),
	keyword(type == TokenType::TT_KEYWORD ? Keywords.at(value) : KeyWord::NONE),
	value(value),
	pos_start(pos_start),
	pos_end(pos_end)
{}

const Token& Token::operator=(const Token& other)
{
	this->type = other.type;
	this->pos_start = other.pos_start;
	this->pos_end = other.pos_end;
	this->value = other.value;
	this->keyword = other.keyword;

	return *this;
}

bool Token::operator==(TokenType type) const
{
	return (this->type == type) && type != TokenType::TT_NONE;
}

bool Token::operator==(KeyWord keyword) const
{
	return (this->keyword == keyword) && keyword != KeyWord::NONE;
}

Token::operator bool() const
{
	return this->type != TokenType::TT_NONE;
}

Token::operator std::string() const
{
	return tokentype.at(this->type) + (
		!this->value.empty() ? ':' + this->value : ""
	);
}
