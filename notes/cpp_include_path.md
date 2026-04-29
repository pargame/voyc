# Include Path 설계

C++ 프로젝트에서 헤더 경로를 설계하고 `#include` 구문을 사용하는 방법을 정리합니다.

## 중첩 폴�더 구조

```
include/
└── mylib/
    ├── header.hpp
    └── utils.hpp
```

CMake에서 `include`를 경로에 추가하면:

```cmake
target_include_directories(mylib PRIVATE ${CMAKE_SOURCE_DIR}/include)
```

소스에서 패키지 이름을 접두사로 붙여 포함합니다:

```cpp
#include "mylib/header.hpp"
```

## 패키지 식별자 역할

`mylib/` 접두사가 없으면 `header.hpp` 같은 일반적인 이름이 외부 라이브러리와 충돌할 수 있습니다. 패키지 이름을 경로에 포함하면 해당 헤더가 어떤 프로젝트의 것인지 명확해집니다.

또한 라이브러리로 배포할 때 `#include <mylib/header.hpp>`처럼 사용하기 자연스럽습니다.

## 큰따옴표 vs 꺾쇠

| 구분 | `""` | `<>` |
|------|------|------|
| 검색 순서 | 현재 파일 경로 → include path | include path만 |
| 용도 | 프로젝트 낸부 헤더 | 시스템/외부 라이브러리 헤더 |
| 예시 | `#include "mylib/header.hpp"` | `#include <iostream>` |

## 이름 충돌 방지

- 헤더를 `include/header.hpp`에 직접 두지 않고 `include/mylib/header.hpp`에 둡니다.
- 프로젝트가 커지면 `include/mylib/lexer.hpp`, `include/mylib/parser.hpp` 같은 구조가 자연스럽습니다.

## 요약

- `include/<package>/` 구조는 헤더 이름 충돌을 방지하고 패키지 식별자 역할을 합니다.
- 프로젝트 낸부 헤더는 `""`, 시스템/외부 헤더는 `<>`를 사용합니다.
