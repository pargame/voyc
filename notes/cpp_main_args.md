# main 함수와 인자 처리

voyc의 `main.cpp`에서 명령행 인자를 받아 파일을 여는 흐름을 정리합니다.

## 시그니처

```cpp
int main(int argc, char **argv)
```

| 인자 | 의미 |
|------|------|
| `argc` | 인자 개수 (`argv[0]` 포함) |
| `argv` | 인자 문자열 배열 (`argv[argc]`는 `nullptr`) |
| `argv[0]` | 프로그램 이름 또는 경로 |
| `argv[1]` | 첫 번째 사용자 인자 |

## voyc 실제 코드

```cpp
int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: 'voyc [file.voy]'" << std::endl;
        return 1;
    }

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
    // 결과 출력...
    return 0;
}
```

## 핵심 체크리스트

| 검사 | 방법 | 실패 시 |
|------|------|---------|
| 인자 개수 | `argc != 2` | 사용법 출력, `return 1` |
| 파일 열기 | `file.is_open()` | 에러 메시지, `return 1` |

## 인자 안전 접근

`argv[1]`에 접근하기 전에 `argc`를 반드시 확인합니다:

```cpp
// ✅
if (argc > 1) {
    std::string_view path{argv[1]};
}

// ❌ 정의되지 않은 동작
std::string_view path{argv[1]};  // argc가 1이면 위험
```

## 요약

- `argc`를 먼저 확인한 뒤 `argv`에 접근합니다.
- voyc는 `argc != 2`일 때 사용법을 출력하고 종료합니다.
- 파일 열기 실패는 별도로 체크하여 명확한 에러 메시지를 제공합니다.
