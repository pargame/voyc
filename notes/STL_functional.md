# functional

voyc에서 `<functional>`의 사용 현황과 필요성을 정리합니다.

## 현재 상태

voyc의 `main.cpp`에서 `<functional>`은 주석 처리되어 있습니다:

```cpp
// #include <functional>
```

현재 lexer는 함수 객체를 직접 저장하거나 콜백을 전달하지 않습니다.

## 핵심 타입

| 타입 | 역할 | voyc에서 필요한가 |
|------|------|-------------------|
| `std::function` | 호출 가능 객체를 타입 소거하여 저장 | 현재 없음 |
| `std::bind` | 부분 적용 | 람다로 대체 가능 |
| `std::ref` | 참조를 값처럼 저장 | 현재 없음 |

## `std::function`의 오버헤드

```cpp
std::function<void(int)> cb = [](int x) { /* ... */ };
```

- 내부에서 타입 소거(type-erasure)를 수행합니다.
- small-object optimization이 있어도 힙 할당 가능성이 있습니다.
- 호출 시 가상 함수처럼 간접 호출이 발생합니다.

성능 민감 경로에서는 템플릿 기반 호출이 더 효율적입니다:

```cpp
// ✅ 인라인 가능, 오버헤드 없음
template<typename F>
void process(F&& callback) {
    callback(42);
}
```

## voyc에서의 필요성

현재 voyc lexer는 상태 기반 파싱이며, 콜백이나 함수 저장이 필요하지 않습니다. 향후 다음 상황에서 고려할 수 있습니다:

- 파서 단계에서 이벤트 기반 콜백 등록
- 플러그인 아키텍처 도입
- 테스트에서 mock 함수 주입

## 요약

- voyc는 현재 `<functional>`을 사용하지 않습니다.
- `std::function`은 편리하지만 호출 오버헤드가 있습니다.
- 콜백이 필요해지면 먼저 람다나 템플릿을 고려한 뒤, 타입 소거가 필요할 때 `std::function`을 사용합니다.
