# 헤더/소스 분할과 include 설계

C++ 프로젝트에서 헤더 파일과 소스 파일을 나누고, include 경로를 설계하는 방법과 그 이유를 정리합니다.

## 파일 분할의 목적

C++는 헤더(.hpp)와 소스(.cpp)를 분리하여 컴파일합니다. 이는 성능과 유지보수를 위한 언어의 기본 구조입니다.

| 파일 | 역할 | 컴파일 |
|------|------|--------|
| 헤더(.hpp) | 선언(declaration): 타입, 함수, 클래스의 **인터페이스** | 직접 컴파일되지 않음 |
| 소스(.cpp) | 정의(definition): 실제 **구현** | 컴파일되어 오브젝트 파일(.o) 생성 |

헤더 파일은 여러 `.cpp`에서 `#include`되어 인터페이스를 공유합니다. 소스 파일은 각각 독립적으로 컴파일되고, 링커가 합쳐서 실행 파일을 만듭니다.

## 왜 패키지 접두사를 쓰는가

```cpp
#include "mylib/lexer.hpp"
```

### 이름 충돌 방지

```cpp
// ❌ 헤더를 include/에 직접 두면
#include "lexer.hpp"

// 다른 라이브러리도 "lexer.hpp"를 가질 수 있음
// 어떤 프로젝트의 헤더인지 모호함
```

```cpp
// ✅ 패키지 접두사로 구분
#include "mylib/lexer.hpp"
#include "otherlib/lexer.hpp"
```

### 설치 경로와 일치

라이브러리로 배포할 때 `/usr/local/include/mylib/`에 설치됩니다. `#include <mylib/lexer.hpp>`가 자연스럽게 됩니다.

### 네임스페이스와 대응

```cpp
#include "mylib/lexer.hpp"

// 헤더 내부
namespace mylib {
    struct Token { /* ... */ };
}
```

디렉터리 경로와 코드의 네임스페이스가 일치하면 코드의 구조를 예측하기 쉬워집니다.

## 큰따옴표 vs 꺾쇠

| 구분 | `""` | `<>` |
|------|------|------|
| 검색 순서 | 현재 파일 경로 → include path | include path만 |
| 용도 | 프로젝트 내부 헤더 | 시스템/외부 라이브러리 헤더 |
| 예시 | `#include "mylib/lexer.hpp"` | `#include <iostream>` |

## include 경로 설정

CMake에서 설정하는 방법:

```cmake
target_include_directories(mylib PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
```

이 설정이 있으면 컴파일러는 `include/`를 헤더 검색 경로에 추가합니다. 소스에서는 접두사부터 경로를 씁니다:

```cpp
// 실제 파일 위치: include/mylib/lexer.hpp
// 소스에서:       #include "mylib/lexer.hpp"
```

컴파일러 명령어로는 `-Iinclude` 플래그가 추가됩니다:

```bash
clang++ -Iinclude -c src/lexer.cpp
```

## 헤더 파일의 역할

헤더 파일은 "이 타입/함수가 존재하며, 이렇게 사용할 수 있다"는 **계약**을 선언합니다.

```cpp
// mylib/lexer.hpp
#pragma once

#include <string_view>
#include <vector>

namespace mylib {

struct Token {
    std::string_view lexeme;
    int line;
    int column;
};

std::vector<Token> lex(std::string_view src);

} // namespace mylib
```

- `struct Token`의 멤버를 공개합니다.
- `lex` 함수의 시그니처(이름, 인자, 반환값)를 알립니다.
- 구현은 `.cpp`에 숨깁니다.

## 소스 파일의 역할

소스 파일은 헤더에서 선언한 것을 **정의**합니다:

```cpp
// lexer.cpp
#include "mylib/lexer.hpp"

namespace mylib {

std::vector<Token> lex(std::string_view src) {
    // 실제 구현
    return tokens;
}

} // namespace mylib
```

### 소스가 자신의 헤더를 포함하는 이유

컴파일러가 헤더의 선언과 소스의 정의가 **일치하는지** 검증합니다. 시그니처가 다륩면 컴파일 에러가 발생합니다.

## 전방 선언(forward declaration)

헤더에 다른 헤더를 포함하지 않고, 타입 이름만 미리 알리는 기법입니다:

```cpp
// ✅ 전방 선언으로 의존성 최소화
namespace otherlib {
    struct Config;  // 정의 없이 이름만 선언
}

namespace mylib {
    void init(otherlib::Config& cfg);  // 참조로 사용
}
```

포인터나 참조로만 사용할 때 전방 선언이 가능합니다. 값 타입으로 사용하려면 전체 정의가 필요합니다.

## 한 번만 포함되게 하기

헤더가 여러 파일에서 포함될 때 중복 정의를 방지해야 합니다.

```cpp
// ✅ #pragma once
#pragma once

namespace mylib {
    // 선언
}
```

```cpp
// ✅ 전통적인 include guard
#ifndef MYLIB_LEXER_HPP
#define MYLIB_LEXER_HPP

namespace mylib {
    // 선언
}

#endif
```

두 방식 모두 같은 효과를 냅니다. `#pragma once`는 대부분의 컴파일러에서 지원하며, 더 간결합니다.

## ODR(One Definition Rule)

C++에서 타입, 함수, 변수는 프로그램 전체에서 **정확히 한 번** 정의되어야 합니다.

| 위치 | 가능한 내용 | 이유 |
|------|-------------|------|
| 헤더 | `inline` 함수, 템플릿, `constexpr` | 여러 `.cpp`에 포함되어도 동일한 정의 |
| 헤더 | 일반 함수 정의 | ❌ 여러 `.cpp`에 포함되면 링크 에러 |
| 소스 | 일반 함수 정의 | ✅ 하나의 `.cpp`에서만 정의됨 |

헤더에는 **선언**만 두고, **정의**는 소스 파일에 두는 것이 ODR을 지키는 가장 안전한 방법입니다.

## 컴파일 흐름

```
main.cpp ──► 컴파일러 ──► main.o ──┐
                                   ├──► 링커 ──► 실행 파일
lexer.cpp ──► 컴파일러 ──► lexer.o ─┘
         ↑                    ↑
    #include "mylib/lexer.hpp"   lexer.hpp의 선언을 참조
```

- 각 `.cpp`는 독립적으로 컴파일됩니다.
- 헤더 변경 시 해당 헤더를 포함하는 모든 `.cpp`가 재컴파일됩니다.
- 소스만 변경하면 해당 파일만 재컴파일하면 됩니다.

## 요약

- 헤더는 **선언**을, 소스는 **정의**를 담습니다.
- 패키지 접두사(`mylib/`)는 이름 충돌을 방지하고 설치 경로와 일치하게 합니다.
- `-Iinclude` 또는 CMake `target_include_directories`로 검색 경로를 설정합니다.
- `#pragma once` 또는 include guard로 중복 포함을 방지합니다.
- 일반 함수의 정의는 헤더가 아닌 소스 파일에 두어 ODR을 지킵니다.
