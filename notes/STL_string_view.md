# C++ `std::string_view`

`std::string_view`는 문자열 데이터를 소유하지 않는 읽기 전용 뷰입니다. 데이터 본체를 복사하지 않고, 기존 문자열 버퍼를 가리키면서 텍스트를 처리할 수 있게 해줍니다.

## 1. 기본 개념

- 헤더: `#include <string_view>`
- 타입: `std::basic_string_view<char>`의 별칭
- 특징: 포인터(`data()`)와 길이(`size()`)를 가진 값 타입
- 소유권이 없으므로, 문자열이 사라지면 뷰는 무효가 됩니다.

## 2. 언제 사용하나

- 함수 인자로 문자열을 읽기 전용으로 전달할 때
- `std::string`이나 C 스타일 문자열을 복사 없이 읽기만 할 때
- 서브스트링을 만들고 싶지만, 복사 비용을 줄이고 싶을 때
- 명령행 인자(`argv`) 같은 외부 문자열을 그대로 참조할 때

## 3. 생성 방법

```cpp
#include <string_view>
#include <string>

std::string_view sv1{"hello"};
std::string str = "hello";
std::string_view sv2{str};
std::string_view sv3{str.data(), 3}; // "hel"
```

- 문자열 리터럴에서 생성할 때는 null 종료 문자열을 뷰로 삼습니다.
- `std::string`에서 생성할 때는 내부 버퍼를 참조합니다.
- 길이를 명시하면 문자열 중간이나 일부분을 가리킬 수 있습니다.

## 4. 주요 동작과 메서드

```cpp
sv.size();
sv.empty();
sv.data();
sv.substr(1, 2);
sv.remove_prefix(2);
sv.remove_suffix(1);
sv == "hi";
sv.compare("hi");
```

- `data()`는 뷰가 참조하는 문자열 버퍼의 시작 주소를 반환합니다.
  - 예를 들어 `sv.data()`는 내부적으로 `const char *` 포인터입니다.
  - `sv.size()`와 함께 사용할 때 안전하게 문자열 내용을 읽을 수 있습니다.
  - `std::string_view` 자체는 null 종료를 보장하지 않으므로, `data()`를 C 문자열처럼 쓸 때는 주의가 필요합니다.
- C++20부터 `starts_with`, `ends_with` 같은 편의 메서드가 있습니다.
- `substr()`는 새로운 `std::string_view`를 반환하지만, 여전히 같은 문자열 데이터를 가리킵니다.

## 5. 소유권과 수명 주의

`std::string_view`는 소유권이 없으므로, 원본 문자열이 살아 있어야 합니다.

좋은 예:

```cpp
std::string input = "hello";
std::string_view view{input};
// input이 살아 있는 동안 view는 유효함
```

나쁜 예:

```cpp
std::string_view view;
{
    std::string temp = "hello";
    view = temp;
}
// temp가 소멸했으므로 view는 더 이상 유효하지 않습니다.
```

- `std::string_view`는 `std::string`과 다르게 소유권 이동(move)이 없고, 값을 복사해도 포인터+크기만 복사됩니다.
- 임시 `std::string`이나 함수 내부 로컬 문자열을 참조하면 위험합니다.

## 6. C 스타일 문자열과 `std::string_view`

```cpp
const char *cstr = "hello";
std::string_view sv{cstr};
```

- `std::string_view`는 null 종료 여부를 보장하지 않습니다.
- `sv.data()`는 뷰가 가리키는 문자열의 시작 주소를 반환합니다.
- `sv.data()`가 가리키는 버퍼에 `\0`이 있더라도, `std::string_view`는 `size()`만큼만 유효하다고 봅니다.
- 예를 들어:

```cpp
std::string str = "hello";
std::string_view sv{str.data() + 1, 3}; // "ell"
```

이 경우 `sv.data()[3]`은 `'o'`이고, `\0`는 그 뒤에 있습니다.
- 따라서 `std::string_view`를 C 문자열처럼 다루면 안 되고, `data()`와 `size()`를 함께 써서 뷰 범위만 읽어야 안전합니다.
- C 스타일 문자열을 `std::string_view`로 받을 때는 항상 `size()`를 기준으로 처리하세요.

## 7. `std::string`과의 차이

- `std::string`은 데이터를 소유하고 수정 가능한 문자열입니다.
- `std::string_view`는 읽기 전용 뷰이며, 수정 메서드가 없습니다.
- 복사가 필요 없다는 점이 가장 큰 장점입니다.

## 8. 함수 인터페이스 권장 패턴

`std::string_view`는 읽기 전용 인자를 받을 때 유용합니다.

```cpp
void printText(std::string_view text) {
    std::cout << text;
}
```

- `std::string`, C 스타일 문자열, 문자열 리터럴 모두 자연스럽게 전달할 수 있습니다.
- 복사 비용 없이 읽기 전용 API를 설계할 때 적합합니다.

## 9. 요약

- `std::string_view`는 복사 없는 문자열 뷰입니다.
- 소유권이 없으므로 원본 데이터의 수명 주의를 지켜야 합니다.
- 읽기 전용 함수 인자나 서브스트링 생성에 적합합니다.
- 수정이나 소유가 필요한 경우에는 `std::string`을 사용합니다.
