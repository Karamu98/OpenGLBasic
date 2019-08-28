xcopy "%~dp0..\resources" "%~dp0..\bin\x64\Debug\resources" /s /y /i /c

start /d "%~dp0..\bin\x64\Debug" BasicLight.exe

pause