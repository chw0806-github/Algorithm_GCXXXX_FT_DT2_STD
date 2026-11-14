@echo off
setlocal EnableExtensions EnableDelayedExpansion

cd /d "%~dp0"

set "HASH=unknown"
set "STATUS=no_git"
set "GIT_EXE="

if exist "%ProgramFiles%\Git\cmd\git.exe" set "GIT_EXE=%ProgramFiles%\Git\cmd\git.exe"
if not defined GIT_EXE if exist "%ProgramFiles%\Git\bin\git.exe" set "GIT_EXE=%ProgramFiles%\Git\bin\git.exe"
if not defined GIT_EXE if exist "%ProgramFiles(x86)%\Git\cmd\git.exe" set "GIT_EXE=%ProgramFiles(x86)%\Git\cmd\git.exe"
if not defined GIT_EXE if exist "%LocalAppData%\Programs\Git\cmd\git.exe" set "GIT_EXE=%LocalAppData%\Programs\Git\cmd\git.exe"
if not defined GIT_EXE if exist "%USERPROFILE%\scoop\apps\git\current\cmd\git.exe" set "GIT_EXE=%USERPROFILE%\scoop\apps\git\current\cmd\git.exe"

if not defined GIT_EXE goto :write_header

echo "%GIT_EXE%" | findstr /I /C:"WindowsApps" >nul
if not errorlevel 1 (
    set "GIT_EXE="
    goto :write_header
)

set "STATUS=no_changes"

for /f "usebackq delims=" %%H in (`"%GIT_EXE%" rev-parse --short HEAD 2^>nul`) do (
    set "HASH=%%H"
)

if /I "!HASH!"=="unknown" (
    set "STATUS=no_git"
    goto :write_header
)

"%GIT_EXE%" diff --quiet HEAD 2>nul
if errorlevel 1 set "STATUS=need_commits"

if /I "!STATUS!"=="no_changes" (
    for /f "usebackq delims=" %%L in (`"%GIT_EXE%" status --porcelain -uall 2^>nul`) do (
        set "STATUS=need_commits"
    )
)

:write_header
set "TMPH=git_version_data.h.tmp"
set "OUTH=git_version_data.h"

(
echo #ifndef GIT_VERSION_DATA_H
echo #define GIT_VERSION_DATA_H
echo #define GIT_COMMIT_HASH "!HASH!"
echo #define GIT_DIFF_STATUS "!STATUS!"
echo #endif
) > "!TMPH!"

set "NEED_COPY=1"
if exist "!OUTH!" (
    fc /b "!TMPH!" "!OUTH!" >nul 2>&1
    if not errorlevel 1 set "NEED_COPY=0"
)

if "!NEED_COPY!"=="1" (
    copy /y "!TMPH!" "!OUTH!" >nul
    echo [git_version] HASH=!HASH! DIFF=!STATUS! ^(updated^)
) else (
    echo [git_version] HASH=!HASH! DIFF=!STATUS! ^(unchanged^)
)

del /q "!TMPH!" >nul 2>&1
exit /b 0
