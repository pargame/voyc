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
  size_t line_;
  size_t column_;
  size_t index_;

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
      next = peekToken();
    }

    return result;
  }

private:
  Token peekToken() {
    if (index_ < src_.size()) {
      skipTrivia();
    }

    TokenType type = tokenTypeFromChar(index_);
    size_t start_column = column_;

    switch (type) {
    case TokenType::EndOfFile: {
      return Token(type, std::string_view{}, line_, column_);
    }

    case TokenType::Identifier: { // Kw..
      size_t i = index_ + 1;
      ++column_;

      while (i < src_.size() && (std::isalnum(src_[i]) || src_[i] == '_')) {
        ++i;
        ++column_;
      }

      TokenType t = type;
      std::string_view lex = std::string_view{src_.data() + index_, i - index_};
      index_ = i;

      {
        if (lex == "ZONE") {
          t = TokenType::KwZone;
        } else if (lex == "LOC") {
          t = TokenType::KwLocal;
        } else if (lex == "OP") {
          t = TokenType::KwOperator;
        } else if (lex == "ret") {
          t = TokenType::KwRet;
        } else if (lex == "ref") {
          t = TokenType::KwRef;
        } else if (lex == "int") {
          t = TokenType::KwInt;
        } else if (lex == "float") {
          t = TokenType::KwFloat;
        } else if (lex == "string") {
          t = TokenType::KwString;
        }
      }

      return Token(t, lex, line_, start_column);
    }

    case TokenType::LitInt: { // LitString
      size_t i = index_ + 1;
      ++column_;
      TokenType t = type;

      while (i < src_.size() && (std::isdigit(src_[i]) || src_[i] == '.')) {
        if (src_[i] == '.') {
          if (t == TokenType::LitInt) {
            if (i + 1 < src_.size() && std::isdigit(src_[i + 1])) {
              t = TokenType::LitFloat;
            } else { // 123.(stop)
              break;
            }
          } else { // 123.456.(stop)
            break;
          }
        }
        ++i;
        ++column_;
      }

      std::string_view lex{src_.data() + index_, i - index_};
      index_ = i;
      return Token(t, lex, line_, start_column);
    }

    case TokenType::LitString: { // Unknown
      size_t i = index_ + 1;
      ++column_;

      while (i < src_.size() && src_[i] != '\n' && src_[i] != '\'') {
        ++column_;
        ++i;
      }

      TokenType t = TokenType::LitString;
      std::string_view lex{src_.data() + index_ + 1, i - index_};

      if (i == src_.size() || src_[i] == '\n') {
        t = TokenType::Unknown;
        lex = std::string_view{src_.data() + index_, i - index_};
      }

      if (t == TokenType::LitString) {
        ++column_;
        ++i;
      }
      index_ = i;
      return Token(t, lex, line_, start_column);
    }

    case TokenType::Less: {
      size_t start_i = index_;

      if (index_ + 1 < src_.size() && src_[index_ + 1] == '=') {
        column_ += 2;
        index_ += 2;
        return Token(TokenType::LessEqual, std::string_view{src_.data() + start_i, 2}, line_, start_column);
      } else {
        ++column_;
        ++index_;
        return Token(type, std::string_view{src_.data() + start_i, 1}, line_, start_column);
      }
    }

    default: {
      ++column_;
      return Token(type, std::string_view{src_.data() + index_++, 1}, line_, start_column);
    }
    }
  }

  void skipTrivia() {
    while (index_ < src_.size() && isWhiteSpace(index_)) {
      if (src_[index_] == '\n') {
        ++line_;
        column_ = 0;
      }
      ++column_;
      ++index_;
    }
  }

  bool isWhiteSpace(size_t i) {
    return src_[i] == ' ' || src_[i] == '\t' || src_[i] == '\r' || src_[i] == '\n';
  }

  void appendError(std::vector<std::string> &errors, Token &next) {
  }
  void appendToken(std::vector<Token> &tokens, Token &next) {
  }

  TokenType tokenTypeFromChar(size_t i) {
    if (src_.size() == i) {
      return TokenType::EndOfFile;
    }

    switch (src_[i]) {
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
    case '\'':
      return TokenType::LitString;
    default:
      if (std::isdigit(src_[i])) {
        return TokenType::LitInt;
      } else if (std::isalpha(src_[i]) || src_[i] == '_') {
        return TokenType::Identifier;
      } else {
        return TokenType::Unknown;
      }
    }
  }
};
} // namespace

LexResult lexSource(const std::string_view src) {
  // 역할: string_view로 된 소스코드를 읽어서 토큰으로 분해하는 역할
  return voyc::Lexer(src).run();
}
} // namespace voyc
