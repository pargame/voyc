# GitHub CLI gh 기초 노트

이 노트는 GitHub CLI인 `gh`의 기본 사용법과 주요 옵션을 정리합니다. GitHub 저장소 관리, 인증, PR/이슈/릴리스 작업을 커맨드 라인에서 빠르게 실행할 수 있도록 설명합니다.

## 1. 설치 및 인증

### 1.1. 설치

- macOS(Homebrew): `brew install gh`
- Linux: 패키지 매니저 또는 GitHub CLI 공식 릴리스 설치
- Windows: `winget install GitHub.cli` 또는 Chocolatey

### 1.2. 로그인

```bash
gh auth login
```

- `--hostname <hostname>`: 기본값은 `github.com`.
- `--web`: 웹 브라우저 인증 사용.
- `--git-protocol <protocol>`: `https` 또는 `ssh`.
- `--scopes <scope1,scope2>`: 인증 토큰에 필요한 권한 범위 지정.

예:

```bash
gh auth login --hostname github.com --web --git-protocol ssh --scopes repo,workflow
```

### 1.3. 로그인 상태 확인

```bash
gh auth status
```

- 인증된 계정 및 권한 정보를 확인합니다.
- `--hostname <hostname>` 옵션으로 다른 GitHub 호스트를 확인할 수 있습니다.

## 2. 저장소 작업

### 2.1. 저장소 클론

```bash
gh repo clone <owner>/<repo>
```

옵션:

- `-- --depth 1`: 얕은 클론.
- `-- --single-branch`: 단일 브랜치만 클론.

예:

```bash
gh repo clone pargame/voyc -- --depth 1
```

### 2.2. 저장소 초기화

```bash
gh repo create <name> --public --source=. --remote=origin
```

옵션:

- `--public` / `--private`: 공개 여부.
- `--source <path>`: 로컬 디렉터리를 원격 저장소에 연결.
- `--remote <name>`: 원격 이름 설정.
- `--description <text>`: 저장소 설명 추가.
- `--confirm`: 상호작용 없이 생성 진행.

## 3. 이슈와 PR

### 3.1. 이슈 생성

```bash
gh issue create
```

주요 옵션:

- `--title <title>`: 이슈 제목.
- `--body <body>`: 이슈 본문.
- `--assignee <user>`: 담당자 지정.
- `--label <label>`: 라벨 지정(여러 개 가능).
- `--project <project>`: 프로젝트 보드 연결.
- `--milestone <milestone>`: 마일스톤 지정.
- `--web`: 브라우저로 생성.

예:

```bash
gh issue create --title "Fix build script" --body "Update CMake setup" --label bug --assignee pargame
```

### 3.2. PR 생성

```bash
gh pr create
```

옵션:

- `--base <branch>`: 대상 브랜치, 기본값 `main` 또는 `master`.
- `--head <branch>`: 소스 브랜치.
- `--title <title>`: PR 제목.
- `--body <body>`: PR 설명.
- `--reviewer <user>`: 리뷰어 지정.
- `--assignee <user>`: 담당자 지정.
- `--label <label>`: 라벨 지정.
- `--draft`: 초안 PR 생성.
- `--web`: 브라우저로 생성.

예:

```bash
gh pr create --base main --head feature/zsh-notes --title "Add zsh basics note" --body "Add zsh usage guide" --reviewer pargame --label documentation
```

### 3.3. PR 보기 및 체크

```bash
gh pr status
```

- 현재 체크아웃된 저장소의 PR 상태를 보여줍니다.
- `--web` 옵션으로 브라우저에서 PR 페이지를 여는 것도 가능.

```bash
gh pr view <number> --web
```

- 특정 PR을 브라우저로 엽니다.
- `--json <fields>`로 JSON 출력도 지원.

## 4. 릴리스 및 배포

### 4.1. 릴리스 생성

```bash
gh release create <tag> --title <title> --notes <notes>
```

옵션:

- `--draft`: 임시 저장 릴리스.
- `--prerelease`: 프리릴리스로 표시.
- `--target <branch>`: 대상 브랜치.
- `--notes-file <file>`: 릴리스 노트를 파일에서 읽기.
- `--discussion-category <category>`: 토론 카테고리 지정.

예:

```bash
gh release create v0.1.0 --title "v0.1.0" --notes-file RELEASE_NOTES.md --draft
```

## 5. 설정 및 도움말

### 5.1. 기본 도움말

```bash
gh help
```

- 전체 명령 목록과 주요 옵션을 보여줍니다.

### 5.2. 서브커맨드 도움말

```bash
gh <command> --help
```

예:

```bash
gh pr create --help
```

### 5.3. 설정 확인

```bash
gh config list
```

- 현재 `gh` 설정을 확인합니다.
- `gh config get <key>`로 특정 설정값을 볼 수 있습니다.

## 6. 자주 쓰는 옵션 값 정리

- `--web`: 브라우저에서 GitHub UI를 여는 옵션.
- `--title`: 제목 지정.
- `--body`: 본문 또는 설명 지정.
- `--label`: 라벨 추가.
- `--assignee`: 담당자 지정.
- `--reviewer`: 리뷰어 지정.
- `--draft`: 초안 상태로 만들기.
- `--public` / `--private`: 저장소 공개 여부.
- `--confirm`: 대화형 확인 없이 실행.

## 7. 요약

- GitHub CLI `gh`는 GitHub 작업을 터미널에서 빠르게 처리할 수 있게 해줍니다.
- 인증은 `gh auth login`, 상태 확인은 `gh auth status`.
- 저장소 생성/클론, 이슈/PR 작성, 릴리스 생성 등의 작업을 명령 하나로 실행할 수 있습니다.
- 옵션값을 구체적으로 쓰면 대화형 입력 없이도 정확한 작업을 수행할 수 있습니다.
