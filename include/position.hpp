#pragma once

#include <string>

struct Position
{
	int index, line, column;
	std::string_view text;
	std::string file_name;

	Position(int index, int line, int column, const std::string& file_name, const std::string_view text);
	Position(const Position& other);
	Position();
	bool is_default() const;
	void operator=(const Position& other);

	void advance(char current_char);
};
