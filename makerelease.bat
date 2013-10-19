@echo off
REM Run this build script through Tools > Visual Studio Command Prompt, otherwise 
REM no compilation will be done, but zip is compressed as usual.

setlocal

set archivename=peisik_-_pilot_a.zip
set tempdir=meta\tempfiles
set extradir=meta\extrafiles
set assetdir=GIGAengine\assets
set binarypath=Release\GIGAengine.exe
REM the final binary name
set binaryname=pilot_a.exe
set compressor="D:\7Zip\7z.exe"
set releasedir=releasepack

goto build
:build_done
goto compress
:compress_done
exit /B

:build

setlocal disableextensions

set x=%VSINSTALLDIR%
if "%x%"=="" (goto no_vs) 
setlocal enableextensions
REM call msbuild GIGAengine.sln /p:Configuration=Release /t:Rebuild /nologo 
call msbuild GIGAengine.sln /p:Configuration=Release /nologo 
goto build_done

:no_vs
echo "No Visual Studio installation found, skipping build."
goto build_done

:compress

rmdir /Q /S %tempdir%
if not exist %tempdir% mkdir %tempdir%
if not exist %extradir% mkdir %extradir%
if not exist %tempdir%\assets mkdir %tempdir%\assets
xcopy %extradir% %tempdir% /Y /E
xcopy %assetdir% %tempdir%\assets /Y /E
copy %binarypath% %tempdir%\%binaryname%

if not exist %releasedir% mkdir %releasedir%

if not exist %compressor% goto no_zip

REM remove old archive
if exist %releasedir%\%archivename% del %releasedir%\%archivename%

pushd %releasedir%
%compressor% a -tzip -y %archivename% "%~dp0\%tempdir%\*"
popd

goto compress_done

exit /B

:no_zip
echo "No 7zip found, skipping zip compression."
exit /B

pause