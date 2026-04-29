# zsh 기본

macOS 기본 셸인 zsh의 핵심 사용법을 정리합니다.

## 설정 파일

| 파일 | 역할 |
|------|------|
| `~/.zshrc` | 셸 시작 시 읽는 사용자 설정 |

변경 후 적용:

```bash
source ~/.zshrc
# 또는
exec zsh
```

## 입력 편집

| 단축키 | 동작 |
|--------|------|
| `Ctrl+U` | 커서 앞 전체 삭제 |
| `Ctrl+K` | 커서 뒤 전체 삭제 |
| `Ctrl+W` | 마지막 단어 삭제 |
| `Ctrl+C` | 입력 취소 |
| `Ctrl+A` | 줄 처음 |
| `Ctrl+E` | 줄 끝 |

## 히스토리

| 단축키 | 동작 |
|--------|------|
| `↑` / `↓` | 이전/다음 명령 |
| `Ctrl+R` | 명령 기록 검색 |
| `history` | 전체 기록 출력 |

## voyc 빌드에 쓰는 명령어 예시

```bash
cd /Users/pargame/repos/voyc/build
cmake ..
make
./voyc ../example.voy
```
