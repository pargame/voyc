# VS Code 설정

voyc 프로젝트를 VS Code에서 개발할 때 필요한 설정을 정리합니다.

> 현재 voyc에는 `.vscode` 폴�더가 없습니다. 아래 설정을 적용하려면 프로젝트 루트에 `.vscode/`를 생성합니다.

## 필요한 확장

| 확장 | 역할 |
|------|------|
| `llvm-vs-code-extensions.vscode-clangd` | C++ 언어 서버 |
| `ms-vscode.cmake-tools` | CMake 통합 |

## `settings.json`

```json
{
    "clangd.arguments": [
        "--compile-commands-dir=build"
    ],
    "cmake.sourceDirectory": "${workspaceFolder}",
    "cmake.buildDirectory": "${workspaceFolder}/build"
}
```

`clangd`가 `build/compile_commands.json`을 읽어 정확한 코드 분석을 수행합니다.

## `tasks.json`

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build voyc",
            "type": "shell",
            "command": "cmake --build build",
            "group": { "kind": "build", "isDefault": true }
        }
    ]
}
```

`⌘+Shift+B`로 빌드를 실행합니다.

## `launch.json`

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "debug voyc",
            "type": "lldb",
            "request": "launch",
            "program": "${workspaceFolder}/build/voyc",
            "args": ["${workspaceFolder}/example.voy"],
            "preLaunchTask": "build voyc"
        }
    ]
}
```

## `.gitignore`에 추가

```
.vscode/
```

> 개인별 설정은 사용자 설정에 두고, 공유가 필요한 설정만 `.vscode/`에 넣습니다.
