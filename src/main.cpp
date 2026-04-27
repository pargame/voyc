#include "voyc/lexer.hpp"

#include <fstream>
//#include <functional>
#include <iostream>
#include <string>

static const char *tokenTypeToString(TokenType type) {
  switch (type) {
  case TokenType::Unknown:
    return "Unknown";
  case TokenType::Comment:
    return "Comment";
  case TokenType::Identifier:
    return "Identifier";
  case TokenType::LitInt:
    return "LitInt";
  case TokenType::LitFloat:
    return "LitFloat";
  case TokenType::LitString:
    return "LitString";
  case TokenType::KwRet:
    return "KwRet";
  case TokenType::KwRef:
    return "KwRef";
  case TokenType::KwInt:
    return "KwInt";
  case TokenType::KwString:
    return "KwString";
  case TokenType::KwFloat:
    return "KwFloat";
  case TokenType::KwZone:
    return "KwZone";
  case TokenType::KwLocal:
    return "KwLocal";
  case TokenType::KwOperator:
    return "KwOperator";
  case TokenType::Plus:
    return "Plus";
  case TokenType::Minus:
    return "Minus";
  case TokenType::Asterisk:
    return "Asterisk";
  case TokenType::Slash:
    return "Slash";
  case TokenType::Equal:
    return "Equal";
  case TokenType::Less:
    return "Less";
  case TokenType::LessEqual:
    return "LessEqual";
  case TokenType::And:
    return "And";
  case TokenType::Or:
    return "Or";
  case TokenType::Not:
    return "Not";
  case TokenType::Colon:
    return "Colon";
  case TokenType::Semicolon:
    return "Semicolon";
  case TokenType::LeftParen:
    return "LeftParen";
  case TokenType::RightParen:
    return "RightParen";
  case TokenType::LeftBrace:
    return "LeftBrace";
  case TokenType::RightBrace:
    return "RightBrace";
  case TokenType::LeftBracket:
    return "LeftBracket";
  case TokenType::RightBracket:
    return "RightBracket";
  case TokenType::Tilde:
    return "Tilde";
  case TokenType::Execute:
    return "Execute";
  default:
    return "UnknownType";
  }
}

static void printLexResult(const voyc::LexResult &result) {
  std::cout << "Tokens: " << result.tokens.size() << "\n";
  for (const auto &token : result.tokens) {
    std::cout << token.line << ":" << token.column << " " << tokenTypeToString(token.type)
              << " '" << token.lexeme << "'\n";
  }
  if (!result.errors.empty()) {
    std::cout << "Errors: " << result.errors.size() << "\n";
    for (const auto &error : result.errors) {
      std::cout << error << "\n";
    }
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: 'voyc [file.voy]'" << std::endl;
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << argv[1] << std::endl;
    return 1;
  }

  std::string const src((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  const voyc::LexResult lexResult = voyc::lexSource(src);
  printLexResult(lexResult);

  return 0;
}