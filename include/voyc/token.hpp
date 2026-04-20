#pragma once
#include <string_view>

enum class TokenType {
  EndOfFile,  // EOF
  Unknown,    // 알 수 없는 토큰
  Comment,    // #
  Identifier, // var names, space names, etc.
  Number,     // 0~9

  Plus,     // +
  Minus,    // -
  Asterisk, // *
  Slash,    // /

  Equal,     // =
  Less,      // <
  LessEqual, // <=
  And,       // &
  Or,        // |
  Not,       // !

  Colon,        // :
  Semicolon,    // ;
  LeftParen,    // (
  RightParen,   // )
  LeftBrace,    // {
  RightBrace,   // }
  LeftBracket,  // [
  RightBracket, // ]

  Period,  // .
  Execute, // >
};

struct Token {
  TokenType type;
  std::string_view lexeme;
  int line;
  int column;
};