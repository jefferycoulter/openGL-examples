@echo OFF

set DIR=%cd%

cd %DIR%/../build

cmake -S %DIR%/.. -B %DIR%/../build -G "Visual Studio 16 2019" -A x64

cmake --build %DIR%/..build --target ALL_BUILD --config Release

pause