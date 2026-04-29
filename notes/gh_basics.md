# gh 기초

voyc 개발에 필요한 GitHub CLI 기본 명령어를 정리합니다.

## 설치

```bash
brew install gh
```

## 로그인

```bash
gh auth login
```

브라우저에서 인증 코드를 입력합니다. 완료 후 상태 확인:

```bash
gh auth status
```

## 저장소 클론

```bash
gh repo clone pargame/voyc
```

## 자주 쓰는 명령어

| 목적 | 명령어 |
|------|--------|
| 로그인 | `gh auth login` |
| 상태 확인 | `gh auth status` |
| 클론 | `gh repo clone <owner>/<repo>` |
| 이슈 목록 | `gh issue list` |
| PR 목록 | `gh pr list` |

## 전체 명령어 확인

```bash
gh help
gh <command> --help   # 예: gh pr --help
```
