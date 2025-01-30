#include "position.hpp"

Position::Position():
	index(-1),
	line(0),
	column(-1)
{}

Position::Position(int index, int line, int column, const std::string& file_name, const std::string_view text):
	index(index),
	line(line),
	column(column),
	file_name(file_name),
	text(text)
{}

Position::Position(const Position& other):
	column(other.column),
	index(other.index),
	line(other.line),
	file_name(other.file_name),
	text(other.text)
{}

bool Position::is_default() const
{
	return this->column == -1 && this->index == -1 && this->line == 0 && this->file_name.empty();
}

void Position::operator=(const Position& other)
{
	this->column = other.column;
	this->index = other.index;
	this->line = other.line;
	this->file_name = other.file_name;
	this->text = other.text;
}

void Position::advance(char current_char)
{
	this->index++;
	this->column++;

	if (current_char == '\n')
	{
		this->column = 0;
		this->line++;
	}
}
