# string_view

`std::string_view`의 개념과 사용법을 정리합니다.

## 개념

문자열 데이터를 소유하지 않는 읽기 전용 뷰입니다. 포인터와 길이만 가진 가벼운 타입입니다.

```cpp
#include <string_view>

std::string_view sv1{"hello"};        // 문자열 리터럴
std::string_view sv2{str};            // std::string
std::string_view sv3{ptr, len};       // (포인터, 길이)
```

## 주요 메서드

| 메서드 | 역할 |
|--------|------|
| `size()` | 길이 |
| `data()` | 시작 포인터 |
| `substr(pos, len)` | 부분 뷰 (복사 아님) |
| `remove_prefix(n)` | 앞 n글자 제거 |
| `starts_with(s)` | 접두사 확인 (C++20) |

## 함수 인자로 사용

읽기 전용 인자에 적합합니다:

```cpp
void print(std::string_view text) {
    std::cout << text;
}

// 어떤 문자열이든 전달 가능
print("hello");      // 리터럴
print(str);          // std::string
print(view);         // string_view
```

## 서브스트링

복사 비용 없이 부분 문자열을 참조합니다:

```cpp
std::string_view src = "hello world";
std::string_view word = src.substr(6, 5);  // "world"
// 원본 데이터를 복사하지 않음
```

## 수명 주의

`string_view`는 원본 데이터를 참조만 합니다:

```cpp
// ✅ 원본이 유효한 동안 사용
std::string src = "hello";
std::string_view view{src};
// src가 살아 있는 동안 view는 안전

// ❌ 임시 객체를 참조하면 위험
std::string_view bad{std::string("hello")};  // 임시 string이 소멸됨
```

## `std::string`과의 차이

| | `std::string` | `std::string_view` |
|--|---------------|--------------------|
| 소유권 | ✅ 소유 | ❌ 참조만 |
| 수정 | ✅ 가능 | ❌ 읽기 전용 |
| 복사 비용 | 데이터 전체 복사 | 포인터+길이만 |
| 수명 | 자체 관리 | 원본 의존 |

## 요약

- `string_view`는 복사 없는 문자열 뷰입니다.
- 읽기 전용 함수 인자나 서브스트링 생성에 적합합니다.
- 원본 데이터의 수명이 뷰보다 길어야 합니다.
- 수정이나 소유가 필요한 경우에는 `std::string`을 사용합니다.
