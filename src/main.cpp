#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <ranges>
#include <functional>
#include <unordered_map>

#include "repl.hpp"

using CommandFn = std::function<void(const std::vector<std::string>&)>;

// Tokenises the `input` string by spaces
std::vector<std::string> tokenize(const std::string& input)
{ 
    auto tokens = input | std::ranges::views::split(' ') 
                        | std::ranges::views::transform([](auto&& range) {
                            return std::string(range.begin(), range.end());
                          });
    return {std::begin(tokens), std::end(tokens)};
}

// Command Implementations
void handle_add(const std::vector<std::string>& tokens) 
{
    try 
    {
        std::cout << add_all(tokens) << "\n";
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "[ERROR] : " << e.what() << "\n";
    }
}

void handle_mod(const std::vector<std::string>& tokens)
{
    try
    {
        std::cout << mod(tokens) << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "[ERROR] : " << e.what() << "\n";
    }
}

void handle_echo(const std::vector<std::string>& tokens) 
{
    print_all_v(tokens);
}

void handle_help(const std::unordered_map<std::string, CommandFn>& commands)
{
    for (const auto& [command, fn] : commands)
    {
        std::cout << command << "\n";
    }
}

// Main
int main()
{
    std::unordered_map<std::string, CommandFn> commands 
    {
        {"add", handle_add},
        {"mod", handle_mod},
        {"echo", handle_echo},
        {"help", handle_help}
    };

    std::string input;
    while (std::cout << ">>> " && std::getline(std::cin, input))
    {
        std::vector<std::string> tokens = tokenize(input);

        if (tokens.empty()) continue;
        if (input == "exit" || input == "quit") break;
        if (input == "help") handle_help(commands);
        
        if (auto it = commands.find(tokens[0]); it != commands.end()) 
        {
            it->second(tokens);
        } 
        else 
        {
            std::cerr << "[ERROR] Unknown command `" << tokens[0] << "'\n";
        }
    }
}
