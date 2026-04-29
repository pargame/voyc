# fstream — 파일 입출력

C++ 파일 스트림을 사용하여 파일을 읽고 쓰는 방법을 정리합니다.

## 스트림 타입

| 클래스 | 역할 | 모드 |
|--------|------|------|
| `std::ifstream` | 읽기 전용 | `std::ios::in` (기본) |
| `std::ofstream` | 쓰기 전용 | `std::ios::out` (기본) |
| `std::fstream` | 읽기/쓰기 | `std::ios::in \| std::ios::out` |

## RAII

`std::ifstream`은 소멸자에서 파일을 자동으로 닫습니다:

```cpp
{
    std::ifstream file("input.txt");
    // 파일 읽기...
} // ← 소멸자가 자동으로 close()
```

명시적 `close()`는 거의 필요 없습니다.

## 파일 열기 상태 확인

```cpp
std::ifstream file("input.txt");
if (!file.is_open()) {
    // 열기 실패 처리
}
// 또는
if (!file) { /* 동일 */ }
```

## 전체 파일 읽기

`istreambuf_iterator` 방식:

```cpp
std::string content(
    std::istreambuf_iterator<char>(file),
    std::istreambuf_iterator<char>()
);
```

`ostringstream` + `rdbuf()` 방식:

```cpp
std::ostringstream buffer;
buffer << file.rdbuf();
std::string content = buffer.str();
```

두 방식 모두 전체 파일을 메모리에 로드합니다.

## 줄 단위 읽기

```cpp
std::string line;
while (std::getline(file, line)) {
    // line 처리
}
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
- 전체 파일 읽기는 `istreambuf_iterator` 또는 `ostringstream + rdbuf()`를 사용합니다.
- 줄 단위 처리는 `std::getline`을 사용합니다.
