# string_view

voyc lexer에서 `std::string_view`를 사용하는 방식을 정리합니다.

## 개념

`std::string_view`는 문자열 데이터를 소유하지 않는 읽기 전용 뷰입니다. 포인터와 길이만 가진 가벼운 타입입니다.

```cpp
#include <string_view>

std::string_view sv{"hello"};        // 문자열 리터럴
std::string_view sv2{str};           // std::string
std::string_view sv3{ptr, len};      // (포인터, 길이)
```

## voyc에서의 핵심 사용

lexer는 소스 코드 전체를 `string_view`로 받아 인덱스 기반으로 파싱합니다:

```cpp
// lexer.cpp
class Lexer {
    std::string_view src_;   // 소스 전체를 참조
    size_t index_;           // 현재 위치

    char c() {
        return src_[index_];  // 인덱스로 문자 접근
    }

    Token nextToken() {
        size_t start = index_;
        // ... 문자 소비
        return Token(type, std::string_view(src_.data() + start, len), ...);
    }
};
```

토큰의 lexeme(원본 문자열)도 `string_view`로 저장하여 복사 비용을 피합니다:

```cpp
// keyword 매칭
std::string_view word(src_.data() + start_index, len);
if (word == "ret") type = TokenType::KwRet;
```

## 주요 메서드

| 메서드 | 역할 |
|--------|------|
| `size()` | 길이 |
| `data()` | 시작 포인터 |
| `substr(pos, len)` | 부분 뷰 (복사 아님) |
| `remove_prefix(n)` | 앞 n글자 제거 |
| `starts_with(s)` | 접두사 확인 (C++20) |

## 수명 주의

`string_view`는 원본 데이터를 참조만 합니다. 원본이 소멸하면 뷰도 무효화됩니다:

```cpp
// ✅ 원본이 유효한 동안 사용
std::string src = readFile("input.voy");
std::string_view view{src};
process(view);  // src가 살아 있는 동안 안전

// ❌ 임시 객체를 참조하면 위험
std::string_view bad{readFile("input.voy")};  // 임시 string이 소멸됨
```

## `std::string`과의 차이

| | `std::string` | `std::string_view` |
|--|---------------|--------------------|
| 소유권 | ✅ 소유 | ❌ 참조만 |
| 수정 | ✅ 가능 | ❌ 읽기 전용 |
| 복사 비용 | 데이터 전체 복사 | 포인터+길이만 |
| 수명 | 자체 관리 | 원본 의존 |

## 함수 인자로 사용

읽기 전용 인자에는 `string_view`를 사용합니다:

```cpp
// ✅ 복사 없이 읽기만 함
void printToken(std::string_view lexeme);

// ✅ 어떤 문자열이든 전달 가능
printToken("hello");           // 리터럴
printToken(str);               // std::string
printToken(view);              // string_view
```

## 요약

- voyc lexer는 소스 코드와 토큰 lexeme을 `string_view`로 관리합니다.
- 복사 비용이 없으므로 파싱 성능에 유리합니다.
- 원본 데이터의 수명이 뷰보다 길어야 합니다.
- 읽기 전용 함수 인자나 서브스트링 생성에 적합합니다.
