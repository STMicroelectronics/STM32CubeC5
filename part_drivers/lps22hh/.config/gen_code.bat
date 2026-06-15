@echo off

REM Get the absolute path of the batch script
set "script_path=%~dp0"

REM Assign command line arguments to variables
set "generatorid=%1"
set "generatorinputfile=%2"
set "gpdsctemplateName=%3"
set "dryRunFlag=%4"

REM Check if cube is installed
where cube >nul 2>nul
if %errorlevel% neq 0 (
    echo [GEN-ERROR] cube wrapper not found: STOP
    exit /b 1
)

REM Check if codegen is installed
cube --list | findstr /i "codegen" >nul
if %errorlevel% neq 0 (
    echo [GEN-ERROR] codegen not found: STOP
    exit /b 2
)

:: GPDSC Generation in stdout (if a generator is dry-run capable)
if "%dryRunFlag%"=="--dry-run" (
cube codegen generategpdsc --path %generatorinputfile% --generatorId "%generatorid%" --templatePath "%script_path%\%gpdsctemplateName%" --dry-run
) else (
:: Code Generation Step
echo [STEP 1/2: CODE-GEN]
cube codegen generatefromlockfile --path %generatorinputfile% --generatorId "%generatorid%"
:: GPDSC Generation Step
echo [STEP 2/2: GPDSC-GEN]
cube codegen generategpdsc --path %generatorinputfile% --generatorId "%generatorid%" --templatePath "%script_path%\%gpdsctemplateName%"
)
