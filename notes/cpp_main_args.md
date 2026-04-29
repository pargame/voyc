# main 함수와 명령행 인자

C++에서 `argc`, `argv`를 다루는 방법을 정리합니다.

## 시그니처

```cpp
int main(int argc, char *argv[])
// 또는
int main(int argc, char **argv)
```

| 인자 | 의미 |
|------|------|
| `argc` | 인자 개수 (`argv[0]` 포함) |
| `argv` | 인자 문자열 배열 (`argv[argc]`는 `nullptr`) |
| `argv[0]` | 프로그램 이름 또는 경로 |
| `argv[1]` | 첫 번째 사용자 인자 |

## 인자 개수 체크

```cpp
int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: program <file>\n";
        return 1;
    }
    // argv[1] 사용
}
```

`argv[1]`에 접근하기 전에 반드시 `argc`를 확인합니다.

## 안전한 인자 접근

```cpp
// ✅ argc 확인 후 접근
if (argc > 1) {
    std::string_view path{argv[1]};
}

// ❌ 정의되지 않은 동작
std::string_view path{argv[1]};  // argc가 1이면 위험
```

## 인자 순회

```cpp
for (int i = 1; i < argc; ++i) {
    std::string_view arg{argv[i]};
    if (arg == "--help") {
        std::cout << "Usage: ...\n";
    }
}
```

실제 인자는 `argv[1]`부터 시작합니다.

## 파일 열기와 에러 체크

```cpp
std::ifstream file(argv[1]);
if (!file.is_open()) {
    std::cerr << "Error: Could not open " << argv[1] << '\n';
    return 1;
}
```

## 요약

- `argc`를 먼저 확인한 뒤 `argv`에 접근합니다.
- `argv[0]`은 프로그램 이름, 실제 인자는 `argv[1]`부터 시작합니다.
- 파일 열기 실패는 별도로 체크하여 명확한 에러 메시지를 제공합니다.
- 복잡한 옵션 파싱은 `std::getopt`나 `argparse` 같은 라이브러리를 사용합니다.
