#include "lexer.hpp"

#include <sstream>
#include <fstream>

static const std::string include = "#include";

//TODO: make into a class, have member var of output_file_buf
//TODO: use std::set to keep track of how many files have already been preprocessed
//TODO: change return types to be <result, error>
//TODO: add intermediate file buf to output_file buf if preprocessed

PREPROCESSOR_API_BEGIN

[[nodiscard]]
auto copy_file_contents(const std::string& path) -> std::string {
    std::fstream file(path);
    std::istringstream iss;
    iss >> file.rdbuf();

    return iss.str();
}

[[nodiscard]]
auto process_include(const std::string& line) -> bool {
    bool found_opening_token {};
    std::string file;
    std::string escape_token;

    for (std::size_t i = 0; i < line.length(); ++i) {
        if (line[i] == ' ')
            continue; // ignore white space

        if (!found_opening_token) {
            switch (line[i]) {
                case '<':
                    escape_token = ">";
                break;
                
                case '\"':
                    escape_token = "\"";
                break;
                
                default:
                    return false; // invalid #include syntax
            }
        }

        auto pos = line.find(escape_token, i);
        if (pos == std::string::npos) {
            return false; // escape token not found
        }

        file = line.substr(++i, pos);
        auto buf = copy_file_contents(file);

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

