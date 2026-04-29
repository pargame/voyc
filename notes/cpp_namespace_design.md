# Namespace 설계

voyc에서 `namespace voyc`와 익명 namespace를 사용하는 방식을 정리합니다.

## `namespace voyc`

프로젝트의 공용 API를 `voyc::` 접두사로 노출합니다:

```cpp
// include/voyc/lexer.hpp
namespace voyc {
    LexResult lexSource(std::string_view src);
}

// main.cpp
const voyc::LexResult result = voyc::lexSource(src);
```

이름 충돌을 방지하고, 외부 코드에서 출처가 명확한 호출을 가능하게 합니다.

## 익명 namespace

`.cpp` 파일 내부에서만 쓰이는 구현 세부사항을 숨깁니다:

```cpp
// lexer.cpp
namespace voyc {
namespace {

class Lexer { /* ... */ };

} // namespace

LexResult lexSource(std::string_view src) {
    return Lexer(src).run();  // 외부에는 Lexer가 보이지 않음
}

} // namespace voyc
```

익명 namespace에 넣은 `Lexer` 클래스는 `lexer.cpp` 외부에서 접근할 수 없습니다. 링크 단위 내부 전용(symbol visibility 제어)입니다.

## `using namespace` 사용처

`.cpp` 파일 내부에서만 제한적으로 사용합니다:

```cpp
// ✅ 구현 파일 내부
using namespace std::placeholders;

// ❌ 헤더 파일이나 전역 영역
using namespace voyc;  // 이름 충돌 위험
```

## voyc 실제 구조

| 위치 | namespace | 공개 여부 |
|------|-----------|-----------|
| `include/voyc/*.hpp` | `namespace voyc` | ✅ 외부 공개 |
| `src/*.cpp` (구현) | `namespace voyc { namespace { ... } }` | ❌ 내부 전용 |

## 요약

- `namespace voyc`는 공용 API를 외부에 노출합니다.
- 익명 namespace는 구현 세부사항을 `.cpp` 내부로 숨깁니다.
- `using namespace`는 헤더나 전역에서 사용하지 않습니다.
