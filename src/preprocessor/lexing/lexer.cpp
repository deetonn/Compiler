#include "lexer.hpp"

#include <sstream>
#include <fstream>
#include <string>

static const std::string single_line_comment_entry_token = "//";
static const std::string multi_line_comment_entry_token = "/*";
static const std::string multi_line_comment_exit_token = "*/";
static const std::string include = "#include";

//TODO: support syntax for: # include
//TODO: make into a class, have member var of output_file_buf
//TODO: add a two bools in the class to check if currently in a single line comment and multi-line comment
//TODO: use std::set to keep track of how many files have already been preprocessed
//TODO: change return types to be <result, error>
//TODO: append intermediate file buf to output_file buf if preprocessed
//TODO: add more preprocessing functionality

PREPROCESSOR_API_BEGIN

[[nodiscard]]
auto process_multi_line_comment(std::fstream& file) -> bool {
    while (!file.eof()) {
        std::string line;
        std::getline(file, line);

        if (line.find(multi_line_comment_exit_token) != std::string::npos) {
            //TODO: remove intermediate buffer from entry token to exit token

            return true; // found exit token
        }
    }

    return false;
}

[[nodiscard]]
auto copy_file_contents(const std::string& path) -> std::string {
    std::fstream file(path);
    std::istringstream iss;
    iss >> file.rdbuf();

    return iss.str();
}

[[nodiscard]]
auto process_include(const std::string& line) -> bool {
    bool found_entry_token {};
    std::string path;
    std::string escape_token;

    for (std::size_t i = 0; i < line.length(); ++i) {
        if (line[i] == ' ')
            continue; // ignore white space

        // search for entry token
        if (!found_entry_token) {
            switch (line[i]) {
                case '<':
                    escape_token = ">";
                    found_entry_token = true;
                continue; // iterate to next index safely
 
                case '\"':
                    escape_token = "\"";
                    found_entry_token = true;
                continue; // iterate to next index safely

                default:
                    return false; // invalid #include syntax
            }
        }

        // search for exit token
        auto pos = line.find(escape_token, i);
        if (pos == std::string::npos) {
            return false; // exit token not found
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

        // remove single line comments
        if (auto pos = line.find(single_line_comment_entry_token); pos != std::string::npos) {
            if (pos == 0) {
                line = "";
                continue; // comment is removed, line is now empty
            }

            // comment is removed, line contains code
            line.resize(pos - 1);
        }

        // remove multi-line comments
        if (auto entry_pos = line.find(multi_line_comment_entry_token); entry_pos != std::string::npos) {
            // check if entry token and exit token are on the same line
            if (auto exit_pos = line.find(multi_line_comment_exit_token, entry_pos + multi_line_comment_entry_token.length()); exit_pos != std::string::npos) {
                // remove substring from string
                line.erase(entry_pos, exit_pos + 1);
            }

            // iterate over each line until an exit token is found
            process_multi_line_comment(file); 
        }

        if (auto pos = line.find(include); pos != std::string::npos) {
           process_include(line.substr(pos + include.length())); 
        }
    }

}

PREPROCESSOR_API_END

