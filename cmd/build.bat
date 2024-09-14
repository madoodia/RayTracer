@REM # --------------------- #
@REM # (C) 2024 madoodia.com #
@REM # --------------------- #


@ECHO OFF

SET ROOT=%1
SET CURRENT_DIR=%~dp0

CALL "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

@REM cmd directory
call "%CURRENT_DIR%\envVars.bat"

pushd "W:"
if exist "build" (
    rmdir /s /q "build"
)

MKDIR "build"
popd
pushd "W:\build"

cmake -G "NMake Makefiles" ".."
@REM cmake -G "Visual Studio 16 2019" ../
cmake --build . --config Debug

popd
