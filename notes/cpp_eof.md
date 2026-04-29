# EOF 처리

C++ iostream에서 파일 끝(EOF)을 올바르게 감지하고 처리하는 방법을 정리합니다.

## Stream 상태 비트

| 비트 | 의미 | 언제 설정되는가 |
|------|------|-----------------|
| `eofbit` | 입력 끝 | 파일 끝에 도달했을 때 |
| `failbit` | 연산 실패 | 형식 불일치, 읽기 불가 |
| `badbit` | 심각한 오류 | 스트림 손상, 하드웨어 오류 |

스트림은 `failbit` 또는 `badbit`이 설정되면 `bool` 평가에서 `false`가 됩니다.

## 권장 루프 패턴

읽기 함수의 반환값으로 루프를 제어합니다:

```cpp
// 한 줄씩 읽기
std::string line;
while (std::getline(std::cin, line)) {
    // line 처리
}

// 형식화된 입력
int x;
while (std::cin >> x) {
    // x 처리
}
```

EOF, 형식 오류, 스트림 오류를 모두 적절히 처리합니다.

## 피해야 할 패턴

```cpp
// ❌ 마지막 줄을 두 번 처리할 수 있음
while (!std::cin.eof()) {
    std::string s;
    std::getline(std::cin, s);
    // s 처리
}
```

`eof()`는 이미 EOF에 도달한 뒤에 `true`가 됩니다. 읽기 함수의 반환값으로 제어하세요.

## 전체 파일 읽기

```cpp
std::string content(
    std::istreambuf_iterator<char>(file),
    std::istreambuf_iterator<char>()
);
```

두 번째 기본 생성 반복자가 EOF를 나타냅니다. 파일 끝에 도달하면 읽기가 자동으로 멈춥니다.

## 파일 열기 상태 확인

```cpp
std::ifstream file("input.txt");
if (!file.is_open()) {
    // 열기 실패 처리
}
// 또는
if (!file) { /* 동일 */ }
```

## 바이너리 읽기

```cpp
char buffer[1024];
if (std::cin.read(buffer, sizeof(buffer))) {
    std::streamsize n = std::cin.gcount();
    // n 바이트 처리
}
```

`read()`가 실패핏도 `gcount()`는 실제로 읽은 바이트 수를 반환합니다.

## 요약

- `while (!stream.eof())`는 피하고, 읽기 함수의 반환값으로 루프를 제어합니다.
- 파일 열기 실패는 `is_open()` 또는 `if (!file)`로 체크합니다.
- 전체 파일 읽기는 `istreambuf_iterator` 또는 `ostringstream + rdbuf()`를 사용합니다.
- 바이너리 읽기 후에는 `gcount()`로 실제 읽은 바이트 수를 확인합니다.
