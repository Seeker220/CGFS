@echo off
echo Compiling project...

:: Compile main.cpp into CGFS.exe
g++ -std=c++23 -o CGFS.exe main.cpp

:: Check if compilation was successful before running
if %errorlevel% == 0 (
    echo Compilation successful. Running program...
    echo ============================================
    CGFS.exe
) else (
    echo Compilation failed.
)