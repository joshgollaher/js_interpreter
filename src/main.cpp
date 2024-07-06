#include <iostream>
#include <fstream>

#include "AST.h"
#include "Lexer.h"
#include "Parser.h"

std::string load_file(const std::string& file_name)
{
    std::ifstream file(file_name, std::ios_base::in | std::ios_base::ate);

    const size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(file_size);
    file.read(buffer.data(), static_cast<long long>(file_size));

    return {buffer.data(), buffer.size()};
}

int main(const int argc, char **argv)
{
    if(argc != 2)
    {
        std::cerr << "Pass file as an argument." << std::endl;
        return EXIT_FAILURE;
    }

    Log::the().set_level(Log::Level::INFO);

    auto file_string = load_file(argv[1]);

    JS::Lexer lexer(file_string);
    auto tokens = lexer.lex("hello_world.js");

    for(auto& token : tokens)
    {
        std::cout << token.to_string() << std::endl;
    }

    JS::Parser parser(tokens);
    JS::AST ast = parser.parse();

    return EXIT_SUCCESS;
}
