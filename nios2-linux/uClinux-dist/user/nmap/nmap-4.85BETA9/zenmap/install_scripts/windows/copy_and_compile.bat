@echo off

echo Setting installation variables...
set PythonEXE=C:\Python26\python.exe
set DistDir=dist
set LibraryDir=%DistDir%\py2exe
set GTKDir=C:\GTK
set Output=win_install.log

IF EXIST %PythonEXE% GOTO GGTK
ECHO No Python 2.6 found!
EXIT 1

:GGTK
IF EXIST %GTKDir% GOTO GWork
ECHO No GTK found!
EXIT 1

:GWork

echo Writing output to %Output%

echo Removing old compilation...
IF EXIST %DistDir% rd %DistDir% /s /q > %Output%

echo Creating dist directory tree...
mkdir %LibraryDir%\etc
mkdir %LibraryDir%\share
mkdir %LibraryDir%\share\themes
mkdir %LibraryDir%\lib


echo Copying GTK files to dist directory...
xcopy %GTKDir%\bin\*.dll %LibraryDir% /S >> %Output%
xcopy %GTKDir%\etc %LibraryDir%\etc /S /I >> %Output%
xcopy %GTKDir%\lib\gtk-2.0 %LibraryDir%\lib\gtk-2.0 /S /I >> %Output%
xcopy %GTKDir%\share\themes\Default %LibraryDir%\share\themes\Default /S /I >> %Output%
xcopy %GTKDir%\share\themes\MS-Windows %LibraryDir%\share\themes\MS-Windows /S /I >> %Output%

echo Compiling using py2exe...
%PythonEXE% setup.py py2exe >> %Output%

echo Removing the build directory...
rd build /s /q >> %Output%


echo Done!

