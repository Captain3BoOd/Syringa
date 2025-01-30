#pragma once

#include <string>
#include <vector>
#include <memory>

#include "position.hpp"
#include "token.hpp"
#include "types.hpp"

class Lexer
{
public:
	Lexer(const std::string& file_name, const std::string& text);
	std::vector<Token>& makeTokens();
private:
	void advance();
	void makeNumber();
	inline void makeComment();
	void makeIdentifier();
	void makeString();
private:
	Position pos;
	std::string text;
	char current_char;
	std::vector<Token> Tokens;
};
