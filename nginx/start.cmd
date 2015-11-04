@echo off 
cd "C:\nginx"
:REST
date /T
time /T
echo 重启nginx，请稍等...... 

:20 * 180 = 3600s
for /l %%i in (1,1,20) do (
	tasklist /nh|find /i "nginx.exe" > c:\tmp
	if ERRORLEVEL 1 ( 
		echo nginx.exe不存在，启动...
		start nginx
	)
	PING.EXE -n 180 127.1>nul
)


:尽量避免不同服务器上，nginx同时重启
set /a rnd=%random%%%100+1
PING.EXE -n %rnd% 127.1>nul

nginx.exe -s stop
start "" "taskkill" /f /im nginx.exe
start "" "taskkill" /f /im nginx.exe
PING.EXE -n 90 127.1>nul

tasklist /nh|find /i "nginx.exe"
if ERRORLEVEL 1 ( echo nginx.exe都已经退出 ) else (shutdown -r -t 0)

GOTO REST
