# EOF — C와 C++에서의 의미 및 처리

이 문서는 C와 C++에서의 EOF(End‑Of‑File) 개념을 설명합니다. EOF가 어떻게 표현되는지, 각 언어의 입력 API에서 어떻게 다루어야 하는지, 예제 코드와 흔한 실수 및 권장 패턴을 정리합니다.

## 1. 개요

EOF(End‑Of‑File)는 입력 스트림의 끝을 나타내는 표식입니다. "더 이상 읽을 데이터가 없음"을 표현하는 방식은 언어와 API에 따라 다릅니다: C의 stdio 계열은 `EOF` 매크로(정수 값)를 반환하고, C++의 iostream은 스트림 상태 비트(`eofbit`, `failbit`, `badbit`)로 이를 표시합니다.

## 2. C (stdio 계열)

- 표현: `<stdio.h>`에서 `EOF` 매크로로 정의됩니다(일반적으로 -1). 함수들(`getchar()`, `fgetc()`, `fscanf()` 등)은 더 이상 읽을 문자가 없으면 `EOF`를 반환하거나 실패 상태를 나타냅니다.
- 반환 타입 주의: 문자 단위 입출력 함수의 반환형은 `int`입니다. 이는 모든 가능한 `unsigned char` 값과 `EOF`(-1)를 구별하기 위해 필요합니다.

예시 — 문자 읽기(올바른 방법):

```c
#include <stdio.h>

int main(void) {
    int c; // 반드시 int
    while ((c = getchar()) != EOF) {
        putchar(c);
    }
    return 0;
}
```

잘못된 예(문제 발생):

```c
#include <stdio.h>

int main(void) {
    char c;                  // 잘못된 타입
    while ((c = getchar()) != EOF) { // EOF(-1)를 char와 비교하므로 오류
        putchar(c);
    }
}
```

설명: `char`는 구현에 따라 부호가 있거나 없을 수 있고 값 범위가 `EOF`와 겹칠 수 있습니다. 따라서 `getchar()` 같은 함수의 반환을 저장할 때는 반드시 `int`를 사용하세요.

유틸리티:
- `feof(FILE *stream)` — 스트림이 EOF 상태인지 확인(주로 읽기 실패 후 체크)
- `ferror(FILE *stream)` — 스트림 오류 여부 확인

## 3. C++ (iostream 계열)

- 표현: C++ iostream은 스트림 상태 비트를 사용합니다. 읽기 연산이 파일 끝에 이르면 `eofbit`가 설정되며, 추출 실패는 `failbit` 또는 `badbit`를 설정합니다. 스트림은 불리언 검사(`if (stream)`)나 `std::getline` / `operator>>`의 반환값을 통해서도 상태를 확인할 수 있습니다.
- 권장 패턴: `while (std::getline(...))` 또는 `while (stream >> value)` 형식을 사용하면 EOF와 오류 처리를 적절히 다룰 수 있습니다.

예시 — 한 줄씩 읽기(권장):

```cpp
#include <iostream>
#include <string>

int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        // line 처리
    }
}
```

예시 — 정수 시퀀스 읽기(권장):

```cpp
#include <iostream>

int main() {
    int x;
    while (std::cin >> x) {
        // x 처리
    }
}
```

피해야 할 패턴:

```cpp
// 잘못된 패턴
while (!std::cin.eof()) {
    std::string s;
    std::getline(std::cin, s);
    // eof 체크만으로 루프 제어하면 마지막 반복에서 빈 읽기가 발생할 수 있음
}
```

설명: `eof()`는 스트림이 이미 EOF에 도달했을 때 true를 반환합니다. 보통 읽기 함수 자체의 성공 여부(예: `std::getline`의 반환값)를 이용해 루프를 제어하는 것이 안전합니다.

추가: 내부 구현에서 스트림이 EOF를 만날 때 `eofbit`뿐 아니라 `failbit`가 동시에 설정될 수 있으므로, 단순히 `eof()`만 확인하는 방법은 권장되지 않습니다.

## 4. 고급/라이브러리 관점

- `std::char_traits<char>::eof()` 또는 `std::istream::traits_type::eof()`는 라이브러리 차원의 EOF 값을 제공합니다(제네릭 코드에서 유용).
- 바이너리 모드(`std::ios::binary`)로 읽는 경우에는 바이트 수를 직접 확인하는 방식(예: `read()`와 `gcount()`)을 사용합니다. 이때도 EOF는 더이상 읽을 데이터가 없음을 의미합니다.

## 5. 흔한 실수 및 권장사항 요약

- `getchar()`/`fgetc()`의 반환을 `char`로 받지 마세요 — 항상 `int`로 받기. (EOF 구별 필요)
- `while (!stream.eof())` 같은 패턴을 사용하지 마세요 — 읽기 연산의 성공 여부를 직접 검사하세요(`while (std::getline(...))`, `while (stream >> val)`).
- 텍스트 모드/바이너리 모드 차이는 EOF의 개념과 별개이나, 데이터 처리 방식에 영향을 줍니다. 바이너리 데이터를 다룰 때는 바이트 카운트를 확인하세요.
- EOF는 문자 값이 아니라 "더 이상 데이터가 없음"을 나타내는 특별값/상태입니다. 문자 비교로 처리하지 마세요.

## 6. 예제 모음

간단한 C 예제(문자 복사):

```c
#include <stdio.h>

int main(void) {
    int c;
    while ((c = getchar()) != EOF) {
        putchar(c);
    }
    return 0;
}
```

간단한 C++ 예제(정수 읽기):

```cpp
#include <iostream>

int main() {
    int n;
    while (std::cin >> n) {
        std::cout << n << '\n';
    }
}
```

## 7. 참고

- ISO C 표준 및 `<stdio.h>` 문서
- cppreference: `std::istream`, `std::getline`, `std::char_traits`
