# `fstream` 노트

이 노트는 C++ 파일 입출력 스트림의 주요 유형과, 파일을 열어 내용을 `std::string`으로 읽어들이는 방법을 정리합니다.

## 라이브러리 인클루드

파일 입출력과 문자열 스트림에서 많이 사용하는 헤더는 다음과 같습니다.

```cpp
#include <fstream>    // std::ifstream, std::ofstream, std::fstream
#include <sstream>    // std::ostringstream
#include <string>     // std::string
#include <iterator>   // std::istreambuf_iterator
```

## 1. 파일 스트림 유형

### 1.1. 스트림 클래스와 헤더 관계

C++ 스트림은 공통된 계층 구조를 가집니다. `std::istream`과 `std::ostream`은 기본 스트림 인터페이스이고, 파일 스트림과 문자열 스트림이 이 위에 확장됩니다.

- `<iostream>`: 기본 입출력 스트림(`std::cin`, `std::cout`, `std::cerr`)과 `std::istream`/`std::ostream`의 기본 타입 정의를 담습니다.
- `<fstream>`: 파일 기반 스트림 클래스 `std::ifstream`, `std::ofstream`, `std::fstream`을 선언합니다.
- `<sstream>`: 메모리 기반 문자열 스트림 클래스 `std::ostringstream`와 그 외 문자열 스트림을 선언합니다.

따라서 `std::ifstream`과 `std::ofstream`은 `iostream` 계열 스트림의 일부지만, 실제 구현은 `<fstream>` 헤더에서 제공합니다.

### 1.2. 입력 스트림과 버퍼 개념

`std::ifstream`은 입력 스트림입니다. 입력 스트림은 외부 소스(파일, 네트워크, 문자열 등)에서 데이터를 읽어오는 역할을 합니다.

입력 스트림은 내부적으로 데이터를 임시로 저장하는 버퍼를 사용합니다. 버퍼는 I/O 비용을 줄이고, 데이터를 일정 단위로 묶어 처리하는 데 유리합니다.

- 입력 스트림: 데이터를 읽어오는 스트림 객체
- 버퍼: 읽은 데이터를 일시적으로 보관하는 메모리 영역

### 1.3. `std::ifstream`

`std::ifstream`은 읽기 전용 파일 스트림입니다. 파일에서 데이터를 가져올 때 사용합니다.

```cpp
std::ifstream file(path);
```

### 1.4. `std::ofstream`

`std::ofstream`은 쓰기 전용 파일 스트림입니다. 파일에 데이터를 기록할 때 사용합니다.

```cpp
std::ofstream out("output.txt");
```

- 기본적으로 `std::ios::out | std::ios::trunc` 모드로 열립니다.
- 기존 파일이 있으면 덮어쓰기하고, 없으면 새로 생성합니다.
- 기존 내용 뒤에 추가하려면 `std::ios::app`을 함께 지정합니다.

### 1.5. `std::fstream`

`std::fstream`은 읽기와 쓰기 모두 가능한 파일 스트림입니다. 한 파일을 읽고 쓰는 용도로 사용합니다.

```cpp
std::fstream inout("file.dat", std::ios::in | std::ios::out);
```

- `std::fstream`을 쓰기/읽기 모두 사용하려면 `std::ios::in | std::ios::out`처럼 모드를 명시합니다.
- 바이너리 모드나 추가 쓰기 모드도 `1.6`의 공통 플래그를 함께 사용합니다.

### 1.6. `std::ostringstream`

`std::ostringstream`는 메모리 기반 출력 스트림입니다. 일반적인 출력 스트림(`std::cout`)과 달리 출력된 내용이 메모리 버퍼에 저장됩니다.

- 문자열 데이터를 누적하거나 여러 출력을 한 번에 결합할 때 유용합니다.
- `std::string`으로 변환할 수 있으므로, 스트림 연산 결과를 텍스트로 재사용하기 좋습니다.
- `std::ostringstream`는 `std::ostream` 인터페이스를 따르므로 `<<` 연산자를 그대로 사용할 수 있습니다.

```cpp
std::ostringstream buffer;
```

### 1.6. 공통 플래그

파일 스트림을 열 때 사용하는 모드 플래그입니다. 여러 모드를 동시에 지정할 때는 비트 단위 OR 연산자(`|`)로 결합합니다. 각 플래그는 서로 다른 비트 위치에 대응하는 값이므로, `std::ios::in | std::ios::out`처럼 OR 연산으로 읽기와 쓰기 옵션을 동시에 설정할 수 있습니다.

- `std::ios::in`: 읽기 전용으로 파일을 엽니다. `std::ifstream`에는 기본값으로 적용됩니다.
- `std::ios::out`: 쓰기 전용으로 파일을 엽니다. `std::ofstream`에는 기본값으로 적용됩니다.
- `std::ios::binary`: 텍스트 변환 없이 바이트를 그대로 처리합니다. 줄 끝 변환이나 인코딩 변환을 피하려면 사용합니다.
- `std::ios::app`: 열 때 파일 끝으로 이동하여 추가 쓰기를 합니다. 기존 내용을 보존합니다.
- `std::ios::trunc`: 파일을 열 때 기존 내용을 모두 제거하고 새로 작성합니다.
- `std::ios::ate`: 파일을 열자마자 파일 끝으로 이동합니다. 기본적으로는 쓰기 위치만 이동하며, 이후에 읽기나 쓰기를 수행할 수 있습니다.

