@echo off
cd /d %~dp0


xcopy	/y	.\Engine\Header\*.*	.\Reference\Header

xcopy	/y	.\Engine\Bin\Debug\Engine.dll	.\Client\Bin\Debug
xcopy	/y	.\Engine\Bin\Debug\Engine.lib	.\Client\Bin\Debug

xcopy	/y	.\Engine\Bin\Release\Engine.dll	.\Client\Bin\Release
xcopy	/y	.\Engine\Bin\Release\Engine.lib	.\Client\Bin\Release

xcopy	/y	.\Engine\Bin\Debug\Engine.lib	.\Reference\Library\Debug
xcopy	/y	.\Engine\Bin\Release\Engine.lib	.\Reference\Library\Release

pause