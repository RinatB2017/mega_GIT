@Echo Off

set FDir="."
set Mask="*.txt"

FOR /R %FDir% %%i IN ("Makefile.*") DO Call :Obrabotka "%%i"
FOR /R %FDir% %%i IN ("*.user") DO Call :Obrabotka "%%i"
rem FOR /R %FDir% %%i IN ("*.exe") DO Call :Obrabotka "%%i"
FOR /R %FDir% %%i IN ("*.o") DO Call :Obrabotka "%%i"
rem FOR /R %FDir% %%i IN (".qmake.stash") DO Call :Obrabotka "%%i"
goto Exit

:Obrabotka
del %1
goto :EOF

:Exit
pause
