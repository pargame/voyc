# fstream — 파일 입출력

voyc에서 파일을 읽어 `std::string`으로 만드는 방법을 정리합니다.

## 스트림 타입

| 클래스 | 역할 | 모드 |
|--------|------|------|
| `std::ifstream` | 읽기 전용 | `std::ios::in` (기본) |
| `std::ofstream` | 쓰기 전용 | `std::ios::out` (기본) |
| `std::fstream` | 읽기/쓰기 | `std::ios::in \| std::ios::out` |

## RAII

`std::ifstream`은 소멸자에서 파일을 자동으로 닫습니다. 명시적 `close()`는 필요 없습니다:

```cpp
{
    std::ifstream file("input.voy");
    // 파일 읽기...
} // ← 소멸자가 자동으로 close()
```

## 파일 열기 상태 확인

```cpp
std::ifstream file(path);
if (!file.is_open()) {
    // 열기 실패 처리
}
// 또는
if (!file) { /* 동일 */ }
```

## 전체 파일 읽기

voyc의 방식 (`istreambuf_iterator`):

```cpp
std::string src(
    std::istreambuf_iterator<char>(file),
    std::istreambuf_iterator<char>()
);
```

대안 (`ostringstream` + `rdbuf()`):

```cpp
std::ostringstream buffer;
buffer << file.rdbuf();
std::string src = buffer.str();
```

두 방식 모두 전체 파일을 메모리에 로드합니다. 큰 파일에는 메모리 사용량을 고려해야 합니다.

## voyc 실제 코드

```cpp
// main.cpp
std::ifstream file(argv[1]);
if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << argv[1] << std::endl;
    return 1;
}

std::string src(
    std::istreambuf_iterator<char>(file),
    std::istreambuf_iterator<char>()
);

const voyc::LexResult result = voyc::lexSource(src);
```

## 모드 플래그

| 플래그 | 의미 |
|--------|------|
| `std::ios::binary` | 텍스트 변환 없이 바이트 그대로 읽기 |
| `std::ios::app` | 파일 끝에 추가 쓰기 |
| `std::ios::trunc` | 열 때 기존 내용 제거 |

```cpp
std::ofstream out("log.txt", std::ios::app);  // 뒤에 추가
std::ifstream bin("data.bin", std::ios::binary);
```

## 요약

- `std::ifstream`은 RAII로 파일을 자동 관리합니다.
- 파일 열기 실패는 `is_open()`으로 체크합니다.
- voyc는 `istreambuf_iterator`로 전체 파일을 `std::string`에 읽습니다.
- 쓰기 시 `std::ios::app`으로 기존 내용을 보존할 수 있습니다.