```cpp
std::ifstream file(path, std::ios::in | std::ios::binary);
std::ofstream out("output.txt", std::ios::out | std::ios::app);
std::fstream inout("file.dat", std::ios::in | std::ios::out | std::ios::binary);
```

## 2. 파일 열기 상태 확인

파일이 정상적으로 열렸는지 확인하는 것은 필수입니다.

```cpp
if (!file.is_open()) {
    // 파일 열기 실패 처리
}
```

또는 다음과 같이 스트림 자체 상태를 검사할 수도 있습니다.

```cpp
if (!file) {
    // 스트림 상태 오류 처리
}
```

- `true`: 파일 스트림이 유효함
- `false`: 파일 열기 실패 또는 스트림 오류

## 3. 전체 파일을 `std::string`으로 읽기

### 3.1. `std::ostringstream` + `rdbuf()` 사용

이 패턴은 파일 스트림의 내부 버퍼를 다른 출력 스트림으로 복사해 한 번에 문자열을 생성합니다.

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

- `file.rdbuf()`는 입력 스트림의 내부 버퍼 객체를 반환합니다. `rdbuf`는 "read buffer"의 줄임말입니다.
- `std::ostringstream`는 버퍼에 데이터를 누적하여 문자열로 변환합니다.
- `buffer.str()`로 전체 내용을 `std::string`으로 얻습니다.

### 3.2. `std::istreambuf_iterator<char>` 사용

이 방법은 `std::string` 생성자가 반복자 범위로부터 내용을 복사합니다.

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

동작 흐름:
1. `std::istreambuf_iterator<char>(file)`가 스트림의 현재 위치(처음)에 연결됩니다.
2. `std::string` 생성자는 이 반복자를 하나씩 증가시키며 문자를 읽습니다.
3. 각 문자를 복사해서 문자열에 추가합니다.
4. 반복자의 상태가 EOF에 도달하면, 활성 상태의 반복자(active iterator)는 기본 생성된 `end` 반복자와 동일한 상태가 됩니다.
   - 여기서 `active`는 아직 스트림에 연결되어 있으며 다음 문자를 읽을 수 있는 반복자를 의미합니다.
   - EOF가 되면 더 이상 읽을 데이터가 없으므로, 이 반복자는 `end` sentinel과 같은 비활성 상태가 됩니다.
5. 반복자 범위가 끝나면 읽기를 멈춥니다.


### 3.3. `3.1`과 `3.2`의 차이

`3.1` 방식은 파일 스트림의 내부 버퍼를 `std::ostringstream`로 복사하는 방식입니다.
- `file.rdbuf()`를 통해 스트림 내부의 버퍼 객체를 가져옵니다.
- `std::ostringstream`가 이 버퍼를 한 번에 읽어서 문자열로 만듭니다.
- 이 방식은 내부적으로 스트림 버퍼를 그대로 복사하므로, 중간에 명시적인 문자 반복자는 없지만 여전히 전체 파일 내용을 메모리에 로드합니다.

`3.2` 방식은 스트림의 문자들을 하나씩 반복자로 읽어 `std::string`에 추가하는 방식입니다.
- 기본 생성된 `end` sentinel은 EOF 상태를 나타냅니다.
- 반복자는 EOF 직전까지의 문자들만 `std::string`에 저장합니다.

둘 다 결과적으로 전체 파일을 메모리로 읽어들이므로, 큰 파일을 처리할 때는 메모리 사용량을 고려해야 합니다. 차이는 구현 방식의 표현 방식일 뿐이며, 버퍼 유무나 성능 특성이 크게 다른 것은 아닙니다.

> 일반적으로 더 표준적이고 흔히 쓰이는 방식은 `3.1`의 `rdbuf()` + `std::ostringstream` 패턴입니다. `3.2`는 표준 반복자 스타일을 선호할 때 유용한 대안입니다.

## 4. 텍스트 파일 줄 단위 읽기

줄 단위로 처리할 때는 `std::getline`을 사용합니다.

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

이 방식은 줄 단위 처리에 적합하며, 전체 내용을 하나 문자열로 합치려면 각 줄 뒤에 `\n`을 추가해야 합니다.

## 5. 프로젝트 내 흐름

1. 실행 인자로 파일 경로를 받습니다.
2. `std::ifstream file(path, std::ios::binary);`로 파일을 엽니다.
3. `if (!file)`로 열기 성공 여부를 확인합니다.
4. `3.1` 혹은 `3.2` 방법으로 `std::string`에 전체 내용을 저장합니다.

## 요약

- 파일 읽기는 `std::ifstream`을 사용합니다.
- `std::ios::binary`는 텍스트 변환 없이 바이트 단위로 읽을 때 사용합니다.
- 파일 열기 상태는 `file.is_open()` 또는 `if (!file)`로 확인합니다.
- 전체 파일을 문자열로 읽을 때는 `std::ostringstream` + `rdbuf()` 또는 `std::istreambuf_iterator<char>`를 사용합니다.
- 줄 단위 텍스트 처리는 `std::getline`을 사용합니다.
