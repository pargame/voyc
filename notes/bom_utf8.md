# UTF-8 BOM (Byte Order Mark)

UTF-8 인코딩 파일 맨 앞에 붙는 3바이트 시그니처입니다. Windows 메모장이나 일부 IDE가 저장 시 자동으로 추가합니다.

## 바이트 구성

| 순서 | 바이트 값 | 16진수 |
|------|-----------|--------|
| 1 | 239 | 0xEF |
| 2 | 187 | 0xBB |
| 3 | 191 | 0xBF |

## lexer에서의 문제

BOM이 그대로 남아 있으면 첫 토큰이 깨집니다.

```
// BOM이 있는 경우
\xEF\xBB\xBFint main() {...}

// lexer가 보는 첫 문자
0xEF (알 수 없는 기호) -> Unknown 토큰 또는 에러
```

## 처리 방법

생성자에서 소스 길이를 먼저 확인하고, BOM이 있으면 `index_`를 3부터 시작합니다.

```cpp
explicit Lexer(const std::string& source) : source_(source) {
    // 길이가 3 이상이고 첫 3바이트가 BOM이면 스킵
    if (source_.size() >= 3 &&
        static_cast<unsigned char>(source_[0]) == 0xEF &&
        static_cast<unsigned char>(source_[1]) == 0xBB &&
        static_cast<unsigned char>(source_[2]) == 0xBF) {
        index_ = 3;
    }
}
```

## 주의사항

- **길이 체크 필수**: 소스가 3바이트 미만일 때 `source_[2]`에 접근하면 UB(undefined behavior)
- **`unsigned char` 캐스팅 필수**: `char`가 signed 타입인 플랫폼에서 0xEF는 -17로 해석됨. 비교 전에 `unsigned char`로 캐스팅해야 원래 값인 239로 비교 가능

## 흔한 실수

| 실수 | 결과 |
|------|------|
| `source[0] == 0xEF` (signed char) | 조건이 항상 false. 0xEF는 -17이고, -17 == 239는 false |
| 길이 체크 없이 `source[2]` 접근 | 소스가 2바이트일 때 힙 버퍼 오버플로우 |
| `std::string_view`에서 BOM 스킵 후 원본 소스 소멸 | `string_view`는 참조용. 원본 `std::string`이 소멸되면 dangling reference |

## 생성자의 explicit

생성자가 1개의 인자를 받으면 컴파일러가 암시적 변환을 시도합니다.

```cpp
// explicit가 없는 경우
Lexer lexer = "int main() {}";  // const char* -> std::string -> Lexer 로 암시적 변환
process("...");                 // 함수 인자로도 암시적 변환 가능
```

`explicit`를 붙이면 반드시 직접 생성해야 합니다.

```cpp
explicit Lexer(const std::string& source);

Lexer lexer("...");     // OK
Lexer lexer = "...";    // 컴파일 에러. 암시적 변환 불가
process(Lexer("..."));  // OK. 명시적 생성
```

lexer처럼 낮은 수준의 리소스를 초기화하는 클래스는 의도하지 않은 변환이 버그를 유발하므로 `explicit`를 붙이는 것이 일반적입니다.

## 요약

- BOM은 3바이트: `0xEF 0xBB 0xBF`
- 처리는 생성자에서 한 번만 수행
- 항상 길이 체크 후 `unsigned char`로 캐스팅해서 비교
- `string_view` 사용 시 원본 소스의 수명 관리 필요
- 생성자에는 `explicit`를 붙여 암시적 변환 방지
