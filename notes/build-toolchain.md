# 🛠️ Build Toolchain — C++ 빌드 도구 실전 정리

이 노트는 macOS(맥북) 표준 개발 환경에서 실제로 쓰는 C++ 빌드 흐름과 VS Code 설정을 간결하게 정리합니다.

*툴체인(toolchain)*은 소스 코드를 실행 가능한 프로그램으로 만들기 위해 함께 쓰이는 도구들의 모음입니다. 여기서는 CMake, Ninja, clang++, 링커 같이 빌드와 컴파일, 링킹을 담당하는 도구들의 연계를 의미합니다.

> 이 프로젝트는 macOS용입니다. `clang++`, `Ninja`, `CMake`는 macOS 환경에서 Homebrew로 설치하는 경우를 기준으로 설명하며, 표준적 맥북 C++ 개발 환경에 어울리는 구성입니다.

---

## 1. macOS 전용 설치 및 환경 변수

### 1.1. 필요한 도구 설치
```bash
brew install cmake ninja llvm
```
* `cmake`: 빌드 설정 생성 도구
* `ninja`: 빠른 빌드 실행기
* `llvm`: Homebrew LLVM 툴체인. macOS 시스템 기본 `clang` 대신 Homebrew LLVM을 쓰는 이유는 다음과 같습니다.
  * `clang`, `clang++`: C/C++ 컴파일러. 프로젝트와 `clangd`가 동일한 컴파일러를 쓰도록 맞춥니다.
  * `clangd`: C/C++ 언어 서버. VS Code에서 빌드 기반 자동 완성, 정의로 이동, 오류 진단을 정확하게 제공합니다.
  * `lldb`, `lldb-server`: 디버거. macOS에서 `CodeLLDB`와 함께 네이티브 디버깅을 지원합니다.
  * `llvm-ar`, `llvm-ranlib`: 정적 라이브러리(`.a`)를 만들 때 사용합니다.
  * `llvm-config`: LLVM 라이브러리/헤더 경로를 확인할 때 유용합니다.

이 프로젝트는 CMake + Ninja 기반 빌드 환경이므로, `llvm` 패키지를 설치하면 `clang++` 컴파일러와 `clangd` 언어 서버가 같은 Homebrew 경로에 존재해 빌드/편집 환경이 일관됩니다.

### 1.2. 환경 변수 설정 이유
Homebrew로 설치한 `llvm`과 `ninja`를 바로 쓰려면, 쉘이 해당 바이너리 경로를 찾아야 합니다.

`~/.zshrc` 또는 `~/.zprofile`에 다음을 추가하세요:
```bash
export PATH="/opt/homebrew/opt/llvm/bin:/opt/homebrew/opt/ninja/bin:$PATH"
export LDFLAGS="-L/opt/homebrew/opt/llvm/lib"
export CPPFLAGS="-I/opt/homebrew/opt/llvm/include"
export PKG_CONFIG_PATH="/opt/homebrew/opt/llvm/lib/pkgconfig:$PKG_CONFIG_PATH"
```

### 1.3. 환경 변수 반영 방법
설정을 바꾼 뒤에는 다음 중 하나를 실행해야 새 터미널에 반영됩니다:
```bash
source ~/.zshrc
```
또는
```bash
source ~/.zprofile
```

VS Code를 이미 열어둔 경우에는:
* 통합 터미널을 닫았다가 새로 열기
* 또는 VS Code 자체를 재시작

이렇게 해야 VS Code가 새 `PATH`를 인식해서 `clang++`, `ninja`, `cmake`를 올바르게 사용할 수 있습니다.

---

## 2. CMake / Ninja / clang++ 역할 요약

* **CMake**: 프로젝트 설정을 읽고 빌드 시스템(Ninja) 구성을 생성합니다.
* **Ninja**: 빌드 순서와 의존성을 계산하고 필요한 컴파일/링크 작업만 실행합니다.
* **clang++**: 각 `.cpp`를 컴파일해서 오브젝트 파일로 만듭니다.
* **ld/lld**: 오브젝트 파일을 합쳐 최종 실행 파일을 만듭니다.

### 핵심 정리
* CMake는 빌드 설정을 생성합니다.
* Ninja는 작업 실행 계획을 만듭니다.
* clang++는 실제로 코드를 번역합니다.
* 링커는 결과물들을 합칩니다.

---

## 3. 언제 어떤 명령어를 쓰나

