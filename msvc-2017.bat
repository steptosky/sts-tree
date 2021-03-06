:: Generate VS 2017 project
::==========================================================
@echo off
:: Fix problems with some symbols.
REM change CHCP to UTF-8
CHCP 1252
CLS
::==========================================================
:: Sometimes conan asks you about to login to private repositories
:: if you have added ones. So you can put into the root repository folder
:: the file called 'set-conan-user.bat' and write there something like this:
::      call conan user -p <password> -r <remote alias>  <user name>
if exist set-conan-user.bat call set-conan-user.bat
::==========================================================
:: Creating building DIR and use it as working one.
set dir="msvc-2017"
if not exist %dir% mkdir %dir%
cd %dir%
::==========================================================
:: Generating Visual Studio project.
call cmake -G "Visual Studio 15 Win64" ../ ^
    -DCMAKE_INSTALL_PREFIX=../output ^
    -DBUILD_TESTING=ON
::==========================================================
:: Building
:: Keep it commented if you want to generate VS project only
:: (without building), otherwise uncomment it.
::call cmake --build . --target install --config Release
::call cmake --build . --target install --config Debug
::==========================================================
cd ../
pause
::==========================================================