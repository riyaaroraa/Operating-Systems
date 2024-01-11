@echo off

set SOURCE_FILES=main.c process.c simulation.c
set HEADER_FILE=process.h
set OUTPUT_FILE=myprogram.exe

gcc -o %OUTPUT_FILE% %SOURCE_FILES%

if exist %OUTPUT_FILE% (
   %OUTPUT_FILE%
) else (
   echo Compilation failed.
)
