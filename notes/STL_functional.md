# functional

C++ `<functional>` 헤더의 핵심 타입과 사용법을 정리합니다.

## 핵심 타입

| 타입 | 역할 | 사용 시점 |
|------|------|-----------|
| `std::function` | 호출 가능 객체를 타입 소거하여 저장 | 콜백 저장, 다형적 호출 |
| `std::bind` | 부분 적용 | 람다로 대체 가능 |
| `std::ref` / `std::cref` | 참조를 값처럼 저장 | 컨테이너에 참조 저장 |

## `std::function`

```cpp
#include <functional>

std::function<void(int)> cb = [](int x) { std::cout << x; };
cb(5);
```

- 다양한 형태(함수 포인터, 람다, 함수 객체)를 하나의 타입으로 저장합니다.
- 타입 소거로 인해 호출 오버헤드가 있습니다.
- small-object optimization이 있어도 힙 할당 가능성이 있습니다.

## 오버헤드

성능 민감 경로에서는 템플릿 기반 호출이 더 효율적입니다:

```cpp
// ✅ 인라인 가능, 오버헤드 없음
template<typename F>
void process(F&& callback) {
    callback(42);
}

// 🟡 편리하지만 간접 호출 오버헤드 있음
void process(std::function<void(int)> callback) {
    callback(42);
}
```

## `std::bind` vs 람다

```cpp
// std::bind — 가독성 떨어짐
auto add5 = std::bind(add, 5, std::placeholders::_1);

// ✅ 람다 — 더 명확
auto add5 = [](int x) { return add(5, x); };
```

가능하면 람다로 대체합니다.

## `std::reference_wrapper`

```cpp
int x = 1;
std::vector<std::reference_wrapper<int>> vr;
vr.push_back(std::ref(x));
vr[0].get() = 5;  // x가 변경됨
```

원본 객체의 수명이 컨테이너보다 길어야 합니다.

## 요약

- `std::function`은 편리하지만 호출 오버헤드가 있습니다.
- 성능 경계선에서는 템플릿 기반 호출을 우선 고려합니다.
- `std::bind`는 람다로 대체하는 것이 일반적으로 더 명확합니다.
- `std::reference_wrapper`는 원본 수명 관리에 주의합니다.
