#pragma once
#include <string_view>

enum class TokenType {
  Unknown,

  //Number
  LitInt,   // 0~9
  LitFloat, // n.n

  //Name
  Identifier, // var names, zone names, etc.
  KwRet,      // ret
  KwRef,      // ref
  KwInt,      // int
  KwString,   // string
  KwFloat,    // float
  KwZone,     // ZONE{...}
  KwLocal,    // LOCAL{...}
  KwOperator, // OP{...}

  // Certain
  Plus,         // +
  Minus,        // -
  Asterisk,     // *
  Slash,        // /
  Comment,      // #
  Equal,        // =
  Less,         // <
  LessEqual,    // <=
  And,          // &
  Or,           // |
  Not,          // !
  LitString,    // 'abc 123 #&*/'
  Colon,        // :
  Semicolon,    // ;
  LeftParen,    // (
  RightParen,   // )
  LeftBrace,    // {
  RightBrace,   // }
  LeftBracket,  // [
  RightBracket, // ]
  Tilde,        // ~
  Execute,      // >
};

struct Token {
  TokenType type;
  std::string_view lexeme;
  size_t line;
  size_t column;
};