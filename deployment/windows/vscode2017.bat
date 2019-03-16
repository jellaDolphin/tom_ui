@echo off
@call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"

set QT5_DIR="C:\Qt\5.12.2\msvc2017_64"

mkdir build
cd build

rem cmake -G "Visual Studio 15" -DCMAKE_BUILD_TYPE=Release ..
rem cmake --build . --config Release

rem Configure the application in the current directory
cmake ^
    -G"NMake Makefiles" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_INSTALL_PREFIX=out ^
    %~dp0\.. || exit /b

rem Build and install the application
nmake || exit /b
nmake install || exit /b

rem Run windeployqt on nitroshare-ui
windeployqt ^
    --verbose 0 ^
    --no-compiler-runtime ^
    --no-angle ^
    --no-opengl-sw ^
    out/bin/nitroshare-ui.exe || exit /b

rem Find the path to InnoSetup from the registry
set rkey="HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Inno Setup 5_is1"
for /f "skip=2 tokens=1,2*" %%i in ('reg query %rkey% /v InstallLocation /reg:32') DO (
    set iscc="%%kiscc.exe"
)

rem Make sure that the ISCC compiler exists
if not exist %iscc% (
    echo "Unable to find the InnoSetup compiler"
    exit /b
)

rem Build the installer
%iscc% ^
    /Q ^
    /DPREFIX="%cd%\out" ^
    deployment/setup.iss || exit /b