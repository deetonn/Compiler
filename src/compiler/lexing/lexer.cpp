#include "lexer.hpp"
#include "constants.hpp"

auto compiler::lexer::lex_tokens() noexcept -> result<void, error> {
    while (m_internals.position <= m_source_info.contents().size()) {
        auto lex_result = this->lex_single_char(this->peek_current());
        if (lex_result.is_err()) {
            // TODO: add an actual diagnostics system for outputting errors.
            const char* msg = lex_result.get_err()->what();
            eprintln("[LEXER]: failed to lex contents. ({})", msg);
            return error("{}", msg);
        }
        else {
            auto&& token = std::move(*lex_result.get());
            // discard empty tokens.
#if LEXER_DEBUG
            eprintln("[LEXER]: lexed token ({}) at ({})", token.to_string(), get_source_location().to_string());
#endif
            if (token.type() != token_type::EMPTY) {
                m_tokens.push_back(token);
            }
        }
    }

    return {};
}

auto compiler::lexer::lex_single_char(char c) noexcept -> result<token, error> {
    switch (c) {
    case '\n':
        m_internals.line += 1;
        m_internals.column = 0;
        return make_token(token_type::EMPTY);
    case '\r':
        if (peek_next() == '\n') {
            move_forward();
            m_internals.line += 1;
            m_internals.column = 0;
        }
        return make_token(token_type::EMPTY);
    case semi_colon:
        return make_token(token_type::SEMI_COLON);
    case space:
        return make_token(token_type::EMPTY);
    case left_paren:
        return make_token(token_type::LEFT_PAREN);
    case right_paren:
        return make_token(token_type::RIGHT_PAREN);
    case left_brace:
        return make_token(token_type::LEFT_BRACE);
    case right_brace:
        return make_token(token_type::RIGHT_BRACE);
    case left_bracket:
        return make_token(token_type::LEFT_BRACKET);
    case right_bracket:
        return make_token(token_type::RIGHT_BRACKET);
    case comma:
        return make_token(token_type::COMMA);
    case dot:
        return make_token(token_type::DOT);
    case minus:
        if (is_valid_number_content(peek_next())) {
            break;        
        }
        if (peek_next() == greater_than) {
            this->move_forward();
            return make_token(token_type::ARROW);
        }
        if (peek_next() == minus) {
            this->move_forward();
            return make_token(token_type::MINUS_MINUS);
        }
        if (peek_next() == equal) {
            this->move_forward();
            return make_token(token_type::PLUS_EQUAL);
        }
        return make_token(token_type::MINUS);
    case plus:
        if (is_valid_number_content(peek_next())) {
            // NOTE: This check does nothing. We need more context.
            break;
        }
        if (peek_next() == equal) {
            this->move_forward();
            return make_token(token_type::PLUS_EQUAL);
        }
        if (peek_next() == plus) {
            this->move_forward();
            return make_token(token_type::PLUS_PLUS);
        }
        return make_token(token_type::ADD);
    case slash:
        if (peek_next() == equal) {
            this->move_forward();
            return make_token(token_type::SLASH_EQUAL);
        }
        return make_token(token_type::SLASH);
    case star:
        if (peek_next() == equal) {
            this->move_forward();
            return make_token(token_type::STAR_EQUAL);
        }
        return make_token(token_type::STAR);
    case lesser_than:
        if (peek_next() == equal) {
            this->move_forward();
            return make_token(token_type::LESSER_EQUALS);
        }
        return make_token(token_type::LESSER_THAN);
    case greater_than:
        if (peek_next() == equal) {
            this->move_forward();
            return make_token(token_type::GREATER_EQUALS);
        }
        return make_token(token_type::GREATER_THAN);
    case equal:
        if (peek_next() == equal) {
            this->move_forward();
            return make_token(token_type::EQUALS_EQUALS);
        }
        return make_token(token_type::EQUALS);
    case ampersand:
        if (peek_next() == ampersand) {
            this->move_forward();
            return make_token(token_type::AND);
        }
        return make_token(token_type::AMPERSAND);
    case pipe:
        if (peek_next() == pipe) {
            this->move_forward();
            return make_token(token_type::OR);
        }
        return make_token(token_type::BITWISE_OR);
    case tilde:
        return make_token(token_type::BITWISE_NOT);
    case percent:
        return make_token(token_type::MODULO);
    case caret:
        if (peek_next() == equal) {
            this->move_forward();
            return make_token(token_type::XOR_EQUALS);
        }
        return make_token(token_type::BITWISE_XOR);
    case bang:
        if (peek_next() == equal) {
            this->move_forward();
            return make_token(token_type::NOT_EQUAL);
        }
        return make_token(token_type::BANG);
    case question_mark:
        return make_token(token_type::QUESTION_MARK);
    case colon:
        return make_token(token_type::COLON);
    case eof:
        return make_token(token_type::END_OF_FILE);
    }

    if (is_valid_identifier_start(c)) {
        return this->lex_identifier();
    }

    if (is_valid_number_start(c)) {
        return this->lex_numeric_literal();
    }

    if (c == double_quote) {
        return this->lex_string_literal();
    }

    if (c == single_quote) {
        return this->lex_char_literal();
    }

    return error("unexpected character ({}) at ({})", c, get_source_location().to_string());
}

