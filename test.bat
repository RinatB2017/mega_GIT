@Echo Off

set FDir="."

FOR /R %FDir% %%i IN (".qmake.stash") DO Call :Obrabotka "%%i"
goto Exit

:Obrabotka
rem del %1
echo %1
goto :EOF

:Exit
pause
