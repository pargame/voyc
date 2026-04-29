# gh — GitHub CLI

GitHub CLI `gh`는 GitHub 작업을 터미널에서 수행하는 공식 클라이언트입니다. `git`이 로컬 저장소를 관리한다면, `gh`는 GitHub의 웹 기능(이슈, PR, 리포지토리 등)을 CLI로 확장합니다.

## gh와 git의 관계

| 도구 | 관리 대상 | 역할 |
|------|-----------|------|
| `git` | 로컬 저장소 | 커밋, 브랜치, 머지 |
| `gh` | GitHub 원격 | PR, 이슈, 리뷰, 릴리스 |

`gh`는 낸부에서 `git`을 호출할 수 있지만, 독립적인 도구입니다. `git` 없이도 `gh auth login`, `gh repo clone` 등을 사용할 수 있습니다.

## 설치

| 플랫폼 | 명령어 |
|--------|--------|
| macOS | `brew install gh` |
| Ubuntu/Debian | `sudo apt install gh` |
| Windows | `winget install GitHub.cli` |

## 인증

### 로그인

```bash
gh auth login
```

1. GitHub.com 또는 GitHub Enterprise 선택
2. HTTPS 또는 SSH 프로토콜 선택
3. 웹 브라우저에서 인증 코드 입력

### 토큰으로 로그인

```bash
echo "ghp_xxxx" | gh auth login --with-token
```

CI나 자동화 환경에서 사용합니다.

### 상태 확인

```bash
gh auth status
```

현재 로그인된 계정과 토큰 스코프를 확인합니다.

## 저장소 작업

### 클론

```bash
# GitHub CLI 방식
gh repo clone owner/repo

# git 방식과의 차이: gh는 권한이 자동 포함됨
gh repo clone myorg/private-repo  # 인증된 상태면 바로 접근
```

### 저장소 생성

```bash
# 현재 디렉터리를 GitHub에 올림
gh repo create --public --source=. --remote=origin --push

# 원격만 생성, 로컬은 연결하지 않음
gh repo create myproject --public
```

| 옵션 | 의미 |
|------|------|
| `--public` / `--private` | 공개 여부 |
| `--source=.` | 현재 디렉터리를 원격에 연결 |
| `--remote=origin` | 원격 이름 |
| `--push` | 생성 후 즉시 푸시 |

### 저장소 정보 확인

```bash
gh repo view
gh repo view owner/repo --web  # 브라우저에서 열기
```

## 브랜치와 PR

### PR 생성

```bash
gh pr create --title "Fix bug" --body "상세 설명"
```

인터랙티브 모드(옵션 없이 실행)도 지원합니다.

| 옵션 | 의미 |
|------|------|
| `--base main` | 대상 브랜치 |
| `--head feature` | 소스 브랜치 |
| `--draft` | 초안 PR |
| `--reviewer user` | 리뷰어 지정 |

### PR 목록 및 검토

```bash
gh pr list              # 열린 PR 목록
gh pr status            # 현재 브랜치 관련 PR 상태
gh pr view 42           # 특정 PR 상세
gh pr checkout 42       # PR 브랜치를 로컬로 가져오기
```

`gh pr checkout`은 리뷰할 때 특히 유용합니다 — PR 작성자의 브랜치를 직접 `fetch`하고 `checkout`합니다.

### PR 머지

```bash
gh pr merge 42
```

| 옵션 | 의미 |
|------|------|
| `--squash` | 스쿼시 머지 |
| `--rebase` | 리베이스 머지 |
| `--delete-branch` | 머지 후 브랜치 삭제 |

## 이슈 관리

```bash
gh issue create --title "Bug" --body "설명"
gh issue list --label bug --assignee me
gh issue view 7
gh issue close 7
```

| 옵션 | 의미 |
|------|------|
| `--label` | 라벨 필터/지정 |
| `--assignee` | 담당자 필터/지정 |
| `--milestone` | 마일스톤 지정 |

## 워크플로우와 Actions

```bash
gh workflow list           # GitHub Actions 워크플로우 목록
gh workflow run ci.yml     # 수동 실행
gh run list                # 최근 실행 목록
gh run view 123456789      # 특정 실행 상세
gh run watch 123456789     # 실시간 로그 확인
```

## 릴리스

```bash
gh release create v1.0.0 --title "v1.0.0" --notes "변경사항"
```

| 옵션 | 의미 |
|------|------|
| `--draft` | 초안 릴리스 |
| `--prerelease` | 프리릴리스 |
| `--generate-notes` | PR 히스토리로 자동 생성 |

## alias

자주 쓰는 명령을 줄여쓸 수 있습니다:

```bash
gh alias set co "pr checkout"
gh alias set pv "pr view"
gh alias set iv "issue view"

# 사용
gh co 42
gh pv
```

## gh와 API

`gh`는 GitHub REST/GraphQL API를 낸부적으로 호출합니다. 직접 API를 호출할 수도 있습니다:

```bash
# REST API 호출
gh api repos/:owner/:repo/issues

# GraphQL 호출
gh api graphql -f query='query { viewer { login } }'
```

`gh`의 장점은 인증과 페이징을 자동으로 처리한다는 것입니다.

## 설정

```bash
gh config list                  # 전체 설정
gh config get git_protocol      # 특정 설정값
gh config set editor vim        # 기본 에디터
```

설정 파일 위치: `~/.config/gh/config.yml`

## 흔한 워크플로우

### 기능 개발 PR

```bash
git checkout -b feature/login
# 작업
git add . && git commit -m "feat: login"
git push -u origin feature/login
gh pr create --title "feat: login" --body "구현 내용"
# 리뷰 후
gh pr merge --squash --delete-branch
```

### 버그 수정

```bash
git checkout -b fix/memory-leak
# 작업
git add . && git commit -m "fix: memory leak in parser"
git push -u origin fix/memory-leak
gh pr create --title "fix: memory leak" --label bug
```

## 요약

- `gh`는 GitHub 웹 기능을 CLI로 제공합니다.
- `git`과 별개의 도구이며, 인증이 내장되어 있습니다.
- PR 생성/검토/머지, 이슈 관리, Actions 실행, 릴리스를 터미널에서 처리할 수 있습니다.
- `alias`로 자주 쓰는 명령을 줄여쓸 수 있습니다.
