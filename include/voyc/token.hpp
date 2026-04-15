#pragma once
#include <string_view>

enum class TokenType {
    Comment, // #
    Identifier, // var names, space names, etc.
    
    Plus, // +
    Minus, // -
    Asterisk, // *
    Slash, // /
    
    Equal, // =
    Less, // <
    And, // &
    Or, // |
    Not, // !

    Colon, // :
    Semicolon, // ;  
    LeftParen, // (
    RightParen, // )
    LeftBrace, // {
    RightBrace, // }
    LeftBracket, // [
    RightBracket, // ]

    Period, // .
    Arrow, // ->
};

struct Token {
    TokenType type;
    std::string_view lexeme;
    int line;
    int column;
};