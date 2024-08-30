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
if /i "%~x1"==".eulr" (
    echo Valid .eulr file detected.
    :: Perform the desired operation here
    echo RUN
) else (
    echo Invalid file extension. Please provide a .eulr file.
    exit /b 1
)

CALL .\build\bin\Debug\EulerLang.exe %1