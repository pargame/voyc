# Include Path 설계

voyc의 `include/voyc/` 구조와 `#include`의 큰따옴표/꺾쇠 규칙을 정리합니다.

## `include/voyc` 구조

```
include/
└── voyc/
    ├── lexer.hpp
    └── token.hpp
```

CMake에서 `include`를 경로에 추가하면, 소스에서 `voyc/`를 접두사로 붙여 포함합니다:

```cpp
#include "voyc/lexer.hpp"
```

## 패키지 식별자 역할

`voyc/` 접두사가 없으면 `token.hpp` 같은 일반적인 이름이 외부 라이브러리와 충돌할 수 있습니다. 패키지 이름을 경로에 포함하면 프로젝트의 공용 API임이 명확해집니다.

## 큰따옴표 vs 꺾쇠

| 구분 | 큰따옴표 `""` | 꺾쇠 `<>` |
|------|---------------|-----------|
| 검색 순서 | 현재 파일 경로 → include path | include path만 |
| 용도 | 프로젝트 내부 헤더 | 시스템/외부 라이브러리 헤더 |
| voyc 예시 | `#include "voyc/token.hpp"` | `#include <iostream>` |

## voyc 실제 예시

```cpp
// main.cpp
#include "voyc/lexer.hpp"    // 프로젝트 내부
#include <iostream>           // 시스템
#include <fstream>            // 시스템

// lexer.cpp
#include "voyc/lexer.hpp"
#include "voyc/token.hpp"
#include <cctype>             // 시스템
#include <string_view>        // 시스템
```

## 요약

- `include/voyc/`는 헤더 이름 충돌을 방지하고 패키지 식별자 역할을 합니다.
- 프로젝트 내부 헤더는 `""`, 시스템/외부 헤더는 `<>`를 사용합니다.
