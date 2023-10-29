#include "lexer.hpp"

#include <sstream>
#include <fstream>
#include <string>

static const std::string single_line_comment = "//";
static const std::string include = "#include";

//TODO: support syntax for: # include
//TODO: make into a class, have member var of output_file_buf
//TODO: use std::set to keep track of how many files have already been preprocessed
//TODO: change return types to be <result, error>
//TODO: append intermediate file buf to output_file buf if preprocessed
//TODO: add more preprocessing functionality

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
    std::string path;
    std::string escape_token;

    for (std::size_t i = 0; i < line.length(); ++i) {
        if (line[i] == ' ')
            continue; // ignore white space

        // search for opening token
        if (!found_opening_token) {
            switch (line[i]) {
                case '<':
                    escape_token = ">";
                    found_opening_token = true;
                continue; // iterate to next index safely
 
                case '\"':
                    escape_token = "\"";
                    found_opening_token = true;
                continue; // iterate to next index safely

                default:
                    return false; // invalid #include syntax
            }
        }

        // search for escape token
        auto pos = line.find(escape_token, i);
        if (pos == std::string::npos) {
            return false; // escape token not found
        }

        // capture the file path
        path = line.substr(i, pos - 1);
        auto buf = copy_file_contents(path);

        return true;
    }

    return false;
}

auto preprocess(const std::string& path) -> void {
    std::fstream file(path);

    while (!file.eof()) {
        std::string line;
        std::getline(file, line);

        // remove comments
        if (auto pos = line.find(single_line_comment); pos != std::string::npos) {
            if (pos == 0) {
                line = "";
                return; // comment is removed, line is now empty
            }

            // comment is removed, line contains code
            line.resize(pos - 1);
        }

        if (auto pos = line.find(include); pos != std::string::npos) {
           process_include(line.substr(pos + include.length())); 
        }
    }

}

PREPROCESSOR_API_END

