@REM # -------------------- #
@REM # (C)2024 madoodia.com #
@REM # -------------------- #

@echo off

SET CURRENT_DIR=%~dp0

@REM cmd directory
call "%CURRENT_DIR%\envVars.bat"
powershell -Command "& run %PROJECT_NAME%.exe" > "W:\output\image.ppm"
