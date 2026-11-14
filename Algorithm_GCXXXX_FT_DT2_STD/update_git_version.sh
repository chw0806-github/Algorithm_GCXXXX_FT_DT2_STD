#!/bin/sh
cd "$(dirname "$0")" || exit 0

HASH=unknown
STATUS=no_git

if command -v git >/dev/null 2>&1; then
    HASH=$(git rev-parse --short HEAD 2>/dev/null)
    [ -z "$HASH" ] && HASH=unknown

    if [ "$HASH" = "unknown" ]; then
        STATUS=no_git
    else
        STATUS=no_changes
        if ! git diff --quiet HEAD 2>/dev/null; then
            STATUS=need_commits
        elif [ -n "$(git status --porcelain -uall 2>/dev/null)" ]; then
            STATUS=need_commits
        fi
    fi
fi

TMPH=git_version_data.h.tmp
OUTH=git_version_data.h

cat > "$TMPH" <<EOF
#ifndef GIT_VERSION_DATA_H
#define GIT_VERSION_DATA_H
#define GIT_COMMIT_HASH "$HASH"
#define GIT_DIFF_STATUS "$STATUS"
#endif
EOF

if [ -f "$OUTH" ] && cmp -s "$TMPH" "$OUTH"; then
    echo "[git_version] HASH=$HASH DIFF=$STATUS (unchanged)"
    rm -f "$TMPH"
else
    mv -f "$TMPH" "$OUTH"
    echo "[git_version] HASH=$HASH DIFF=$STATUS (updated)"
fi
exit 0
