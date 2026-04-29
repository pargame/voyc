# 🛠️ Build Toolchain — C++ 빌드 도구 실전 정리

이 노트는 macOS 표준 환경에서 `voyc`를 빌드하고 실행하는 흐름을 정리합니다.

> **툴체인(toolchain)**: 소스 코드를 실행 파일로 만드는 도구들의 모음. 여기서는 CMake, 컴파일러, 링커의 연계를 다룹니다.

---

## 1. 필요한 도구 설치

```bash
brew install cmake
```

| 도구 | 역할 | 필수 여부 |
|------|------|-----------|
| `cmake` | 빌드 설정 생성 | ✅ 필수 |
| `ninja` | 빠른 빌드 실행기 | 선택 (미설치 시 Make 사용) |
| `llvm` (Homebrew) | `clang++` + `clangd` + `lldb` | 권장 (IDE 사용 시 유용) |

> Apple Clang은 시스템에 기본 설치되어 있으므로 `brew install llvm` 없이도 빌드는 가능합니다. Homebrew LLVM은 `clangd` 언어 서버와 버전 일치가 필요할 때 설치합니다.

---

## 2. 빌드 → 실행 (핵심 흐름)

### 처음 설정
```bash
cd /Users/pargame/repos/voyc
mkdir -p build && cd build
cmake ..
make
```

> `-G Ninja`를 붙이면 Ninja를 사용합니다. 생략하면 시스템 기본(보통 Make)을 사용합니다.

### 소스 수정 후 다시 빌드
```bash
cd build
make
```
> 변경된 파일만 다시 컴파일합니다.

### 실행
```bash
./voyc example.voy
```

---

## 3. 상황별 명령어

| 상황 | 명령어 |
|------|--------|
| 처음 설정 | `cmake ..` |
| 코드만 수정 | `make` |
| `CMakeLists.txt` 수정 | `cmake .. && make` |
| 새 `.cpp` 파일 추가 | `cmake .. && make` |
| 빌드 상세 로그 보기 | `make VERBOSE=1` |
| 빌드 산출물 정리 | `make clean` 또는 `rm -rf build/` |
| 완전 재구성 | `rm -rf build/ && mkdir build && cd build && cmake .. && make` |

---

## 4. CMakeLists.txt 실제 구성

```cmake
cmake_minimum_required(VERSION 4.3)
project(voyc VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(voyc
    src/main.cpp
    src/lexer.cpp
)

target_include_directories(voyc PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_options(voyc PRIVATE
    $<$<CXX_COMPILER_ID:Clang>:-Wall;-Wextra;-Wpedantic>
)
```

### 핵심 설정 설명

| 설정 | 의미 |
|------|------|
| `VERSION 4.3` | 이 프로젝트가 요구하는 최소 CMake 버전 |
| `CMAKE_CXX_STANDARD 26` | C++26 표준 사용 (`-std=c++26` 자동 추가) |
| `CMAKE_CXX_STANDARD_REQUIRED ON` | C++26을 지원하지 않는 컴파일러면 구성 단계에서 실패 |
| `CMAKE_CXX_EXTENSIONS OFF` | GNU 확장을 끄고 표준 C++ 모드 사용 (이식성↑) |
| `CMAKE_EXPORT_COMPILE_COMMANDS ON` | `build/compile_commands.json` 생성 → `clangd`가 빌드 옵션을 읽음 |
| `PRIVATE` | include 경로/옵션이 이 타겟 낸부에서만 적용됨 (다른 타겟으로 전파되지 않음) |

> `compile_commands.json`은 `build/` 폴�더 안에 생성됩니다. `.gitignore`에 `build/`만 추가하면 됩니다.

---

## 5. `.gitignore` 포함 항목

```
build/
.vscode/
*.dSYM
```

> `compile_commands.json`은 `build/` 안에 있으므로 별도로 추가할 필요 없습니다.

---

## 6. VS Code 설정

VS Code에서 빌드 버튼, `clangd`, 디버깅 설정은 별도 문서를 참고하세요:
- [`vscode-configs.md`](./vscode-configs.md)

---

## 7. 문제 해결

### 빌드 실패 시 체크리스트

1. **CMake 버전 확인**
   ```bash
   cmake --version
   ```
   4.3 이상이어야 합니다.

2. **컴파일러 버전 확인**
   ```bash
   clang++ --version
   ```
   C++26을 지원하는 버전이어야 합니다.

3. **빌드 폴�더 완전 재구성**
   ```bash
   rm -rf build/
   mkdir build && cd build
   cmake ..
   make
   ```
   오래된 캐시(`CMakeCache.txt`)가 원인일 때 유효합니다.

4. **상세 로그로 원인 확인**
   ```bash
   make VERBOSE=1
   ```

---

*작성일: 2026-04-30*
