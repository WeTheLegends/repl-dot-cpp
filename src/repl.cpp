#include "repl.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <numeric>
#include <ranges>
#include <algorithm>
#include <cmath>

// Prints all elements except the first one
void print_all_v(const std::vector<std::string>& v)
{
    // Skip the first element
    auto rest = v | std::views::drop(1);

    for (auto& s : rest)
        std::cout << s << ' ';
    std::cout << '\n';
}

long double add_all(const std::vector<std::string>& v)
{
    if (v.size() <= 1) 
        return 0.0L;
    
    // Safe way to parse
    auto parse = [](const std::string& s) 
    {
        try 
        {
            return std::stold(s);
        } 
        catch (const std::invalid_argument&) 
        {
            throw std::runtime_error("Invalid operand in the expression: '" + s + "'");
        } 
        catch (const std::out_of_range&) 
        {
            throw std::runtime_error("Number too large to convert: '" + s + "'");
        }
    };

    // Skip the first, then transform each string to long double
    auto nums = v
        | std::views::drop(1)
        | std::views::transform(parse);

    // Now just accumulate over that view
    return std::accumulate(nums.begin(), nums.end(), 0.0L);
}

long double mod(const std::vector<std::string>& v) {
    if (v.size() <= 1) return 0;

    auto parse = [](const std::string& s) -> long double {
        try {
            return std::stold(s);
        } catch (const std::invalid_argument&) {
            throw std::runtime_error("Invalid operand: '" + s + "'");
        } catch (const std::out_of_range&) {
            throw std::runtime_error("Number too large: '" + s + "'");
        }
    };

    auto nums = v | std::views::drop(1) | std::views::transform(parse);
    
    if (std::ranges::begin(nums) == std::ranges::end(nums)) return 0; // Handle empty case

    return std::accumulate(std::ranges::begin(nums) + 1, std::ranges::end(nums), *std::ranges::begin(nums), fmodl);
}

