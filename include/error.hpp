#pragma once

#include <string>
#include <format>
#include <vector>
#include <ostream>
#include <unordered_map>

#include "position.hpp"

inline std::string string_with_arrows(const std::string& text, const Position& pos_start, const Position& pos_end)
{
	std::string result;

	size_t idx_start = text.rfind('\n', pos_start.index);
	if (idx_start == std::string::npos) idx_start = 0;
	size_t idx_end = text.find('\n', idx_start + 1);
	if (idx_end == std::string::npos) idx_end = text.length();

	int line_count = pos_end.line - pos_start.line + 1;
	for (int i = 0; i < line_count; i++)
	{
		std::string line = text.substr(idx_start, idx_end - idx_start);
		int col_start = (i == 0) ? pos_start.column : 0;
		int col_end = (i == line_count - 1) ? pos_end.column : static_cast<int>(line.length());

		col_start = std::min(col_start, static_cast<int>(line.length()));
		col_end = std::min(col_end, static_cast<int>(line.length()));

		result += line + '\n';
		result += std::string(col_start, ' ') + std::string(col_end - col_start, '^') + '\n';

		idx_start = idx_end;
		idx_end = text.find('\n', idx_start + 1);
		if (idx_end == std::string::npos)
			idx_end = text.length();
	}

	result.erase(std::remove(result.begin(), result.end(), '\t'), result.end());

	return result;
}

enum class ErrorType : uint8_t
{
	NONE = 0,
	RunTime,
	TypeError,
	UnKnownType,
	IllegalChar,
	InvalidSyntax,
};

class Errorr
{
public:
	Errorr(const Errorr& other);
	Errorr(Position start, Position end, ErrorType type, std::string details);
	friend std::ostream& operator<<(std::ostream& out, const Errorr& error);
	friend void orderingErrors();
	ErrorType get__type() const;
protected:
	std::string details;
	Position pos_start;
	Position pos_end;
private:
	ErrorType type;
};

inline Errorr::Errorr(const Errorr& other):
	pos_start(other.pos_start),
	pos_end(other.pos_end),
	details(other.details),
	type(other.type)
{}

inline Errorr::Errorr(Position pos_start, Position pos_end, ErrorType type, std::string details):
	type(type),
	details(details),
	pos_end(pos_end),
	pos_start(pos_start)
{}

inline ErrorType Errorr::get__type() const
{
	return this->type;
}

inline const std::unordered_map<ErrorType, std::string> error_ =
{
	{ ErrorType::IllegalChar, "Illegal Character" },
	{ ErrorType::InvalidSyntax, "Invalid Syntax" },
	{ ErrorType::UnKnownType, "UnKnown Type" },
	{ ErrorType::RunTime, "Run Time Error" },
	{ ErrorType::TypeError, "Type Error" },
	{ ErrorType::NONE, "" },
};

inline std::ostream& operator<<(std::ostream& out, const Errorr& error) // not finished
{
	const std::string error_name = error_.at(error.type);

	if (
		error.pos_start.is_default()
		|| error.pos_end.is_default()
	) {
		out << std::format("{}: {}\n", error_name, error.details);
		return out;
	}

	std::string result = std::format("{}: {}\n", error_name, error.details);
	result += std::format("File {} at Line {}\n", error.pos_start.file_name, error.pos_start.line + 1);
	result += "\n\n" + string_with_arrows(error.pos_start.text.data(), error.pos_start, error.pos_end);

	out << result;
	return out;
}
/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/
#ifndef INTERPRETER
inline std::vector<Errorr> Errors;

#define IllegalCharError(pos_start, pos_end, details) Errors.emplace_back(pos_start, pos_end, ErrorType::IllegalChar, details)
#define UnKnownTypeError(pos_start, pos_end, details) Errors.emplace_back(pos_start, pos_end, ErrorType::UnKnownType, details)
#define InvalidSyntaxError(pos_start, pos_end, details) Errors.emplace_back(pos_start, pos_end, ErrorType::InvalidSyntax, details)
#define TypeError(pos_start, pos_end, details) Errors.emplace_back(pos_start, pos_end, ErrorType::TypeError, details)
#define RunTimeError(pos_start, pos_end, details) Errors.emplace_back(pos_start, pos_end, ErrorType::RunTime, details)
#else
#define InvalidSyntaxError(pos_start, pos_end, details) throw make_error(pos_start, pos_end, ErrorType::InvalidSyntax, details)
#define IllegalCharError(pos_start, pos_end, details) throw make_error(pos_start, pos_end, ErrorType::IllegalChar, details)
#define UnKnownTypeError(pos_start, pos_end, details) throw make_error(pos_start, pos_end, ErrorType::UnKnownType, details)
#define RunTimeError(pos_start, pos_end, details) throw make_error(pos_start, pos_end, ErrorType::RunTime, details)
#define TypeError(pos_start, pos_end, details) throw make_error(pos_start, pos_end, ErrorType::TypeError, details)
#endif