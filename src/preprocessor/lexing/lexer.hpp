#ifndef PREPROCESSOR_LEXER_HPP
#define PREPROCESSOR_LEXER_HPP

#include "../../common.hpp"

#include <cstddef>
#include <string>
#include <sstream>
#include <fstream>

PREPROCESSOR_API_BEGIN

static const std::string include = "#include";

auto copy_file(const std::string& path) -> std::string {
    std::fstream file(path);
    std::istringstream iss;
    iss >> file.rdbuf();

    return iss.str();
}

auto process_include(const std::string& line) -> bool {
    bool found_opening, found_closed {};
    std::string file;

    for (std::size_t i = 0; i < line.length(); ++i) {
        if (line[i] == ' ')
            continue; // ignore white space

        if (!found_opening) {
            if (line[i] != '<')
                return false; // error
        }

        auto pos = line.find(">", i);
        if (pos == std::string::npos) {
            return false; // error
        }

        file = line.substr(++i, pos);
        copy_file(file);

        return true;
    }

    return false;
}

auto preprocess(const std::string& path) -> void {
    std::fstream file(path);
    
    while (!file.eof()) {
        std::string line;
        std::getline(file, line);

        if (auto pos = line.find("#include") != std::string::npos) {
           process_include(line.substr(pos + include.length())); 
        }
    }

}

PREPROCESSOR_API_END

#endif // !PREPROCESSOR_LEXER_HPP
