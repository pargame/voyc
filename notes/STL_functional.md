# STL_functional — <functional> 핵심 노트

이 문서는 C++ 표준 라이브러리 `<functional>` 헤더에 포함된 주요 타입과 유틸리티를 정리합니다. `<functional>`은 호출 가능한 객체(callable)를 다루기 위한 래퍼와 함수 객체(function object), 멤버 호출 보조 도구 등을 제공하며, 실무에서는 콜백 저장, 일반화된 호출(invoke), 함수 적응자(adaptor) 등으로 자주 사용됩니다. 본 노트는 중요도 순으로 항목을 배열하고 각 항목마다 사용 예제와 주의사항을 구체적으로 적습니다.

## 핵심 타입 및 유틸리티

### `std::function<R(Args...)>`
- 설명: 타입 소거(type-erasure) 기반의 호출 가능한 래퍼입니다. 다양한 형태의 호출자(함수 포인터, 함수 객체, 람다)를 하나의 타입으로 저장할 수 있습니다.
- 동작 및 비용: 내부에 대상(callable)을 복사·보관하며, 구현에 따라 small-object optimization(소형 내부 버퍼)이 있을 수 있으나 힙 할당이 발생할 수 있습니다. 호출 오버헤드가 존재하므로 성능 민감 경로에서는 템플릿 기반 호출(인라인 람다 또는 템플릿 파라미터)을 우선 고려하세요.
- 예제:
```cpp
#include <functional>
#include <iostream>
#include <memory>

void print(int x) { std::cout << x << '\n'; }

int main() {
    std::function<void(int)> cb = print;
    cb(5);

    auto up = std::make_unique<int>(42);
    std::function<int()> f = [p = std::move(up)]() { return *p; }; // move-capture
    (void)f;
}
```

### `std::reference_wrapper<T>`, `std::ref`, `std::cref`
- 설명: 참조를 값으로 보관할 수 있게 해주는 어댑터입니다. 컨테이너에 원본 객체의 참조를 넣고 싶을 때 사용합니다.
- 주의: `reference_wrapper`는 원본 객체의 수명에 의존하므로, 컨테이너가 원본보다 오래 살아서는 안 됩니다.
- 예제:
```cpp
#include <functional>
#include <vector>
#include <iostream>

int main() {
    int x = 1;
    std::vector<std::reference_wrapper<int>> vr;
    vr.push_back(std::ref(x));
    vr[0].get() = 5; // x가 변경됨
    std::cout << x << '\n';
}
```

### `std::bind` / `std::placeholders`
- 설명: 기존 함수나 멤버 함수를 부분 적용(partial application)하여 새로운 호출자를 생성합니다.
- 권장사항: 표현력이 떨어지거나 가독성이 나빠지는 경우가 있으므로, 가능하면 명시적 람다로 대체하세요. `std::bind`는 플레이스홀더의 위치와 평가 시점을 주의해야 합니다.
- 예제:
```cpp
#include <functional>
#include <iostream>

int add(int a, int b) { return a + b; }

int main() {
    using namespace std::placeholders;
    auto add5 = std::bind(add, 5, _1); // add5(x) == add(5, x)
    std::cout << add5(3) << '\n';
}
```

### `std::mem_fn` / `std::invoke`
- 설명: 멤버 함수 포인터 및 멤버 포인터를 호출 가능한 형태로 변환하거나, 임의의 호출자를 일관되게 호출하기 위한 도구입니다.
- 차이: `std::mem_fn`은 멤버 함수/멤버 객체에 대한 호출자 래퍼를 반환하고, `std::invoke`는 전달된 호출자를 직접 호출합니다.
- 예제:
```cpp
#include <functional>
#include <iostream>

struct S { int f(int x) const { return x + 1; } };

int main() {
    S s;
    auto mf = std::mem_fn(&S::f);
    std::cout << mf(s, 3) << '\n';
    std::cout << std::invoke(&S::f, s, 4) << '\n';
}
```

### 함수 객체(function objects)와 어댑터
- 설명: `std::plus`, `std::minus`, `std::less`, `std::equal_to` 등과 같은 표준 함수 객체는 상태가 없고 재사용이 가능합니다. C++14 이후에는 템플릿 매개변수 없는 버전(`std::plus<>`)을 제공해 타입을 유추합니다.
- `std::not_fn` 등 어댑터는 기존 호출자의 논리 반대를 얻을 때 유용합니다.

## 예제 패턴 (실무)
- 콜백 목록 저장:
```cpp
#include <vector>
#include <functional>

std::vector<std::function<void(int)>> callbacks;

void register_cb(std::function<void(int)> cb) {
    callbacks.push_back(std::move(cb));
}
```
- 멤버 함수 호출자 생성:
```cpp
struct Logger { void log(const std::string&); };
auto log_fn = std::mem_fn(&Logger::log);
```

## 주의사항 및 권장사항
- `std::function`은 편리하지만 호출 오버헤드와 힙 할당 가능성이 있으므로 성능 경계선에서는 템플릿(인라인 람다 또는 함수 템플릿)을 우선 고려하세요.
- `std::bind`는 가독성 문제와 복잡한 플레이스홀더 규칙 때문에 람다로 대체하는 것이 일반적으로 더 명확합니다.
- 람다로 move-only 타입을 캡처할 때는 명시적으로 `p = std::move(...)` 형태로 캡처하세요.
- `std::reference_wrapper`는 원본 수명 관리에 주의하세요(참조 파편화 위험).
- `std::mem_fn`/`std::invoke`는 제네릭 코드에서 호출을 통일할 때 유용합니다.

## 참고
- cppreference: `<functional>` 항목 (검색 권장)
