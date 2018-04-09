@Echo Off

Set FDir="С:\"
Set Maska="sk*"

FOR /R %FDir% %%i IN (%Maska%) DO Call :Obrabotka "%%i"
GoTo :EOF 

:Obrabotka
Echo %1
