@title ע��MES��̬Dll

@echo off 
%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)

:Begin

:: �û�ѡ����ע�ỹ��ж��
@echo ��ѡ��Ҫ���еĲ�����Ȼ�󰴻س�
@echo [0]ע��
@echo [1]ж��
@echo.

::echo %SystemRoot%

@echo off
set /p var=��ѡ��:
@echo.

IF "%var%"=="0" goto Install 
IF "%var%"=="1" goto Uninstall 
IF NOT "%var%"=="0" goto Begin
IF NOT "%var%"=="1" goto Begin

:Install
:: ִ��ע��
::regasm Kel.IFactory.Mqtt.Client.dll /codebase /tlb:Kel.IFactory.Mqtt.Client.tlb
@echo off 
%SystemRoot%/Microsoft.NET/Framework/v4.0.30319/RegAsm.exe Kel.IFactory.Mqtt.Client.dll /codebase /tlb:Kel.IFactory.Mqtt.Client.tlh
IF ERRORLEVEl 1 goto Failed
IF ERRORLEVEl 0 goto Success

:Uninstall
:: ִ��ж��
::regasm Kel.IFactory.Mqtt.Client.dll /unregister
@echo off 
%SystemRoot%/Microsoft.NET/Framework/v4.0.30319/RegAsm.exe Kel.IFactory.Mqtt.Client.dll /unregister
IF ERRORLEVEl 1 goto Failed
IF ERRORLEVEl 0 goto Success

:Success
@echo ����ִ�гɹ�
goto End

:Failed
@echo ����ִ��ʧ��
@echo %errorlevel%
goto End

:End
pause