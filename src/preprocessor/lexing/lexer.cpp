#include "lexer.hpp"
#include "constants.hpp"

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

lexer::lexer() 
  : m_preprocessed{}
  , m_buffer{}
  , m_is_single_line_comment{}
  , m_is_multi_line_comment{}
{}

[[nodiscard]]
auto lexer::process_multi_line_comment(std::fstream& file) -> bool {
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
auto lexer::copy_file_contents(const std::string& path) -> std::string {
    std::fstream file(path);
    std::istringstream iss;
    iss >> file.rdbuf();

    return iss.str();
}

[[nodiscard]]
auto lexer::process_include(const std::string& line) -> bool {
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

auto lexer::trim_leading_whitespace(std::string& line) -> void {
    auto pos = line.find_first_not_of(' ');
    if (pos == std::string::npos) {
        line = "";
        return; // line contains no non-whitespace characters
    }

    line = line.substr(pos);
}

auto lexer::lex_directive(const std::string& line) -> bool {
    std::string buffer = "";
    for (const auto ch : line) {
        if (!is_valid_directive_char(ch)) {
            if (buffer.length() > 0) {
                break;
            }
        }

        buffer += ch;
    }

    return tokens.contains(buffer);
}

auto lexer::lex_tokens(const std::string& line) -> bool {
    trim_leading_whitespace(line);

    for (std::size_t i = 0; i < line.length(); ++i) {
        if (line[i] == ' ')
            continue; // ignore whitespace

        // preprocessor directive
        if (line[i] == '#') {
            if (i + 1 >= line.length())
                return false; // error

            lex_directive(line.substr(i + 1));
        }
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

    return false;
}

auto lexer::preprocess(const std::string& path) -> void {
    if (m_preprocessed.contains(path))
        return;

    m_preprocessed.insert(path);
    std::fstream file(path);

    while (!file.eof()) {
        std::string line;
        std::getline(file, line);

        preprocess_line(line);
    }

}

PREPROCESSOR_API_END

