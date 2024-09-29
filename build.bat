@ECHO off
rem madoodia@gmail.com
rem ..................

SET HOME_DIR=%~dp0
for %%a in ("%cd%") do set "FOLDER_NAME=%%~na"

call "%VCVARS_LOCATION%/vcvarsall.bat" x64

pushd %HOME_DIR%

call nmake

popd

ECHO ---= Building is finished! =---

Pause
cls
@REM CALL %FOLDER_NAME%.exe