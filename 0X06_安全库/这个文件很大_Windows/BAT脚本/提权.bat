@echo off
COLOR B9
CLS
:init
setlocal DisableDelayedExpansion
set "batchPath=%~0"
for %%k in (%0) do set batchName=%%~nk
set "vbsGetPrivileges=%temp%\OEgetPriv_%batchName%.vbs"
setlocal EnableDelayedExpansion
:checkPrivileges
NET FILE 1>NUL 2>NUL
if '%errorlevel%' == '0' ( goto gotPrivileges ) else ( goto getPrivileges )
:getPrivileges
if '%1'=='ELEV' (echo ELEV & shift /1 & goto gotPrivileges)
ECHO.
ECHO ********************************
ECHO ���� UAC Ȩ����׼����
ECHO ********************************
ECHO Set UAC = CreateObject^("Shell.Application"^) > "%vbsGetPrivileges%"
ECHO args = "ELEV " >> "%vbsGetPrivileges%"
ECHO For Each strArg in WScript.Arguments >> "%vbsGetPrivileges%"
ECHO args = args ^& strArg ^& " "  >> "%vbsGetPrivileges%"
ECHO Next >> "%vbsGetPrivileges%"
ECHO UAC.ShellExecute "!batchPath!", args, "", "runas", 1 >> "%vbsGetPrivileges%"
"%SystemRoot%\System32\WScript.exe" "%vbsGetPrivileges%" %*
exit /B
:gotPrivileges
setlocal & pushd .
cd /d %~dp0
if '%1'=='ELEV' (del "%vbsGetPrivileges%" 1>nul 2>nul  &  shift /1)

ECHO. ===========================================================================================
ECHO.                                  ZUCC������ͬʱ����
ECHO.                                                            ���ߣ���^&��       �汾��V07
ECHO. ʹ��˵����
ECHO. ������������������ZUCC����ѧ�����ң���ʹ�����ڲ��жϿ������ӵ�����·���������
ECHO. ÿ����Ҫ������ͬʱ����ǰ��ʹ�ñ�����
ECHO. Ŀǰ��֧��win7�����ϼ�������ϵͳ���°汾֧��һ����Ȩ+�Զ�����-��-
ECHO. �汾˵����
ECHO. �ܸ��˵�֪ͨ���ϸ��汾�г�����Ȩģ���ֻ�����0.0����Ȼ���κ��ˣ�
ECHO. ������Ҫ����� https://hdsky.pw/   ���԰��̳����ÿ�������
ECHO. ���������뼰ʱ��ϵhd852025@hotmail.com�ṩ������żȻ�鵽�һ�ظ��ġ�
ECHO. ===========================================================================================
pause

for /f "skip=1 tokens=2,3,4,5,6" %%i in ('wmic os get Caption') do set win=%%i%%j
echo ��ǰϵͳ�汾%win%
cd.>Gateway.txt
ipconfig/all>ipconfig.txt

if /i %win%==Windows7 goto Win7
if /i %Win%==Windows8 goto Win10
if /i %Win%==Windows10 goto Win10
if /i %Win%==Windows8.1 goto Win10

:win7
set flag=vsbat
for /f "delims=" %%i in (ipconfig.txt) do (
if  defined flag (
echo "%%i"|findstr /c:"��������" && set flag=
) else (
echo %%i|findstr /c:"Ĭ������"  && (set "t= %%i" & goto :DEAL)
)
)

:win10
set flag=vsbat
for /f "delims=" %%i in (ipconfig.txt) do (
if  defined flag (
echo "%%i"|findstr /c:"��̫��" && set flag=
) else (
echo %%i|findstr /c:"Ĭ������"  && (set "t= %%i" & goto :DEAL)
)
)

:DEAL
for /f "tokens=2 delims=:" %%i in ("%t%") do echo %%i >> Gateway.txt
echo	�ɹ���ȡ��
set/p var=<Gateway.txt
echo %var%
route add 10.0.0.0 mask 255.0.0.0%var%
route add 210.32.0.0 mask 255.255.240.0%var%
route add 210.32.128.0 mask 255.255.192.0%var%
route add 222.205.0.0 mask 255.255.128.0%var%
echo  ����DNS��ַing....
Netsh interface IP Set dns "ZUCC" static 114.114.114.114 primary
Netsh interface IP add dns "ZUCC" 10.61.10.10
ipconfig /flushdns
Echo  ���óɹ� 
goto END

:END
del Gateway.txt
del ipconfig.txt
pause