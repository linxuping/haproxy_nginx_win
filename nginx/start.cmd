@echo off 
cd "C:\nginx"
:REST
date /T
time /T
echo ����nginx�����Ե�...... 

:20 * 180 = 3600s
for /l %%i in (1,1,20) do (
	tasklist /nh|find /i "nginx.exe" > c:\tmp
	if ERRORLEVEL 1 ( 
		echo nginx.exe�����ڣ�����...
		start nginx
	)
	PING.EXE -n 180 127.1>nul
)


:�������ⲻͬ�������ϣ�nginxͬʱ����
set /a rnd=%random%%%100+1
PING.EXE -n %rnd% 127.1>nul

nginx.exe -s stop
start "" "taskkill" /f /im nginx.exe
start "" "taskkill" /f /im nginx.exe
PING.EXE -n 90 127.1>nul

tasklist /nh|find /i "nginx.exe"
if ERRORLEVEL 1 ( echo nginx.exe���Ѿ��˳� ) else (shutdown -r -t 0)

GOTO REST
