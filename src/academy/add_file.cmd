@echo off
if "%2"=="" goto :error

set filename=%1
set cpp_file=%2\%1.cpp
set h_file=%2\%1.h

if "%WRITE_H%" == "" (
if exist %h_file% goto exists
if exist %cpp_file% goto exists
)

if "%WRITE_H%" == "" (
    set WRITE_H=yes
    cmd.exe /c %0 %* >%h_file%
    set WRITE_H=
    exit /b %ERRORLEVEL%
)

if not "%WRITE_H%" == "no" (
echo /*! \file %filename%.h
echo     \brief 
echo     \author Kiselev Kirill
echo     \date %DATE%
echo */
echo.
echo #ifndef %filename%_H
echo #define %filename%_H
echo.
echo /*!
echo \class %filename%
echo \brief 
echo */
echo class %filename%
echo {
echo public:
echo    //! Конструктор
echo   %filename%^(^);
echo   //! Деструктор
echo   virtual ~%filename%^(^) ^{^}
echo protected:
echo };
echo.
echo #endif
echo.
)

if "%WRITE_CPP%" == "" (
    set WRITE_H=no
    set WRITE_CPP=yes
    cmd.exe /c %0 %* >%cpp_file%
    exit /b %ERRORLEVEL%
)

if not "%WRITE_CPP%" == "no" (
echo #include "%filename%.h"
echo.
echo.
)

if "%FINISH%" == "" (
    set WRITE_H=
    set WRITE_CPP=
    rem qmake -tp vc
    exit /b %ERRORLEVEL%
)

:error
echo format: add_file.cmd "filename" "folder"
goto finish
:exists
echo File %h_file% or %cpp_file% exists already.
:finish
