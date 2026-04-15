# VS Code 설정 노트: .vscode 파일 역할과 사용법

이 문서는 이 프로젝트에서 사용하는 `.vscode` 설정 파일의 역할을 정리합니다. 각 파일은 서로 다른 목적을 가지며, 함께 쓰여서 개발 환경을 자동화하고 일관되게 만듭니다.

## 1. `.vscode/launch.json`
### 역할
- 디버깅 세션을 정의하는 파일입니다.
- VS Code의 실행/디버그 버튼을 눌렀을 때 어떤 프로그램을, 어떤 방식으로 실행할지 설정합니다.

### 이 프로젝트에서 사용하는 주요 항목
- `name`: 디버그 구성 이름입니다. VS Code에서 선택할 때 이 이름이 표시됩니다.
- `type`: 디버거 종류입니다. 여기서는 `lldb`로 설정되어 있어 CodeLLDB 확장을 사용합니다.
- `request`: `launch`는 새 실행 프로세스를 시작한다는 의미입니다.
- `program`: 디버깅할 실행 파일 경로입니다. `${workspaceFolder}/build/voyc`처럼 워크스페이스 기준으로 지정합니다.
- `cwd`: 디버거가 실행될 현재 작업 디렉터리입니다.
- `preLaunchTask`: 디버그 시작 전에 자동으로 실행할 빌드 작업입니다.
- `terminal`: 디버거 출력이 표시될 터미널 종류입니다. 여기서는 통합 터미널을 사용합니다.

### 어떻게 쓰나?
1. `Run and Debug` 사이드바에서 `C++ Debug (CodeLLDB)`를 선택합니다.
2. 디버그 버튼을 누르면 `preLaunchTask`에 설정된 빌드가 먼저 실행되고, 이후에 `program`이 실행됩니다.
3. `program` 경로를 변경하면 다른 빌드를 디버깅할 수 있습니다.

## 2. `.vscode/tasks.json`
### 역할
- VS Code에서 실행할 수 있는 작업(Task)을 정의하는 파일입니다.
- 빌드, 테스트, 린트, 코드 생성 등 반복되는 명령어를 하나의 이름으로 관리할 수 있습니다.

### 이 프로젝트에서 사용하는 주요 항목
- `label`: 작업 이름입니다. `preLaunchTask`나 `Run Task`에서 이 이름으로 참조합니다.
- `type`: 작업 종류입니다. 일반적인 쉘 명령은 `shell`로 지정합니다.
- `command`: 실제로 실행할 명령어입니다.
- `args`: 명령어에 전달할 인수 리스트입니다.
- `group`: 빌드 작업인지 테스트 작업인지 등을 구분합니다. `build` 그룹은 기본 빌드 단축키와 연결될 수 있습니다.
- `presentation`: 작업 출력의 표시 방식과 포커스 여부를 제어합니다.
- `problemMatcher`: 컴파일 오류를 자동으로 Problems 패널에 매핑할 때 쓰는 설정입니다.

### 이 프로젝트의 빌드 흐름
- `build cpp project`라는 작업은 `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja`로 구성한 뒤 `cmake --build build --config Debug`를 실행합니다.
- `preLaunchTask`에 이 작업을 연결하면, 디버깅 전에 항상 최신 빌드가 수행됩니다.
- `group.kind`가 `build`이므로 VS Code의 기본 빌드 단축키(`⌘+Shift+B` 등)로도 실행할 수 있습니다.

## 3. `.vscode/settings.json`
### 역할
- 워크스페이스 전용 설정을 저장하는 파일입니다.
- 이 프로젝트에 특화된 편집기, 확장, 언어 서버 옵션 등을 정의합니다.

### 이 프로젝트에서 사용하는 주요 항목
- `clangd.arguments`: `clangd` 언어 서버 실행 시 전달할 추가 인수입니다.
  - `--compile-commands-dir=build`를 지정해서, `.clangd`가 없더라도 `build/compile_commands.json`을 참조하도록 합니다.
- `cmake.sourceDirectory`: CMake가 소스 루트를 찾을 때 사용할 디렉터리입니다.
- `cmake.buildDirectory`: CMake 빌드 출력과 메타데이터를 저장할 빌드 디렉터리입니다.

### 어떻게 쓰나?
- VS Code에서 워크스페이스를 열면 `.vscode/settings.json`이 자동으로 적용됩니다.
- `clangd.arguments`로 `build` 디렉터리를 지정하면, `clangd`가 각 소스 파일의 컴파일 옵션을 올바르게 가져옵니다.
- CMake Tools 확장을 쓰는 경우, `cmake.buildDirectory`가 빌드 위치를 일관되게 유지하도록 돕습니다.

## 4. 추가로 알면 좋은 점
- `.vscode` 폴더는 워크스페이스 전용 설정을 저장하기 때문에 다른 개발자와 공유하기 적합합니다.
- 개인별 설정은 `settings.json` 대신 사용자 설정(User Settings)에 두는 것이 좋습니다.
- 만약 `c_cpp_properties.json`이 있으면 Microsoft C/C++ 확장의 인클루드, 정의, 컴파일러 경로 등을 추가로 설정할 수 있습니다.
- 이 프로젝트는 `configs/.clangd`에 `clangd` 설정을 두고, VS Code에서는 `.vscode/settings.json`을 통해 `clangd`가 `build/compile_commands.json`을 읽도록 구성했습니다.

## 5. 정리
- `launch.json`: 디버깅 환경과 실행 프로그램을 정의합니다.
- `tasks.json`: 빌드와 같은 반복 작업을 정의하고 자동화합니다.
- `settings.json`: 워크스페이스 특화 편집기/언어 서버 설정을 보관합니다.

이 파일들을 함께 쓰면, VS Code에서 C++ 빌드와 디버깅 환경을 비교적 적은 수동 작업으로 안정적으로 구성할 수 있습니다.