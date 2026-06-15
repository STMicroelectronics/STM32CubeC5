#!/bin/bash
# export DEBUG=1 in your local env to enable debug console print
[ "$DEBUG" = "" ] || set -x

# Variables Initialization: values passed as arguments to the script respect the same order of declaration in the PDSC generator <arguments> node
shell_script_abs_path=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
generatorid=$1
generatorinputfile=$2
gpdsctemplateName=$3
dryRunFlag=$4

# Check cube is installed
which cube &> /dev/null
if [ $? -ne 0 ]; then
    echo "[GEN-ERROR] cube wrapper not found: STOP"
    exit 1
fi

# Check codegen is installed
cube --list | grep codegen
if [ $? -ne 0 ]; then
    echo "[GEN-ERROR] codegen not found: STOP"
    exit 2
fi

# GPDSC Generation in stdout (if a generator is dry-run capable)
if [ "$dryRunFlag" = "--dry-run" ]; then
echo "[STEP 1/1: dry-run]"
cube codegen generategpdsc --path $generatorinputfile --generatorId "$generatorid" --templatePath "$shell_script_abs_path/$gpdsctemplateName" --dry-run
else
# Code Generation Step:
echo "[STEP 1/2: CODE-GEN]"
cube codegen generatefromlockfile --path $generatorinputfile --generatorId "$generatorid"
# GPDSC Generation Step:
echo "[STEP 2/2: GPDSC-GEN]"
cube codegen generategpdsc --path $generatorinputfile --generatorId "$generatorid" --templatePath "$shell_script_abs_path/$gpdsctemplateName"
fi