template<class T, typename ...Others>
bool is_any_of(T val, Others... others) {
    // if val is ANY of others, return true.
    return ((val == others) || ...);
}

auto compiler::lexer::lex_numeric_literal() noexcept -> result<token, error> {
    auto contents = std::string{};

    // TODO: Integrals can contain postfixes like "i" or "u" to infer the type. 
    //       Handle these cases.

    if (!is_valid_number_start(peek_current())) {
        return error("invalid character for the start of an integral literal ({})", peek_current());
    }
    else {
        contents.push_back(peek_current());
    }

    DISCARD(advance());

    char next;
    bool encountered_dot = false;
    while (is_valid_number_content(next = peek_current())) {
        if (next == '.') {
            if (encountered_dot) {
                return error("invalid numeric literal. floating point numbers can only contain one \".\"");
            }
            encountered_dot = true;
        }
        contents.push_back(next);
        move_forward();
    }

    next = peek_current();

    if (is_any_of(next, 'f', 'u', 'i', 'l', 'd')) {
        move_forward();
        contents.push_back(next);
        if (is_any_of('f', 'd')) {
            return make_token_with_explicit_contents(token_type::FLOATING_POINT_LITERAL, std::move(contents));
        }
        else {
            if (encountered_dot) {
                return error("invalid suffix, cannot use \"{}\" suffix on a floating point number.", next);
            }
            return make_token_with_explicit_contents(token_type::INTEGER_LITERAL, std::move(contents));
        }
    }

    if (encountered_dot) {
        return make_token_with_explicit_contents(token_type::FLOATING_POINT_LITERAL, std::move(contents));
    }
    else {
        return make_token_with_explicit_contents(token_type::INTEGER_LITERAL, std::move(contents));
    }
}

auto compiler::lexer::lex_identifier() noexcept -> result<token, error> {
    std::string contents{};

    if (!is_valid_identifier_start(peek_current())) {
        return error("invalid character for the start of an identifier ({}) (A-z+_ is supported)", peek_current());
    }

    while (is_valid_identifier_rest(peek_current())) {
        contents.push_back(peek_current());
        move_forward();
    }

    if (keywords.contains(contents)) {
        // return make_token_with_explicit_contents(keywords.at(contents), std::move(contents));
        return make_token(keywords.at(contents));
    }

    return make_token_with_explicit_contents(token_type::IDENTIFIER, std::move(contents));
}

