#pragma once
#include <string_view>

enum class TokenType {
  EndOfFile,  // EOF
  Unknown,    // 알 수 없는 토큰
  Comment,    // #
  Identifier, // var names, zone names, etc.

  LitInt,    // 0~9
  LitFloat,  // n.n
  LitString, // 'abc 123 #&*/'

  KwRet,      // ret
  KwRef,      // ref
  KwInt,      // int
  KwString,   // string
  KwFloat,    // float
  KwZone,     // ZONE{...}
  KwLocal,    // LOCAL{...}
  KwOperator, // OP{...}

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
  size_t line;
  size_t column;
};