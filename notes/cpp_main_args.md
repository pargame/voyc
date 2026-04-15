# C++ main 함수 인자 처리

C++에서 `main` 함수는 프로그램 실행 시 전달되는 명령행 인자를 받는 진입점입니다. 이 문서는 `argc`, `argv`의 의미, C++에서 안전하게 인자를 처리하는 방법, 그리고 실제 코드에서 자주 쓰는 패턴을 중심으로 설명합니다.

## 1. `main` 함수 시그니처

C++ 표준에서 다음 두 형태는 유효한 `main` 시그니처입니다.

```cpp
int main()
int main(int argc, char *argv[])
```

`argv`는 다음과 같이도 선언할 수 있습니다.

```cpp
int main(int argc, char **argv)
```

- `char *argv[]`와 `char **argv`는 의미상 동일합니다.
- `argv[0]`은 일반적으로 실행된 프로그램의 이름 또는 경로입니다.
- `argv[argc]`는 `nullptr`로 끝납니다.

### 1.1. 확장 시그니처

표준에는 포함되지 않지만, 실제로 많이 쓰이는 확장 시그니처도 있습니다.

```cpp
int main(int argc, char *argv[], char *envp[])
```

- 환경 변수 배열을 함께 받는 비표준 확장입니다.
- 플랫폼 종속적이므로 포터블 코드에서는 권장되지 않습니다.

Windows 전용 유니코드 시그니처:

```cpp
int wmain(int argc, wchar_t *argv[])
```

- Windows에서 유니코드 명령행 인자를 받을 때 사용합니다.
- 표준 C++에는 포함되지 않습니다.

## 2. `argc`와 `argv` 의미

### 2.1. `argc`

- 명령행 인자의 개수를 나타냅니다.
- `argv[0]`을 포함한 배열 길이이므로, 사용자가 입력한 인자는 보통 `argc - 1`개입니다.
- 최소 `0` 이상이며, 일부 플랫폼에서는 `argc == 0`이 될 수 있습니다.

### 2.2. `argv`

- `argv`는 `char*` 포인터의 배열입니다.
- 각 요소는 null 종료된 C 스타일 문자열입니다.
- `argv[argc]`는 항상 `nullptr`입니다.
- `argv[0]`은 프로그램 이름이나 실행 경로를 담습니다. 정확한 내용은 플랫폼과 런처에 따라 달라질 수 있습니다.

## 3. 안전하게 접근하기

### 3.1. 범위 검사

```cpp
if (argc > 1) {
    const char *input_path = argv[1];
}
```

- `argv[1]`에 접근하기 전에 항상 `argc`를 확인하세요.
- `argc`를 확인하지 않고 접근하면 정의되지 않은 동작이 발생할 수 있습니다.

### 3.2. C++ 문자열 뷰

```cpp
#include <string_view>

int main(int argc, char *argv[]) {
    if (argc <= 1) return 1;
    std::string_view input{argv[1]};
    // input 처리
}
```

- `std::string_view`는 `argv` 요소를 복사하지 않고 문자열로 취급할 수 있게 해줍니다.
- `argv` 문자열은 프로그램 시작 시 OS가 관리하므로, `std::string_view`로 읽기 전용 처리할 때 유용합니다.

### 3.3. `std::string` 변환

```cpp
#include <string>

std::string input{argv[1]};
```

- 복사가 필요하거나, 문자열 조작을 하려면 `std::string`으로 변환합니다.
- 변환 시 `argc` 범위를 먼저 검사해야 합니다.

## 4. 명령행 인자 순회

```cpp
for (int i = 1; i < argc; ++i) {
    std::string_view arg{argv[i]};
    // arg 처리
}
```

- `argv[0]`은 프로그램 이름이므로 실제 인자는 `1`부터 시작합니다.
- `argv[argc]`는 `nullptr`이므로, `argc` 기반 순회가 안전합니다.

## 5. 옵션 파싱 패턴

간단한 옵션 처리는 다음처럼 작성할 수 있습니다.

```cpp
#include <iostream>
#include <string_view>

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; ++i) {
        std::string_view arg{argv[i]};
        if (arg == "--help") {
            std::cout << "Usage: main [options] <file>\n";
        } else if (arg == "--version") {
            std::cout << "version 1.0\n";
        } else {
            std::cout << "input file: " << arg << '\n';
        }
    }
}
```

- 옵션과 값이 분리된 경우에는 인덱스를 하나 더 증가시키는 방식으로 처리합니다.
- 인자 파싱 로직이 복잡해지면 `std::getopt`, `boost::program_options`, `argparse` 같은 라이브러리를 사용하는 것이 안전합니다.

## 6. 자주 발생하는 실수

- `argc == 0`을 가정하지 않기
  - 일부 플랫폼에서 `argc`가 0일 수 있습니다.
- `argv[argc]`를 사용해 종료를 검사하려고 하기
  - `argv[argc]`는 `nullptr`이지만, `argc`을 기준으로 순회하는 것이 명확합니다.
- `argv` 요소를 `char *`로 그대로 수정하기
  - 명령행 인자는 읽기 전용으로 취급해야 합니다.

## 7. 실제 실행 예시

```bash
./main file.voy --verbose
```

이때:

- `argc == 3`
- `argv[0] == "./main"`
- `argv[1] == "file.voy"`
- `argv[2] == "--verbose"`

## 8. 요약

- C++ `main`에서 `argc`는 인자 개수, `argv`는 인자 문자열 배열입니다.
- `argv[0]`은 프로그램 이름이나 경로 정보를 담고, 실제 인자는 `argv[1]`부터 시작합니다.
- `argc`를 먼저 검사하고 `argv`에 접근하세요.
- `std::string_view`로 읽기 전용 처리하거나, `std::string`으로 변환해서 조작하세요.
- 복잡한 옵션 파싱은 검증된 라이브러리를 사용하는 것이 안정적입니다.
