IF NOT EXIST .\GitRelease.bat GOTO END
CALL qq
C:\Factory\SubTools\GitFactory.exe /ow . C:\home\GitHub\NetworkTest
rem CALL C:\var\go.bat
:END
