# Stream 상태와 EOF

voyc에서 파일을 읽을 때 stream 상태를 확인하고, lexer는 문자열 끝을 인덱스로 체크하는 방식을 정리합니다.

## voyc에서의 흐름

```
main.cpp: ifstream → string (전체 읽기)
         ↓
lexer.cpp: string_view → index_ < src_.size() (끝 체크)
```

lexer는 stream을 직접 다루지 않습니다. 파일 내용을 한 번에 메모리에 올린 뒤, `string_view`로 파싱합니다.

## Stream 상태 비트

| 비트 | 의미 | voyc에서 체크하는 위치 |
|------|------|------------------------|
| `failbit` | 연산 실패 | `if (!file.is_open())` |
| `badbit` | 심각한 오류 | (현재 미사용) |
| `eofbit` | 입력 끝 | `istreambuf_iterator`가 자동 처리 |

## 파일 열기 실패 체크

voyc의 `main.cpp`:

```cpp
std::ifstream file(argv[1]);
if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << argv[1] << std::endl;
    return 1;
}
```

`if (!file)`도 동일하게 동작합니다. `failbit` 또는 `badbit`이 설정되면 `false`가 됩니다.

## 파일 전체 읽기

voyc에서는 `istreambuf_iterator`로 파일 끝까지 읽어 `string`에 저장합니다:

```cpp
std::string src(
    std::istreambuf_iterator<char>(file),
    std::istreambuf_iterator<char>()
);
```

두 번째 인자의 기본 생성 반복자가 EOF를 나타냅니다. 파일 끝에 도달하면 읽기가 자동으로 멈춥니다.

## 피해야 할 패턴

```cpp
// ❌ 마지막 줄을 두 번 처리할 수 있음
while (!file.eof()) {
    std::getline(file, line);
    // line 처리
}
```

`eof()`는 이미 EOF에 도달한 뒤에 `true`가 됩니다. 읽기 함수의 반환값으로 제어하세요.

```cpp
// ✅ 읽기 성공 여부로 제어
while (std::getline(file, line)) {
    // line 처리
}
```

## voyc lexer의 끝 체크

lexer에서는 stream 상태가 아닌 문자열 경계를 체크합니다:

```cpp
while (index_ < src_.size()) {
    Token next = nextToken(msg);
    // ...
}
```

`string_view`는 `size()`를 알고 있으므로, EOF 개념 없이 경계 검사만으로 충분합니다.

## 요약

- voyc의 파일 읽기는 `ifstream` + `istreambuf_iterator`를 사용합니다.
- 파일 열기 실패는 `is_open()` 또는 `if (!file)`로 체크합니다.
- lexer는 메모리상의 `string_view`를 파싱하므로 `index_ < size()`로 끝을 체크합니다.
- `while (!stream.eof())`는 피하고, 읽기 함수의 반환값으로 루프를 제어합니다.
