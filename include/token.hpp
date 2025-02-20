#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "position.hpp"
#include "types.hpp"
#include "error.hpp"

enum class TokenType : uint8_t
{
	TT_INT,
	TT_FLOAT,
	TT_IDENTIFIER,
	TT_KEYWORD,
	TT_STRING,
/*******************************/
	TT_PLUS,
	TT_MINUS,
	TT_ASSIGN,
	TT_EQUAL,
	TT_MUL,
	TT_DIV,
	TT_MODULO,
	TT_MINUS_EQUAL,
	TT_MINUS_MINUS,
	TT_PLUS_EQUAL,
	TT_DIV_EQUAL,
	TT_MUL_EQUAL,
	TT_PLUS_PLUS,
/*******************************/
	TT_GREATER_THAN_EQUAL,
	TT_LESS_THAN_EQUAL,
	TT_GREATER_THAN,
	TT_LESS_THAN,
	TT_AND,
	TT_OR,
	TT_NOT,
	TT_NOT_EQUAL,
	TT_BIT_AND,
	TT_BIT_OR,
	TT_BIT_NOT,
	TT_NOR,
/*******************************/
	TT_LPAREN,
	TT_RPAREN,
	TT_LSQUARE,
	TT_RSQUARE,
	TT_LBRACE,
	TT_RBRACE,
	TT_COMMA,
	TT_DOT,
	TT_ARROW,
	TT_SEMI_COLON,
/*******************************/
	TT_WHITE_SPACE,
	TT_NEW_LINE,
	TT_EOF,
	TT_NONE
};

enum class KeyWord: uint8_t
{
	IF,
	ELIF,
	ELSE,
	WHILE,
	FOR,
	VAR,
	CONST,
	THIS,
	RETURN,
	CONTINUE,
	BREAK,
	FUN,
	LAMBDA,
	CLASS,
	// NULl,
	IMPORT,
	NONE
};

struct Token
{
	Token();
	Token(TokenType type);
	Token(const Token& other);
	Token(TokenType type, const std::string& value);
	Token(TokenType type, const std::string& value, const Position& pos_start);
	Token(TokenType type, const std::string& value, const Position& pos_start, const Position& pos_end);

	const Token& operator= (const Token& other);
	bool operator==(TokenType type) const;
	bool operator==(KeyWord type) const;
	operator bool() const;

	TokenType type;
	KeyWord keyword; // faster than string
	std::string value;

	Position pos_start;
	Position pos_end;

	operator std::string() const;
};