auto compiler::lexer::lex_string_literal() noexcept -> result<token, error>
{
    // expect the current character to be the first double quote.
    if (peek_current() != double_quote) {
        return error("expected double quote at ({})", get_source_location().to_string());
    }

    // move forward to the next character.
    move_forward();

    std::string contents{}; 
    while (peek_current() != double_quote) {
        if (peek_current() == eof) {
            return error("unexpected end of file while lexing string literal.");
        }
        contents.push_back(peek_current());
        move_forward();
    }

    // move forward to the next character.
    move_forward();
    return make_token_with_explicit_contents(token_type::STRING_LITERAL, std::move(contents));
}

auto compiler::lexer::move_forward() noexcept -> void {
    m_internals.position++;
    m_internals.column++;
    m_span.end++;
}

auto compiler::lexer::lex_char_literal() noexcept -> result<token, error>
{
    // lex a single character, if the character begins with the escape character, then
    // we need to lex the next character as well.
    // TODO: in the future we should support unicode characters.

    // expect the current character to be the first single quote.
    if (peek_current() != single_quote) {
        return error("expected single quote at ({})", get_source_location().to_string());
    }

    // move forward to the next character.
    move_forward();
    char contents = peek_current();

    if (contents == '\\') {
        move_forward();
        contents = peek_current();

        if (contents == single_quote) {
            return error("expected escape character after backslash at ({})", get_source_location().to_string());
        }

        auto escaped_result = lex_escape_character(contents);

        if (escaped_result.is_err())
            return std::move(*escaped_result.get_err());

        contents = *escaped_result.get();
    }

    // move forward to the next character.
    move_forward();

    // expect the current character to be the second single quote.
    if (peek_current() != single_quote) {
        return error("expected single quote at ({})", get_source_location().to_string());
    }

    // move forward to the next character.
    move_forward();

    return make_token_with_explicit_contents(token_type::CHARACTER_LITERAL, std::string{contents});
}

auto compiler::lexer::lex_escape_character(char c) noexcept -> result<char, error>
{
    switch (c) {
    case 'n':
        return '\n';
    case 'r':
        return '\r';
    case 't':
        return '\t';
    case '0':
        return '\0';
    case '\\':
        return '\\';
    case '\'':
        return '\'';
    case '\"':
        return '\"';
    default:
        return error("unrecognized escape character ({}) at ({})", c, get_source_location().to_string());
    }
}

auto compiler::lexer::make_token_with_explicit_contents(token_type kind, std::string&& content) noexcept -> token
{
    return token(kind, m_span, get_source_location(), std::move(content));
}

auto compiler::lexer::peek_current() const noexcept -> char {
    auto& src = m_source_info.contents();
    if (m_internals.position >= src.size()) {
        return eof;
    }
    return src.at(m_internals.position);
}

auto compiler::lexer::peek_next() const noexcept -> char {
    auto& src = m_source_info.contents();
    if (m_internals.position + 1 >= src.size()) {
        return eof;
    }
    return src.at(m_internals.position + 1);
}

auto compiler::lexer::advance() noexcept -> char {
    auto& src = m_source_info.contents();
    if (m_internals.position + 1 >= src.size()) {
        return eof;
    }
    m_span.end++;
    return src.at(++m_internals.position);
}

auto compiler::lexer::make_token(token_type kind, bool use_source) noexcept -> token {
    if (use_source) {
        std::string contents = this->get_current_contents();
        m_internals.position += contents.size();
        m_internals.column += contents.size();
        m_span.begin = m_span.end;
        return token(kind, m_span, get_source_location(), contents);
    }
    else {
        m_internals.position += 1;
        m_internals.column += 1;
        if (m_span.begin != m_span.end) {
            m_span.begin = m_span.end;
        }
        return token(kind, m_span, get_source_location());
    }
}

auto compiler::lexer::get_source_location() noexcept -> source_location {
    return source_location::from(m_source_info.file_name(),
        m_internals.line,
        m_internals.column
    );
}

auto compiler::lexer::get_current_contents() const noexcept -> std::string {
    auto& contents = m_source_info.contents();
    auto start_pos = m_span.begin;
    auto count = m_span.end - m_span.begin;
    return contents.substr(start_pos, count);
}