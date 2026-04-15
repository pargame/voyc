# `fstream` 노트

이 노트는 C++ 표준 라이브러리의 파일 입출력 타입과, 파일을 열어 내용을 `std::string`으로 읽어들이는 방법을 정리합니다.

## `std::ifstream`

`std::ifstream`은 입력용 파일 스트림입니다. 파일에서 데이터를 읽을 때 사용합니다.

```cpp
std::ifstream file(argv[1]);
```

- `std::ifstream`: 읽기 전용 파일 스트림 타입
- `file`: 스트림 객체 이름
- `argv[1]`: 열 파일 경로

### `std::ios::binary`

바이너리 데이터를 그대로 읽거나 텍스트 모드 변환을 피하려면 다음과 같이 엽니다.

```cpp
std::ifstream file(argv[1], std::ios::binary);
```

## 열기 상태 확인

파일 스트림이 정상적으로 열렸는지 확인해야 합니다.

```cpp
if (!file.is_open()) {
    // 파일 열기 실패 처리
}
```

또는 다음과 같이도 쓸 수 있습니다.

```cpp
if (!file) {
    // 스트림 상태가 유효하지 않음
}
```

- `true`: 파일이 열렸고 스트림 상태가 정상
- `false`: 파일 열기 실패 또는 스트림 오류

## 파일 전체를 `std::string`에 읽기

### `std::ostringstream`와 `rdbuf()` 사용

```cpp
#include <fstream>
#include <sstream>
#include <string>

std::ifstream file(path, std::ios::binary);
if (!file) {
    // 실패 처리
}

std::ostringstream buffer;
buffer << file.rdbuf();
std::string content = buffer.str();
```

### `std::istreambuf_iterator<char>` 사용

```cpp
#include <fstream>
#include <iterator>
#include <string>

std::ifstream file(path, std::ios::binary);
if (!file) {
    // 실패 처리
}

std::string content{
    std::istreambuf_iterator<char>(file),
    std::istreambuf_iterator<char>()
};
```

## 텍스트 파일을 줄 단위로 읽기

```cpp
#include <fstream>
#include <string>

std::ifstream file(path);
if (!file) {
    // 실패 처리
}

std::string line;
while (std::getline(file, line)) {
    // line 처리
}
```

해당 방식은 이미 줄 단위로 처리할 때 적합합니다. 전체 내용을 하나 문자열로 합치려면 각 줄 뒤에 `\n`을 추가해야 합니다.

## `std::ofstream`과 `std::fstream`

- `std::ofstream`: 쓰기 전용 파일 스트림
- `std::fstream`: 읽기/쓰기 모두 가능한 파일 스트림

```cpp
std::ofstream out("output.txt");
std::fstream inout("file.dat", std::ios::in | std::ios::out | std::ios::binary);
```

## 프로젝트 내 흐름

현재 소스 코드에서는 일반적으로 다음 흐름을 따릅니다.

1. 실행 인자로 파일 경로를 받음
2. `std::ifstream file(path, std::ios::binary);`로 파일을 염
3. `if (!file)`로 열기 성공 여부를 확인
4. `std::ostringstream` 또는 `std::istreambuf_iterator<char>`를 사용해 `std::string`에 전체 내용을 저장

## 요약

- 파일을 읽을 때는 `std::ifstream`을 사용합니다.
- `std::ios::binary`는 텍스트 변환 없이 바이트를 그대로 읽을 때 사용합니다.
- 열기 성공 여부는 `file.is_open()` 또는 `if (!file)`로 확인합니다.
- 전체 파일을 문자열로 읽을 때는 `std::ostringstream` + `rdbuf()` 또는 `std::istreambuf_iterator<char>`를 사용합니다.
- 텍스트 라인 단위 처리에는 `std::getline`을 사용합니다.
