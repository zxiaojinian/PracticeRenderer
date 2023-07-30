@echo off
pushd ..\
call GenProjects\premake5.exe vs2022
popd
PAUSE