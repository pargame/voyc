# cpp include path

이 노트는 `include/voyc` 폴더 구조와 헤더 경로 설계, `#include "voyc/token.hpp"`를 사용하는 이유를 정리합니다.

## 1. public 헤더 경로와 include path 분리
이 프로젝트에서는 실제 헤더 파일이 `include/voyc/token.hpp`에 있습니다.
CMake에서 다음과 같이 include 경로를 지정하면:
```cmake
target_include_directories(voyc PRIVATE ${CMAKE_SOURCE_DIR}/include)
```
소스 코드에서는 `#include "voyc/token.hpp"`처럼 `include` 디렉터리를 생략한 경로를 사용할 수 있습니다.

## 2. 패키지 식별자 역할
`voyc/` 디렉터리는 헤더 이름에 패키지 이름을 포함시키는 역할을 합니다.
이 방식은 외부 라이브러리와 헤더 이름이 겹치는 것을 방지하고, 해당 헤더가 프로젝트의 공용 API임을 명확히 합니다.

## 3. 설치 가능한 라이브러리 구조와 호환
만약 프로젝트를 라이브러리로 배포하면, 일반적으로 `include` 폴더 아래에 패키지 이름을 두고 설치합니다.
이 구조는 다른 프로젝트에서 `#include <voyc/token.hpp>`처럼 사용할 때 자연스럽습니다.

## 4. 왜 `include/voyc`처럼 중첩된 폴더 구조인가

### 4.1. 프로젝트 루트와 API 루트 분리
`include`는 헤더 파일의 루트 역할을 합니다.
그 아래 `voyc` 폴더를 둠으로써, 프로젝트의 공개 API를 별도의 네임스페이스처럼 구성할 수 있습니다.

### 4.2. 이름 충돌 방지
`token.hpp`를 바로 `include/token.hpp`에 두면, 다른 라이브러리 또는 시스템 헤더와 이름이 충돌할 가능성이 높아집니다.
`voyc/token.hpp`는 그런 충돌을 방지합니다.

### 4.3. 유지보수성과 확장성
프로젝트가 커지면 추가 기능별로 `include/voyc/lexer.hpp`, `include/voyc/parser.hpp` 같은 구조가 자연스럽습니다.
파일 이름이 단순하고, 프로젝트 이름이 포함된 디렉터리 안에 정리되므로 관리가 편합니다.
