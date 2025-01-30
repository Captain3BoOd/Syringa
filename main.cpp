#include <filesystem>
#include <iostream>
#include <fstream>
#include <span>

#include "interpreter.hpp"
#include "builtin.hpp"
#include "parser.hpp"
#include "lexer.hpp"

static std::string Read_file(std::filesystem::path path);
static bool Write_file(std::filesystem::path path, std::string_view code);

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
	const std::string code = Read_file(file_path);

	if (code.empty()) return 0;

	Lexer lexer(file_name.empty()? "<stdin>" : file_name, code);
	std::vector<Token>& tokens = lexer.makeTokens();
	if (!Errors.empty())
	{
		for (Errorr error : Errors)
			std::cout << error << '\n';
		return 0;
	}
	DONE(lexer);

	Parser parser(tokens);
	NodeVector& trees = parser.Parse();
	if (!Errors.empty())
	{
		for (Errorr error : Errors)
			std::cout << error << '\n';

		DestroyNodes(trees);
		return 0;
	}
	tokens.clear();
	DONE(parser);

	Interpreter interpreter(trees);
	SymbolTable& global_symbol = interpreter.get_vars();

	#pragma region Built In Objects
	{
		/* Functions */
		global_symbol.add("print", make_function("print", BuiltIn::print, 1, 0, {}), true);
		global_symbol.add("info", make_function("print", BuiltIn::info, 1, 1, {}), true);
		global_symbol.add("input", make_function("input", BuiltIn::input, 1, 1, {}), true);
		global_symbol.add("address", make_function("id", BuiltIn::print, 1, 1, {}), true);

		/* Variables */
		global_symbol.add("None", make_none(), true);
	}
	#pragma endregion
	/***************************************************************************/
	try { interpreter.interpret(); }
	catch (const ObjectPtr& error) { std::cout << std::endl << error->__str__() << std::endl; }
	DONE(interpreter);
	/***************************************************************************/
	DestroyNodes(trees);
	DONE(Destroing Nodes);
}

std::string Read_file(std::filesystem::path path)
{
	std::ifstream file(path);

	if (!file.is_open())
	{
		std::cerr << "File didin't found" << std::endl;
		return "";
	}

	std::string text, line;

	while (std::getline(file, line))
		text += line + '\n';

	if (!text.empty())
		if (text.back() == '\n')
			text.pop_back();

	return text;
}

bool Write_file(std::filesystem::path path, std::string_view code)
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
