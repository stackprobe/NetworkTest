C:\Factory\Tools\RDMD.exe /RC out

SET RAWKEY=5d47556eb3051f23420ffc9069d3ea72

C:\Factory\SubTools\makeAESCluster.exe Picture.txt     out\Data1.dat %RAWKEY% 100000000
C:\Factory\SubTools\makeAESCluster.exe Music.txt       out\Data2.dat %RAWKEY% 200000000
C:\Factory\SubTools\makeAESCluster.exe SoundEffect.txt out\Data3.dat %RAWKEY% 300000000

COPY /B NetworkTest\Release\NetworkTest.exe out
C:\Factory\Tools\xcp.exe doc out

C:\Factory\SubTools\zip.exe /O out NetworkTest

PAUSE
