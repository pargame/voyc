#include "voyc/lexer.hpp"
#include "voyc/token.hpp"

#include <cctype>
#include <string_view>

namespace voyc {

namespace {

class Lexer {
  // 역할: string_view로 된 소스코드를 읽어서 토큰으로 분해하는 역할
private:
  std::string_view src_;
  int line_;
  int column_;
  int index_;

public:
  Lexer(std::string_view src) : src_(src), line_(1), column_(1), index_(0) {
  }

  LexResult run() {
    LexResult result;

    Token next = peekToken();
    while (next.type != TokenType::EndOfFile) {
      if (next.type == TokenType::Unknown) {
        appendError(result.errors, next);
      } else {
        appendToken(result.tokens, next);
      }
    }

    return result;
  }

private:
  Token peekToken() {
  }

  void appendError(std::vector<std::string> &errors, Token &next) {
  }
  void appendToken(std::vector<Token> &tokens, Token &next) {
  }

  TokenType tokenTypeFromChar(int index) {
    switch (src_[index]) {
    case '#':
      return TokenType::Comment;
    case '+':
      return TokenType::Plus;
    case '-':
      return TokenType::Minus;
    case '*':
      return TokenType::Asterisk;
    case '/':
      return TokenType::Slash;
    case '=':
      return TokenType::Equal;
    case '<':
      return TokenType::Less;
    case '&':
      return TokenType::And;
    case '|':
      return TokenType::Or;
    case '!':
      return TokenType::Not;
    case ':':
      return TokenType::Colon;
    case ';':
      return TokenType::Semicolon;
    case '(':
      return TokenType::LeftParen;
    case ')':
      return TokenType::RightParen;
    case '{':
      return TokenType::LeftBrace;
    case '}':
      return TokenType::RightBrace;
    case '[':
      return TokenType::LeftBracket;
    case ']':
      return TokenType::RightBracket;
    case '.':
      return TokenType::Period;
    case '>':
      return TokenType::Execute;
    case '_':
      return TokenType::Identifier;
    default:
      if (std::isdigit(src_[index_])) {
        return TokenType::Number;
      } else if (std::isalpha(src_[index_])) {
        return TokenType::Identifier;
      } else {
        return TokenType::Unknown;
      }
    }
  }

  bool isEmptyChar(char c) {
    return c == ' ' || c == '\t' || c == '\r';
  }
};
} // namespace

LexResult lexSource(const std::string_view src) {
  // 역할: string_view로 된 소스코드를 읽어서 토큰으로 분해하는 역할
  return voyc::Lexer(src).run();
}
} // namespace voyc