### 3.1. 처음 한 번만 실행
```bash
cmake -S . -B build -G Ninja
```
* `-S .`: 소스 루트 디렉터리를 지정합니다. 현재 폴더(`.`)의 `CMakeLists.txt`를 읽습니다.
* `-B build`: out-of-source 빌드 디렉터리를 지정합니다. 빌드 결과와 캐시가 `build/` 안에 생성됩니다.
* `-G Ninja`: Ninja 빌드 시스템을 생성기로 선택합니다.
* 이 명령은 CMake 구성 파일을 읽고 `build/` 폴더에 빌드 환경을 생성합니다.
* `build/compile_commands.json`이 생성되어 `clangd`가 올바른 옵션을 읽을 수 있게 됩니다.

### 3.2. 소스 코드만 수정했을 때
```bash
cmake --build build
```
* `--build build`: 이미 생성된 `build/` 디렉터리에서 빌드를 수행합니다.
* `build` 폴더의 CMake 설정을 그대로 재사용해서 실행합니다.
* 변경된 파일만 다시 컴파일합니다.

### 3.3. CMakeLists.txt나 소스 파일 추가/삭제 시
```bash
cmake -S . -B build -G Ninja
cmake --build build
```
* 빌드 구성이 바뀌었으므로 CMake 재실행 후 빌드해야 합니다.

### 3.4. 빌드 결과 실행
```bash
./build/voyc example.voy
```
* `./build/voyc`는 CMake에서 `add_executable(voyc ...)`로 생성한 실행 파일입니다.
* `build/` 폴더는 CMake가 빌드 메타데이터와 바이너리를 저장하는 위치이므로, 실행할 때는 `./build/voyc`를 사용합니다.
* `example.voy`는 입력 파일로, 프로그램에 사용될 스크립트/데이터 파일입니다.

---

## 4. CMakeLists.txt 실제 구성

이 프로젝트에서 쓰는 표준 CMake 설정입니다. 실제로는 `CMakeLists.txt`가 어떤 역할을 하는지, 왜 빌드 폴더를 따로 쓰는지, `clangd`와 어떤 관계인지 이해하는 것이 중요합니다.

