@echo off

:: Check if exactly one argument is passed
if "%~1"=="" (
    echo No argument provided.
    exit /b 1
)

if not "%~2"=="" (
    echo Too many arguments provided.
    exit /b 1
)

:: Check if the argument has a .eulr extension
if /i "%~x1"==".eulr" GOTO RUNEULR
if /i "%~x1"==".elr" GOTO RUNEULR
if /i "%~x1"==".eu" GOTO RUNEULR

echo Invalid file extension. Please provide a .eulr file.
exit /b 1

:RUNEULR
if EXIST "%~1" (
    echo:
    CALL .\build\bin\Debug\EulerLang.exe %1
    echo:
    echo "Euler ran. Check execution.log for more details..."
    exit /b 0
) ELSE (
    echo Error: File not found
    exit /b 1
)
