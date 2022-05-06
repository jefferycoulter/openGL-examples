@echo OFF

set DIR=%cd%

cd %DIR%/../build

cmake -S %DIR%/.. -B %DIR%/../build

pause