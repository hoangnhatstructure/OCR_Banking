@echo off
setlocal
cd /d "%~dp0"
cmake -S . -B build
cmake --build build --config Release
if exist build\Release\LCOCR.exe (
    build\Release\LCOCR.exe
) else (
    build\LCOCR.exe
)
pause
