@echo off

echo ===========================
echo Clean
echo ===========================
if exist build (
    rmdir /s /q build
)

echo ===========================
echo Configure
echo ===========================
cmake -S . -B build

if %ERRORLEVEL% neq 0 (
    echo Configure Failed!
    pause
    exit /b %ERRORLEVEL%
)

echo ===========================
echo Build
echo ===========================
cmake --build build --config Debug

if %ERRORLEVEL% neq 0 (
    echo Build Failed!
    pause
    exit /b %ERRORLEVEL%
)

echo ===========================
echo Run
echo ===========================

build\Debug\EmbeddedPowerManager.exe

pause
