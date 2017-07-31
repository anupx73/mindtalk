@echo off
cls
cd .

del /s /q .\*.ncb
del /s /q .\*.suo /a:h
del /s /q .\*.user

rd /s /q .\client\release
rd /s /q .\client\debug

rd /s /q .\server\release
rd /s /q .\server\debug

rd /s /q .\updater\release
rd /s /q .\updater\debug

del .\binary\*.pdb
del .\binary\*.db /a:h
del .\binary\*.ilk