```cmake
cmake_minimum_required(VERSION 3.24)
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

### CMake 설정 항목별 설명

#### `cmake_minimum_required(VERSION 3.24)`
* 프로젝트가 요구하는 최소 CMake 버전을 명시합니다.
* CMake가 제공하는 문법이나 기능이 변경될 때, 오래된 버전에서 예기치 않은 동작을 방지하는 역할을 합니다.

#### `project(voyc VERSION 0.1 LANGUAGES CXX)`
* CMake 내부에서 `PROJECT_NAME`, `PROJECT_VERSION` 같은 변수를 설정합니다.
* `LANGUAGES CXX`는 CMake가 C++ 컴파일러를 찾고 C++ 관련 규칙을 활성화하도록 합니다.

#### `set(CMAKE_CXX_STANDARD 26)`
* `clang++`에 `-std=c++26` 옵션을 자동으로 추가하도록 합니다.
* C++26을 명시하면, 최신 언어 기능을 사용하는 코드에서도 일관된 표준을 보장할 수 있습니다.

#### `set(CMAKE_CXX_STANDARD_REQUIRED ON)`
* 지정한 표준이 필수임을 뜻합니다.
* C++26을 지원하지 않는 컴파일러에서는 구성 단계에서 즉시 실패하기 때문에, 빌드 전 호환성 문제가 조기에 발견됩니다.

#### `set(CMAKE_CXX_EXTENSIONS OFF)`
* 기본 GNU 확장을 끄고 표준 C++ 모드를 사용하도록 합니다.
* 플랫폼에 따라 다르게 동작하는 컴파일러 확장 기능을 배제해 코드 이식성을 높입니다.

#### `set(CMAKE_EXPORT_COMPILE_COMMANDS ON)`
* `build/compile_commands.json`를 생성해 빌드 명세를 외부 툴에 제공합니다.
* `clangd`는 이 파일을 통해 실제 컴파일 명령과 include 경로를 읽어 정확한 코드 분석을 수행합니다.
* 이 파일은 `build/` 폴더에 저장되므로, 소스 트리와 분리된 out-of-source 빌드의 장점을 살릴 수 있습니다.

#### `add_executable(voyc src/main.cpp src/lexer.cpp)`
* `voyc` 실행 파일을 만들고 빌드 대상 소스 파일을 명시합니다.
* 파일을 명시적으로 나열하는 방식은 빌드 결과를 예측 가능하게 합니다. `file(GLOB ...)`처럼 흐름을 숨기는 방식은 권장되지 않습니다.

#### `target_include_directories(voyc PRIVATE ${CMAKE_SOURCE_DIR}/include)`
* `include/` 폴더를 해당 타겟의 헤더 검색 경로에 추가합니다.
* `PRIVATE`은 이 경로가 이 타겟 내부에서만 적용된다는 의미입니다.
* 다른 타겟으로 헤더 검색 경로가 전파되지 않아, 의존성 그래프가 더 명확해집니다.

#### `target_compile_options(voyc PRIVATE $<$<CXX_COMPILER_ID:Clang>:-Wall;-Wextra;-Wpedantic>)`
* Clang 전용 경고 플래그를 설정합니다.
* `generator expression`으로 Clang일 때만 옵션을 넣어 다른 컴파일러와 충돌하지 않게 합니다.
* `-Wall -Wextra -Wpedantic`는 코드 문제를 초기에 발견하는 데 유용한 기본 경고 세트입니다.

### 핵심 팁
* `CMakeLists.txt`를 수정했다면, 기존 `build/` 구성과 동기화하려면 `cmake -S . -B build -G Ninja`를 다시 실행하세요.
* 이 단계는 CMake가 실제 컴파일러 명령어를 다시 생성하도록 합니다.
* 이후 `cmake --build build`로 빌드를 실행하면 수정된 설정이 반영됩니다.

### `.clangd` vs `compile_commands.json`
* `compile_commands.json`은 CMake가 만든 빌드 명세입니다. 각 소스 파일을 어떤 컴파일러, 어떤 옵션, 어떤 include 경로로 컴파일할지를 정확히 기록합니다.
* `clangd`는 이 파일을 읽어서 VS Code의 자동 완성과 경고를 실제 빌드 환경과 일치하게 만듭니다.
* `.clangd`는 `clangd` 자체의 설정 파일입니다. 프로젝트별 동작 방식을 조정하거나, `compile_commands.json`이 없는 경우에 폴백 설정을 제공할 때 사용합니다.
* 이 프로젝트처럼 CMake가 `compile_commands.json`을 생성하는 환경에서는, 표준적인 워크플로우로 `.clangd`에 컴파일 플래그를 다시 쓰지 않아도 됩니다. `clangd`는 `compile_commands.json`을 우선 참조합니다.
* `.clangd`는 기본적으로 프로젝트 루트에서 자동 탐지됩니다. 루트가 아닌 다른 폴더(`configs/.clangd` 등)에 두면, VS Code에서 별도 `clangd.arguments` 설정이나 `--enable-config` 설정이 필요합니다.
* 현재 이 프로젝트는 `.clangd`를 `configs/.clangd`로 옮긴 상태이지만, 실제로는 `build/compile_commands.json` 정보가 우선 사용됩니다. 표준 macOS CMake 개발 환경에서는 루트 `.clangd`나 `compile_commands.json`만으로 충분합니다.

---

## 5. VS Code에서 CMake Tools 버튼 사용하기

### 5.1. 버튼이 보이려면
* `CMake Tools` 확장을 설치해야 합니다.
* 설치 후 상태바에서 `Configure` / `Build` / `Run` 버튼이 표시됩니다.
* 설치 명령어 예시:
  ```bash
  code --install-extension ms-vscode.cmake-tools
  ```

### 5.2. 버튼이 없을 때
* 확장을 사용하지 않으면 상태바 버튼 없이도 터미널 명령으로 동일한 작업이 가능합니다.
* `CMake Tools`가 설치되어 있어도 CMake가 아직 구성되지 않았으면, VS Code는 CMake 빌드 버튼 대신 `Compile Active File` 같은 단일 파일 컴파일 UI를 보여줄 수 있습니다.
  * 이 경우는 VS Code가 전체 CMake 프로젝트를 인식하지 못한 상태입니다.
  * 먼저 `CMake: Configure`를 실행해 `build` 폴더와 `compile_commands.json`이 생성되는지 확인해야 합니다.

### 5.3. VS Code CMake 실행 순서
1. `CMake: Select a Kit` 실행
2. macOS용 `darwin` kit 선택
3. `CMake: Configure` 실행
4. `CMake: Build` 실행

### 5.4. darwin vs msvc kit 차이
* `darwin` kit은 macOS용 빌드 환경을 뜻합니다.
* `msvc` kit은 Windows용 Microsoft Visual C++ 빌드 환경을 뜻합니다.
* macOS에서는 `darwin`을 선택해야 합니다. `msvc`는 macOS에서 사용할 수 없습니다.
* `darwin`이 여러 개 보일 때는 흔히 다음 두 종류 중 하나입니다:
  * 시스템 기본 Apple clang (`/usr/bin/clang++`)
  * Homebrew LLVM clang (`/opt/homebrew/opt/llvm/bin/clang++`)
* `darwin`을 선택하면 macOS 플랫폼과 clang 계열 컴파일러가 연결된 상태가 됩니다.

### 5.5. VS Code 확장 조합 차이
* `clangd` 확장은 `clangd` 언어 서버를 사용합니다. CMake의 `compile_commands.json`을 읽어 실제 빌드 옵션을 그대로 반영하므로, CMake 프로젝트에서 정확한 진단과 자동완성을 받기 좋습니다.
* `C/C++` 확장은 Microsoft가 만든 `cpptools`입니다. 일반적으로는 빠른 IntelliSense, 사전 설정된 include 경로, Windows/MSVC 환경을 잘 지원합니다.
* `CodeLLDB`는 LLDB 기반 디버거입니다. macOS에서 `clangd`와 함께 쓰기에 적합하며, `CMake Tools`와 함께 디버깅을 원활하게 해 줍니다.

#### 어떤 조합을 쓰면 좋은가
* macOS + CMake 프로젝트라면 `clangd` + `CodeLLDB` 조합이 가장 자연스럽습니다.
* `C/C++` 확장을 추가로 설치하면 편리할 수 있지만, `clangd`가 이미 빌드 기반 진단을 담당하고 있으므로 동일한 역할을 중복할 수 있습니다.
* `C/C++` 확장만으로도 기본 코딩은 가능하지만, CMake와 `compile_commands.json`이 제대로 연결된 `clangd` 환경이 더 정확합니다.

---

## 6. 추가 실전 팁

### 6.1. 새 `.cpp` 파일 추가 시
* `CMakeLists.txt`에서 새 파일을 등록합니다.
* 파일 추가 후 반드시 `cmake -S . -B build -G Ninja`를 재실행합니다.

### 6.2. 소스 수정 후
* `cmake --build build`만 실행하면 됩니다.

### 6.3. `.clangd` 설정
* `compile_commands.json`을 사용하는 프로젝트라면 `.clangd`에 `CompileFlags`를 중복해서 적지 않아도 됩니다.
* 이 프로젝트는 `.clangd`를 `configs/.clangd`로 옮겨 두었습니다.

### 6.4. `.gitignore`에 포함해야 할 항목
* `build/`
* `.vscode/`
* `compile_commands.json`
* `main`
* `*.dSYM`

---

## 7. 확인해야 할 상황별 요약

| 상황 | 명령어 |
|---|---|
| 처음 설정 | `cmake -S . -B build -G Ninja` |
| 코드만 수정 | `cmake --build build` |
| 새 파일 추가 | `cmake -S . -B build -G Ninja` + `cmake --build build` |
| 실행 | `./build/voyc example.voy` |
| VS Code 버튼 사용 | `CMake Tools` 설치 후 `Configure` → `Build` |

---

## 8. 핵심 정리

* 이 문서는 macOS Homebrew 기반 CMake + Ninja + clang++ 워크플로우를 기준으로 합니다.
* 프로젝트 빌드는 항상 `cmake -S . -B build -G Ninja`로 구성하고, 이후에는 `cmake --build build`를 쓰세요.
* 새 소스 파일을 추가하거나 CMakeLists.txt를 수정하면 CMake 구성을 다시 실행해야 합니다.
* `compile_commands.json`이 있으면 `clangd`가 실제 빌드 옵션을 그대로 사용합니다.
* `.clangd`는 보조 설정일 뿐이며, 루트에 없으면 추가 탐지 설정이 필요합니다.
* VS Code에서 `CMake Tools` 버튼을 보려면 확장 설치가 필요하며, 설치 후에도 `Configure`가 먼저 실행되어야 합니다.

## 9. 추가 개선 포인트

### 9.1. Homebrew LLVM과 macOS 기본 clang의 선택 기준
* Homebrew LLVM은 `clang`, `clang++`, `clangd`, `lldb`가 같은 경로에 있어, 빌드와 에디터 진단 환경을 일관되게 맞출 수 있습니다.
* macOS 기본 `clang`은 시스템 SDK와 Xcode 버전에 묶여 있어 최신 C++ 표준이나 `clangd` 버전과 불일치가 발생할 수 있습니다.
* 실무에서는 `clangd`가 사용하는 컴파일러와 실제 빌드 컴파일러를 동일하게 유지하는 것이 중요합니다.

### 9.2. `build/` 디렉터리를 다시 구성해야 할 때
* CMake 옵션을 변경하거나 `CMakeLists.txt`를 크게 수정한 뒤에도 이상한 캐시 상태가 유지되면, `build/`를 삭제하고 다시 구성하는 것이 안전합니다.
* 기본 절차:
  ```bash
  rm -rf build
  cmake -S . -B build -G Ninja
  cmake --build build
  ```
* `CMakeCache.txt`가 오래된 구성 정보를 담고 있으면, `compile_commands.json`이나 타깃 링크 설정이 예상과 다른 결과를 줄 수 있습니다.

