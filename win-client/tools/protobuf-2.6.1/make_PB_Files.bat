@ECHO off
SETLOCAL enabledelayedexpansion
cls
COLOR 1f

ECHO.
ECHO.
ECHO   ##############################################################
ECHO   #               ��ӭʹ�� TeamTalk ����������               #
ECHO   #                   version 1.0                              #
ECHO   ##############################################################
ECHO.
ECHO.

rem ����PBЭ���ļ�
echo make IM protocol buffer files...
cd %~dp0
mkdir %~dp0\..\..\include\ProtocolBuffer
copy  %~dp0\..\..\..\pb\*.proto %~dp0\*.proto
echo Copy IM protocol buffer files finished
rem ����PBЭ���C++�汾
protoc.exe --cpp_out=%~dp0\..\..\include\ProtocolBuffer *.proto
echo make the pb files of c++ version has finished.
pause