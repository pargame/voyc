#include "voyc/lexer.hpp"
#include "voyc/token.hpp"

#include <cctype>
#include <cstddef>
#include <string>
#include <string_view>

namespace voyc {

namespace {

enum class TokenKind {
  Unknown,
  Certain,
  Number,
  Name,
};

class Lexer {
  // 역할: string_view로 된 소스코드를 읽어서 토큰으로 분해하는 역할
private:
  std::string_view src_;
  size_t line_;
  size_t column_;
  size_t index_;

public:
  explicit Lexer(std::string_view src) : src_(src), line_(1), column_(1), index_(0) {
    if (src_.size() >= 3 &&
        static_cast<unsigned char>(src_[0]) == 0xEF &&
        static_cast<unsigned char>(src_[1]) == 0xBB &&
        static_cast<unsigned char>(src_[2]) == 0xBF) {
      index_ = 3;
    }
  }

  LexResult run() {
    LexResult result;

    while (index_ < src_.size()) {
      std::string msg;
      Token next = nextToken(msg);
      if (next.type == TokenType::Unknown) {
        result.errors.push_back(msg);
      } else {
        result.tokens.push_back(next);
      }
    }

    return result;
  }

private:
  Token nextToken(std::string &msg) {
    skipTrivia();

    TokenType type = TokenType::Unknown;
    size_t len = 0;
    size_t start_index = index_;
    size_t start_column = column_;
    TokenKind kind = singleKind();

    switch (kind) {
    case TokenKind::Certain: {
      switch (c()) {
      case '#':
        type = TokenType::Comment;
        while (index_ < src_.size() && c() != '\n') {
          consume(len);
        }
        break;
      case '+':
        type = TokenType::Plus;
        consume(len);
        break;
      case '-':
        type = TokenType::Minus;
        consume(len);
        break;
      case '*':
        type = TokenType::Asterisk;
        consume(len);
        break;
      case '/':
        type = TokenType::Slash;
        consume(len);
        break;
      case '=':
        type = TokenType::Equal;
        consume(len);
        break;
      case '<':
        type = TokenType::Less;
        consume(len);
        if (index_ < src_.size() && c() == '=') {
          type = TokenType::LessEqual;
          consume(len);
        }
        break;
      case '&':
        type = TokenType::And;
        consume(len);
        break;
      case '|':
        type = TokenType::Or;
        consume(len);
        break;
      case '!':
        type = TokenType::Not;
        consume(len);
        break;
      case '(':
        type = TokenType::LeftParen;
        consume(len);
        break;
      case ')':
        type = TokenType::RightParen;
        consume(len);
        break;
      case '{':
        type = TokenType::LeftBrace;
        consume(len);
        break;
      case '}':
        type = TokenType::RightBrace;
        consume(len);
        break;
      case '[':
        type = TokenType::LeftBracket;
        consume(len);
        break;
      case ']':
        type = TokenType::RightBracket;
        consume(len);
        break;
      case ':':
        type = TokenType::Colon;
        consume(len);
        break;
      case ';':
        type = TokenType::Semicolon;
        consume(len);
        break;
      case '~':
        type = TokenType::Tilde;
        consume(len);
        break;
      case '>':
        type = TokenType::Execute;
        consume(len);
        break;
      case '\'':
        type = TokenType::LitString;
        consume(len);
        while (index_ < src_.size() && c() != '\n' && c() != '\'') {
          consume(len);
        }
        if (index_ == src_.size() || c() != '\'') {
          msg = "[line: " + std::to_string(line_) + " col: " + std::to_string(start_column) + "] " + "String Literal must be ended with single quote(')";
          type = TokenType::Unknown;
        } else if (c() == '\'') {
          consume(len);
        }
        break;
      }
    } break;
    case TokenKind::Name: {
      type = TokenType::Identifier;
      while (index_ < src_.size() && (std::isalnum(c()) || c() == '_')) {
        consume(len);
      }
      std::string_view word(src_.data() + start_index, len);
      if (word == "ret") {
        type = TokenType::KwRet;
      } else if (word == "ref") {
        type = TokenType::KwRef;
      } else if (word == "int") {
        type = TokenType::KwInt;
      } else if (word == "float") {
        type = TokenType::KwFloat;
      } else if (word == "string") {
        type = TokenType::KwString;
      } else if (word == "ZONE") {
        type = TokenType::KwZone;
      } else if (word == "LOC") {
        type = TokenType::KwLocal;
      } else if (word == "OP") {
        type = TokenType::KwOperator;
      }
      break;
    }
    case TokenKind::Number: {
      type = TokenType::LitInt;
      size_t cnt_prd = 0;
      bool wrong_id = false;
      while (index_ < src_.size() && (std::isalnum(c()) || c() == '_' || c() == '.')) {
        if (std::isalpha(c()) || c() == '_') {
          wrong_id = true;
        } else if (c() == '.') {
          ++cnt_prd;
        }
        consume(len);
      }
      if (1 < cnt_prd || wrong_id) {
        type = TokenType::Unknown;
        msg = "[line: " + std::to_string(line_) + " col: " + std::to_string(start_column) + "] ";
        if (wrong_id) {
          msg += "Wrong Identifier Name. Can not start with numbers.";
        } else {
          msg += "Wrong Float Literal format. Check period symbols.";
        }
      } else if (1 == cnt_prd) {
        type = TokenType::LitFloat;
      }
      break;
    }
    case TokenKind::Unknown: {
      type = TokenType::Unknown;
      msg = "[line: " + std::to_string(line_) + " col: " + std::to_string(start_column) + "] " + "Undefined Symbol used.";
      consume(len);
      break;
    }
    }

    std::string_view lex(src_.data() + start_index, len);

    skipTrivia();

    return Token(type, lex, line_, start_column);
  }

  void consume(size_t &len) {
    ++index_, ++column_, ++len;
  }

  char c() {
    return src_[index_];
  }

  TokenKind singleKind() {
    char c = src_[index_];
    switch (c) {
    case '#':
    case '+':
    case '-':
    case '*':
    case '/':
    case '=':
    case '<':
    case '&':
    case '|':
    case '!':
    case '(':
    case ')':
    case '{':
    case '}':
    case '[':
    case ']':
    case ':':
    case ';':
    case '~':
    case '>':
    case '\'':
      return TokenKind::Certain;
    default:
      if (std::isdigit(c)) {
        return TokenKind::Number;
      } else if (std::isalpha(c) || c == '_') {
        return TokenKind::Name;
      } else {
        return TokenKind::Unknown;
      }
    }
  }

  void skipTrivia() {
    while (index_ < src_.size() && isWhite(c())) {
      if (c() == '\n') {
        ++line_;
        column_ = 0;
      }
      size_t dummy = 0;
      consume(dummy);
    }
  }

  bool isWhite(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
  }
}; // namespace voyc

} // namespace

LexResult lexSource(const std::string_view src) {
  // 역할: string_view로 된 소스코드를 읽어서 토큰으로 분해하는 역할
  return voyc::Lexer(src).run();
}

} // namespace voyc
