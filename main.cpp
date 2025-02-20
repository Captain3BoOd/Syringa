#include <filesystem>
#include <iostream>
#include <fstream>
#include <span>

#include "interpreter.hpp"
#include "builtin.hpp"
#include "parser.hpp"
#include "lexer.hpp"
#include "make.hpp"
#include "help.hpp"

std::vector<NodeVector> all_nodes;
SymbolTable global_symbol_table;

inline NodeVector lex_parse(const std::string& file_name, const std::string& code);
inline bool Write_file(std::filesystem::path path, std::string_view code);
inline std::pair<std::string, bool> Read_file(std::filesystem::path path);
inline void init_global_symbol_table();

int main(const int argc, const char* argv[])
{
	using namespace std::string_view_literals;
	const std::filesystem::path serenga_compiler_path(argv[0]);
	const std::vector<std::string_view> args{ argv + 1, argv + argc };
	if (args.empty()) return 0;
	/*****************************************************************************/
	const std::filesystem::path file_path = args[0];
	const std::string file_name = file_path.string();
	const std::string file_name_stem = file_path.stem().string();
	const auto [code, status] = Read_file(file_path);

	if (!status) std::cerr << "file " + file_name + "not found" << std::endl;
	if (code.empty()) return 0;
	NodeVector trees = lex_parse(file_name, code);
	if (trees.empty()) return 0;
	all_nodes.emplace_back(trees);

	Interpreter interpreter(trees, &global_symbol_table);
	init_global_symbol_table();
	/***************************************************************************/
	try { interpreter.interpret(); }
	catch (const ObjectPtr& error) { std::cerr << std::endl << error->__str__() << std::endl; }
	DONE(interpreter);
	/***************************************************************************/
	for (NodeVector& trees : all_nodes) DestroyNodes(trees);
	DONE(Destroing All Nodes);
}

inline void init_global_symbol_table()
{
	#define ADD(name, value, is_const) global_symbol_table.add(name, value, is_const)
	/*****************************************************************************************/
	/* Functions */
	ADD("print", make_function("print", BuiltIn::print, 1, 0, {}), true);
	ADD("info", make_function("print", BuiltIn::info, 1, 1, {}), true);
	ADD("input", make_function("input", BuiltIn::input, 1, 1, {}), true);
	ADD("address", make_function("id", BuiltIn::print, 1, 1, {}), true);

	/* Variables */
	ADD("None", make_none(), true);
	/*****************************************************************************************/
	#undef ADD(name, value, is_const)
}

NodeVector lex_parse(const std::string& file_name, const std::string& code)
{
	Lexer lexer(file_name, code);
	std::vector<Token>& tokens = lexer.makeTokens();
	if (!Errors.empty())
	{
		for (Errorr error : Errors)
			std::cout << error << '\n';
		return {};
	}
	DONE(lexer);

	Parser parser(tokens);
	NodeVector& trees = parser.Parse();
	if (!Errors.empty())
	{
		for (Errorr error : Errors)
			std::cout << error << '\n';

		DestroyNodes(trees);
		return {};
	}
	tokens.clear();
	DONE(parser);

	return trees;
}

inline std::pair<std::string, bool> Read_file(std::filesystem::path path)
{
	std::ifstream file(path);
	if (!file.is_open()) return { "", false };
	std::string text, line;

	while (std::getline(file, line))
		text += line + '\n';

	if (!text.empty())
		if (text.back() == '\n')
			text.pop_back();

	return { text, true };
}

inline bool Write_file(std::filesystem::path path, std::string_view code)
{
	std::ofstream file(path);

	if (!file.is_open())
	{
		std::cerr << "File didin't found" << std::endl;
		return false;
	}

	file.write(code.data(), code.size());
	return true;
}
