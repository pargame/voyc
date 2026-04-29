# Namespace 설계

C++에서 네임스페이스를 설계하고 사용하는 방법을 정리합니다.

## 기본 역할

```cpp
namespace mylib {
    struct Result { /* ... */ };
    Result process(const std::string& input);
}

// 사용
mylib::Result r = mylib::process("hello");
```

네임스페이스는 이름 충돌을 방지하고, 코드의 논리적 묶음을 명확히 합니다.

## 익명 namespace

`.cpp` 파일 내부에서만 쓰이는 구현 세부사항을 숨깁니다:

```cpp
// parser.cpp
namespace {

class ParserImpl { /* ... */ };

} // namespace

Result parse(const std::string& input) {
    return ParserImpl(input).run();  // 외부에는 ParserImpl이 보이지 않음
}
```

익명 namespace에 넣은 심볼은 해당 소스 파일 외부에서 접근할 수 없습니다.

## `using namespace` 사용처

```cpp
// ✅ 구현 파일 내부, 제한된 범위
void helper() {
    using namespace std::literals;
    auto s = "hello"s;
}

// ❌ 헤더 파일이나 전역 영역
using namespace mylib;  // 이름 충돌 위험
```

## 중첩 namespace

```cpp
namespace mylib::lexer {
    struct Token { /* ... */ };
}

// C++17 이상
```

## 구조

| 위치 | namespace | 공개 여부 |
|------|-----------|-----------|
| 헤더 파일 | `namespace mylib` | ✅ 외부 공개 |
| 소스 파일 | `namespace { /* 구현 */ }` | ❌ 내부 전용 |

## 요약

- 네임스페이스는 이름 충돌을 방지하고 API 경계를 명확히 합니다.
- 익명 namespace는 구현 세부사항을 `.cpp` 낸부로 숨깁니다.
- `using namespace`는 헤더나 전역에서 사용하지 않습니다.
