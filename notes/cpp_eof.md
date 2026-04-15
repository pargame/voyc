# EOF — C++에서의 의미 및 처리

이 문서는 C++ iostream에서 EOF(End‑Of‑File)가 어떻게 표현되고 처리되는지를 중심으로 설명합니다. C의 `EOF` 매크로와 비교할 때, C++에서는 입력 스트림의 상태 비트와 함수 반환값을 활용해 EOF를 안전하게 다루어야 합니다.

## 1. 개요

C++에서 EOF는 "데이터가 더 이상 없음을 나타내는 상태"로 이해해야 합니다. iostream은 `eofbit`, `failbit`, `badbit`라는 상태 비트를 통해 스트림 상태를 관리하고, 읽기 연산 함수는 그 성공 여부를 반환합니다. 이 문서에서는 C++ 표준 라이브러리 관점에서 EOF 처리 권장 패턴을 제시하고, 흔한 오용 사례를 정리합니다.

## 2. C++ iostream의 EOF 처리

### 2.1. 스트림 상태 비트

C++ iostream은 다음 상태 비트를 사용합니다.

- `eofbit`: 입력이 끝났음을 나타냅니다.
- `failbit`: 입력이 실패했음을 나타냅니다. 형식이 맞지 않거나, 읽기가 불가능한 경우에 설정됩니다.
- `badbit`: 심각한 오류가 발생했음을 나타냅니다. 일반적으로 하드웨어 오류나 스트림 손상이 원인입니다.

스트림은 불리언 평가에서 `failbit` 또는 `badbit`가 설정되면 `false`가 됩니다. `eofbit`만 단독으로 설정된 경우에도 `failbit`가 함께 설정될 수 있습니다.

### 2.2. 권장 루프 패턴

C++에서는 읽기 함수 자체의 반환값을 사용해 루프를 제어하는 것이 안전합니다.

```cpp
while (std::getline(std::cin, line)) {
    // line 처리
}
```

```cpp
int x;
while (std::cin >> x) {
    // x 처리
}
```

이 패턴은 EOF, 형식 오류, 스트림 오류를 모두 적절히 처리합니다.

### 2.3. 피해야 할 패턴

다음처럼 `eof()`만으로 루프를 제어하는 코드는 잘못된 결과를 초래할 수 있습니다.

```cpp
while (!std::cin.eof()) {
    std::string s;
    std::getline(std::cin, s);
    // 마지막 반복에서 빈 문자열을 처리하거나 중복된 처리가 발생할 수 있음
}
```

`eof()`는 이미 EOF에 도달한 뒤에 `true`가 됩니다. 따라서 읽기 함수 호출 후 성공 여부를 검사해야 합니다.

### 2.4. 스트림 상태 확인 방법

- `if (stream)` 또는 `while (stream)`
  * `failbit` 또는 `badbit`가 설정되지 않은 경우 참입니다.
- `stream.good()`
  * 스트림이 정상인 경우 참입니다.
- `stream.eof()`
  * 스트림이 EOF 상태에 있는지를 확인합니다. 읽기 후에 확인하는 용도로 사용해야 합니다.
- `stream.fail()`
  * 입력/출력 연산이 실패했는지 확인합니다.
- `stream.bad()`
  * 데이터 손상이나 I/O 오류가 발생했는지 확인합니다.

### 2.5. 예외 모드

`std::ios::exceptions()`를 설정하면 `failbit`나 `badbit` 발생 시 예외를 던질 수 있습니다. 일반적으로는 기본 예외 모드를 사용하지 않고, 반환값 기반 루프를 권장합니다.

```cpp
std::cin.exceptions(std::ios::badbit);
```

예외 모드를 쓰면 EOF를 감지하는 방식이 달라질 수 있으므로, 사용 시에는 의도한 상태 비트만 예외로 처리하도록 설정해야 합니다.

## 3. C 스타일 EOF와의 비교

C++에서는 C의 `EOF` 매크로가 직접 사용되는 경우가 거의 없습니다. 그러나 비교를 위해 C의 방식도 이해하면 도움이 됩니다.

### 3.1. C의 EOF 표식

- `<stdio.h>`의 `EOF`는 정수값으로 정의됩니다. 일반적으로 `-1`입니다.
- `getchar()`, `fgetc()`는 더 이상 읽을 문자가 없으면 `EOF`를 반환합니다.
- 반환형이 `int`인 이유는 `EOF`와 모든 `unsigned char` 값을 구분하기 위해서입니다.

### 3.2. C++에서의 호환성

`std::cin.get()`는 `int_type`를 반환하고, `traits_type::eof()` 값을 EOF로 사용할 수 있습니다. 하지만 대부분의 C++ 코드에서는 `operator>>`, `std::getline`, `read()`와 같은 함수의 반환값을 통해 EOF를 처리합니다.

## 4. 바이너리 읽기와 `gcount()`

바이너리 모드(`std::ios::binary`)에서는 EOF를 직접 확인하기보다는 실제로 읽은 바이트 수를 검사하는 것이 일반적입니다.

```cpp
char buffer[1024];
if (std::cin.read(buffer, sizeof(buffer))) {
    std::streamsize n = std::cin.gcount();
    // n 바이트 처리
}
```

`read()`가 실패하면 `gcount()`는 실제로 읽은 바이트 수를 반환합니다. 이 방식은 데이터의 끝과 오류를 구분할 때 유용합니다.

## 5. 라이브러리 차원의 EOF

- `std::char_traits<char>::eof()` 또는 `std::istream::traits_type::eof()`는 C++ 라이브러리 차원에서 사용하는 EOF 값을 제공합니다.
- 제네릭 코드에서는 이러한 traits 기반 EOF 값을 사용해 문자 타입별로 일관된 처리를 할 수 있습니다.

## 6. 흔한 실수 및 권장사항

- `while (!stream.eof())`는 피합니다. 읽기 함수의 성공 여부를 직접 검사하세요.
- `std::cin >> value`를 사용할 때, 마지막 값 뒤 공백이나 줄바꿈으로 EOF가 발생하면 루프가 자동 종료됩니다.
- `std::getline`은 빈 줄을 읽어도 `true`를 반환합니다. `eofbit`는 해당 호출 이후에 설정될 수 있습니다.
- `std::istream::read()` 후에는 `gcount()`로 실제 읽은 바이트 수를 확인합니다.
- EOF는 문자 자체가 아니며, 입력 스트림의 상태를 나타내는 개념입니다.

## 7. C++ 예제 모음

### 7.1. 한 줄씩 읽기

```cpp
#include <iostream>
#include <string>

int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        std::cout << line << '\n';
    }
}
```

### 7.2. 정수 시퀀스 읽기

```cpp
#include <iostream>

int main() {
    int n;
    while (std::cin >> n) {
        std::cout << n << '\n';
    }
}
```

### 7.3. 바이너리 블록 읽기

```cpp
#include <iostream>

int main() {
    char buffer[1024];
    while (std::cin.read(buffer, sizeof(buffer)) || std::cin.gcount() > 0) {
        std::streamsize n = std::cin.gcount();
        // buffer[0..n) 처리
    }
}
```

## 8. 참고

- cppreference: `std::istream`, `std::getline`, `std::ios_base::iostate`, `std::char_traits`
- ISO C 표준 `<stdio.h>` 및 C++ 표준 라이브러리 문서
