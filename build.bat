
@echo off

rem =========================================

SETLOCAL

set ROOT_DIR=%~dp0
set BUILD_DIR=build

if not defined BISON_SIMPLE set BISON_SIMPLE=%ROOT_DIR%\win_programs\bison.simple

rem =========================================

cd %ROOT_DIR% || goto End

if not exist %BUILD_DIR% ( 
  mkdir %BUILD_DIR% || goto End
)

pushd %BUILD_DIR% || goto End
cmake -G "NMake Makefiles" %1 %2 %3 %4 %5 %6 %7 %8 %9 .. || goto End
nmake || goto End
popd

rem =========================================

if %ERRORLEVEL% GTR 0 (
echo.
echo Build failure.
echo.
popd
exit /b 1
) else (
echo.
echo Successfully build
echo.
popd
exit /b 0
)

rem =========================================
