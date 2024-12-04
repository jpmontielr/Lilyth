@echo off

IF NOT EXIST ..\build mkdir ..\build 
pushd ..\build

call cl -MT -nologo -Gm- -GR- -EHa- -Od -Oi -WX -W4 -wd4996 -wd4201 -wd4189 -wd4505 -wd4100 -FC -Z7 ..\code\win32_lilyth.cpp /link -opt:ref
call win32_lilyth.exe

lilypond --loglevel=WARNING lilyth.ly

popd
