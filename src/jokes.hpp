#pragma once
#include "locales.hpp"
#include <string>
#include <vector>

namespace FATfetch {

class JokeGenerator {
public:
    static std::string getRandomJoke(Language lang = Language::PL);
    static std::string getRandomRoast(Language lang = Language::PL);
    static std::string getDietPlan(Language lang = Language::PL);
};

} // namespace FATfetch
