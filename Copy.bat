cd..
cd..

xcopy .\TWEngine\Include\*.h .\Engine\Include\ /s /y /d
xcopy .\TWEngine\Bin\*.* .\Engine\Bin\ /s /y /d /exclude:exclude.txt
xcopy .\TWEngine\Bin\*.* .\TWEditor\Bin\ /s /y /d /exclude:exclude.txt
xcopy .\TWEngine\Bin\*.* .\3DClient\bin\ /s /y /d /exclude:exclude.txt

rem 리소스복사하기위함
xcopy .\3DClient\Bin\*.* .\WinFormEditor\bin\Debug\ /s /y /d /exclude:exclude.txt
xcopy .\3DClient\Bin\*.* .\WinFormEditor\bin\Release\ /s /y /d /exclude:exclude.txt